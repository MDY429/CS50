import cs50
import sys



def main():
    CheckCommand(sys.argv)
    
    counter = 0
    length = len(sys.argv[1])
    
    print("plaintext: ", end="")
    plaintext = cs50.get_string()
    
    print("ciphertext: ", end="")
    for i in plaintext:
        if i.isalpha():
            Enchiper(i, sys.argv[1].upper(), counter, length)
            counter+=1
        else:
            print(i, end="")
    
    print()
    exit(0)

def CheckCommand(word):
    if len(word) != 2:
        print("{} k".format(word[0]))
        exit(1)
    for i in word[1]:
        if i.isalpha() == False:
            print("{} k".format(word[1]))
            exit(1)
            
def Enchiper(s, enc, j, k):
    if s.isupper():
        print(chr((ord(s) - 65 + (ord(enc[j % k]) - 65))%26 +65), end="")
    if s.islower():
        print(chr((ord(s) - 97 + (ord(enc[j % k]) - 65))%26 +97), end="")



if __name__ == "__main__":
    main()
