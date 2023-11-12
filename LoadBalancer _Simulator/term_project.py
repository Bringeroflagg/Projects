#!/usr/bin/env python3

import random
import numpy as np
from SplayTree import SplayTree
from SplayTree import Node
import queue
import time
# queue methods: q1.empty(), q1.full(), q1.put(x), q1.get(), q.qsize()

class Sim:
    ORDER = 0
    MAX_SERVERS = 10
    MAX_QUEUE_SIZE = 100
    MAX_QUEUE_LEN = 0
    ARRIVAL_LAMBDA_RATE = 3
    SERVICE_RATE = 4.5
    serviceTime = 0
    DEPARTURE_RATE = 3.2
    type_arrival = 1
    type_departure = 2
    DICT_OF_SERVERS = {}# dict of servers have qXX: queue_object
    DICT_OF_QUEUES = {}
    stream = random.Random()
    futureEventList = SplayTree()
    queueLength = []
    clock = 0
    NUMBER_OF_CUSTOMERS = 400
    numberInService = 0
    lastEventTime = 0
    totalBusy = 0
    sumResponseTime = 0
    numberOfDepartures = 0
    runCounter = 1
    def PureRand(self): 
        # print('-----')
        # print(random.randint(1, self.MAX_SERVERS))
        return random.randint(1, self.MAX_SERVERS)    # random server returned

    def RoundRobin(self):
        # ORDER 1,2,3,..,MAX_SERVERS,1,2,3,...
        # global ORDER
        if self.ORDER == self.MAX_SERVERS:
            self.ORDER = 1
        else:
            self.ORDER = self.ORDER + 1
        
        return self.ORDER

    def RandMin(self):
        # 1 - Randomly Select # of servers to be selected
        number_of_servers = self.PureRand()

        # 2 - Select servers
        selected_Servers = []
        for i in range(number_of_servers):
            # select a random server
            selected_Servers.append(self.PureRand())

        # Ensure no duplicates
        final_list = set(selected_Servers)
        final_list = list(final_list)

        # 3 - Check queue length of the selected servers and return the smalled queued server
        # [server_id, size]
        smallest_server = [0,0]

        # insert the first 
        server = 'Q' + str(final_list[0])
        smallest_server[0] = server
        smallest_server[1] = self.DICT_OF_SERVERS[server].qsize()

        # compares the lenght of each server, and returns the shortest length
        for i in range(1, len(final_list)):
            server = 'Q' + str(final_list[i])
            if self.DICT_OF_SERVERS[server].qsize() <= smallest_server[1]:
                smallest_server[0] = server
                smallest_server[1] = self.DICT_OF_SERVERS[server].qsize()

        #print('Smallest Server: ',smallest_server[0])
        return smallest_server[0]

    def load_balancer(self, method):
        
        if method == "PureRand":
            queue = 'Q' + str(self.PureRand())
        elif method == "RoundRobin":
            queue = 'Q' + str(self.RoundRobin())
        else:
            # returns the server 'Q1','Q2',..
            queue = self.RandMin()
        
        return queue


    def initialize(self):
        # Dictionary of the queues {queue1: [], ...}, where [0] is the first in line of the queue

        for i in range(1, self.MAX_SERVERS + 1):
            self.DICT_OF_SERVERS['Q'+str(i)] = queue.Queue(maxsize = self.MAX_QUEUE_SIZE)

        for i in range(1, self.MAX_SERVERS + 1):
            self.DICT_OF_QUEUES['Q'+str(i)] = {
                "busy": False,
                "size": 0,
            }

        time = np.random.exponential(1/self.ARRIVAL_LAMBDA_RATE)
        self.futureEventList.insert(time, self.type_arrival, 'NA')

    def ScheduleDeparture(self, event, queue_no):

        self.serviceTime = np.random.exponential(self.SERVICE_RATE)
        time = event.key + self.serviceTime
        self.futureEventList.insert(time, self.type_departure, queue_no)
        self.DICT_OF_QUEUES[queue_no]["size"] -= 1
        self.queueLength.append(self.DICT_OF_QUEUES[queue_no]["size"])
        self.DICT_OF_QUEUES[queue_no]["busy"] = True

    def ProcessDeparture(self, event):

        queue = self.DICT_OF_SERVERS[event.queue]
        finished = queue.get()

        if self.DICT_OF_QUEUES[event.queue]["size"] > 0:
            self.ScheduleDeparture(finished, event.queue)
        else:
            self.DICT_OF_QUEUES[event.queue]["busy"] = False

        response = self.clock - finished.key
        self.sumResponseTime += response
        self.totalBusy += self.clock - self.lastEventTime
        self.numberOfDepartures += 1
        self.lastEventTime = self.clock

    def ProcessArrival(self, event, queue_no):

        queue = self.DICT_OF_SERVERS[queue_no]
        queue.put(event)
        self.DICT_OF_QUEUES[queue_no]["size"] += 1
        self.queueLength.append(self.DICT_OF_QUEUES[queue_no]["size"])

        if not self.DICT_OF_QUEUES[queue_no]["busy"]:
            self.ScheduleDeparture(event, queue_no)
        else:
            self.totalBusy += (self.clock - self.lastEventTime)
        
        if self.MAX_QUEUE_LEN < self.DICT_OF_QUEUES[queue_no]["size"]:
            self.MAX_QUEUE_LEN = self.DICT_OF_QUEUES[queue_no]["size"]
        
        # schedule the next arrival
        time = self.clock + np.random.exponential(1/self.ARRIVAL_LAMBDA_RATE)
        self.futureEventList.insert(time, self.type_arrival, 'NA')
        self.lastEventTime = self.clock


    def reset_simulation(self):


        self.ORDER = 0
        self.MAX_SERVERS = 10
        self.MAX_QUEUE_SIZE = 100
        self.MAX_QUEUE_LEN = 0
        self.ARRIVAL_LAMBDA_RATE = 3
        self.SERVICE_RATE = 4.5
        self.serviceTime = 0
        self.DEPARTURE_RATE = 3.2
        self.type_arrival = 1
        self.type_departure = 2
        self.DICT_OF_SERVERS = {}# dict of servers have qXX: queue_object
        self.DICT_OF_QUEUES = {}
        self.stream = random.Random()
        self.futureEventList = SplayTree()
        self.queueLength = []
        self.clock = 0
        self.NUMBER_OF_CUSTOMERS = 20
        self.numberInService = 0
        self.lastEventTime = 0
        self.totalBusy = 0
        self.sumResponseTime = 0
        self.numberOfDepartures = 0
        # self.runCounter = 1

    def ReportGeneration(self,method):
        print(method)
        print(f"MAX QUEUE LENGTH: {self.MAX_QUEUE_LEN}")
        print(f"AVG QUEUE LENGTH: {sum(self.queueLength)/len(self.queueLength)}")
        print(f"AVG SYSTEM TIME: {self.sumResponseTime/self.NUMBER_OF_CUSTOMERS}")

        # Average time customers spents in system


    def main(self):
    
        # arrival - [5, 8, 20, 15, 25]
        # service - [5, 15,22,10, 22]
        # m - [4, 7, 10, 15, 20]
        param_combinations = [
        {'ARRIVAL_LAMBDA_RATE': 5, 'SERVICE_RATE': 5,'MAX_SERVERS': 4, },
        {'ARRIVAL_LAMBDA_RATE': 8, 'SERVICE_RATE': 15,'MAX_SERVERS': 7, },
        {'ARRIVAL_LAMBDA_RATE': 20, 'SERVICE_RATE': 22,'MAX_SERVERS': 10, },
        {'ARRIVAL_LAMBDA_RATE': 15, 'SERVICE_RATE': 10,'MAX_SERVERS': 15, },
        {'ARRIVAL_LAMBDA_RATE': 25, 'SERVICE_RATE': 22,'MAX_SERVERS': 20, }
        ]
        # count = 0
        methods = ['PureRand','RoundRobin','RandMin']
        seed_collection = [100,200,300,400,500]

        for params in param_combinations:
            # Update the parameters
            self.MAX_SERVERS = params['MAX_SERVERS']
            self.ARRIVAL_LAMBDA_RATE = params['ARRIVAL_LAMBDA_RATE']
            self.SERVICE_RATE = params['SERVICE_RATE']

            # self.reset_simulation()
            # self.initialize()

            # Run the simulation
            for method in methods:
                for element in seed_collection:
                    self.reset_simulation()
                    self.initialize()
                    print("Run: ", self.runCounter)
                    np.random.seed(element)

                    while self.numberOfDepartures < self.NUMBER_OF_CUSTOMERS:
                        event = self.futureEventList.findMin()
                        self.futureEventList.remove(event.key)
                        self.clock = event.key # advance time
                        if event.type == self.type_arrival:
                            q_no = self.load_balancer(method)
                            # print('random number is')
                            # print(method)
                            
                            # print(q_no)
                            self.ProcessArrival(event, q_no)
                        else:
                            self.ProcessDeparture(event)
                    # Print the performance metrics
                    # print("count")
                    self.ReportGeneration(method)
            self.runCounter += 1

        exit(0)
        
if __name__ == "__main__":
    # seed = "123"
    sim = Sim()
    sim.main()
