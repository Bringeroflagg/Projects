from email import header
import random, sys, math, os
import tkinter as tk
import pip._vendor.requests as requests
import time
import json
from datetime import datetime

URL = "https://api.openweathermap.org/data/2.5/weather?q="
API_KEY = "INSERT_API_KEY_HERE"
KELVIN_TEMP = 273.15

def set_background(type1):
    if type1 == "Cloudy":
        pass
    pass

def request_url(st1):
    return URL + st1 + "&appid=" + API_KEY

# When random city bottom is cliced choose a city and gets it content to display
def random_city():
    # Search by id?=randomnum
    random_cities = ["london","new_york","delhi","vancouver","madrid","beijing","paris","sao_paolo","tokyo","capetown","munich"]
    
    return ( random.choice( random_cities ) )

# might not be able to print the png in terminal therefore have to print in GUI
def get_icon(temp):

    # Send request for the icon image
    icon_url = "http://openweathermap.org/img/wn/" + str( temp ) + "@2x.png"
    response_icon = requests.get( icon_url )
    #res_dict = json.loads(response_icon.text)
    #print(res_dict)
    #print(str(response_icon.content))
    #print(response_icon.content)
    return response_icon

def print_dict(dict1):

    response_data = dict1.json()
    for k,v in response_data.items():
        print( "{}:{}".format( k, v ) )

def turn_time(date1):
    return datetime.fromtimestamp( date1 )

def main():

    request_url_main = request_url( "Vancouver" )
    res = requests.get( request_url_main )

    # Check for succesful reponse
    if not res:
        print( "Error - Invalid City Name -> Status Code: {}".format( res.status_code ) )
        #print(res.status_code)

    else:
        response_data = res.json()
        print(response_data)
        forecast = response_data['weather'][0]['main']
        set_background(forecast)
        weather_description = response_data['weather'][0]['description']
        icon_pic = get_icon(response_data['weather'][0]['icon'])
        humidity = response_data['main']['humidity']
        temperature = response_data['main']['temp'] - KELVIN_TEMP
        sunrise = turn_time(response_data['sys']['sunrise'])
        sunset = turn_time(response_data['sys']['sunset'])
        wind_speed = response_data['wind']['speed']
        feels_like = response_data['main']['feels_like'] - KELVIN_TEMP

        print( "Outside Description : {}".format(weather_description) )
        print( "Wind Speed: {} km/h\nFeels Like: {:.2f}°".format( wind_speed,feels_like ) )
        print( "Humidity: {}%\nTemperature: {:.2f}°C\nSunrise: {} AM\nSunset: {} PM".format( humidity, temperature, sunrise, sunset ) )

    # sunrise and sunset how to decifier
    #print(res.url)
    #print(res.headers)

#weather for city - date - time

if __name__ == "__main__":
    main()
