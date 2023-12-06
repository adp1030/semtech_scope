# Importing Libraries 
import serial 
import time 

#Define Arduino
arduino = serial.Serial(port='COM10', baudrate=9600, timeout=.1) 

arduino.write(bytes('0', 'utf-8')) 
time.sleep(0.05) 
data = arduino.readline() 
