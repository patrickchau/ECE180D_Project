import socket
import time

#get mac address
def getMAC(interface='wlan0'):
  # Return the MAC address of the specified interface
  try:
    str = open('/sys/class/net/%s/address' %interface).read()
  except:
    str = "00:00:00:00:00:00"
  return str[0:17]

mac_string = 'start' + getMAC()

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(('192.168.4.1', 8080))
client.send(mac_string.encode('utf-8'))
time.sleep(5)
client.send(mac_string.encode('utf-8'))
client.close()

