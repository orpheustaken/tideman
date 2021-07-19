### TODO
1. Adapt the code to work without the cs50's library **DONE**

2. Implement GTK

3. Clear all ocurrencies of `variable == 0` and `variable == 1` **DONE**

4. Fix all the bugs that [Victor](https://github.com/victorinknov) found while testing

### Notes
`original/orig_tideman.c` is the original file taken from CS50 IDE </br>
`tideman.c` is where I am working on

C doesn't have any built-in boolean types, libcs50 also takes care of that </br>
`#include <stdbool.h>` solves the problem, but keep in mind:

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

Thanks to this [stackoverflow answer](https://stackoverflow.com/questions/1921539/using-boolean-values-in-c) for the quote and advice
