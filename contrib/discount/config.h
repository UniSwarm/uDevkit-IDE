/*
 * configuration for markdown, generated samedi 31 août 2019, 12:54:44 (UTC+0200)
 * by seb@seb-pc
 */
#ifndef __AC_MARKDOWN_D
#define __AC_MARKDOWN_D 1


#define OS_LINUX 1
#define THEME_CF 1
#define HAS_GIT 1

#ifdef _MSC_VER
#define DESTRUCTOR
#else
#define DESTRUCTOR  __attribute__((__destructor__))
#endif

#if _MSC_VER >= 1500 /* VC 9.0, MSC_VER 15, Visual Studio 2008 */
#define inline __inline
#else
#define inline
#endif

#ifdef _MSC_VER
#ifndef strncasecmp
#include <string.h>
#define bzero(p, n) memset(p, 0, n)
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif
#endif

#define while(x) while( (x) != 0 )
#define if(x) if( (x) != 0 )
#define HAVE_INTTYPES_H 1
#define HAVE_UINT32_T 1
#define HAVE_UINT16_T 1
#define HAVE_UINT8_T 1
#define DWORD uint32_t
#define WORD uint16_t
#define BYTE uint8_t
#define HAVE_BASENAME 1
#define HAVE_LIBGEN_H 0
#define HAVE_STDLIB_H 1
#define HAVE_ALLOCA_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_PWD_H 1
#define HAVE_GETPWUID 1
#define HAVE_SYS_STAT_H 1
#define HAVE_STAT 1
#define HAS_ISSOCK 1
#define HAS_ISCHR 1
#define HAS_ISFIFO 1
#define HAVE_SRANDOM 1

#ifdef _MSC_VER
#define INITRNG(x) srand((unsigned int)x)
#define COINTOSS() (rand()&1)
#else
#define INITRNG(x) srandom((unsigned int)x)
#define COINTOSS() (random()&1)
#endif

#define HAVE_MEMSET 1
#define HAVE_RANDOM 1
#define HAVE_STRCASECMP 1
#define HAVE_STRNCASECMP 1
#define HAVE_FCHDIR 1
#define TABSTOP 4
#define GITHUB_CHECKBOX 1
#define HAVE_MALLOC_H 1
#define PATH_FIND "/usr/bin/find"
#define PATH_SED "/bin/sed"
#define BRANCH "1"
#define VERSION "1"

#endif/* __AC_MARKDOWN_D */
