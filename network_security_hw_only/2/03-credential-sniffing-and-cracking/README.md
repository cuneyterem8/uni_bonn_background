# Credential Sniffing and Cracking (2 points)
The machine ```mikkeller.netsec.seclab-bonn.de``` hosts a web site with restricted access to 
certain areas. Your task is to gain access to these areas.

## Part 1
Have a look at ```http://mikkeller.netsec.seclab-bonn.de/``` in the SecLab and try to get access
to the restricted areas. After you gained access, fill out the following table. The first line 
is an example of how your answer should look like.

| Source Host                      | Subpage/Path           | Authentication Method      | User     | Password   |
| ---------------------------------| ---------------------- | -------------------------- | -------- | ---------- |
| *somehost.netsec.seclab-bonn.de* | */some/page/*          | *super secure auth scheme* | *netsec* | *passw0rd* |
|                                  | /part1/                |                            |          |            |
|                                  | /better-than-part-1/   |                            |          |            |
|                                  | /next-try/             |                            |          |            |
|                                  | /tokens-to-the-rescue/ |                            |          |            |

Note that this task is about authentication and credentials. The system running the webserver is
fully patched and hosting some static webpages only. While you can try to attack the machine with
things like Metasploit, the results will most likely not help you here. Instead, sniffing traffic
to and from ```mikkeller.netsec.seclab-bonn.de``` should yield more promising information for you.

## Part 2
Please answer the following questions in a short and concise way.

#### Are all of the authentication schemes used in part 1 always insecure? Should they never be used nowadays?

#### Find an example of an authentication scheme not used in part 1 and explain the basic concept in two or three sentences.
