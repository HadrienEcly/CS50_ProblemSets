from cs50 import get_int

#Algorith that create the brick pattern asked by the exerice
def print_bricks(number):
    gap = 2
    for i in range(1, number+1):
        print_spaces(number-i)
        print_hash(i)
        print_spaces(gap)
        print_hash(i)
        print("")

#Print spaces that stays on the same line
def print_spaces(number):
    for i in range(number):
        print(" ", end="")

#Print # that stays on the same line
def print_hash(number):
    for i in range(number):
        print("#", end="")

#Ask users until conditions are met
while True:
    num = get_int("Height : ")
    if num > 0 and num < 9:
        break

print_bricks(num)
