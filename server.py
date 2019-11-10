class client:
    def __init__(self):
        self.macId=None
        self.next=None
        self.prev=None

from collections import defaultdict
import socket
from _thread import *
import threading
import time
macToClient=defaultdict(client)
macToId=defaultdict(int)
print_lock=threading.Lock()
addLock=threading.Lock()
size=0
id=0
dummyHead=client()
dummyHead.next=dummyHead
dummyHead.prev=dummyHead
tail=dummyHead
startPointer=dummyHead
lightUp=[]
interval=1

def each_client(c):
    macId=None
    while True:
        buffer=None
        c.settimeout(10) #timeout 10 secs for now,client need to constantly send msg
        try:
            buffer=c.recv(4096)
        except socket.timeout:
            print_lock.acquire()
            print("time out occured on ",c)
            print_lock.release()
            if macId:
                target=macToClient[macId]
                addLock.acquire()
                target.prev.next=target.next
                target.next.prev=target.prev
                del macToClient[macId]
                size-=1

                addLock.release()
                data="closed"
                c.send(data)
                break
        
        type,msg=buffer.split(",") #assume clinet sends in this format
        if type=="start":
            if macId:
                data="client already exist"
                c.send(data)
                continue
            print_lock.acquire()
            print("add a new client ",c)
            print_lock.release()
            
            macId=msg
            newClient=client()
            newClient.macId=msg

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
            addLock.release()
            data="registered"
            c.send(data)
        elif type=="close":
            target=macToClient[macId]
            addLock.acquire()
            target.prev.next=target.next
            target.next.prev=target.prev
            del macToClient[macId]
            del macToId[macId]
            size-=1

            addLock.release()
            data="closed"
            c.send(data)
            break
        if macId in lightUp:
            data="lightUp"
            c.send(data)
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

def runServer():
    host=""
    serv=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    serv.bind((host,12345))
    serv.listen(10) #in case it takes time to acquire lock 

    t1 = threading.Thread(target=lookForConnection, args=(serv,)) 
    t2 = threading.Thread(target=loopThrough, ) 
  
    t1.start() 
    t2.start()
    
if __name__ == "__main__":
    runServer()

