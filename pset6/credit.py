import cs50

def main():
    creditnum = 0
    card = 0
    
    while True:
        print("Number: ", end="")
        creditnum = cs50.get_int()
        if creditnum > 0:
            break
        
    if Verify(creditnum):
        card = CheckCard(creditnum)
        if card == 4:
            print("VISA")
        elif card == 34 or card == 37:
            print("AMEX")
        elif card >= 51 and card <=55:
            print("MASTERCARD")
        else:
            print("INVALID")
    else:
        print("INVALID")
        
def CheckCard(number):
    
    quotient = 0
    quotient = (number // 1000000000000)
    
    for i in range(3):
        #0~39,50~99 for MASTERCARD, AMEX and VISA
        if quotient < 100 and (quotient <39 or quotient > 50):
            break
        quotient //= 10
        
    return quotient
        
def Verify(number):
    digits = 0
    count = 1
    evensumdigits = 0
    summary = 0
    
    #Check even digits.
    for i in range(8):
        digits = (number % (100 * count) - number % (10 * count)) // (10 * count);
        count *= 100
        evensumdigits = (digits * 2) // 10 + (digits * 2) % 10
        summary += evensumdigits

    
    count = 1
    
    #Check odd digits.
    for i in range(8):
        digits = (number % (10 * count) - number % (1 * count)) // (1 * count)
        count *= 100
        summary += digits
    
    #Check last digit whether is 0.
    if((summary % 10) == 0):
        return True
    else:
        return False
    
    
if __name__ == "__main__":
    main()