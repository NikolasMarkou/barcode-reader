#include <poll.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stropts.h>

#include "barcodeParser.h"

#define POLL_TIME_MS    500
#define MAX_BUFFER_SIZE 1024

int continuePolling = 1;

// check if file exists
int fileExists(char* filepath){
    if( access( filepath, F_OK ) != -1 ) {
	return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

// The signal handler just clears the flag and re-enables itself
void catchSignal(int signal){
    continuePolling = 0;
}

int main(int argc, char** argv){
    // check if enough arguments are passed
    if (argc < 2){
	printf("Not enough arguments\n");
	exit(EXIT_FAILURE);
    }
	
    // check if valid file path
    if (fileExists(argv[1]) == EXIT_FAILURE){
	printf("Not valid file path %s\n", argv[1]);
	exit(EXIT_FAILURE);
    }
	
    // Establish a handler for signals
    signal(SIGINT | SIGKILL, catchSignal);
	
    // The structure for two events
    struct pollfd fds[1] = {{0}};
	 
    int fd = open(argv[1], O_RDONLY | O_NONBLOCK);
	
    if (fd < 0 ){
	printf("Error opening file %s\n", argv[1]);
	exit(EXIT_FAILURE);
    }
	
    printf("Polling device %s\n", argv[1]);
	
    // Monitor file for input
    fds[0].fd = fd;
    fds[0].events = POLLIN;
    fds[0].revents = 0;
		
    barcodeContext context = initializeBarcodeContext();
    barcodeOutput output = {0};

    // stop polling only when flag is set to 0	
    while (continuePolling){
	int ret = poll(fds, 1, POLL_TIME_MS);

	if (ret > 0 && (fds[0].revents & POLLIN)){
	    // If we detect the event, 
	    // zero it out so we can reuse the structure
	    int readBytes = 0;
	    char buffer[MAX_BUFFER_SIZE] = {0};
				
	    if ((readBytes = read(fds[0].fd, buffer, sizeof(buffer))) > 0){
		if (validString(buffer, readBytes) == SUCCESS){
		    barcodeInput inputLine = convertStringToLineInput(buffer, readBytes);
		    addInputLineToContext(&context, inputLine);
		    bzero(buffer, sizeof(buffer));
		}
	    }
				
	    if (parseBarcodeContext(context, &output) == SUCCESS){
	        printf("BARCODE RESULT : %s\n", output.line);
		context = initializeBarcodeContext();
		bzero(&output,sizeof(output)) ;
	    }
	}
		
	// zero it out so we can reuse the structure
	fds[0].revents = 0;
    }

    exit(EXIT_SUCCESS);
}
