from cs50 import get_int


# Function

def check_card():
    StartNumber = int(GetFirstTwoDigit())

    if (StartNumber == 34 or StartNumber == 37) and length == 15:  # Amex check
        print("AMEX")
    elif (StartNumber > 50 and StartNumber < 56) and length == 16:  # Mastercard check
        print("MASTERCARD")
    elif (StartNumber > 39 and StartNumber < 50) and (length == 13 or length == 16):  # Visa check
        print("VISA")
    else:
        print("INVALID")


def GetFirstTwoDigit():  # Get the first two digit to check which card it is later
    firstTwoDigit = card_number / pow(10, length - 2)
    firstTwoDigit = firstTwoDigit % 100
    return firstTwoDigit


def checksum_is_legit():
    checksum = calcul_firsthalf()+calcul_secondhalf()
    if checksum % 10 == 0:
        return True
    else:
        return False


def calcul_firsthalf():
    firsthalf_checksum = 0
    i = length-2
    while i >= 0:
        number = str(card_number)[i]
        number = int(number)*2
        number = str(number)
        firsthalf_checksum += sum(int(digit) for digit in number)
        i = i-2
    return firsthalf_checksum


def calcul_secondhalf():
    secondhalf_checksum = 0
    i = length-1
    while i >= 0:
        number = str(card_number)[i]
        secondhalf_checksum += int(number)
        i = i-2
    return secondhalf_checksum

# Main script


while True:
    card_number = get_int("Card number : ")
    if card_number > 0:
        break

length = len(str(card_number))
checksum = calcul_firsthalf()+calcul_secondhalf()

if checksum_is_legit():
    check_card()
else:
    print("INVALID")
