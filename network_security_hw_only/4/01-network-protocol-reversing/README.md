# Network Protocol Reversing (3 points)

We have seen some unknown traffic in our SecLab, apparently originating from
a process on host `mantra.netsec.seclab-bonn.de`. Unfortunately, the binary is
highly obfuscated and we have no idea what network protocol is being used.

## Part 1
Sniff the traffic between hosts `mantra.netsec.seclab-bonn.de` and
`night-shift.netsec.seclab-bonn.de`, find out and describe
what is happening (e.g. which protocol or protocols are being used (higher than TCP), how do they work and what are they used for?). Afterwards, write a script that reimplements the behavior
of `mantra.netsec.seclab-bonn.de`. For these taks, you need to take a close look at
the messages sent to the server. Your final script should initiate the connection
and do everything that is needed to achieve the same outcome as the legitimate
host `mantra.netsec.seclab-bonn.de`.

In order to solve this part, submit your description and answers as well as the
script reimplementing the behavior of `mantra`.

## Part 2
Now, find out how you can authenticate as someone else. Change your script
accordingly in order to authenticate as your group. 
[The JSON Web Token Toolkit](https://github.com/ticarpi/jwt_tool)
tool may be helpful for this part.

For your solution, submit your updated script, a description of what you did
and why you did it as well as a pcap of your successfull conversation with
host `night-shift.netsec.seclab-bonn.de` (only the conversation).
