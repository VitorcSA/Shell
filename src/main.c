#include "shell.h"
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>


extern Mode mode;

int main(int argc,char *argv[]){
	if(argc != 1){
		if(argc > 2) return 0;

		InitTableJob();

		FILE *file = fopen(argv[1],"r");
		if(file == NULL){
			perror("Erro ao abrir o arquivo");
			return 1;
		};

		char **inputs = NULL, *line = NULL;
		size_t inputSize;

		while(getline(&line,&inputSize,file) != -1){;
			inputs = SliceInput(line);

			for(size_t i = 0;inputs[i] != NULL;i++){

				Execute(inputs[i]);

			};

			free(inputs);
		};

		free(line);
		fclose(file);

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

		if(mode == PARALLEL){
			int status;
			waitpid(-1,&status,0);
		};
		
		free(inputs);

	};

	return 0;
}

