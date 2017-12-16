import cs50
import sys

if len(sys.argv) != 2:
    print("{} k".format(sys.argv[0]))
    exit(1)
    
k = int(sys.argv[1])
print(k)

print("plaintext: ", end="")
plaintext = cs50.get_string()

print("ciphertext: ", end="")
for i in plaintext:
    if i.isalpha() and i.isupper():
        print(chr((ord(i) - 65 + k) % 26 + 65), end="")
    elif i.isalpha() and i.islower():
        print(chr((ord(i) - 97 + k) % 26 + 97), end="")
    else:
        print(i, end="")
        
print()
exit(0)