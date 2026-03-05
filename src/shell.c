#include "utilities.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef enum Mode{
	SEQUENTIAL,
	PARALLEL,
}Mode;

typedef struct Job{
	pid_t processId;
	bool isActive;
	char *command;
}Job;

typedef	struct TableJob{
	size_t capacity;
	Job *data;
}TableJob;

TableJob tableJob = {0};

Mode mode = SEQUENTIAL;

char **CheckIsInputRedirection(char *command);
char **CheckIsOutputRedirection(char *command,bool *isAppend);
bool TryRedirection(char *command,int *originalStdin,int *originalStdout);
char **CheckIsPipeCommand(char *command);
void ExecutePipeline(char **pipeCommands);
bool TryPipelineExecution(char *command);
bool NormalExecutation(char *command);
bool CheckIsBackground(char *command);

void GetInput(char **str,size_t *size){
	mode == SEQUENTIAL ? printf("seq > ") : printf("par >");
	getline(str,size,stdin);
	printf("\n");
};

char **SliceInput(char *input){
	char **inputs = SliceStr(input,";\n");

	return inputs;
};

bool Execute(char *command){
	int originalStdout = -1;
	int originalStdin = -1;
	bool redirected;

	redirected = TryRedirection(command,&originalStdin,&originalStdout);

	if(!TryPipelineExecution(command)){
		NormalExecutation(command);
	};

	
	if(redirected){
		RestoreStdout(originalStdout);
		RestoreStdin(originalStdin);
	};

	return true;
};

bool TryRedirection(char *command,int *originalStdin,int *originalStdout){
	*originalStdin = -1;
	*originalStdout = -1;

	bool isAppend, redirected = false;
	char **redirection;

	if((redirection = CheckIsInputRedirection(command)) != NULL){
		Trim(redirection[1]);
		if((*originalStdin = ChangeStdin(redirection[1])) < 0){
			return false;
		};

		free(redirection);
		redirected = true;
	};
	
	if((redirection = CheckIsOutputRedirection(command,&isAppend)) != NULL){
		Trim(redirection[1]);
		int flags = O_CREAT | O_WRONLY;
		flags |= isAppend ? O_APPEND : O_TRUNC;

		*originalStdout = ChangeStdout(redirection[1],flags);
		free(redirection);
		redirected = true;
	};

	return redirected;

};

char **CheckIsOutputRedirection(char *command,bool *isAppend){
	for(int i = 0;command[i] != '\0';i++) 
		if(command[i] == '>' && command[i+1] == '>'){ 
			*isAppend = 1; 
			break;
		}else *isAppend = 0;

	char **redirectionCommand = SliceStr(command,">\n");
    if(redirectionCommand[1] == NULL){
        free(redirectionCommand);
        return NULL;
    };

    return redirectionCommand;
};

char **CheckIsInputRedirection(char *command){
	char **inputRedirectionCommand = SliceStr(command,"<\n");
	if(inputRedirectionCommand[1] == NULL){
		return NULL;
	}

	return  inputRedirectionCommand;
};

bool TryPipelineExecution(char *command){
	char **pipeCommands;

	if((pipeCommands = CheckIsPipeCommand(command)) == NULL){
		return false;
	};

	ExecutePipeline(pipeCommands);
	free(pipeCommands);
	return true;
};

char **CheckIsPipeCommand(char *command){
    char **pipeCommands = SliceStr(command,"|\n");
    if(pipeCommands[1] == NULL){
        free(pipeCommands);
        return NULL;
    };

    return pipeCommands;
};

void ExecutePipeline(char **pipeCommands){
	int fileDescriptors[2],lastReadFileDescriptor = 0;
	char **args = NULL;

	for(size_t i = 0;pipeCommands[i] != NULL;i++){

		if(pipeCommands[i+1] != NULL){
			pipe(fileDescriptors);
		};

		pid_t processId = fork();

		if(processId == 0){
			if(lastReadFileDescriptor != 0){
				dup2(lastReadFileDescriptor,0);
				close(lastReadFileDescriptor);
			};

			if(pipeCommands[i+1] != NULL){
				dup2(fileDescriptors[1],1);

				close(fileDescriptors[1]);
				close(fileDescriptors[0]);
			};

			args = SliceStr(pipeCommands[i]," \n");

			if(execvp(args[0],args) == -1){
				perror("Comando Não Encontrado");
				free(args);
				exit(EXIT_FAILURE);
			};
		};

		if(lastReadFileDescriptor != 0){
			close(lastReadFileDescriptor);
		};

		if(pipeCommands[i+1] != NULL){
			close(fileDescriptors[1]);

			lastReadFileDescriptor = fileDescriptors[0];
		}

	};

	while (wait(NULL) > 0){};
};

bool NormalExecutation(char *command){
	bool isBackground = CheckIsBackground(command);
	char **args = SliceStr(command," \n");

	if(args[0] == NULL){
		free(args);
		return false;
	};

	if(!strcmp("cd",args[0])){
		chdir(args[1]);
		return true;
	}else if(!strcmp("exit",args[0])){
		free(args);
		exit(0);
	}

	pid_t processId = fork();
	if(processId < 0) perror("fork falhou.");

	if(processId == 0){
		if(execvp(args[0],args) == -1){
			perror("Comando não encontrado");
			exit(EXIT_FAILURE);
		};
		return true;
	};

	int status;
	if(mode == SEQUENTIAL && !isBackground){
		waitpid(processId,&status,0);
	}

	free(args);
	return true;
};

bool CheckIsBackground(char *command){
	int indexOfAmpersand = -1;
	int isBackground = 0;

	for(int i = 0;command[i] != '\0';i++){
		 if (command[i] == '&') {
		 	isBackground = 1;
			indexOfAmpersand = i;
		 }else if((command[i] != ' ') && (command[i] != '\n')){
		 	isBackground = 0;
			indexOfAmpersand = -1;
		 }

	};

	if(indexOfAmpersand != -1){
		command[indexOfAmpersand] = ' ';
	};

	return isBackground;

};

void InitTableJob(){
	tableJob.capacity = 8;
	tableJob.data = calloc(tableJob.capacity,sizeof(Job));
};

bool AddJob(TableJob *tableJob,pid_t processId,const char *command){
	size_t end = tableJob->capacity;
	
	for(size_t i = 0; i < end ; i++){
		if(tableJob->data[i].isActive == false){
			tableJob->data[i].isActive = true;
			tableJob->data[i].processId = processId;
			tableJob->data[i].command = strdup(command);
			return true;
		};
	};

	tableJob->capacity *= 2;
	tableJob->data = realloc(tableJob->data,sizeof(Job) * tableJob->capacity);
	for(size_t i = end+1,newEnd = tableJob->capacity;i < newEnd;i++){
		tableJob->data[i].isActive = false;
	};
	
	tableJob->data[end].isActive = true;
	tableJob->data[end].processId = processId;
	tableJob->data[end].command = strdup(command);

	return true;

};

bool DeleteJob(TableJob *tableJob,int index){
	if(index >= tableJob->capacity) return false;

	if(tableJob->data[index].isActive == true){
		tableJob->data[index].isActive = false;
		free(tableJob->data[index].command);
		tableJob->data[index].command = NULL;

		return true;
	};

	return false;
};