# HMAC (2 points)

In the lecture you heard about HMAC. Now it is your task to create your own
custom HMAC implementation.

Implement the HMAC algorithm as described in
[RFC 2104](https://tools.ietf.org/html/rfc2104). However, make the hash
function to be used configurable. Your implementation has to support at least
MD5, SHA-1, SHA-256, and SHA-512. Instead of the standard values for `ipad`
and `opad`, your implementation is supposed to use the values `ipad = 0x13`
and `opad = 0x31`.

Access the server `omnipollo` at `https://omnipollo.netsec.seclab-bonn.de/hmac/group-NN`
where `NN` is your group number. You will receive a message and a key and you
are supposed to send back the following response:

```json
{
    "group": "NN",
    "message": "the-message-you-received-from-the-server",
    "hmac": {
        "md5": "hmac-md5-of-the-message",
        "sha-1": "hmac-sha-1-of-the-message",
        "sha-256": "hmac-sha-256-of-the-message",
        "sha-512": "hmac-sha-512-of-the-message"
    }
}
```

In the message above, you have to replace `NN` with your group number,
`the-message-you-received-from-the-server` with the message you received,
and the different `hmac-...` strings with the hex-digests of the HMAC with the
corresponding hash algorithm. The message has to be `POST`ed to `onmipollo`.

If you computed the HMACs correctly, the server will return your secret.

To solve this task, submit the source code of your HMAC implementation and
your secret.

## References
* [HMAC: Keyed-Hashing for Message Authentication (RFC 2104)](https://tools.ietf.org/html/rfc2104)
* [Updated Security Considerations for the MD5 Message-Digest and the HMAC-MD5 Algorithms (RFC 6151)](https://tools.ietf.org/html/rfc6151)
* [HMAC - Wikipedia](https://en.wikipedia.org/wiki/HMAC)
