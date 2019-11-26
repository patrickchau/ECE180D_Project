#!/usr/local/bin/python3.7
# each client must have an IP, MAC, row, col associated
class client:
    def __init__(self):
        self.macId=None
        self.ipAddr = None
        self.row=None
        self.col=None

from collections import defaultdict
import socket
from _thread import *
import threading
import time

clientList = []
macToClient=defaultdict(client) # map for mac->client
macToId=defaultdict(int)        # mac to client ID
print_lock=threading.Lock()     # print locks
addLock=threading.Lock()        # add client locks


# linked list variables - maybe move to own object?
size=0
id=0
dummyHead=client()
dummyHead.next=dummyHead
dummyHead.prev=dummyHead
tail=dummyHead
startPointer=dummyHead

global lightUp=[]   # list of MACs for clients to light up
interval=1

def each_client(c):
    global macToClient
    global macToId
    global size
    global id
    global dummyHead
    global tail
    macId=None
    while True:
        buffer=None
        c.settimeout(10) #timeout 10 secs for now,client need to constantly send msg
        try:
            buffer=c.recv(4096).decode('utf-8')
            print("received message: " + buffer)
        except socket.timeout:
            print_lock.acquire()
            print("time out occured on ",c)
            print_lock.release()
            # if timeout, then close the connection to client
            if macId:
                target=macToClient[macId]
                addLock.acquire()
                target.prev.next=target.next
                target.next.prev=target.prev
                del macToClient[macId]
                size-=1
                lightUp.remove(macId)
                addLock.release()
                data="closed"
                c.send(data.encode('utf-8'))
                break
        
        # Block to parse all possible client inputs
        _type,msg=buffer.split(",") #assume client sends in this format
        print("type: " + _type)
        if _type=="start":
            print("this is a start message!")
            if macId:
                data="client already exists"
                c.send(data.encode('utf-8'))
                continue
            # print locks
            print_lock.acquire()
            print("add a new client ",c)
            print_lock.release()

            # create new client to macID -> client map
            macId=msg
            newClient=client()
            newClient.macId=msg

            # update the linked list for snake
            addLock.acquire()
            newClient.prev=tail
            tail.next=newClient
            newClient.next=dummyHead
            dummyHead.prev=newClient
            tail=newClient
            macToClient[macId]=newClient
            size+=1
            id+=1
            macToId[macId]=id
            lightUp.append(macId) # this adds new client to light up
            addLock.release()

            # let client know that it is now registered
            data="registered"
            c.send(data.encode('utf-8'))
        elif _type=="close":
            # if client sends close request separate from socket timeout
            print("this is a closing message")
            print("mac id: " + macId)
            target=macToClient[macId]
            addLock.acquire()
            if not target.next:
                target.prev.next=target.next
            if not target.prev:
                target.next.prev=target.prev
            del macToClient[macId]
            del macToId[macId]
            size-=1
            lightUp.remove(macId)
            addLock.release()
            data="closed"
            c.send(data.encode('utf-8'))
            break
        elif _type=="position": # position message of format "position, row.col"
            print("now getting row and column from hardware token")
            r,c = msg.split('.')
            macToClient[macId].row = r
            macToClient[macId].col = c
        # block to send data to client
        if macId in lightUp:
            data="lightUp " + macId
            c.send(data.encode('utf-8'))
        else:
            data = "nothing"
            c.send(data.encode('utf-8'))

    # if we break from the while True, close the client connection.
    # this should only happen when we send a close statement
    c.close()

def lookForConnection(serv):
    while True:
        c,addr=serv.accept()
        print_lock.acquire()
        print("connection established")
        print_lock.release()
        start_new_thread(each_client,(c,))
    serv.close()
    
def loopThrough():
    # set the global variable lightup to be desired client
    # lit up
    # global lightUp = []
    while True:
        print("Clients: " lightUp)
        sleep(5)

    """
    global lightUp
    global interval
    global startPointer
    global dummyHead
    while size>1:
        time.delay(5) #lets move the snake every 5 secs
        lightUp=[]
        steps=interval
        loopPointer=startPointer
        addLock.acquire()
        while steps>0:
            if loopPointer!=dummyHead:
                lightUp.append(loopPointer.macId)
                steps-=1
            loopPointer=loopPointer.next
        addLock.release()
    """

def runServer():
    host="192.168.4.1"
    serv=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    serv.bind((host,8080))
    serv.listen(10) #in case it takes time to acquire lock 

    t1 = threading.Thread(target=lookForConnection, args=(serv,)) 
    t2 = threading.Thread(target=loopThrough, ) 
  
    t1.start() 
    t2.start()
    
if __name__ == "__main__":
    runServer()


