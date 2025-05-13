//
// Created by shhan on 2021-07-28.
//

#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
typedef unsigned char         boolean;       /*        TRUE .. FALSE           */

typedef signed char           sint8;         /*        -128 .. +127            */
typedef unsigned char         uint8;         /*           0 .. 255             */
typedef signed short          sint16;        /*      -32768 .. +32767          */
typedef unsigned short        uint16;        /*           0 .. 65535           */
typedef signed int           sint32;        /* -2147483648 .. +2147483647     */
typedef unsigned int         uint32;        /*           0 .. 4294967295      */

typedef signed char           sint8_least;   /* At least 7 bit + 1 bit sign    */
typedef unsigned char         uint8_least;   /* At least 8 bit                 */
typedef signed short          sint16_least;  /* At least 15 bit + 1 bit sign   */
typedef unsigned short        uint16_least;  /* At least 16 bit                */
typedef signed long           sint32_least;  /* At least 31 bit + 1 bit sign   */
typedef unsigned long         uint32_least;  /* At least 32 bit                */

#if defined(__TASKING__)  /* valid for Tasking */
# if (__STDC_VERSION__ >= 199901L) /* ISO-C version is C99 or higher*/
#define PLATFORM_SUPPORT_SINT64_UINT64
typedef signed    long long  sint64;   /* -9223372036854775808 .. 9223372036854775807      */
typedef unsigned  long long  uint64;   /*                    0 .. 18446744073709551615     */
# endif
#else /* valid for GNU and Diab*/
#define PLATFORM_SUPPORT_SINT64_UINT64
typedef signed    long long  sint64;   /* -9223372036854775808 .. 9223372036854775807      */
typedef unsigned  long long  uint64;   /*                    0 .. 18446744073709551615     */
#endif

typedef float                 float32;
typedef double                float64;

#ifndef TRUE
#define TRUE (1u)
#endif

#ifndef FALSE
#define FALSE (0u)
#endif

#define STD_HIGH     1u /* Physical state 5V or 3.3V */
#define STD_LOW      0u /* Physical state 0V */

#define STD_ACTIVE   1u /* Logical state active */
#define STD_IDLE     0u /* Logical state idle */

#define STD_ON       1u
#define STD_OFF      0u

#define NULL_PTR  ((void *)0)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef uint8 Std_ReturnType;
typedef uint8 Gpio_LevelType;


#endif //PLATFORM_TYPES_H
