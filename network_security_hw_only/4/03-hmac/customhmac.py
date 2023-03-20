import base64
from Crypto.Hash import MD5
from Crypto.Hash import SHA
from Crypto.Hash import SHA256
from Crypto.Hash import SHA512
from Crypto.Util.strxor import strxor_c     #xor
from Crypto.Util.py3compat import *         #padding

# Setup

keyb64 = b'p9qA1AHWDpmwIHFNEeColG6wEHjDI7rFO8N9o0bpkE4='
key = base64.b64decode(keyb64) #got the memo too late, the xor function accepts only a string, not a binary
data = b'greer-heroes-grief-weierstrass-stickle'

ipad = 0x13 # Exercise 0x13 RFC 0x36
opad = 0x31 # Exercise 0x31 RFC 0x5C

# Algorithm

def customhmac(key,data,alg,blocksize):
    outer = alg.new()
    inner = alg.new()

    B = blocksize

    if len(key) > B:
        print("Key was too long, a Hash of the Key was used instead")
        key = alg.new(key).digest()

    key = key + bchr(0) * (B - len(key))
    outer.update(strxor_c(key, opad))
    inner.update(strxor_c(key, ipad))

    inner.update(data)

    h = outer.copy()
    h.update(inner.digest())

    return "".join(["%02x" % bord(x) for x in tuple(h.digest())])

# Calculate HMACs
md5 = customhmac(key,data,MD5.new(),64)
SHA = customhmac(key,data,SHA.new(),64)
SHA256 = customhmac(key,data,SHA256.new(),64)
SHA512 = customhmac(key,data,SHA512.new(),128)

print("\nMessage: "+str(data))
print("Key(Base64): "+str(keyb64))
print("ipad: "+str(ipad))
print("opad: "+str(opad))
print("\nHMACs:\n")
print("md5: '"+md5+"'")
print("SHA1: '"+SHA+"'")
print("SHA256: '"+SHA256+"'")
print("SHA512: '"+SHA512+"'\n")

# Response

response = """{
    "group": "11",
    "message": """+'"'+data+"""",
    "hmac": {
        "md5": """+'"'+md5+"""",
        "sha-1": """+'"'+SHA+"""",
        "sha-256": """+'"'+SHA256+"""",
        "sha-512": """+'"'+SHA512+""""
    }
}"""

print("Response:\n\n"+response+"\n")
