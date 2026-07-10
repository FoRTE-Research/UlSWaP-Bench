/* +++Date last modified: 05-Jul-1997 */

/*
**        A Pratt-Boyer-Moore string search, written by Jerry Coffin
**  sometime or other in 1991.  Removed from original program, and
**  (incorrectly) rewritten for separate, generic use in early 1992.
**  Corrected with help from Thad Smith, late March and early
**  April 1992...hopefully it's correct this time. Revised by Bob Stout.
**
**  This is hereby placed in the Public Domain by its author.
**
**  10/21/93 rdg  Fixed bug found by Jeff Dunlop
*/

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

#include "inputs.h"
#include "common.h"

static size_t table[UCHAR_MAX + 1];
static size_t len;
static char *findme;

/*
**  Call this with the string to locate to initialize the table
*/

void init_search(const char *string)
{
    size_t i;

    len = strlen(string);
    for (i = 0; i <= UCHAR_MAX; i++) /* rdg 10/93 */
    {
        table[i] = len;
    }
    for (i = 0; i < len; i++)
    {
        table[(unsigned char)string[i]] = len - i - 1;
    }
    findme = (char *)string;
}

/*
**  Call this with a buffer to search
*/

char *strsearch(const char *string)
{
    register size_t shift = 0;
    register size_t pos = len - 1;
    char *here;
    size_t limit = strlen(string);

    while (pos < limit)
    {
        while (pos < limit && (shift = table[(unsigned char)string[pos]]) > 0)
        {
            pos += shift;
        }
        if (0 == shift)
        {
            if (0 == strncmp(findme, here = (char *)&string[pos - len + 1], len))
            {
                return (here);
            }
            else
            {
                pos++;
            }
        }
    }
    return NULL;
}

int benchmark_main()
{
    char *here;

    for (int i = 0; find_strings[i]; i++)
    {
        init_search(find_strings[i]);
        here = strsearch(search_strings[i]);
        printf("\"%s\" is%s in \"%s\"", find_strings[i], here ? "" : " not", search_strings[i]);
        if (here)
        {
            printf(" [\"%s\"]", here);
        }
        printf("\r\n");
    }

    return 0;
}
