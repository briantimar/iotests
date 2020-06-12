// records times required for the readmonitor to acknowledge changes in the signal file.
//

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#define BUFSIZE 5

// expects to receive the monitor's output on stdin...
int main(int argc, char* argv[]){

	int nmsg = 10000;
	int i;
	char* signalfname = "signal";
	char writebuf[BUFSIZE];
	char readbuf[BUFSIZE];
	struct timeval t0, t1;
	int monitorFD;

	// open the signal file for writing.
	int monitorFlags = O_CREAT | O_RDWR; 
	mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; 
	// open the signal file 
	if ((monitorFD = open(signalfname, monitorFlags, filePerms)) < 0){ 
		fprintf(stderr, "open failed: errno %d\n", errno);
      		exit(1);
  	}
	
	ssize_t numread;
	double dt;

	for (i=0; i<nmsg; i++) {
		// send a new message
		sprintf(writebuf, "%d", i%2);
		lseek(monitorFD, 0, SEEK_SET);
		write(monitorFD, writebuf, BUFSIZE);
		fflush(NULL);
		gettimeofday(&t0, NULL);
		// await response

		while ((numread=read(STDIN_FILENO, readbuf, BUFSIZE))==0);
		gettimeofday(&t1, NULL);
		if (numread < 0){
			fprintf(stderr, "read failed, errno %d\n", errno);
			exit(1);
		}
		writebuf[1] = 0;
		readbuf[1] = 0;
	        if (strcmp(writebuf, readbuf) !=0) {
			fprintf(stderr, "write %s read %s\n", writebuf, readbuf);
			exit(1);
		}
	  	dt = (float) (t1.tv_sec - t0.tv_sec) + 1e-6 * ((float) (t1.tv_usec - t0.tv_usec));
		printf("%.4e\n", dt);
		fflush(NULL);
	}
	exit(0);
}	
