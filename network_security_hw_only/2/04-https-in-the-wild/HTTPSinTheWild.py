from scapy.all import *
import csv

#working:
    #Reading URL's from Excel File
    #Handshake
    #HTTP get
#notWorking:
    #Differentiating between HTTP and HTTPS // Can't figure out how to get the actual TCP-Payload
    #Part2

with open('top-1m.csv', 'r') as f:
    reader = csv.reader(f)
    list = list(reader)

for x in range(1):
#for x in range(len(list)):
    print(list[x][1])

    dest = list[x][1]

    #Look for HTTP
    dest_port = 80

    #Create SYN
    syn = IP(dst=dest) / TCP(sport=random.randint(1024,65535), dport=dest_port, flags='S')

    #Send SYN and get SYNACK
    syn_ack = sr1(syn)
    print("SYN send to "+list[x][1])
    print("SYNACK recieved from "+list[x][1])

    #Send ACK
    out_ack = send(IP(dst=dest) / TCP(dport=dest_port, sport=syn_ack[TCP].dport,seq=syn_ack[TCP].ack + 1, ack=syn_ack[TCP].seq + 1, flags='A'))
    print("ACK send to "+list[x][1])

    #Send the HTTP
    getStr = 'GET / HTTP/1.1\r\nHost:' + dest + '\r\nAccept-Encoding: gzip, deflate\r\n\r\n'
    packet = sr1(IP(dst=dest) / TCP(dport=dest_port, sport=syn_ack[TCP].dport,seq=syn_ack[TCP].ack, ack=syn_ack[TCP].seq, flags='A') / getStr)
    print("HTTP GET send to "+list[x][1])
    packet.show()

    #Look for HTTPS
    dest_port = 443

    #Create SYN
    syn = IP(dst=dest) / TCP(sport=random.randint(1024,65535), dport=dest_port, flags='S')

    #Send SYN and get SYNACK
    syn_ack = sr1(syn)
    print("SYN send to "+list[x][1])
    print("SYNACK recieved from "+list[x][1])

    #Send ACK
    out_ack = send(IP(dst=dest) / TCP(dport=dest_port, sport=syn_ack[TCP].dport,seq=syn_ack[TCP].ack + 1, ack=syn_ack[TCP].seq + 1, flags='A'))
    print("ACK send to "+list[x][1])

    #Send the HTTP
    getStr = 'GET / HTTP/1.1\r\nHost:' + dest + '\r\nAccept-Encoding: gzip, deflate\r\n\r\n'
    packets = sr(IP(dst=dest) / TCP(dport=dest_port, sport=syn_ack[TCP].dport,seq=syn_ack[TCP].ack, ack=syn_ack[TCP].seq, flags='A') / getStr, multi=1, timeout=1)
    print("HTTP GET send to "+list[x][1])
    #packet.show()
    for p in packets:
        p.show()
