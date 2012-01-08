
#include <stdio.h>
#include <windows.h>
#include <process.h>

UINT __stdcall Thread(void *ArgList) {

	HANDLE hEvent = *((HANDLE*)ArgList);

	while (WaitForSingleObject(hEvent, 0) != WAIT_OBJECT_0) {
		Sleep( Second>>2 ); // sleep for a quarter of a second
		printf( "Waiting for signal\n" );
	}
	
	printf( "Thread ending\n" );
	return 0;
}

int main(int argc, char *argv[], char *envp[]) {
	
	int ErrorNumber, DOSErrorNumber;
	UINT ThreadId;
	HANDLE hThread, hEvent;

	// Create a manual-reset nonsignaled unnamed event
	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	hThread = (HANDLE)_beginthreadex(NULL, 0, Thread, &hEvent, 0, &ThreadId);
	
	if (hThread == 0) {
		ErrorNumber = errno;
		DOSErrorNumber = _doserrno;
		printf( "Begin thread error: %s\n", strerror(ErrorNumber) );
		printf( "Begin thread DOS error code: %d\n", DOSErrorNumber );
		return 16;
	}
	
	printf( "Thread begun\n" );

	Sleep(Second<<1); // wait 2 seconds
	
	SetEvent(hEvent); // signal thread to end
	
	int rv = WaitForSingleObject(hThread, Second<<3); // wait up to 8 seconds
	
	printf( "Thread wait rv %d\n", rv );
	
	CloseHandle(hEvent);
	
	CloseHandle(hThread);

	return 0;
}
