#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// Locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

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
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int i_position, j_position;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int k = 0; k < candidate_count; k++)
        {
            if (i == ranks[k])
            {
                i_position = k;
            }
        }
        for (int j = 0; j < candidate_count; j++)
        {
            for (int l = 0; l < candidate_count; l++)
            {
                if (j == ranks[l])
                {
                    j_position = l;
                }
            }
            preferences[i][j] += (i_position < j_position) ? 1 : 0;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Unused variable, CS50's Makefile is configured to NOT warn about that
    int /*strength,*/ left_strength, right_strength, r_winner, r_loser;
    int swap_counter = -1;
    do
    {
        swap_counter = 0;
        for (int i = 0; i < pair_count; i++)
        {
            left_strength = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
            right_strength = preferences[pairs[i + 1].winner][pairs[i + 1].loser] - preferences[pairs[i + 1].loser][pairs[i + 1].winner];
            if (left_strength < right_strength)
            {
                r_winner = pairs[i + 1].winner;
                r_loser = pairs[i + 1].loser;
                pairs[i + 1].winner = pairs[i].winner;
                pairs[i + 1].loser = pairs[i].loser;
                pairs[i].winner = r_winner;
                pairs[i].loser = r_loser;
                swap_counter++;
            }
        }
    }
    while (swap_counter != 0);
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        // Lock
        locked[pairs[i].winner][pairs[i].loser] = true;

        // Search for edges
        bool edges_pointing[candidate_count];
        for (int j = 0; j < candidate_count; j++)
        {
            int arrow = 0;
            for (int k = 0; k < candidate_count; k++)
            {
                if (locked[k][j] == true)
                {
                    arrow++;
                }
            }
            edges_pointing[j] = (arrow == 0) ? false : true;
        }

        // Check if current lock created a cycle
        int cycles = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (edges_pointing[j] == false)
            {
                cycles++;
            }
        }

        // Unlock if cycle
        if (cycles == 0)
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int is_source = 0;
        for (int j = 0; j < pair_count; j++)
        {
            is_source += (locked[j][i] == true) ? 1 : 0;
        }
        if (is_source == 0)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

