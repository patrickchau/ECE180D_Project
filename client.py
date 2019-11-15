import socket
import time

def getMAC(interface='wlan0'):
  # Return the MAC address of the specified interface
  try:
    str = open('/sys/class/net/%s/address' %interface).read()
  except:
    str = "00:00:00:00:00:00"
  return str[0:17]

def gestures(): # recognize gestures

def sendPosition(): # collect row and position 

def readServer(): # take in server messages

def contolLED(): # run the LED sequence based on server input

def readIMU(): # take in IMU data and analyze it somehow


def sendMAC(host, port):
  mac_string = 'start' + getMAC()
  client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  client.connect((host, port))
  # delay of 5 seconds before sending first MAC address message
  time.sleep(5)
  client.send(mac_string.encode('utf-8'))

def runClient():
  host = "192.168.4.1"
  port = 8080
  client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  client.connect((host, port))
  sendMAC(host, port) # send over the MAC address for the 
  client.close()
  

if __name__ == "__main__":
  runClient()
