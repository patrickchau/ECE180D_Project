class client:
    def __init__(self):
        self.macId=None
        self.row=None
        self.col=None
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
mode=0 #0 for snake, 1 for 'U'
hwUp=False
lastPerson=None

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

        type,msg=buffer.split(',') #assume client sends in this format
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
            newClient.macId=macId
            
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
            lastPerson=newClient
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
        elif type=="hw":
            row,col=msg.split(".")
            lastPerson.col=col
            lastPerson.row=row
            key=lastPerson.macId
            addLock.acquire()
            macToClient[key]=lastPerson
            addLock.release()
            data="hw_ack"
            c.send(data)
        if macId in lightUp:
            data="lightUp"
            c.send(data)
        else:
            data="turn off"
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
    global lightUp
    global interval
    global startPointer
    global dummyHead
    global mode
    time.sleep(5) 
    lightUp=[]
    if mode==0:
        while size>1:            
            steps=interval
            loopPointer=startPointer
            addLock.acquire()
            while steps>0:
                if loopPointer!=dummyHead:
                    lightUp.append(loopPointer.macId)
                    #you can print stuff like row, and col here, eg.
                    print(str(loopPointer.row)+" "+str(loopPointer.col)) 
                    steps-=1
                loopPointer=loopPointer.next
            startPointer+=1
            addLock.release()
    else:
        clientList=[macToClient[key] for key in macToClient]
        rowList=sorted(clientList,key=lambda x:x.row)
        colList=sorted(clientList,key=lambda x:x.col)
        leftBound=colList[0].col
        rightBound=colList[size-1].col
        upBound=rowList[size-1].row
        lowBound=rowList[0].row
    if mode==1 and size>5:
        r1,r2,r3,r4,r5=[],[],[],[],[]
        for client in clientList:
            r,c=client.row,client.col
            if ( c<=leftBound+(rightBound-leftBound)/3 and r>lowBound+(upBound-lowBound)/3 ):
                r1.append(client)
            if ( c<=leftBound+(rightBound-leftBound)/3 and r<=lowBound+(upBound-lowBound)/3 ):
                r2.append(client)
            if ( ( c>leftBound+(rightBound-leftBound)/3 and c<=leftBound+2*(rightBound-leftBound)/3 ) and ( r<=lowBound+(upBound-lowBound)/3 ) ):
                r3.append(client)
            if ( c> leftBound + 2*(rightBound-leftBound)/3 and r<=lowBound+(upBound-lowBound)/3 ):
                r4.append(client)
            if ( c>=leftBound + 2*(rightBound-leftBound)/3 and r>lowBound+(upBound-lowBound)/3 ):
                r5.append(client)
        if r1 and r2 and r3 and r4 and r5:
            LightUp=r1+r2+r3+r4+r5

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

