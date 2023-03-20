

from scapy.all import *

IFACE = 'Wi-Fi'

pktsListUdp=sniff(iface=IFACE, filter='udp', count=10)
wrpcap('udp.pcap',pktsListUdp)

pktsListHttp=sniff(iface=IFACE, filter="tcp port 80", count=10)
wrpcap('http.pcap',pktsListHttp)

pktsListTcp=sniff(iface=IFACE,  lfilter=lambda x: x.haslayer(TCP), count=10)
wrpcap('tcp.pcap',pktsListTcp)

pktsListDns=sniff(iface=IFACE,  lfilter=lambda z: z.haslayer(DNS) and z.getlayer(DNS).qr==0, count=10) 
wrpcap('dns.pcap',pktsListDns)

pktsListNot=sniff(iface=IFACE, filter='not (udp and tcp and dns)', count=10)
wrpcap('not.pcap',pktsListNot)

print("ending")




