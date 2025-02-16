#include <cs50.h>
#include <stdio.h>
#include <string.h>

/*
 * This program implements the Tideman election system, ensuring the selection
 * of the Condorcet winner when possible.
 *
 * Steps:
 * 1. Collect voter preferences and store them in a 2D array.
 * 2. Determine all head-to-head candidate matchups and store them as pairs.
 * 3. Sort pairs in decreasing order based on the strength of victory.
 * 4. Lock pairs into a directed graph, ensuring no cycles are formed.
 * 5. Identify the source of the graph (the candidate with no incoming edges)
 *    and declare them as the winner.
 *
 * Key Data Structures:
 * - `preferences[i][j]`: Stores the number of voters who prefer candidate i over j.
 * - `pairs[]`: Stores pairs of candidates in head-to-head matchups.
 * - `locked[i][j]`: Boolean adjacency matrix representing the final graph.
 *
 * Functions Implemented:
 * - `vote()`: Records a voter's ranked preferences.
 * - `record_preferences()`: Updates global preferences based on votes.
 * - `add_pairs()`: Stores winning pairs of candidates.
 * - `sort_pairs()`: Sorts pairs by strength of victory.
 * - `lock_pairs()`: Constructs the graph without cycles.
 * - `print_winner()`: Determines and prints the election winner.
 *
 */


// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool creates_cycles(int winner, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Look for a candidate called name
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        { // If candidate found, update ranks and return true. ranks[i] is the voter's ith pref
            ranks[rank] = i;
            return true;
        }
    }

    // If no candidate dound don't update any ranks ans return false
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i < j)
            {
                preferences[ranks[i]][ranks[j]]++;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Add each pair of candidates to pairs array if one candidate is preffered over the other
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pair_count++;
                pairs[pair_count - 1].winner = i;
                pairs[pair_count - 1].loser = j;
            }
        }
    }
    // Update global variable pair_count to be the total number of pairs
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int quotaPair1;
    int quotaPair2;
    pair temp_pair;
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            quotaPair1 = preferences[pairs[i].winner][pairs[i].loser] -
                         preferences[pairs[i].loser][pairs[i].winner];
            quotaPair2 = preferences[pairs[j].winner][pairs[j].loser] -
                         preferences[pairs[j].loser][pairs[j].winner];
            if (quotaPair1 < quotaPair2)
            {
                temp_pair = pairs[j];
                pairs[j] = pairs[i];
                pairs[i] = temp_pair;
            }
        }
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (creates_cycles(pairs[i].winner, pairs[i].loser) == 0)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    int winner;
    bool isSource;
    for (int i = 0; i < candidate_count; i++)
    {
        isSource = true;

        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                isSource = false;
            }
        }

        if (isSource == true)
        {
            printf("%s \n", candidates[i]);
        }
    }

    return;
}

bool creates_cycles(int winner, int loser)
{
    if (loser == winner)
    {
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i] == true)
            if (creates_cycles(winner, i))
                return true;
    }
    return false;
}
