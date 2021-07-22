#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define CANDIDATE_MAX 9     // Max number of candidates
#define INTEGER_MAX 5       // Max size for fgets one integer
#define NAME_MAX 12         // Max size for fgets candidate name

// Define ANSI colors for outputs
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"

#define RESET   "\x1b[0m"

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
bool candidates_status[CANDIDATE_MAX];
pair pairs[CANDIDATE_MAX * (CANDIDATE_MAX - 1) / 2];

int pair_count = 0;
int candidate_count;

// Function prototypes
int vote(int rank, char* name, int ranks[]);
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
        printf(RED    "\nUsage: ./tideman [candidate1] [candidate2] ...\n"    RESET);
        return 1;
    }

    // Check if input is greater than 10 characterss long
    for (int i = 0; i < argc - 1; i++)
    {
        if (strlen(argv[i + 1]) > NAME_MAX - 2)
        {
            printf(RED    "\nError: %s\n"    RESET, argv[i + 1]);
            printf(RED    "Candidate's name cannot be greater than 10 characters long\n"    RESET);
            return 2;
        }
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > CANDIDATE_MAX)
    {
        printf(RED    "\nMaximum number of candidates is %i\n"    RESET, CANDIDATE_MAX);
        return 3;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs and candidates status
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
        candidates_status[i] = false;
    }

    // int voter_count = get_int("Number of voters: ");
    int voter_count;

    char* end;
    char int_buffer[INTEGER_MAX];

    do
    {
        printf(CYAN    "Number of voters: "    RESET);
        if (!fgets(int_buffer, INTEGER_MAX, stdin)) break;

        // Remove \n and check for buffer overflow
        if (int_buffer[strlen(int_buffer) - 1] == '\n')
        {
            int_buffer[strlen(int_buffer) - 1] = 0;
        }
        else
        {
            printf(RED    "\nThe number of voters cannot exceed 3 digits\n"    RESET);
            return 4;
        }

        voter_count = strtol(int_buffer, &end, 10);

        if (end != int_buffer + strlen(int_buffer) || !*int_buffer) printf(YELLOW    "\nInput must be a valid integer\n\n"    RESET);
    }
    while (end != int_buffer + strlen(int_buffer) || !*int_buffer);

    if (voter_count <= 0)
    {
        printf(RED    "\nTideman cannot process an election without voters\n"    RESET);
        return 5;
    }
    printf("\n");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // Specify current voter
        printf(MAGENTA    "_____________________ Voter %i\n"    RESET, i + 1);

        // Ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            // char* name = get_string("Rank %i: ", j + 1);
            char* name = NULL;
            char name_buffer[NAME_MAX];

            int status = -1;

            do
            {
                printf(CYAN    "Rank %i: "    RESET, j + 1);
                if (!fgets(name_buffer, NAME_MAX, stdin)) break;

                // Remove \n
                name_buffer[strlen(name_buffer) - 1] = 0;

                // Allocate heap memory
                name = malloc(strlen(name_buffer) + 1);

                strcpy(name, name_buffer);

                status = vote(j, name, ranks);

                if (!status) printf(YELLOW    "\nThere is no candidate called %s\n\n"    RESET, name);
                if (status == 2) printf(YELLOW    "\n%s was already chosen\n\n"    RESET, name);
            }
            while (!status || status == 2);

            // Free allocated memory
            free(name);
        }

        // Clear status for the next voter
        for (int j = 0; j < candidate_count; j++)
        {
            candidates_status[j] = false;
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
        printf(RED    "\nTideman was not able to process a winner\n"    RESET);
        return 6;
    }

    return 0;
}

// Update ranks given a new vote
int vote(int rank, char* name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (!strcmp(candidates[i], name) && !candidates_status[i])
        {
            candidates_status[i] = true;
            ranks[rank] = i;
            return 1;
        }
        else if (!strcmp(candidates[i], name))
        {
            return 2;
        }
    }
    return 0;
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
            printf(GREEN    "%s is the winner\n"    RESET, candidates[i]);
            winner++;
        }
    }
    if (!winner) return false;

    return true;
}

