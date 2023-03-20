# XSS: Steal This Cookie (2 points)

## Part 1
There is a comment system running on `austmann.netsec.seclab-bonn.de`. You can visit your group's
area using the URL `https://austmann.netsec.seclab-bonn.de/group-NN/comments`, where `NN` is your
group number. Try to get access to the admin site hosted at 
`https://austmann.netsec.seclab-bonn.de/group-NN/admin`, where `NN` is again your group number.

Note that you can trigger the `admin` user to visit your comments by sending the string `group-NN`
to the service running at `de-molen.netsec.seclab-bonn.de:8000`. You can do this, for instance, 
with netcat:
```
$ echo group-00 | nc de-molen.netsec.seclab-bonn.de 8000
```

To solve part 1, submit your secret, a description of how you obtained your secret, all scripts
and data you used.

## Part 2
To solve part 2, answer the following questions in a short an concise way.

##### What kind of XSS attack did you execute in part 1?

##### What other types of XSS attacks do you know? Briefly describe them.

##### How can XSS attacks be prevented?
