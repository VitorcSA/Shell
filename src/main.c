#include "shell.h"
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc,char *argv[]){
	if(argc != 1){
		if(argc > 2) return 0;
	return 0;
	};

	system("clear");

	InitTableJob();
	struct sigaction sigAgction;

	sigAgction.sa_handler = HandleSigchld;
	sigemptyset(&sigAgction.sa_mask);
	sigAgction.sa_flags = SA_NOCLDSTOP;
	sigaction(SIGCHLD,&sigAgction,0);

	char *line = NULL, **inputs = NULL;
	size_t inputSize;

	while (1) {
		GetInput(&line,&inputSize);
		inputs = SliceInput(line);

		for(size_t i = 0;inputs[i] != NULL;i++){

			Execute(inputs[i]);
		};

		free(inputs);

	};

	return 0;
}

