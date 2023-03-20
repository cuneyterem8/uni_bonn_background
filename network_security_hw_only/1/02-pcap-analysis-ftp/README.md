# Pcap Analysis (2 points)
Have a look at the file `pooradmin.pcap` and describe what happened.

To solve the task, answer the following questions in a short and concise way: 

#### What did you observe in the network capture?

ans:
dns-query, http-download, ftp-connecrion,login,list,upload, ssh-connection protocols

#### What is the password of the user pooradmin?

ans:
pooradmin$6$q3...

cat pooradmin.hash pooradmin:$6$q3...
john --wordlist=rockyou.txt pooradmin.hash
john --show pooradmin.hash
pooradmin:1234test

#### In your opinion, is the attacker a very skilled one? Why or why not?

ans:
AUTH TLS
USER Hackerman 
PASS hackH4ckhacK

really fast to crack the pwd


#### What would you recommend to the pooradmin user?

ans:
change your pwd
use more secure pwd not common
find out how the attacker got access to the system
check possible backdoors

