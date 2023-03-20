# Packet Capturing (2 points)
Write a [Scapy](https://scapy.net/) script which captures traffic and writes the captured packets to five different pcap files. The following table lists the pcaps to create and what type of traffic they should contain:

| Pcap Name | Traffic Type |
| --------- | ------------ |
| http.pcap | HTTP traffic only |
| dns.pcap | DNS traffic only |
| tcp.pcap | all TCP traffic (except for HTTP) |
| udp.pcap | all UDP traffic |
| rest.pcap | everything that is not in one of the above pcaps |

ans:
def pcap_handler(packet):
	if packet.haslayer(TCP) and (packet[TCP].dport == 80 or packet[TCP].sport == 80):
		wrpcap('http.pcap', packet, appent=True)
	elif packet.haslayer(TCP):
		wrpcap('tcp.pcap', packet, appent=True)
	elif packet.haslayer(DNS):
		wrpcap('dns.pcap', packet, appent=True)
		wrpcap('udp.pcap', packet, appent=True)
	elif packet.haslayer(UDP):
		wrpcap('udp.pcap', packet, appent=True)
	else:
		wrpcap('rest.pcap', packet, appent=True)



After working with Scapy you are supposed to use two of the most prominent capturing tools: [tcpdump](https://www.tcpdump.org/) and [tshark](https://www.wireshark.org/docs/man-pages/tshark.html). Use both tools to capture HTTP and HTTPS traffic. The following tables lists the pcaps you are supposed to create:

| Pcap Name | Content |
| --------- | ------- |
| web-tcpdump.pcap | HTTP and HTTPS traffic captured with tcpdump |
| web-tshark.pcap | HTTP and HTTPS traffic captured with tshark |

ans:
sudo tcpdump -i any -s0 -w web-tcpdump.pcap port 80 or port 443
tshark -i any -f "tcp port 80 or tcp port 443" -w web-tshark.pcap


Finally, there are multiple ways to filter HTTP and HTTPS traffic using tshark. Read them up and briefly discuss the differences and possible advantages/drawbacks.

ans:
capture filters pcap filter: cannot be modified during the capture
display filter: can be modified
read filter: can be specified during capturing or reading from captured file


To solve the task submit:
 - all of the pcaps listed above
 - your Scapy script
 - a brief description of how you called tcpdump to create your tcpdump pcap
 - a brief description of how you called tshark to create your tshark pcap
 - a brief description of the different methods to capture HTTP/HTTPS traffic using tshark

