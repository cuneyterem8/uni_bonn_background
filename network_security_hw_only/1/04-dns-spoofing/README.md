# DNS Spoofing (2 points)
Write a DNS spoofer which is able to spoof DNS queries for a specific host. Use it in the SecLab to spoof queries for the IPv4 address of the host `malheur.netsec.seclab-bonn.de`. Note that the spoofer must not spoof all DNS queries but only the ones for the aforementioned host. The fake IP address should be `10.10.10.YOUR_GROUP_NUMBER`, e.g. if you are in group 4 then the IP address in your response should be `10.10.10.4`.

To verify that your spoofer is working correctly, start your spoofer and run 
```
$ echo netsec | nc rodenbach.netsec.seclab-bonn.de 5353
```
If your spoofing attempt was successful, you should receive an answer containing a secret in the format `netsec{<some-hash>}`.

Please do not forget to stop your spoofer after testing it, in order not to disturb the other groups.

To solve this task, submit
 - the source code of your spoofer
 - the response you recevied when running the `netcat` command above

