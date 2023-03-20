
asus-ubu@asusubu-N550JV:~$ sudo tcpdump -i wlp4s0 -l port http or port https -w tcpdumpfile.pcap
-i shows wifi/internet connection,
-l shows buffered stdout line
-port http and port https show http/https protocols
-w creates and writes pcap file after colecting data

tcpdump: listening on wlp4s0, link-type EN10MB (Ethernet), capture size 262144 bytes
^C1788 packets captured
1794 packets received by filter
0 packets dropped by kernel

