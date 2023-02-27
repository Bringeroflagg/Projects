#!/usr/bin/python3

import socket
import sys
import random
import time
import select
import queue
import re

if len(sys.argv) < 4:
    print("Invalid number of Arguments")
    sys.exit()

serverAddressPort = (sys.argv[1], int(sys.argv[2]))
write_file = sys.argv[4]
echoServer = ("10.10.1.100", 8888)
buffer_size = 1024

# [0] = sequence, [1] = window
# sender sends sequence, length size, receiver sends acknowledgment, window size
sender_data = [0,0]
# [0] = ackowledgement, [1] = Window
receiver_data = [1,5120]

# Create a UDP socket at client side
UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
UDPClientSocket.setblocking(0)
UDPClientSocket.bind(serverAddressPort)

# UDP select options
potential_reader = [UDPClientSocket]
potential_writer = [UDPClientSocket]
potential_exp = [UDPClientSocket]

# read file and store, change packets_send to dictionary with key = seq#, value = data
packets_send = {}

# print in the end
receiver_data_append = {}

# receiver message queue
receiver_queue = queue.Queue()

def main():    
    
    sender_syn = None
    receiver_syn = None
    sender_fin = None
    receiver_fin = None
    sender_timer = 0
        
    packetize_file(sys.argv[3])
    
    while True:
        (readable, writable, exceptional) = select.select(potential_reader, potential_writer, potential_exp,.4)

        if UDPClientSocket in writable:
            
            # ----- Start of handshake for SYN
            if sender_syn == None and sender_timer == 0:
                # Send; SYN; Sequence: 0; Length: 0
                print(current_time() + " Send; SYN; Sequence: " + str(sender_data[0]) + "; Length: 0")
                message = "SYN\r\nSequence: " + str(sender_data[0]) + "\r\nLength: " + str(sender_data[1]) + "\r\n\r\n"
                sender_timer = 1
                UDPClientSocket.sendto(message.encode(), echoServer)
                potential_writer.remove(UDPClientSocket)
                
            elif receiver_syn == None:
                receiver_syn = True
                msg1 = receiver_queue.get()
                print(current_time() + " Send; ACK; Acknowledgment: " + str(receiver_data [0]) + "; Window: " + str(receiver_data [1]))
                receiver_syn = True
                UDPClientSocket.sendto(msg1.encode(), echoServer)       
            # ----- end of SYN
          
            # ----- send packets
            # this is for receiver fin message
            elif not receiver_queue.empty() and receiver_fin == True:
                msg_receiver_send = receiver_queue.get()
                write_to_file(write_file)
                UDPClientSocket.sendto(msg_receiver_send.encode(), echoServer)
                potential_writer.remove(UDPClientSocket)
                
            # this is for ack messages for data
            elif not receiver_queue.empty():
                msg_receiver_send = receiver_queue.get()
                receiver_data[1] += len1
                UDPClientSocket.sendto(msg_receiver_send.encode(), echoServer)
                potential_writer.remove(UDPClientSocket)
            
            # ----- this is for sending packets to receiver
            elif len(packets_send) > 0 and sender_syn == True and sender_data[1] > 1:
                for key, value in packets_send.items():
                    # print log
                    if sender_data[1] > 1 and len(packets_send) > 1 :
                        print(current_time() + " Send; DAT; Sequence: " + str(key) + "; Length: " + str(buffer_size))
                        sender_data[1] = sender_data[1] - buffer_size
                        send_msg = "DAT\r\nSequence: " + str(key) + "\r\nLength: 1024\r\n\r\n" + value
                        UDPClientSocket.sendto(send_msg.encode(), echoServer)
                        
                    elif sender_data[1] > 1 and len(packets_send) == 1 and sender_data[1] > 1:
                        print(current_time() + " Send; DAT; Sequence: " + str(key) + "; Length: " + str(buffer_size))
                        sender_data[1] = 0
                        send_msg = "DAT\r\nSequence: " + str(key) + "\r\nLength: 1024\r\n\r\n" + value
                        UDPClientSocket.sendto(send_msg.encode(), echoServer)
                # remove writer        
                potential_writer.remove(UDPClientSocket)
            # ----- end of sending of any packets
            
            # Start fin process, if we get to here, and there are no packets to send
            elif len(packets_send) == 0 and sender_data[1] > 0 and sender_fin == None:
                print(current_time() + " Send; FIN; Sequence: " + str(sender_data[0]) + "; Length: 0")
                send_fin_msg = "FIN\r\nSequence: " + str(sender_data[0]) + "; Length: 0"
                sender_data[1] = 0
                sender_fin = True
                UDPClientSocket.sendto(send_fin_msg.encode(), echoServer)
            
        
        if UDPClientSocket in readable:
            
            data_msg, addr = UDPClientSocket.recvfrom(10000)
            data_msg = data_msg.decode()
            # store for DAT sends
            DAT_msg = data_msg
            # split line by line
            data_msg = data_msg.split()      
            
            # ----- start syn process, receiver receives
            if data_msg[0] == "SYN" and receiver_syn == None:
                
                # print msg received      
                print(current_time() + " Receive; SYN; Sequence: 0; Length: 0")
                # ACK; Acknowledgment: 1; Window: 2048
                send_msg = "ACK\r\nAcknowledgment: " + str(receiver_data[0]) + "\r\nWindow: " + str(receiver_data[1]) + "\r\n\r\n"
                receiver_queue.put(send_msg)
                potential_writer.append(UDPClientSocket)
            
            elif sender_syn == None and data_msg[0] == "ACK":
                
                print(current_time() + " Receive; ACK; Acknowledgment: " + data_msg[2] + "; Window: " + data_msg[4])
                # set to this number
                ack = int(data_msg[2])
                sender_data[0] = ack
                window = int(data_msg[4])
                sender_data[1] = window
                sender_syn = True
            # ----- end syn process
 
            # ----- receivers get DAT msg
            elif data_msg[0] == "DAT":
                DAT_msg = DAT_msg.split("\r\n\r\n")
                seq = int(data_msg[2])
                len1 = int(data_msg[4])

                print(current_time() + " Receive; DAT; Sequence: " + str(seq) + "; Length: " + str(len1))
                
                receiver_data_append[receiver_data[0]] = DAT_msg[1]

                # window
                receiver_data[1] = receiver_data[1] - len1
                # acknowledgment
                receiver_data[0] = receiver_data[0] + len1 

                #Send; ACK; Acknowledgment: 1025; Window: 1024
                print(current_time() + " Send; ACK; Acknowledgment: " + str(receiver_data[0]) + "; Window: " + str(receiver_data[1]))
                msgACK = "ACK\r\nAcknowledgment: " + str(receiver_data[0]) + "\r\nWindow: " + str(receiver_data[1]) + "\r\n\r\n"
                
                receiver_queue.put(msgACK)

            # use DAT_msg[1] == payload
            elif data_msg[0] == "ACK":
                # end program
                if sender_fin == True:
                    print(current_time() + " Receive; ACK; Acknowledgment: " + str(data_msg[2]) + "; Window: " + str(data_msg[4]))
                    #print(current_time() + " Receive; ACK; Acknowledgment: " + str(data_msg[2]) + "; Window: " + str(data_msg[4]) )
                    sys.exit()
                else:
                    print(current_time() + " Receive; ACK; Acknowledgment: " + str(data_msg[2]) + "; Window: " + str(data_msg[4]))
                    # delete packet since we have confirmation it exists
                    del packets_send[int(data_msg[2]) - buffer_size]
                    # increase len by 1024, since we have received packet confirm
                    sender_data[1] += buffer_size
                    sender_data[0] = int(data_msg[2])

            
            # receiver gets FIN, writes file and sends message to end program
            elif data_msg[0] == "FIN":
                print(current_time() + " Receive; FIN; Sequence: " + str(data_msg[2]) + " Length: " + str(data_msg[4]))
                receiver_fin = True
                receiver_data[0] += 1
                print(current_time() + " Send; ACK; Acknowledgment: " + str(receiver_data[0]) + "; Window: " + str(receiver_data[1]))
                msg_fin_send = "ACK\r\nAcknowledgment: " + str(receiver_data[0]) + "\r\nWindow: " + str(receiver_data[1]) + "\r\n\r\n"
                receiver_queue.put(msg_fin_send)
                
            # If code shows up here, then send reset
            else:
                if data_msg[0] == "RST":
                    print(current_time() + "Receive; RST; Acknowledgment" + str(data_msg[2]) + "; Window: " + str(data_msg[4]))
                    sys.exit()
                print(current_time() + "Send; RST; Acknowledgment: " + str(receiver_data[0]) + "; Window: " + str(receiver_data[1]))
                msg_rst = "RST\r\nAcknowledgment: " + str(receiver_data[0]) + "; Window: " + str(receiver_data[1])
                receiver_queue.put(msg_rst)
                
        # if timeout reached for a packet, then resend
        if not (readable or writable or exceptional):
            sender_fin = None
            #sender_data[1] = sender_data[1] + 1024
            potential_writer.append(UDPClientSocket)
            
# write to file; key is pointed to a single string
def write_to_file(write_file):

    w_file = open(write_file, "w")
    content = ""
    for k, v in receiver_data_append.items():
        content += v
    w_file.write(content)
    w_file.close()

# packetize by reading until hitting null, read 1024 bytes max
def packetize_file(read_file):

    global packets_send
    r_file = open(read_file, "r")
    i = 1
    while True:
        buffer = r_file.read(buffer_size)
        if not buffer == "":
            packets_send[i] = buffer
            i += buffer_size
        else:
            break
    
    r_file.close()         

# receive current time
def current_time():
    time_obj = time.localtime()
    time_now = time.strftime("%a %b %d %H:%M:%S PDT %Y:", time_obj)
    # to print Wed Sep 15 21:44:35 PDT 2021:
    return time_now

if __name__ == '__main__':
    main()