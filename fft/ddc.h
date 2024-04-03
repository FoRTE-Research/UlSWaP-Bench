/*============================================================================

	 ddc.h  -  Don Cross,  October 1992.

	 Generic ddclib stuff.

============================================================================*/

#ifndef __DDC_DDC_H
#define __DDC_DDC_H

#include <stdint.h>

// If you add something to DDCRET, please add the appropriate string
// to the function DDCRET_String() in the file 'source\ddcret.cpp'.

enum DDCRET
{
   DDC_SUCCESS,           // The operation succeded
   DDC_FAILURE,           // The operation failed for unspecified reasons
   DDC_OUT_OF_MEMORY,     // Operation failed due to running out of memory
   DDC_FILE_ERROR,        // Operation encountered file I/O error
   DDC_INVALID_CALL,      // Operation was called with invalid parameters
   DDC_USER_ABORT,        // Operation was aborted by the user
   DDC_INVALID_FILE       // File format does not match
};


const char *DDCRET_String ( DDCRET );   // See source\ddcret.cpp


#define  TRUE     1
#define  FALSE    0

typedef int dBOOLEAN;

typedef uint8_t BYTE;

typedef uint8_t        UINT8;
typedef int8_t         INT8;

typedef uint16_t   UINT16;
typedef int16_t    INT16;
typedef uint32_t   UINT32;
typedef int32_t    INT32;

#ifdef __BORLANDC__
	#if sizeof(UINT16) != 2
	  #error Need to fix UINT16 and INT16
	#endif

	#if sizeof(UINT32) != 4
	  #error Need to fix UINT32 and INT32
	#endif
#endif

#endif /* __DDC_DDC_H */

/*--- end of file ddc.h ---*/
