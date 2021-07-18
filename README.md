# Tideman's Ranked Pairs
> Ranked pairs (RP) or the Tideman method is an electoral system developed in 1987 by Nicolaus Tideman that selects a single winner using votes that express preferences. RP can also be used to create a sorted list of winners.
> 
> If there is a candidate who is preferred over the other candidates, when compared in turn with each of the others, RP guarantees that candidate will win. Because of this property, RP is, by definition, a Condorcet method. 

This is my personal implementation and improvements on CS50's problem set week 3, written in C.

## Compiling
Everything needed is already specified in the Makefile:
``` bash
$ make tideman
```

## Usage
```bash
$ ./tideman Alice Bob Charlie
Number of voters: 5
Rank 1: Alice
Rank 2: Charlie
Rank 3: Bob

Rank 1: Alice
Rank 2: Charlie
Rank 3: Bob

Rank 1: Bob
Rank 2: Charlie
Rank 3: Alice

Rank 1: Bob
Rank 2: Charlie
Rank 3: Alice

Rank 1: Charlie
Rank 2: Alice
Rank 3: Bob

Charlie is the winner
```

## License
[MIT](https://choosealicense.com/licenses/mit/)
