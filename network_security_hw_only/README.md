# Network Security


Most of the exercises include heavily theory and some practical coding


## Content

Threats and attack scenarios, organizational aspects, technical aspects: securing networks using different firewall concepts, IDS and IPS (intrusion detection systems and intrusion prevention systems), security protocols for different protocol layers, integrity protection: hash functions and their weaknesses, certificates, privacy protection, encryption


### Theory example from task


Part1:

With Wireshark, I created a 'https_sample.pcap' which contains HTTPS traffic (port 443).
I used SSLKEYLOGFILE by configuring system requirements and environments properties giving a path for chrome
and mozilla browsers. After that, I flowed the data into 'sslkeylog.txt' by using pre-master-key 
feature via Wireshark (when I export httpssample keys, they are equivalent to each other but 
I did not put the whole informaiton because there were some private keys also).


Part3:

After we defined ssl and tsl (https) property by pre-master-key path with pem or txt file,
we can store the decrytpted https features into this pcap file by using wireshark.
This is one of the example below, also, there is a photo p4 shows that,
at the bottom of the photo, this line can be seen as decrypted.


Packet 6 XXXX
- 1904
- 21.314009
- 216.58.207.38
- 212.201.75.252
- TLSv1.3
- 266
- Server Hello, Change Cipher Spec, Encrypted Extensions, Finished


Packet 163 from C:\Users\Asus\Desktop\https_sample.pcap
- 563
- 10.774685
- 10.148.252.58
- 91.198.174.192
- TLSv1.2
- 139
- Client Key Exchange, Change Cipher Spec, Finished


Part4:

Does the cipher suite that your browser and the server agreed on provide perfect forward secrecy?


Perfect Forward Secrecy is better to use because it can create unique sessions for each transaction.
If attackers do succesful attack and have a private key, they can only access this single transaction data. 
According to these cipher for the https website it is not, but for some other https websites, 
browser and server have perfect forward secracy in which it can be seen via certificate property on browser.

Why does perfect forward secrecy not help in this scenario?
On the internet world, approximately 35% of the websites and the browsers have forward secrecy property. 
Therefore, both website server and browser do not have an ability for forward secracy, it will not help.


