### TODO

1. adapt the code to work without the cs50's library
lines to change:
`68  get_int`
`79  get_string`

C doesn't have any built-in boolean types, libcs50 takes care of that too
`#include <stdbool.h>` solves the problem

2. implement gtk

orig_tideman.c is the original file taken from my cs50 ide
tideman.c is where i am improving it

> Whatever the boolean constants are called, use them only for initialization. Never ever write something like
> ````
> if (ready == TRUE) ...
> while (empty == FALSE) ...
> ````
> These can always be replaced by the clearer
> ````
> if (ready) ...
> while (!empty) ...
> ````
