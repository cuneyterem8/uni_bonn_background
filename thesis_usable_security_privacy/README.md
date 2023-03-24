# Usable Security and Privacy Thesis: Improving Let’s Hash: Development of a website helping developers with security-related programming tasks


This thesis is based on the improvement of the existing LetsHashSalt website for developers to increase password security awareness while implementing code for six different programming languages with source code, reports, and presentations


## Content

Relevant implementations are partial due to avoid non-allowed code content, the documents can be found in reports and presentations in detail, full video of website can be seen in video file.


### Motivation

Password security has always been a significant issue for software developers and users. At first glance, it may seem that software developers care a lot about security. However, many studies have shown that software developers should be at least as careful as users due to a lack of security concerns. For this reason, different infrastructures have been developed to support software developers in many fields. One example is the LetsHashSalt website, a platform that allows software developers to write more secure password authentication. 

The primary purpose of this thesis is to expand the features of the LetsHashSalt website, make it appealing to more programmers, and help them to write secure password authentication codes in different languages. That is why we modified and added new programming languages to the website, such as Python, Java, Javascript, PHP, Golang, C# languages, and libraries like Argon2, Bcrypt, Pbkdf2, regex, zxcvbn and totp. In addition, we asked five professional software developers who are experts in their fields to test the security of the code blocks. 

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/thesis_usable_security_privacy/thesis_website.gif?raw=true" width="60%" height="60%">

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/thesis_usable_security_privacy/photos/1.JPEG?raw=true" width="60%" height="60%">


<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/thesis_usable_security_privacy/photos/2.JPEG?raw=true" width="60%" height="60%">

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/thesis_usable_security_privacy/photos/3.JPEG?raw=true" width="60%" height="60%">

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/thesis_usable_security_privacy/photos/4.JPEG?raw=true" width="60%" height="60%">

Finally, we made it a more comprehensive, secure password authentication website that many software developers can use

# Website Languages

## Python

### Password Storage

### Argon2id

```
pip install argon2-cffi
```

```
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

```

### Bcrypt

```
pip install bcrypt
```

```
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
```

### Pbkdf2

```
pip install pbkdf2
```

```
# Pbkdf2 is one of the best hashing algorithm for password storage. 
# By different parameters, it hashes password by using default salt
# hash can be verified for different passwords

from pbkdf2 import crypt

# iteration and salt are default, if needed increase the values
def hash_password(password):
    return crypt(password)

def verify(pw_hash, password):
    if pw_hash == crypt(password, pw_hash):
        return True
    else:
        return False

def main():
    # example password, change with your database password or input password
    pw_hash = hash_password('s3cr3t')       
    print(pw_hash)                          #print hash as string
    print(verify(pw_hash, 's3cr3t'))        #print true
    print(verify(pw_hash, 's3cr4t'))        #print false

if __name__ == '__main__':
    main()
```

### Password Policy

### zxcvbn

```
pip install zxcvbn
```

```
# Password policy is important for defining minimum requirements 
# for creating strong passwords
# it creates patterns with having at least one 
# upper letter/lower letter/number/special character/8-64 length
# check password strength with zxcvbn library

import re
from zxcvbn import zxcvbn
from dataclasses import dataclass
from typing import Tuple

PATTERNS = dict(
    number = re.compile('[0-9]'),
    lower = re.compile('[a-z]'),
    upper = re.compile('[A-Z]'),
    special = re.compile('[^A-Za-z0-9]'),
    length = re.compile('.{8,64}$'),
)

def composition(password):
    return {
        label : bool(rgx.search(password))
        for label, rgx in PATTERNS.items()
    }

STRENGTHS = {
    0: 'Worst',
    1: 'Bad',
    2: 'Weak',
    3: 'Good',
    4: 'Strong'
}

@dataclass(frozen = True)
class PasswordStrength:
    score: int
    label: str
    warning: str
    suggestions: Tuple[str]
    
def check_strength(password):
    z = zxcvbn(password, user_inputs=['John', 'Smith'])
    return PasswordStrength(
        z['score'],
        STRENGTHS[z['score']],
        z['feedback']['warning'],
        tuple(z['feedback']['suggestions']),
    )

def main():
    # example password, change with your database password or input password
    password = 'Expassword0.'

    x = composition(password)
    print("composition: ", x)               #print each requirement as true

    y = check_strength(password)
    print("check_strength: ", y)            #print strength features

if __name__ == '__main__':
    main()
```

### Two Factor Authentication

### totp

```
pip install pyotp
```

```
# OTP with TOTP and HOTP are used for password authentication to increase security
# TOTP is commonly used version, create totp secret and verify secret within valid time period
# create provisioning_uri and use it by scanning barcode with Google Authenticator

import pyotp
import time

# if needed, change interval parameters
def generate_totp_secret():
    return pyotp.TOTP(pyotp.random_base32(), interval=30)

def generate_uri(second_factor, user_mail, issuer_name):
    return second_factor.provisioning_uri(user_mail, issuer_name)

def main():
    # create totp secret and verify within valid time period
    totp_secret = generate_totp_secret()
    totp_secret_first = totp_secret.now()

    print(totp_secret.verify(totp_secret_first))        #print true 
    time.sleep(30)
    print(totp_secret.verify(totp_secret_first))        #print false

    # change mail and issuer parameters
    # provisioning_uri can be used for QR-code scan by Google Authenticator
    provisioning_uri = generate_uri(totp_secret, "example@mail.com", "issuer name")
    print(provisioning_uri)                             #print provisioning uri

if __name__ == '__main__':
    main()
```


## Java

### Password Storage

### Argon2id

```
<dependency>
    <groupId>de.mkammerer</groupId>
    <artifactId>argon2-jvm</artifactId>
    <version>2.11</version>
</dependency>
```

```
// Argon2 is one of the best hashing algorithm for password storage.
// By different parameters, it hashes password by using default salt
// hash can be verified for different passwords

import de.mkammerer.argon2.Argon2;
import de.mkammerer.argon2.Argon2Factory;
import de.mkammerer.argon2.Argon2Factory.Argon2Types;
import de.mkammerer.argon2.Argon2Helper;

public class Argon2_class {
    public static String hash_password(String password) {
        // hash_length, salt_length, type: argon2id are default, if needed, increase the values
        // if needed, regulate parameters maxMillisecs, memory, parallelism for your system
        Argon2 argon2 = Argon2Factory.create(Argon2Types.ARGON2id);
        int iterations = Argon2Helper.findIterations(argon2, 1000, 65536, 4);
        char[] passwordArray = password.toCharArray();
        String hash = argon2.hash(iterations, 65536, 4, passwordArray);
        argon2.wipeArray(passwordArray);
        return hash;
    }

    public static Boolean verify(String password, String hashed) {
        Argon2 argon2 = Argon2Factory.create(Argon2Types.ARGON2id);
        char[] passwordArray = password.toCharArray();
        return argon2.verify(hashed, passwordArray);
    }

    public static void main(String[] args) {
        // example password, change with your database password or input password
        String pw_hash = hash_password("s3cr3t");
        System.out.println(pw_hash);                                  //print hash as string

        System.out.println(verify("s3cr3t", pw_hash));      //print true
        System.out.println(verify("s3cr4t", pw_hash));      //print false
    }
}
```

### Bcrypt

```
<dependency>
    <groupId>org.mindrot</groupId>
    <artifactId>jbcrypt</artifactId>
    <version>0.4</version>
</dependency>
```

```
// Bcrypt is one of the best hashing algorithm for password storage.
// By different parameters, it hashes password by using default salt
// hash can be verified for different passwords

import org.mindrot.jbcrypt.*;

public class Bcrypt_class {
    public static String hashPassword(String password) {
        // gensalt specifies round 10, if needed increase the value
        return BCrypt.hashpw(password, BCrypt.gensalt());
    }

    public static Boolean verifyPassword(String password, String hashed) {
        return BCrypt.checkpw(password, hashed);
    }

    public static void main(String[] args) {
        // example password, change with your database password or input password
        String hash = hashPassword("s3cr3t");
        System.out.println(hash);                                       //print hash as string

        System.out.println(verifyPassword("s3cr3t", hash));     //print true
        System.out.println(verifyPassword("s3cr4t", hash));     //print false
    }
}
```

### Pbkdf2

```
no installation needed
```

```
// Pbkdf2 is one of the best hashing algorithm for password storage.
// By different parameters, it hashes password by using default salt
// hash can be verified for different passwords

import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.PBEKeySpec;
import java.math.BigInteger;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.spec.InvalidKeySpecException;

public class Pbkdf2_class {

    private static String hash_password(String password)
            throws NoSuchAlgorithmException, InvalidKeySpecException
    {
        // iteration and salt are default, if needed increase the values
        int iterations = 1000;
        char[] chars = password.toCharArray();
        byte[] salt = getSalt();

        PBEKeySpec spec = new PBEKeySpec(chars, salt, iterations, 64 * 8);
        SecretKeyFactory skf = SecretKeyFactory.getInstance("PBKDF2WithHmacSHA1");

        byte[] hash = skf.generateSecret(spec).getEncoded();
        return iterations + ":" + toHex(salt) + ":" + toHex(hash);
    }

    private static byte[] getSalt() throws NoSuchAlgorithmException
    {
        SecureRandom sr = SecureRandom.getInstance("SHA1PRNG");
        byte[] salt = new byte[16];
        sr.nextBytes(salt);
        return salt;
    }

    private static String toHex(byte[] array) throws NoSuchAlgorithmException
    {
        BigInteger bi = new BigInteger(1, array);
        String hex = bi.toString(16);

        int paddingLength = (array.length * 2) - hex.length();
        if(paddingLength > 0)
        {
            return String.format("%0"  +paddingLength + "d", 0) + hex;
        }else{
            return hex;
        }
    }

    private static boolean verify(String originalPassword, String storedPassword)
            throws NoSuchAlgorithmException, InvalidKeySpecException
    {
        String[] parts = storedPassword.split(":");
        int iterations = Integer.parseInt(parts[0]);

        byte[] salt = fromHex(parts[1]);
        byte[] hash = fromHex(parts[2]);

        PBEKeySpec spec = new PBEKeySpec(originalPassword.toCharArray(),
                salt, iterations, hash.length * 8);
        SecretKeyFactory skf = SecretKeyFactory.getInstance("PBKDF2WithHmacSHA1");
        byte[] testHash = skf.generateSecret(spec).getEncoded();

        int diff = hash.length ^ testHash.length;
        for(int i = 0; i < hash.length && i < testHash.length; i++)
        {
            diff |= hash[i] ^ testHash[i];
        }
        return diff == 0;
    }
    private static byte[] fromHex(String hex) throws NoSuchAlgorithmException
    {
        byte[] bytes = new byte[hex.length() / 2];
        for(int i = 0; i < bytes.length ;i++)
        {
            bytes[i] = (byte)Integer.parseInt(hex.substring(2 * i, 2 * i + 2), 16);
        }
        return bytes;
    }

    public static void main(String[] args) throws NoSuchAlgorithmException, InvalidKeySpecException
    {
        // example password, change with your database password or input password
        String pw_hash = hash_password("s3cr3t");
        System.out.println(pw_hash);                                         //print hash as string

        System.out.println(verify("s3cr3t", pw_hash));        //print true
        System.out.println(verify("s3cr4t", pw_hash));        //print false
    }
}
```

### Password Policy

### zxcvbn

```
<dependency>
  <groupId>com.nulab-inc</groupId>
  <artifactId>zxcvbn</artifactId>
  <version>1.7.0</version>
</dependency>
```

```
// Password policy is important for defining minimum requirements
// for creating strong passwords
// it creates patterns with having at least one
// upper letter/lower letter/number/special character/8-64 length
// check password strength with zxcvbn library

import com.nulabinc.zxcvbn.Strength;
import com.nulabinc.zxcvbn.Zxcvbn;
import java.util.HashMap;
import java.util.Map;

public class Policy_class {

    public static String composition(String password) {
        String number = "(?=.*[0-9]).{2,}";
        String upper = "(?=.*[A-Z]).{2,}";
        String lower = "(?=.*[a-z]).{2,}";
        String special = "(?=.*[^A-Za-z0-9]).{2,}";
        String length = ".{8,64}";
        return "" + password.matches(number) + ", " + password.matches(upper) + ", " + password.matches(lower) + ", " + password.matches(special) + ", " + password.matches(length);
    }

    public static String check_strength(String password) {
        Zxcvbn zxcvbn = new Zxcvbn();
        Strength strength_levels = zxcvbn.measure(password);
        Map map = new HashMap();
        map.put(0, "Worst");
        map.put(1, "Bad");
        map.put(2, "Weak");
        map.put(3, "Good");
        map.put(4, "Strong");

        return "" + strength_levels.getScore() + ", " + map.get(strength_levels.getScore()) + ", " + strength_levels.getFeedback();
    }

    public static void main(String[] args) {
        // example password, change with your database password or input password
        String password = "Expassword0.";

        System.out.println("composition: " + composition(password));                //print each requirement as true
        System.out.println("check_strength: " + check_strength(password));          //print strength features
    }
}
```

### Two Factor Authentication

### totp

```
<dependencies>
	<dependency>
		<groupId>com.j256.two-factor-auth</groupId>
		<artifactId>two-factor-auth</artifactId>
		<version>1.3</version>
	</dependency>
</dependencies>
```

```
// OTP with TOTP and HOTP are used for password authentication to increase security
// TOTP is commonly used version, create totp secret and verify secret within valid time period
// create provisioning_uri and use it by scanning barcode with Google Authenticator

import java.util.concurrent.TimeUnit;
import com.j256.twofactorauth.TimeBasedOneTimePasswordUtil;

public class Totp_class {
    public static int generate_totp_secret(String shared_secret) throws Exception{
        return TimeBasedOneTimePasswordUtil.generateCurrentNumber(shared_secret);
    }

    public static String generate_uri(String keyId, String shared_secret) {
        return TimeBasedOneTimePasswordUtil.generateOtpAuthUrl(keyId, shared_secret);
    }

    public static void main(String[] args) throws Exception {
        //create totp secret and verify within valid time period
        String shared_secret = TimeBasedOneTimePasswordUtil.generateBase32Secret();
        int totp_secret_first = generate_totp_secret(shared_secret);
        
        // if needed, change interval parameters
        System.out.println(TimeBasedOneTimePasswordUtil.validateCurrentNumber(shared_secret, totp_secret_first, 30));       //print true
        TimeUnit.SECONDS.sleep(30);
        System.out.println(TimeBasedOneTimePasswordUtil.validateCurrentNumber(shared_secret, totp_secret_first, 30));       //print false

        // change mail and issuer parameters
        // provisioning_uri can be used for QR-code scan by Google Authenticator
        String keyId = "example@mail.com";
        System.out.println(generate_uri(keyId, shared_secret));
        // String imageURL = TimeBasedOneTimePasswordUtil.qrImageUrl(keyId, shared_secret);
    }
}
```

## JavaScript

### Password Storage

### Argon2id

```
brew install gcc
npm install -g node-gyp
CXX=g++-9 npm install argon2
```

```
// Argon2 is one of the best hashing algorithm for password storage. 
// By different parameters, it hashes password by using default salt
// hash can be verified for different passwords

const argon2 = require('argon2');

// hash_length, salt_length, type: argon2id are default, if needed increase the values 
// if needed regulate parameters time_cost, memory_cost, parallelism for your system
async function hash_password(password) {
  try {
    return await argon2.hash(password, { type: argon2.argon2id });
  } catch (err) {
    console.log("error1: " + err)
  }
}

async function verify(pw_hash, password) {
  try {
    if (await argon2.verify(pw_hash, password)) {
      return true;
    } else {
      return false;
    }
  } catch (err) {
    console.log("error2: " + err)
  }
}

async function main() {
  // example password, change with your database password or input password
  const hash = await hash_password("s3cr3t");
  console.log(hash);                            //print hash as string

  console.log(await verify(hash, "s3cr3t"));    //print true
  console.log(await verify(hash, "s3cr4t"));    //print false
}

main();
```

### Bcrypt

```
npm install bcrypt
```

```
// Bcrypt is one of the best hashing algorithm for password storage. 
// By different parameters, it hashes password by using default salt
// hash can be verified for different passwords

const bcrypt = require('bcrypt');

// saltRound specifies round 12, if needed increase the value
async function hash_password(password) {
    const saltRounds = 12;
    try {
        return await bcrypt.hash(password, saltRounds);
    } catch (err) {
        console.log("error1: " + err);
    }
}

async function verify_password(pw_hash, password) {
    try {
        if (await bcrypt.compare(password, pw_hash)) {
            return true;
        } else {
            return false;
        }
    } catch (err) {
        console.log("error2: " + err);
    }
}

// example password, change with your database password or input password
async function main() {
    const hash = await hash_password("s3cr3t");
    console.log(hash);                                      //print hash as string

    console.log(await verify_password(hash, "s3cr3t"));     //print true
    console.log(await verify_password(hash, "s3cr4t"));     //print false
}

main();
```

### Pbkdf2

```
npm install crypto-js
```

```
// Pbkdf2 is one of the best hashing algorithm for password storage. 
// By different parameters, it hashes password by using default salt
// hash can be verified for different passwords

const crypto = require('crypto-js');

const salt = crypto.lib.WordArray.random(128 / 8);

// iteration and salt are default, if needed increase the values
async function hash_password(password) {
    const key = crypto.PBKDF2(password, salt, {
        keySize: 128 / 32,
        iterations: 1000
    });

    return key.toString(crypto.enc.Base64);
}


async function verify_password(pw_hash, password) {
    const key = crypto.PBKDF2(password, salt, {
        keySize: 128 / 32,
        iterations: 1000
    });
    const hash = key.toString(crypto.enc.Base64);

    if (hash === pw_hash) {
        return true;
    }

    return false;
}


async function main() {
    // example password, change with your database password or input password
    const hash = await hash_password("s3cr3t");
    console.log(hash);                           //print hash as string

    console.log(await verify_password(hash, "s3cr3t"));     //print true
    console.log(await verify_password(hash, "s3cr4t"));     //print false
}

main();
```

### Password Policy

### zxcvbn

```
npm install zxcvbn
```

```
// Password policy is important for defining minimum requirements 
// for creating strong passwords
// it creates patterns with having at least one 
// upper letter/lower letter/number/special character/8-64 length
// check password strength with zxcvbn library

function composition(password) {
    
    var number = /(?=.*[0-9])/;
    var upper_lower = /(?=.*[a-z])(?=.*[A-Z])/;
    var special = /[^A-Za-z0-9]/;
    var length = /.{8,64}$/;

    return [number.test(password), upper_lower.test(password), special.test(password), length.test(password)];
}

function check_strength(password) {
    var zxcvbn = require('zxcvbn');
    var result = zxcvbn(password);
    var strength = {
        0: "Worst",
        1: "Bad",
        2: "Weak",
        3: "Good",
        4: "Strong"
    }

    return [result.score, strength[result.score], result.feedback.warning, result.feedback.suggestions];
}


// example password, change with your database password or input password
var password = 'Expassword0.';

console.log('composition: ' + composition(password));       //print each requirement as true
console.log('check_strength: ' + check_strength(password)); //print strength features
```

### Two Factor Authentication

### totp

```
npm i otpauth
npm i totp-generator
```

```
// OTP with TOTP and HOTP are used for password authentication to increase security
// TOTP is commonly used version, create totp secret and verify secret within valid time period
// create provisioning_uri and use it by scanning barcode with Google Authenticator

const OTPAuth = require('otpauth');

// if needed, change interval parameters
function generate_totp_secret(shared_features) {
	return new OTPAuth.TOTP(shared_features);
}

function generate_uri(totp_secret) {
	let uri = totp_secret.toString(); // or "OTPAuth.URI.stringify(totp)"
	return OTPAuth.URI.parse(uri);
}

function main() {
	// create totp secret and verify within valid time period
	let totp_secret = generate_totp_secret({
		issuer: 'issuer name',
		label: 'name@mail.com',
	});
	let totp_secret_first = totp_secret.generate();

	// print true 
	console.log(totp_secret.validate({ token: totp_secret_first, window: 1 }) == 0 ? true : false);
	
	// change mail and issuer parameters
    // provisioning_uri can be used for QR-code scan by Google Authenticator
	console.log("" + generate_uri(totp_secret));
}

main();
```

## PHP

### Password Storage

### Argon2id

### Bcrypt

### Pbkdf2

### Password Policy

### zxcvbn

### Two Factor Authentication

### totp




## lang

### Password Storage

### Argon2id

### Bcrypt

### Pbkdf2

### Password Policy

### zxcvbn

### Two Factor Authentication

### totp




## lang

### Password Storage

### Argon2id

### Bcrypt

### Pbkdf2

### Password Policy

### zxcvbn

### Two Factor Authentication

### totp


