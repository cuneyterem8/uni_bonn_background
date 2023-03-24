# Usable Security and Privacy Thesis: Improving Let’s Hash: Development of a website helping developers with security-related programming tasks


This thesis is based on the improvement of the existing LetsHashSalt website for developers to increase password security awareness while implementing code for six different programming languages with source code, reports, and presentations


## Content

Relevant implementations are partial due to avoid non-allowed code content, the documents can be found in reports and presentations in detail, full video of website can be seen in video file.


### Motivation

Password security has always been a significant issue for software developers and users. At first glance, it may seem that software developers care a lot about security. However, many studies have shown that software developers should be at least as careful as users due to a lack of security concerns. For this reason, different infrastructures have been developed to support software developers in many fields. One example is the LetsHashSalt website, a platform that allows software developers to write more secure password authentication. 

The primary purpose of this thesis is to expand the features of the LetsHashSalt website, make it appealing to more programmers, and help them to write secure password authentication codes in different languages. That is why we modified and added new programming languages to the website, such as Python, Java, Javascript, PHP, Golang, C# languages, and libraries like Argon2, Bcrypt, Pbkdf2, regex and totp. In addition, we asked five professional software developers who are experts in their fields to test the security of the code blocks. 

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/thesis_usable_security_privacy/thesis_website.gif?raw=true" width="60%" height="60%">

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/thesis_usable_security_privacy/photos/1.JPEG?raw=true" width="60%" height="60%">


<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/thesis_usable_security_privacy/photos/2.JPEG?raw=true" width="60%" height="60%">

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/thesis_usable_security_privacy/photos/3.JPEG?raw=true" width="60%" height="60%">

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/thesis_usable_security_privacy/photos/4.JPEG?raw=true" width="60%" height="60%">

Finally, we made it a more comprehensive, secure password authentication website that many software developers can use

# Website Languages

## Python

### Argon2id

'''
pip install argon2-cffi
'''

'''
# Argon2 is one of the best hashing algorithm for password storage. 
# By different parameters, it hashes password by using default salt
# hash can be verified for different passwords

import argon2
from argon2.exceptions import VerifyMismatchError

# hash_length, salt_length, type: argon2id are default, if needed increase the values 
# if needed regulate parameters time_cost, memory_cost, parallelism for your system
# example: PasswordHasher(1, 8, 1, 16, 16)
ph = argon2.PasswordHasher()

def hash_password(password):
    return ph.hash(password)

def verify(pw_hash, password):
    try:
        return ph.verify(pw_hash, password)
    except VerifyMismatchError:
        return False

def main():
    # example password, change with your database password or input password
    pw_hash = hash_password('s3cr3t')       
    print(pw_hash)                          #print hash as string
    print(verify(pw_hash, 's3cr3t'))        #print true
    print(ph.check_needs_rehash(pw_hash))   #print false
    print(verify(pw_hash, 's3cr4t'))        #print false

if __name__ == '__main__':
    main()

'''

### Bcrypt

'''
pip install bcrypt
'''

'''
# Bcrypt is one of the best hashing algorithm for password storage. 
# By different parameters, it hashes password by using default salt
# hash can be verified for different passwords

import bcrypt

# gensalt specifies round 12, if needed increase the value
def hash_password(password):
    pw_hash = bcrypt.hashpw(password.encode(), bcrypt.gensalt())
    return pw_hash

def verify(pw_hash, password):
    return bcrypt.checkpw(password.encode(), pw_hash)

def main():
    # example password, change with your database password or input password
    pw_hash = hash_password('s3cr3t')       
    print(pw_hash)                          #print hash as string
    print(verify(pw_hash, 's3cr3t'))        #print true
    print(verify(pw_hash, 's3cr4t'))        #print false

if __name__ == '__main__':
    main()
'''

