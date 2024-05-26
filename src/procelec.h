#pragma once

#define CANDIDATE_MAX   9       // Max number of candidates
#define INTEGER_MAX     5       // Max size for fgets one integer - 2 (\n and \0)
#define NAME_MAX        12      // Max size for fgets candidate name - 2

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
pair pairs[CANDIDATE_MAX * (CANDIDATE_MAX - 1) / 2];

// Specify if a candidate has been ranked by a voter
bool candidates_status[CANDIDATE_MAX];

int pair_count;
int candidate_count;

// Function prototypes
int vote(int rank, char* name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
int find_winner(void);

