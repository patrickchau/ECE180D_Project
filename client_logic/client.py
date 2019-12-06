import socket
import IMU
import RPi.GPIO as GPIO
import time
import datetime
import math

# static IP of server
host = "192.168.4.1"
port = 8080
_BAUDRATE = 4096
# button to send position to server
_POS_SWITCH = 17
# LED GPIO pins
_LED1 = 18
_LED2 = 19
_LED3 = 20
_LED4 = 21
i_time = datetime.datetime.now() # get start time

closecondition = True
client = None

def getMAC(interface='wlan0'):
  # Return the MAC address of the specified interface
  try:
    str = open('/sys/class/net/%s/address' %interface).read()
  except: 
    str = "00:00:00:00:00:00"
  return str[0:17]

def gestures(): # recognize gestures
  # once we know what walking looks like in terms of accelerations, we can set thresholds
  # a_x, a_y, a_z = readIMU()
  # take the time average of total acceleration and if its higher
  # than the average acceleration of a person sitting, then theyre moving
  # first pull the most recent 50 norms of accel from the file

  return 1

def sendPosition(): # request row and position
  # if a button is pressed, then send a message to server to collect position
  if GPIO.input(_POS_SWITCH) == True:
    sendMessage("position") # TODO: add position option to server receive
  return 1


def readServer(c): # take in server messages
  buffer = None
  c.settimeout(5)  # wait 1 second before throwing an exception
  try:
    buffer = c.recv(_BAUDRATE).decode('utf-8')
    print("Received server message: " + buffer)
  except socket.timeout: 
    print("buffer is empty")
  return buffer

def controlLED(c): # run the LED sequence based on server input
  received = readServer(c)
  if not received is None and "lightUp" in received: # if buffer isn't empty
    print("LEDs high")
    GPIO.output(_LED1, GPIO.HIGH)
    GPIO.output(_LED2, GPIO.HIGH)
    GPIO.output(_LED3, GPIO.HIGH)
    GPIO.output(_LED4, GPIO.HIGH)
    time.sleep(5)
    print("LEDS low")
    GPIO.output(_LED1, GPIO.LOW)
    GPIO.output(_LED2, GPIO.LOW)
    GPIO.output(_LED3, GPIO.LOW)
    GPIO.output(_LED4, GPIO.LOW)
  return 1

def initIMU():
  IMU.detectIMU()
  IMU.initIMU()
  f = open("accel.csv","w+")   # delete and create a new file
  f.write("time," + "ACCx" + "," + "ACCy" + "," + "ACCz" + "," + "ACCnorm" + "\n"  )
  f.close()
  return 1

def readIMU(): # take in IMU data and analyze it somehow
  print("reading IMU")
  ACCx = (IMU.readACCx() * 0.244)/1000 # math for converting raw data to g's
  ACCy = (IMU.readACCy() * 0.244)/1000
  ACCz = (IMU.readACCz() * 0.244)/1000 
  ACC_norm = math.sqrt(math.pow(ACCx, 2) + math.pow(ACCy, 2) + math.pow(ACCz, 2))
  t_stamp = (datetime.datetime.now() - i_time).microseconds/(1000000*1.0)
  f = open("accel.csv", "a+")   # append to file
  f.write(str((t_stamp)) + "," + str(ACCx) + "," + str(ACCy) + "," + str(ACCz) + "," + str(ACC_norm) + "\n"  )
  f.close()
  return (ACCx, ACCy, ACCz, ACC_norm)

def setGPIO():
  GPIO.setmode(GPIO.BCM)
  GPIO.setwarnings(False)
  GPIO.setup(_LED4, GPIO.OUT) # LED pin 1
  GPIO.setup(_LED3, GPIO.OUT) # LED pin 2
  GPIO.setup(_LED2, GPIO.OUT) # LED pin 3
  GPIO.setup(_LED1, GPIO.OUT) # LED pin 4
  GPIO.setup(_POS_SWITCH, GPIO.IN) # Position button input

def sendMessage( type, client ): # send messages to the server
  message = ""
  if type == "start":
    # upon startup send MAC address to the server
    message = "start," + getMAC()
  elif type == "close":
    # send a close message (data field doesn't matter)
    message = "close,a"
  elif type == "position":
    # send the position of the client to the server
    # data field doesn't matter since row, col taken from token
    message = "position," 
  elif type == "nothing":
    message = "nothing, a"
  client.send(message.encode('utf-8'))
  return 1

def checkClose(): # check if we need to close connection, if so set closeCondition true
  readIMU()
  return 1

def runClient():
  print("running")
  # initiate connection to server
  closeCondition = True
  client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  client.connect((host, port))
  time.sleep(5) # allow for time for handshake to complete
  sendMessage("start", client) # send over the MAC address for this pi
  setGPIO() # set all GPIO pins
  initIMU() # detect and start IMU
  print(client.recv(_BAUDRATE).decode('utf-8'))
  while True:
    gestures()
    controlLED(client)
    checkClose()
    sendMessage("nothing", client)
    time.sleep(2) # for readability
    # close condition set when person is walking away(leaving server)
    if(closeCondition):
      print("now closing")
      sendMessage("close", client)
      # wait for return message to ensure server has removed correctly
      succ = client.recv(_BAUDRATE).decode('utf-8')
      print(succ)
      client.close()
      while True:
        print("attempting to reconnect")
        time.sleep(10)
        try:
            print("attempting")
            client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client.connect((host, port))
            time.sleep(5)
            sendMessage("start", client) # send over the MAC address for this pi
            print("Connected")
            closeCondition = False;
            break
        except socket.error:
            time.sleep(10)
            print("did not connect")
  #TODO: allow for reconnection

if __name__ == "__main__":
  runClient()