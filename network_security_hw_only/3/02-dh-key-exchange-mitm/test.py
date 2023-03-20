import socket
import math
from Crypto.Cipher import Salsa20

alvURL = "alvinne.netsec.seclab-bonn.de"
alvport = 1976
dhalv="dhalv.json"
dhmalv="dhmalv.json"
encralv1 = "encralv1.txt"
plainalv1 = "plainalv1.txt"
compstralv = "public_alvinne\": \""
encralv2 = "encralv2.txt"
plainalv2 = "plainalv2.txt"
messforalv1 = "messageforalv1.txt"

fouURL = "founders.netsec.seclab-bonn.de"
fouport = 1969
dhfou="dhfou.json"
dhmfou = "dhmfou.json"
encrfou1 = "encrfou1.txt"
plainfou1 = "plainfou1.txt"
compstrfou = "public_founders\": \""
encrfou2 = "encrfou2.txt"
plainfou2 = "plainfou2.txt"
messforfou1 = "messageforfou1.txt"

def recvfile(soc, filename):
    with open(filename, 'wb') as f:
        data = soc.recv(2048)
        print(data)
        f.write(data)


def moderator():
    global alvURL
    global alvport
    global dhalv
    global dhmalv
    global encralv1
    global plainalv1
    global messforfou1
    global encralv2
    global plainalv2
    global compstralv

    global fouURL
    global fouport
    global dhfou
    global dhmfou
    global encrfou1
    global plainfou1
    global messforalv1
    global encrfou2
    global plainfou2
    global compstrfou

    print("starting a talk with alvinne")

    salv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print("- establishing the connection to alvinne")
    salv.connect((alvURL, alvport))
    print("- receiving a dh key exchange file from alvinne")
    recvfile(salv,dhalv)
    print("- sending a dh key exchange file to alvinne")
    with open(dhmalv, 'rb') as f:
        salv.sendfile(f)

    print("- receiving the secret file from alvinne")
    recvfile(salv,encralv1)


    print("")
    print("decrpyting the encoded message of alvinne")
    decrypt(encralv1, dhalv, plainalv1, compstralv)


    print("")
    print("starting a talk with founders")

    sfou = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print("- establishing a connection to founders")
    sfou.connect((fouURL,fouport))
    print("- sending a dh key exchange file to founders")
    with open(dhmfou, 'rb') as f:
        sfou.sendfile(f)
    print("- receiving a dh key change file from founders")
    recvfile(sfou, dhfou)


    print("")
    print("encrypting the decrypted message of alvinne")
    encrypt(plainalv1, dhfou, messforfou1, compstrfou)


    print("sending the for founders encrypted message from alvinne")
    with open(messforfou1, 'rb') as f:
        sfou.sendfile(f)
    print("- receiving a message from founders")
    recvfile(sfou, encrfou1)

    
    print("")
    print("decrypting the encoded message of founders")
    decrypt(encrfou1, dhfou, plainfou1, compstrfou)


    for i in range(10):
        print("")
        print("encrypting the decrypted message of founders")
        encrypt(plainfou1, dhalv, messforalv1, compstralv)


        print("")
        print("Continuing talk with alvinne")
        with open(messforalv1, 'rb') as f:
            salv.sendfile(f)
        print("- receiving message from alvinne")
        recvfile(salv, encralv2)


        print("")
        print("decrypting the encoded message of alvinne")
        decrypt(encralv2, dhalv, plainalv2, compstralv)


        print("")
        print("encrypting the decrypted message from alvinne")
        encrypt(plainalv2, dhfou, messforfou1, compstrfou)
    

        print("")
        print("Continuing to talk with founders")
        with open(messforfou1, 'rb') as f:
            sfou.sendfile(f)
        print("- receiving message from founders")
        recvfile(sfou, encrfou2)


        print("")
        print("decrypting the encoded message of founders")
        decrypt(encrfou2, dhfou, plainfou1, compstrfou)

    


    print("")
    print("shutting down the connection with alvinne")
    salv.shutdown(socket.SHUT_WR)
    print("closing the socket")
    salv.close()
    print("")
    print("shutiing down the connection with founders")
    sfou.shutdown(socket.SHUT_WR)
    print("closing the socket")
    sfou.close()

#in my opinion this code form the mail is wrong, it does the same as [:32] - to pad correctly there should be max(32, bytes_needed(shared_secret)) not just bytes_needed(shared_secret) in get_salsa_key
#but as every key we get from alvinne and founders is normally long enough this wont play a role here...
def bytes_needed(n):
    if n==0:
        return 1
    return int(math.log(n,256))+1

def get_salsa_key(shared_secret):
    return shared_secret.to_bytes(max(bytes_needed(shared_secret),32), 'big')


#note: as we send 2=g as public key, our private key is 1, hence the exchanged secret key is the public key of alvinne/founders, they have sent
def calckey(filename,compstring):
    with open(filename, 'r') as f:
        stuff = f.read(2048)
    
    for i in range (0,len(stuff)):
        if stuff[i:i+len(compstring)] == compstring:
            stuff = stuff[i+len(compstring):len(stuff)-3]
            break
    #stuff = stuff.encode('utf-8')
    print("- the exchanged key is (ascii encoded):")    
    print(stuff)
    print("- the exchanged key is (int encoded):")
    stuff = int(stuff,16)
    print(stuff)
    print("- the exchanged key is (completly in hex):")
    stuff = get_salsa_key(stuff)
    print(stuff)
    return stuff


def decrypt(cifina, keyfina, plfina, compstring):
    print("- reading the encrypted secret out of the file")
    with open(cifina, 'rb') as f:
        m = f.read(2048)
    nonce = m[:8]
    print("- the nonce is:")
    print(nonce)
    ciphertext = m[8:]
    print("- the ciphertext is:")
    print(ciphertext)
    print("- calculating the exchanged key")
    key = calckey(keyfina, compstring)
    print("- the shortened key is:")
    key = key[:32]
    print(key)
    cipher = Salsa20.new(key, nonce)
    print("- decrypting the plaintext")
    plaintext = cipher.decrypt(ciphertext)
    print("- the plaintext is:")
    print(plaintext)
    print("")
    with open(plfina, 'wb') as f:
        f.write(plaintext)

def encrypt(plfina, keyfina, cifina, compstring):

    print("- reading plaintext of the encrypted message sent by alvinne")
    with open(plfina, 'rb') as f:
        plaintext = f.read(2048)
    
    print("- calculating the exchanged key")
    key = calckey(keyfina, compstring)
    nonce = "01234567".encode('utf-8')
    #print(len(nonce))
    print("- the shortened key is:")
    key = key[:32]
    print(key)
    cipher = Salsa20.new(key, nonce)
    print("- encrypting the plaintext")
    ciphertext = nonce + cipher.encrypt(plaintext)
    print("- the encrypted plaintext is:")
    print(ciphertext)
    with open(cifina, 'wb') as f:
        f.write(ciphertext)

def main():
    moderator()

main()

