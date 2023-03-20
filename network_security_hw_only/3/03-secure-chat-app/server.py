import sys
import socket
import ast
import pickle
from threading import Thread

import Crypto
from Crypto.PublicKey import RSA
from Crypto import Random
from Crypto.Cipher import PKCS1_OAEP
from Crypto.Signature import PKCS1_PSS
from Crypto.Hash import SHA256

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

IP_address = '127.0.0.1'
Port = 8081

server.bind((IP_address, Port))
server.listen(1)

random_generator = Random.new().read
myCryptoKey = RSA.generate(1024, random_generator)
myCryptoPKCS = PKCS1_OAEP.new(myCryptoKey)

random_generator = Random.new().read
mySignatureKey = RSA.generate(1024, random_generator)
mySignaturePKCS = PKCS1_PSS.new(mySignatureKey)
hash = SHA256.new()

connectionFound = False
keyRecieved = False

print("")
print("Waiting for connection...\n")

def userinput(conn,hisCryptoPKCS):
    while keyRecieved == True:
        message = sys.stdin.readline()
        if keyRecieved: print("")
        if message.strip() != "exit":
            encrypted = hisCryptoPKCS.encrypt(message)
            hash.update(encrypted)
            signature = mySignaturePKCS.sign(hash)
            conn.send(pickle.dumps([encrypted,signature]))
        else:
            print("Forwarding suspension...\n")
            conn.send(pickle.dumps(["exit",'']))

while True:

    if connectionFound == False:
        conn, addr = server.accept()
        conn.send(pickle.dumps([myCryptoKey.publickey().exportKey(),mySignatureKey.publickey().exportKey()]))
        connectionFound = True
        print("Connection established! :: IP<" + str(addr[0]) + "> Port<" + str(Port) + ">\n")

    else:
        message = pickle.loads(conn.recv(2048))

        if keyRecieved == False:
            try:
                hisCryptoKey = RSA.importKey(message[0])
                hisCryptoPKCS = PKCS1_OAEP.new(hisCryptoKey)
                print("Encryption-Key recieved!\n")
                print(message[0]+"\n")

                hisSignatureKey = RSA.importKey(message[1])
                hisSignaturePKCS = PKCS1_PSS.new(hisSignatureKey)
                print("Signature-Key recieved!\n")
                print(message[1]+"\n")

                keyRecieved = True

                print("Messages will be encrypted with PKCS#1 v1.5 OAEP\nand signed with PKCS#1 PSS RSA using SHA256.\n\nType in exit to close the application.\n")
                print("--------------------------------------\n")

                thread = Thread(target = userinput, args = (conn,hisCryptoPKCS))
                thread.start()
            except:
                pass

        else:
            if message[0].strip() != "exit":
                hash.update(message[0])
                if hisSignaturePKCS.verify(hash,message[1]) == True:
                    decrypted = myCryptoPKCS.decrypt(message[0])
                    if decrypted:
                        print("<" + str(IP_address) + "> " + decrypted)
                else:
                    print("Message recieved with a signatur that is not authentic!!!\n")
            else:
                print("Connection suspended! Press Enter to close...")
                keyRecieved = False;
                thread.join()
                break;

conn.close()
server.close()
