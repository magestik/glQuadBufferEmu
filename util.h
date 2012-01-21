/* util.h */
#define PROGRAM_NAME    "glQuadBufferEmu"
#define PROGRAM_VERSION "ALPHA"

/* Utility macros */
#define HERE puts("Here.")

#define FATAL_ERROR(str)\
    {fprintf (stderr,"[EE] %s\n",str); exit(EXIT_FAILURE);}

#define WARNING(str)\
    fprintf (stderr,"[WW] %s\n", str)

// #define DEBUG_VERBOSE

