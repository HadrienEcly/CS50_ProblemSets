from cs50 import get_string
import re

# Functions


def calculIndex(text):
    letters = len(re.findall("[a-zA-Z]", text))
    words = len(re.findall(" ", text)) + 1
    sentences = len(re.findall("[.!?]", text))
    l = letters / words * 100
    s = sentences / words * 100
    index = 0.0588 * l - 0.296 * s - 15.8
    return round(index)


# Main
text = get_string("Enter text : ")


index = calculIndex(text)

if index < 1:  # If below 1 is before Grade 1, if above 16 it is grade 16+ and give the rounded grade otherwise
    print("Before Grade 1")
elif index > 16:
    print("Grade 16+")
else:
    print("Grade ", index)
