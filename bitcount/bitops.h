/* +++Date last modified: 05-Jul-1997 */

/*
**  Macros and prototypes for bit operations
**
**  public domain for SNIPPETS by:
**    Scott Dudley
**    Auke Reitsma
**    Ratko Tomic
**    Aare Tali
**    J. Blauth
**    Bruce Wedding
**    Bob Stout
*/

#ifndef BITOPS__H
#define BITOPS__H

#include <stdio.h>
#include <stdint.h>   /* For uint32_t         */
#include <stdlib.h>   /* For size_t           */
#include <limits.h>   /* For CHAR_BIT         */
#include "sniptype.h" /* For TOBOOL()         */

/*
**  Macros to manipulate bits in any integral data type.
*/

#define BitSet(arg, posn) ((arg) | (1L << (posn)))
#define BitClr(arg, posn) ((arg) & ~(1L << (posn)))
#define BitFlp(arg, posn) ((arg) ^ (1L << (posn)))
#define BitTst(arg, posn) TOBOOL((arg) & (1L << (posn)))

/*
**  Macros to manipulate bits in an array of char.
**  These macros assume CHAR_BIT is one of either 8, 16, or 32.
*/

#define MASK CHAR_BIT - 1
#define SHIFT ((CHAR_BIT == 8) ? 3 : (CHAR_BIT == 16) ? 4 : 8)

#define BitOff(a, x) ((void)((a)[(x) >> SHIFT] &= ~(1 << ((x) & MASK))))
#define BitOn(a, x) ((void)((a)[(x) >> SHIFT] |= (1 << ((x) & MASK))))
#define BitFlip(a, x) ((void)((a)[(x) >> SHIFT] ^= (1 << ((x) & MASK))))
#define IsBit(a, x) ((a)[(x) >> SHIFT] & (1 << ((x) & MASK)))


/*
** BITSTRNG.C
*/

void bitstring(char *str, uint32_t byze, int biz, int strwid);

/*
**  BSTR_I.C
*/

uint32_t bstr_i(char *cptr);

/*
**  BITCNT_1.C
*/

uint32_t bit_count(uint32_t x);

/*
**  BITCNT_2.C
*/

uint32_t bitcount(uint32_t i);

/*
**  BITCNT_3.C
*/

uint32_t ntbl_bitcount(uint32_t x);
uint32_t BW_btbl_bitcount(uint32_t x);
uint32_t AR_btbl_bitcount(uint32_t x);

/*
**  BITCNT_4.C
*/

uint32_t ntbl_bitcnt(uint32_t x);
uint32_t btbl_bitcnt(uint32_t x);

#endif /*  BITOPS__H */
