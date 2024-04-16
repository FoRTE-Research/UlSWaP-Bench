/* +++Date last modified: 05-Jul-1997 */

/*
**  SNIPTYPE.H - Include file for SNIPPETS data types and commonly used macros
*/

#ifndef SNIPTYPE__H
#define SNIPTYPE__H

#include <stdlib.h> /* For free()           */
#include <string.h> /* For NULL & strlen()  */

typedef enum
{
    Error_ = -1,
    Success_,
    False_ = 0,
    True_
} Boolean_T;

typedef uint8_t BYTE;
typedef uint32_t DWORD;
typedef uint16_t WORD;

#define NUL '\0'
#define LAST_CHAR(s) (((char *)s)[strlen(s) - 1])
#define TOBOOL(x) (!(!(x)))
#define FREE(p) (free(p), (p) = NULL)

#endif /* SNIPTYPE__H */
