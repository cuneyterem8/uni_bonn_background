from scapy.all import *
import time

#sniffURL="mikkeller.netsec.seclab-bonn.de"
sniffURL = "10.3.3.59"
sniffingtime = 5*60

def snifffu(pkt):
    global sniffURL

    if(pkt.haslayer(IP) and (pkt[IP].src == sniffURL or pkt[IP].dst == sniffURL)):
        wrpcap("save.cap", pkt, append=True)
        #pkt.show()

def main():
    global sniffingtime
    ttend = time.time() + sniffingtime

    while (time.time()<ttend):
        sniff(timeout = 10, count = 1, prn= snifffu)

main()
