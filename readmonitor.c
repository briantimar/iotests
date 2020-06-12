/* polling a file with read calls to handle changes
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define BUFSIZE 5


// polls the given signal file
// when a change is detected in the first BUFSIZE bytes, writes them to stdout
int main(int argc, char* argv[]){
	if (argc != 2) {
		fprintf(stderr, "usage: %s signalfile\n", argv[0]);
		exit(1);
	}

	int numRead;
	off_t offset;
	char buf[BUFSIZE+1]; // descriptor of the monitored file.  
	char prev[BUFSIZE+1]; // message received at last poll
	int monitorFD; 
	
	
	int monitorFlags = O_CREAT | O_RDONLY; 
	mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; 
	// open the signal file 
	if ((monitorFD = open(argv[1], monitorFlags, filePerms)) < 0){ 
		fprintf(stderr, "open failed: errno %d\n", errno);
      		exit(1);
  	}
	buf[BUFSIZE] = 0;
	// read from it continuously.	
	while (1)  {
		// read only the first BUFSIZE bytes of the file
		offset = lseek(monitorFD, 0, SEEK_SET);
		if ( (numRead = read(monitorFD, buf, BUFSIZE)) < 0) {
			fprintf(stderr, "read failed: errno %d\n", errno);
			exit(1);
		}
		if (numRead > 0) {
			buf[numRead] = 0;
			if (strcmp(buf, prev) != 0) {	
				printf("%s", buf);
				fflush(NULL);
				strcpy(prev, buf);
			}
		}
	}
	exit(0);
}




