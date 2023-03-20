# SYN Flooding (2 points)

## Part 1
Write a simple SYN flooder. Run it in the SecLab against the host `orval.netsec.seclab-bonn.de` on port 22 and capture the traffic. 
After that have a look at the captured traffic and answer the following questions:

#### What **client-side** problem can you find within the network capture? Find out and solve it. 

#### What  **server-side** protection against SYN flooding countermeasures could the host have been used?

To solve part 1, submit the source code of the SYN flooder, your PCAP, and the answers to the two questions.

## Part 2
Use your SYN flooder in the SecLab against the host `chimay.netsec.seclab-bonn.de` on port 22. Do you find any difference in the behaviour compared to `orval.netsec.seclab-bonn.de`? Think of ways to enable your flooder to detect a successful SYN flooding and implement it.

To solve part 2, submit the modified SYN flooder and an answer to the following question:

#### How can a SYN flooding attack be detected?

