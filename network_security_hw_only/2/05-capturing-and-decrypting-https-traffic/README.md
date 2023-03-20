# Capturing and Decrypting HTTPS Traffic (2 points)
In this task you will capture and decrypt HTTPS traffic.

## Part 1: Getting the Data
Use Wireshark (or a tool of choice) to create a pcap file containing HTTPS traffic. Furthermore,
configure your browser to provide you with the corresponding encryption keys. Use the 
`SSLKEYLOGFILE` environment variable for this.

To solve this task, provide the pcap and the dumped encryption keys. **NB:** do not enter any
sensitive data when capturing data!

## Part 2: Identifying the Encrypted Conversations
Write a tool which reads a pcap file and displays the `client_random` of each HTTPS session. Use
this tool to identify all sessions for which you have successfully captured the key in part 1.

To solve this task, provide the source code of your tool and its output when run with the pcap of
part 1.

## Part 3: Decrypting the Traffic
Use Wireshark to decrypt the HTTPS traffic and export the decrypted data to a new pcap file.

To solve this task, provide the pcap containing the decrypted HTTP conversation.

## Part 4: Perfect Forward Secrecy
Find out how perfect forward secrecy works and answer the following questions in a short and
concise way.

#### Does the cipher suite that your browser and the server agreed on provide perfect forward secrecy?

#### Why does perfect forward secrecy not help in this scenario?
