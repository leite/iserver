
#include <stdio.h>
#include "log.h"

void log ( int level, const char *fmt, ... ) {
   
   FILE* log;
   log = fopen( LOGFILE, "a+" );
   
   if (log == NULL) printf("Error in log: %d (%s)\n", errno, strerror(errno)); return -1;
   
   fprintf( log, *fmt, ... );
   fclose( log );
   
   return 0;
}
