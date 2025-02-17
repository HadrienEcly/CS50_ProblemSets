import csv
import sys

database = []
longest_matches = {"AGATC": "", "AATG": "", "TATC": ""}


def main():

    # Check comand line argument, if 2 assuming it is a correct db and sequence
    if len(sys.argv) != 3:
        print("Usage : ./dna [database] [sequence])")
        sys.exit()

    # Read database file into a variable
    db_file = open(sys.argv[1], "r")
    db_reader = csv.DictReader(db_file)
    for row in db_reader:
        database.append(row)
        # print(row)
    longest_matches = {key: 0 for key in row if key != 'name'}
    # Read DNA sequence file into a variable
    dna_file = open(sys.argv[2], "r")
    dna = dna_file.read()

    # Find longest match of each STR in DNA sequence
    for key in longest_matches:
        longest_matches[key] = longest_match(dna, key)
        # print(f"Longest match for {key}: {longest_matches[key]}")

    # Check database for matching profiles
    for i in range(0, len(database)):
        if is_matching(dna, database[i], longest_matches):
            print(database[i]["name"])
            return
    else:
        print("No match")
    return


def is_matching(sequence, database_entry, matching):
    for key in matching:
        # print(f"Comparing {key}: database value {database_entry[key]} with longest match {matching[key]}")
        if int(database_entry[key]) != matching[key]:
            return False
    return True


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
