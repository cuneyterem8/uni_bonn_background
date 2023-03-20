import hashlib

method="GET"
username="martin"
realm="This is the 2nd part"
nonce="49d81be55cdc64a8"
digestURI="/better-than-part-1/"
cnonce="NDU2NGEwZjgwNzE4MTc3ZTljYWYxODY5ZDk2OGZmNWI="
nonceCount="00000001"
qop="auth"
response="f173f5e8e36be7aeb9a39f0f936e3810"

def main():
	global method
	global username
	global realm
	global nonce
	global digestURI
	global cnonce
	global nonceCount
	global qop
	global response
	
	f = open("rockyou.txt", "r")

	for password in f:
		HA1 = str(hashlib.md5((username+":"+realm+":"+password.strip()).encode('utf-8')).hexdigest())
		HA2 = str(hashlib.md5((method+":"+digestURI).encode('utf-8')).hexdigest())
		response2 = str(hashlib.md5((HA1+":"+nonce+":"+nonceCount+":"+cnonce+":"+qop+":"+HA2).encode('utf-8')).hexdigest())
		
		#print(response2)
		if(response == response2):
			print(password)
			f.close()
			return

	f.close()
		
main()
