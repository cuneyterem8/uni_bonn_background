# Block Cipher Modes of Operation (2 points)
Consider the following schematic plain text p = p1, p2, ..., p8 to be encrypted with a block cipher:

```
+------+  +------+  +------+  +------+  +------+  +------+  +------+  +------+
|  p1  |  |  p2  |  |  p3  |  |  p4  |  |  p5  |  |  p6  |  |  p7  |  |  p8  |  
+------+  +------+  +------+  +------+  +------+  +------+  +------+  +------+
```
Let c = c1, c2, ..., c8 be the cipher text after the encryption:
```
+------+  +------+  +------+  +------+  +------+  +------+  +------+  +------+
|  c1  |  |  c2  |  |  c3  |  |  c4  |  |  c5  |  |  c6  |  |  c7  |  |  c8  |  
+------+  +------+  +------+  +------+  +------+  +------+  +------+  +------+
```

Now assume that during the transmission of the cipher text, the block c3 is lost.

To solve the task, answer the following questions in a short and concise way: 
                                                                                                                                                                                                                                                                                 
#### Can the message be decrypted when the ECB mode has been used? If yes, which blocks can be decrypted and why? If not, why?

#### Can the message be decrypted when the CBC mode has been used? If yes, which blocks can be decrypted and why? If not, why?

#### Can the message be decrypted when the CTR mode has been used? If yes, which blocks can be decrypted and why? If not, why?

