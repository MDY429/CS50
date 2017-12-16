import cs50

change = 0
coin = 0
remaind = 0

while True:
    print("O hai! How much change is owed?");
    change = cs50.get_float()*100
    if change > 0 :
        break

#Quarters can be used.
remaind = change // 25;
coin += remaind;
change %= 25;

#dimes can be used.
remaind = change // 10;
coin += remaind;
change %= 10;

#nickels can be used.
remaind = change // 5;
coin += remaind;
change %= 5;

#pennies can be used.
remaind = change // 1;
coin += remaind;
change %= 1;

#Print the coin you have.
print(int(coin));