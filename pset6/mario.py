import cs50

def main():
    while True:
        print("Height: ", end="")
        height = cs50.get_int()
        if height>0 and height<23:
            break
        
    for i in range(height):
        for j in range(height):
            if i+j+1 < height:
                print(" ", end="")
            else:
                print("#", end="")
        
        print(" ", end="")
        
        for k in range(height):
            if k <= i:
                print("#", end="")
        
        print()

if __name__ == "__main__":
    main()