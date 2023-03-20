import socket
import select
import sys
import ast
import pickle

import Crypto
from Crypto import Random
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP
from Crypto.Signature import PKCS1_PSS
from Crypto.Hash import SHA256

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


IP_address = '127.0.0.1'
Port = 8081

print("")
print("Connecting...\n")

server.connect((IP_address, Port))

print("Connection established! :: IP<" + str(IP_address) + "> Port<" + str(Port) +">\n")

random_generator = Random.new().read
myCryptoKey = RSA.generate(1024, random_generator)
myCryptoPKCS = PKCS1_OAEP.new(myCryptoKey)

random_generator = Random.new().read
mySignatureKey = RSA.generate(1024, random_generator)
mySignaturePKCS = PKCS1_PSS.new(mySignatureKey)
hash = SHA256.new()

server.send(pickle.dumps([myCryptoKey.publickey().exportKey(),mySignatureKey.publickey().exportKey()]))

keyRecieved = False
connectionEnded = False

while connectionEnded == False:
    sockets_list = [sys.stdin, server]

    read_sockets, write_socket, error_socket = select.select(sockets_list, [], [])

    for socks in read_sockets:
        if socks == server:
            message = pickle.loads(socks.recv(2048))

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
                    server.send(pickle.dumps(["exit",'']))
                    print("Connection suspended!\n")
                    connectionEnded = True;
                    break;

        elif keyRecieved == True:
            message = sys.stdin.readline()
            print("")
            if message.strip() != "exit":
                encrypted = hisCryptoPKCS.encrypt(message)
                hash.update(encrypted)
                signature = mySignaturePKCS.sign(hash)
                server.send(pickle.dumps([encrypted,signature]))
            else:
                server.send(pickle.dumps(["exit",'']))
                print("Disconnecting...\n")
                connectionEnded = True;
                break;
