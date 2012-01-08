/*
*
*
*/

#include <stdio.h>
#include <sigar.h>
#include <sigar_format.h>
#include "main.h"

void log ( int level, const char *fmt, ... ) {
	
	printf( *fmt, ... );
}

BOOL get_process_list_json( &sigar ) {
		
	int status, i, is_not_admin_warn;
	sigar_proc_list_t proclist;

    status = sigar_proc_list_get(sigar, &proclist);

    if (status != SIGAR_OK) {
        log ( SYS_LOG_ERR, "proc_list error: %d (%s)\n", status, sigar_strerror(sigar, status) ); return FALSE;
    }

    for ( i=0; i<proclist.number; ++i ) {
        
		sigar_pid_t pid = proclist.data[i];
        sigar_proc_state_t pstate;
        sigar_proc_time_t ptime;

        status = sigar_proc_state_get(sigar, pid, &pstate);
        
		if (status != SIGAR_OK) {
            log( SYS_LOG_INFO, "error: %d (%s) proc_state(%d)\n", status, sigar_strerror(sigar, status), pid ); ++is_not_admin_warn; continue;
        }

        status = sigar_proc_time_get(sigar, pid, &ptime);
        
		if (status != SIGAR_OK) {
            log( SYS_LOG_INFO, "error: %d (%s) proc_time(%d)\n", status, sigar_strerror(sigar, status), pid ); ++is_not_admin_warn; continue;
        }

        printf("%d %s\n", (long)pid, pstate.name);
    }

    sigar_proc_list_destroy(sigar, &proclist);	
	
	
}

int main ( void ) {
	
	int status, i;
	
    sigar_t *sigar;
    
	sigar_cpu_list_t cpulist;
	sigar_cpu_info_list_t cpuinfo;

    sigar_open(&sigar);

    status = sigar_cpu_list_get(sigar, &cpulist);

    if (status != SIGAR_OK) {
        printf("cpu_list error: %d (%s)\n", status, sigar_strerror(sigar, status)); return 1;
    }

    for (i=0; i<cpulist.number; i++) {
        sigar_cpu_t cpu = cpulist.data[i];
		
		printf("Core [%d]\n", i+1 );
		printf("User:  %d%\n", ( (cpu.user * 100) / cpu.total ) );
		printf("Idle:  %d%\n", ( (cpu.idle * 100) / cpu.total ) );
		printf("Sys:   %d%\n", ( (cpu.sys * 100) / cpu.total ) );
		printf("Nice:  %d%\n", ( (cpu.nice * 100) / cpu.total ) );
		
		printf("\n");
    }
	
    sigar_cpu_list_destroy(sigar, &cpulist);
	
	status = sigar_cpu_info_list_get( sigar, &cpuinfo );
	
	if (status != SIGAR_OK) {
        printf("cpu_info_list error: %d (%s)\n", status, sigar_strerror(sigar, status)); return 1;
    }
	
    sigar_cpu_info_t cpu = cpuinfo.data[0];
		
	printf("User: %s\n", cpu.model);
		
	printf("\n");

	sigar_cpu_info_list_destroy( sigar, &cpuinfo );
	
    sigar_close(sigar);
	
	printf( "ha" );
	
	return 0;
}
