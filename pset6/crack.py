import cs50
import crypt
import sys

#crypt.crypt(word, salt)
password = ""
salt = [0,0]
key =   [
             'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
             'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ''
        ]

if len(sys.argv) != 2:
    print("hash")
    exit(1)
else:
    salt = sys.argv[1][0:2]

    for i in key:
        for j in key:
            for k in key:
                for l in key:
                    password = i +j + k + l + password[4:]
                    if(sys.argv[1] == crypt.crypt(password, salt)):
                        print(password)
                        exit(0)
                        
print("Cannot crack hash")
exit(1)