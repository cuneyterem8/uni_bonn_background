from scapy.all import *
import sys

dst_ip1 = "orval.netsec.seclab-bonn.de"
dst_ip2 = "chimay.netsec.seclab-bonn.de"
dst_port = 22
noanswer = False
plist=[]

def snifffu(pkt):
    global plist
    global noanswer
    noanswer = False
    plist.append(pkt)
    return pkt.show()

def main():
    global plist
    global noanswer
    global dst_ip1
    global dst_ip2

    if(int(input("Please write 1 to sniff orval and 2 to sniff chimay: "))==1):
        dst_ip=dst_ip1
    else:
        dst_ip=dst_ip2

    #global dst_port
    noanswer = False
    plist =[]
    while (noanswer == False):
        print(".", end=" ", flush = True);
        noanswer = True
        sniff(filter=("src host " + dst_ip),timeout = 10, count = 1, prn= snifffu)
        #sniff(filter=("src host " + dst_ip + " and src port "  + str(dst_port)), timeout = 10, count = 1, prn = snifffu)
    wrpcap("save.cap",plist)

main()
