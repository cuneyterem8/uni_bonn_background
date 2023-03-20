import socket
import websocket
import hashlib
import time

serverURL = "night-shift.netsec.seclab-bonn.de"
servertktport = 80
serverwsport = "5000"
sstring = "1695fc804cdb7b73a715269a1ec5d0b78b7b12c0"
mjson = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJncm91cC1pZCI6IjAwMDAxIiwiaWF0IjoxNTE2MjM5MDIyfQ.aUQB-sj_DF6Mw9B8Lk9t5Z2gs8IL6C9foATpfPhMxOg"
#derive this by using https://jwt.io/
grpjson = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJncm91cC1pZCI6IjAwMDExIiwiaWF0IjoxNTE2MjM5MDIyfQ.m4PiE0pK5qvxZMzQrom0OKxQucgJdsLPUTuu3yasUlg"
mtest = "mantratest.txt"
grptest = "grouptest.txt"
msign = "mantrasignature.txt"
grpsign = "group11signature.txt"

def getsig1(filename, token):
    global serverURL
    global servertktport 
    with open(filename, "w") as f:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((serverURL, servertktport))
            s.send(("{\"ticket\":\"" + token + "\"}").encode('utf-8'))
            answer = s.recv(1024)
            f.write(answer.decode('utf-8'))

def getsig2():
    print("test")

def getsecr(filename):
    global serverURL
    ws = websocket.WebSocket()
    ws.connect("ws://" + serverURL + ":" + serverwsport)
    with open(filename, "r") as f:
        signature = f.read()
    ws.send(signature)
    answer = ws.recv()
    hashgen = hashlib.md5()
    hashgen.update(answer)
    hashvalue = hashgen.hexdigit()
    ws.send(hashvalue)
    secret = ws.recv()
    print("The secret is:\n")
    print(secret)
    ws.close()

def mantrasig1():
    global mtest
    global mjson
    getsig1(mtest, mjson)

def mantrasig2():
    print("test")

def mantraws():
    global msign
    getsecr(msign)

def groupsig1():
    global grptest
    global grpjson
    getsig1(grptest, grpjson)

def groupsig2():
    print("test")
    

def groupws():
    global grpsign
    getsecr(grpsign)

def main():
   mantrasig1()
   #mantrasig2()
   #mantraws()
   groupsig1()
   #groupsig2()
   #groupws()

main()
