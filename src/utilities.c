#include "utilities.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

size_t CountChars(char *str,char *chars);

char **SliceStr(char *str,char *tag){
    if(!str) return NULL;

	char *token, **slices;
	size_t actualSlice = 0,totalSlices = CountChars(str,tag)+2;

	slices = malloc(sizeof(char *) * totalSlices);
	token = strtok(str,tag);

	while (token){
		slices[actualSlice++] = token;

		token = strtok(NULL,tag);
	}

	slices[actualSlice] = NULL;

	return slices;
};

size_t CountChars(char *str,char *chars){
	if(!str) return 0;
	uint8_t buffer[256] = {0};

	for(int i = 0;chars[i] != '\0';i++){
		buffer[(unsigned char)chars[i]] = 1;
	};

	size_t count = 0;
	for (int i = 0;str[i] != '\0';i++) {
		if(buffer[(unsigned char)str[i]]){
		count++;
		}
	}

	return count;

};

void Trim(char *str){
	if(!str) return;

	int start;
	for(start = 0;str[start] == ' ';start++){};

	int i;
	for(i = 0;str[start + i] != '\0';i++){
		str[i] = str[start+i];
	};
	str[i] = '\0';

	for(int end = i-1;str[end] == ' ';end--){
		str[end] = '\0';
	};

};

int ChangeStdout(const char *newStdout,int flags){
	int fileDescriptor = open(newStdout,flags,0666);
	if(fileDescriptor < 0){
		perror("Erro ao criar ou abrir o arquivo");
		exit(1);
	}

	fflush(stdout);

	int oldStdout = dup(STDOUT_FILENO);

	dup2(fileDescriptor,STDOUT_FILENO);

	close(fileDescriptor);

	return oldStdout;
};

int ChangeStdin(const char *newStdin){

	fflush(stdin);
	int oldStdin = dup(STDIN_FILENO);

	int fileDescriptor = open(newStdin,O_RDONLY);
	if(fileDescriptor < 0){
		perror("Arquivo não encontrado");
		return -1;
	};

	dup2(fileDescriptor,STDIN_FILENO);

	close(fileDescriptor);

	return oldStdin;
};

void RestoreStdout(int originalStdout){
	if(originalStdout == -1) return;
	fflush(stdout);

	dup2(originalStdout,STDOUT_FILENO);

	close(originalStdout);
};

void RestoreStdin(int originalStdin){
	if(originalStdin == -1) return; 

	fflush(stdin);
	dup2(originalStdin,STDIN_FILENO);

	close(originalStdin);
};
