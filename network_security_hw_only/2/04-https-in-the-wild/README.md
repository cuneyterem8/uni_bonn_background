# HTTP(S) in the Wild (2 points)

## Part 1: HTTP vs. HTTPS
Write a program which is able check whether a web site is served via HTTP, HTTPS, or both. 
Test at least 100 popular web sites with your tool and fill out the table below. The first line is an example of how your entries should look like.

| Domain        | HTTP only   | HTTP and HTTPS | HTTPS only  |
| --------------| ----------- | -------------- | ----------- |
| *example.com* |             |  *x*           |             |
| *...*         |             |                |             | 
| Summary       | COUNT (X %) | COUNT (X %)    | COUNT (X %) |

There are several lists of popular web sites available on the Internet (e.g. the 
[Alexa Top 1M](http://s3.amazonaws.com/alexa-static/top-1m.csv.zip),
the [Moz Top 500](https://moz.com/top500), or the
[Cisco Umbrella 1M](https://umbrella.cisco.com/blog/2016/12/14/cisco-umbrella-1-million/)).

Note that it is not enough to check whether port 80 is open as there could be only a redirect
to the HTTPS version.

To solve this task, fill out the table, provide your list of domains, and the program you wrote.

## Part 2: SSL/TLS Cipher Suites
Extend your tool from part 1 to be able to list all SSL/TLS cipher suites supported by the web sites
you chose in part 1. Run your tool again and create a histogram of the supported cipher suites.

Note that you don't have to write everything from scratch. If you find tools which serve your
purpose, it is okay to just glue them together.

To solve this task, provide the program you wrote, the output of your program, and the histogram.
