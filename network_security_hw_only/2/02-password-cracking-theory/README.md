# Password Cracking Theory (2 points)
Let's have a look at some simple/simplified theoretical aspects of password cracking.

When passwords are stored, they are typically not stored in plain text (if they are, someone is doing horribly wrong).
Instead, the passwords are stored in a hashed form. That is, only the result h of h = H(p) is stored. 
H is a one way function which takes the password (and possibly more parameters) and transforms it to a non-reversible hash value h.

When trying to crack a password, you usually do not try to attack the actual hash function. 
Instead, you generate password guesses, apply the hash function H, and check whether the result is the same as the 
target hash of the password you are trying to crack.

There are several ways to generate password guesses: two of the most basic and simple ones are dictionaries 
and brute-force.

When using a dictionary a list of candidate passwords is tried sequentially. This is usually called a dictionary 
or word list attack. When all possible combinations out of a set of characters are used as password guesses, 
this is called a brute-force attack.

## Dictionary Attacks
Have a look at the [Probable Wordlists](https://github.com/berzerk0/Probable-Wordlists) project and answer the following questions.

#### What do we learn about how most users picked their passwords?

#### Is your favorite password in the list (optional)?

## Brute-Force Attacks
Derive functional relations between the length of the password, the number of characters of your 
alphabet (letters, upper case, lower case, mixed, numbers, special characters, ...), and the time 
your computer takes to crack the password.

To solve this task, provide the function you used to compute the size of the search space, the number of 
passwords that your computer is able to crack, and some example computation for password hashing algorithms 
of your choice.

## Secure Passwords
Please answer the following questions in a short and concise way.

#### What is more efficient to increase the size of the password search space: increasing the password length of increasing the size of the alphabet?

#### How would you define "weak" and "strong" passwords in general? Why?


## Storing Passwords
Please answer the following questions in a short and concise way.

#### Why is a hash function used and not some kind of encryption to store passwords securely?

#### Find examples of hash functions which are recommended to store passwords nowadays.
