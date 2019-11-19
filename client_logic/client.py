import socket
import time
import berryIMU
import RPi.GPIO as GPIO

# static IP of server
host = "192.168.4.1"
port = 8080
closeCondition = False
_BAUDRATE = 4096
_POS_SWITCH = 17
# LED GPIO pins
_LED1 = 18
_LED2 = 19
_LED3 = 20

def getMAC(interface='wlan0'):
  # Return the MAC address of the specified interface
  try:
    str = open('/sys/class/net/%s/address' %interface).read()
  except: 
    str = "00:00:00:00:00:00"
  return str[0:17]

def gestures(): # recognize gestures
  # once we know what walking looks like in terms of accelerations, we can set thresholds
  return 1

def sendPosition(): # collect row and position 
  # if a button is pressed, then send a message to server to collect position
  if GPIO.input(_POS_SWITCH) == true
    sendMessage("position") # TODO: add position option to server
  return 1


def readServer(c): # take in server messages
  buffer = c.recv(_BAUDRATE).decode('utf-8')
  if not buffer: # a message taken in from server
    print("server sent message")

def contolLED(): # run the LED sequence based on server input
  return 1

def readIMU(): # take in IMU data and analyze it somehow
  ACCx = IMU.readACCx()
  ACCy = IMU.readACCy()
  ACCz = IMU.readACCz() 
  f = open("accel.csv")  
  f.write( str(ACCx) , "," , str(ACCy), "," ,str(ACCz), "\n"  )
  f.close()

def setGPIO():
  GPIO.setmode(GPIO.BCM)
  GPIO.setwarnings(False)
  GPIO.setup(_LED4, GPIO.OUT) # LED pin
  GPIO.setup(_LED3, GPIO.OUT) # LED pin
  GPIO.setup(_LED2, GPIO.OUT) # LED pin
  GPIO.setup(_LED1, GPIO.OUT) # LED pin
  GPIO.setup(_POS_SWITCH, GPIO.IN) # Position button input

def sendMessage( type ): # send messages to the server
  message = ""
  if type == "start":
    # upon startup send MAC address to the server
    message = "start," + getMAC()
  elif type == "close":
    message = "close, "
  # delay of 1 second before sending first MAC address message
  time.sleep(1)
  client.send(mac_string.encode('utf-8'))
  return 1

def runClient():
  # initiate connection to server
  client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  client.connect((host, port))
  sendMessage("start") # send over the MAC address for this pi
  setGPIO()
  while True:
    readIMU()
    controlLED()
    readServer(client)
    # close condition set when person is walking away(leaving server)
    if(closeCondition):
      sendMessage("close")
      # wait for return message to ensure server has removed correctly
      succ = client.recv(_BAUDRATE).decode('utf-8')
      print(succ)
      client.close()
      break

if __name__ == "__main__":
  runClient()
