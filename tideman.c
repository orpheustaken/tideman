#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define CANDIDATE_MAX 9     // Max number of candidates
#define INTEGER_MAX 3       // Max size for fgets one integer
#define NAME_MAX 12         // Max size for fgets candidate name

// Preferences[i][j] is number of voters who prefer i over j
int preferences[CANDIDATE_MAX][CANDIDATE_MAX];

// Locked[i][j] means i is locked in over j
bool locked[CANDIDATE_MAX][CANDIDATE_MAX];

// Each pair has a winner and loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
char* candidates[CANDIDATE_MAX];
pair pairs[CANDIDATE_MAX * (CANDIDATE_MAX - 1) / 2];

int pair_count = 0;
int candidate_count;

// Function prototypes
bool vote(int rank, char* name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool print_winner(void);

int main(int argc, char* argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Check if input is greater than 10 characterss long
    for (int i = 0; i < argc - 1; i++)
    {
        if (strlen(argv[i + 1]) > NAME_MAX - 2)
        {
            printf("Error 2: %s\n\n", argv[i + 1]);
            printf("Candidate's name cannot be greater than 10 characters long\n");
            return 2;
        }
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > CANDIDATE_MAX)
    {
        printf("Maximum number of candidates is %i\n", CANDIDATE_MAX);
        return 3;
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

    // int voter_count = get_int("Number of voters: ");
    int voter_count;

    char* end;
    char int_buffer[INTEGER_MAX];

    do
    {
        printf("Number of voters: ");
        if (!fgets(int_buffer, INTEGER_MAX, stdin)) break;

        // Remove \n
        int_buffer[strlen(int_buffer) - 1] = 0;

        voter_count = strtol(int_buffer, &end, 10);
    }
    while (end != int_buffer + strlen(int_buffer));

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // Ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            // char* name = get_string("Rank %i: ", j + 1);
            char* name = NULL;
            char name_buffer[NAME_MAX];

            printf("Rank %i: ", j + 1);
            if (!fgets(name_buffer, NAME_MAX, stdin)) break;

            // Remove \n
            name_buffer[strlen(name_buffer) - 1] = 0;

            // Allocate heap memory
            name = malloc(strlen(name_buffer) + 1);

            strcpy(name, name_buffer);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 4;
            }

            // Free allocated memory
            free(name);
        }

        record_preferences(ranks);
        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();

    // Check if Tideman was able to print a winner
    if(!print_winner())
    {
        printf("Tideman was not able to process a winner\n");
        return 5;
    }

    return 0;
}

// Update ranks given a new vote
bool vote(int rank, char* name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (!strcmp(candidates[i], name))
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
    int left_strength, right_strength, r_winner, r_loser;
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
                if (locked[k][j])
                {
                    arrow++;
                }
            }
            edges_pointing[j] = (!arrow) ? false : true;
        }

        // Check if current lock created a cycle
        int cycles = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (!edges_pointing[j])
            {
                cycles++;
            }
        }

        // Unlock if cycle
        if (!cycles)
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
    return;
}

// Print the winner of the election
bool print_winner(void)
{
    int winner = 0;
    for (int i = 0; i < pair_count; i++)
    {
        int not_source = 0;
        for (int j = 0; j < pair_count; j++)
        {
            not_source += (locked[j][i]) ? 1 : 0;
        }
        if (!not_source)
        {
            printf("%s is the winner\n", candidates[i]);
            winner++;
        }
    }
    if (!winner) return false;

    return true;
}

