# -*- coding: utf-8 -*-
"""
Created on Tue Oct 18 22:16:03 2022

@author: wiktor
"""

# sunpos.py


import math
import time
import serial
from datetime import datetime
import signal



stnow = datetime.now()
#arduino = serial.Serial(port='COM4', baudrate=460800, timeout=.1)

def handler(signum, frame):
    print("Ctrl-c was pressed. Do you really want to exit? y/n ")
    arduino.close()
    exit(1)

'''
def writeSerial(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.5)
    print(arduino.readline())
'''

def sunpos(when, location, refraction):
    
    
  # Extract the passed data
    year, month, day, hour, minute, second, timezone = when
    latitude, longitude = location
    
  # Math typing shortcuts
    rad, deg = math.radians, math.degrees
    sin, cos, tan = math.sin, math.cos, math.tan
    asin, atan2 = math.asin, math.atan2
    
  # Convert latitude and longitude to radians
    rlat = rad(latitude)
    rlon = rad(longitude)
    
  # Decimal hour of the day at Greenwich
    greenwichtime = hour - timezone + minute / 60 + second / 3600
    
  # Days from J2000, accurate from 1901 to 2099
    daynum = (
        367 * year
        - 7 * (year + (month + 9) // 12) // 4
        + 275 * month // 9
        + day
        - 730531.5
        + greenwichtime / 24
    )
    
  # Mean longitude of the sun
    mean_long = daynum * 0.01720279239 + 4.894967873
    
  # Mean anomaly of the Sun
    mean_anom = daynum * 0.01720197034 + 6.240040768
    
  # Ecliptic longitude of the sun
    eclip_long = (
        mean_long
        + 0.03342305518 * sin(mean_anom)
        + 0.0003490658504 * sin(2 * mean_anom)
    )
    
  # Obliquity of the ecliptic
    obliquity = 0.4090877234 - 0.000000006981317008 * daynum
    
  # Right ascension of the sun
    rasc = atan2(cos(obliquity) * sin(eclip_long), cos(eclip_long))
    
  # Declination of the sun
    decl = asin(sin(obliquity) * sin(eclip_long))
    
  # Local sidereal time
    sidereal = 4.894961213 + 6.300388099 * daynum + rlon
    
  # Hour angle of the sun
    hour_ang = sidereal - rasc
    
  # Local elevation of the sun
    elevation = asin(sin(decl) * sin(rlat) + cos(decl) * cos(rlat) * cos(hour_ang))
    
  # Local azimuth of the sun
    azimuth = atan2(
        -cos(decl) * cos(rlat) * sin(hour_ang),
        sin(decl) - sin(rlat) * sin(elevation),
    )
    
  # Convert azimuth and elevation to degrees
    azimuth = into_range(deg(azimuth), 0, 360)
    elevation = into_range(deg(elevation), -180, 180)
    
  # Refraction correction (optional)
    if refraction:
        targ = rad((elevation + (10.3 / (elevation + 5.11))))
        elevation += (1.02 / tan(targ)) / 60
        
   # Return azimuth and elevation in degrees
    return (round(azimuth, 2), round(elevation, 2))


def into_range(x, range_min, range_max):
    shiftedx = x - range_min
    delta = range_max - range_min
    return (((shiftedx % delta) + delta) % delta) + range_min


if __name__ == "__main__":
        
    '''
    while(1):
        tmp = arduino.readline()
        print(tmp)
        if (tmp == b'Magnes OK'):
            print("system ok")
            break
        else:
            time.sleep(2)
    '''     

    location = (53.4, 14.5072)
    t = time.localtime()
    hour = 10
    minu = 45
    day = 10
    mon = 10
    
    when = (2022, mon, day, hour, minu, 0, 1)
    azimuth, elevation = sunpos(when, location, True)
    #elevation = 90.0 - elevation
    startaz = azimuth
    startelv = elevation
    toSend = str(int(azimuth / 0.087)) + ","  + str(int(elevation + 20))
    print("--------------------------------------")
    print("------------STARTING------------------")
    print("day: ",day, " month:",  mon, " hour:", hour, ":",minu)

    print(datetime.now() - stnow)
    print("Azimuth: ", azimuth)
    print("Elevation: ", elevation)
    print(" ")
    print("--------------------------------------")
    print("--------------------------------------")
    print("--------------------------------------")
    '''
    while(1):
        writeSerial(toSend)
        time.sleep(2)
        tmp = arduino.readline()
        print(tmp)
        if (tmp == b'R1'):
            break      
        signal.signal(signal.SIGINT, handler)
        
    while(1):
        time.sleep(2)
        tmp = arduino.readline()
        print(tmp)
        if (tmp == b'R2'):
            break     
        signal.signal(signal.SIGINT, handler)
    '''
    minu = 45
    hour = 10
    while(1):
        t = time.localtime()
        hour = hour
        minu = minu
        day = 10
        mon = 10
      # Close Encounters latitude, longitude
        location = (53.4, 14.5072)
        
      # Fourth of July, 2022 at 11:20 am MDT (-6 hours)
        when = (2022, mon, day, hour, minu, 0, 1)
        
      # Get the Sun's apparent location in the sky
        azimuth, elevation = sunpos(when, location, True)
        #elevation = 90.0 - elevation
      # Output the results
        #print("\nWhen: ", when)
        #print("Where: ", location)
        print("day:",day, "month:",  mon, "hour:", hour, ":",minu)

        print("Runnung time: 01:25:20:099970")
        print("Azimuth:", azimuth)
        print("Elevation:", elevation)
        print("diff form start: azimuth=", round(azimuth-startaz,2), " elevation=", round(elevation-startelv,2))
             
        if elevation > 0:
            toSend = str(int(azimuth / 0.087)) + ","  + str(int(elevation + 20))
        else:
            when = (2022, 7, day, 6, 0, 0, 1)
            azimuth, elevation = sunpos(when, location, True)
            toSend = str(int(azimuth / 0.087)) + ","  + str(int(elevation + 20))
            time.sleep(1)
        print("Message to send:",toSend)

        #writeSerial(toSend)
     
        

        minu = minu + 10
        if minu >= 60:
            minu = 0
            hour = hour + 1
        '''
        if hour == 24:
            hour = 0
            day= day + 1
        '''
        time.sleep(2)
        print ("-----Data received correctly") 
        signal.signal(signal.SIGINT, handler)
        print ("----------------------------------")   
    
# When:  (2022, 7, 4, 11, 20, 0, -6)
# Where:  (40.602778, -104.741667)
# Azimuth:  121.38
# Elevation:  61.91