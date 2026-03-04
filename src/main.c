#include "shell.h"
#include <stddef.h>
#include <stdlib.h>


int main(int argc,char *argv[]){
	if(argc != 1){
		if(argc > 2) return 0;
	return 0;
	};

	system("clear");

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

