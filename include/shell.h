#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>
#include <utilities.h>

void GetInput(char **str,size_t *size);
char **SliceInput(char *input);
bool Execute(char *command);

void InitTableJob();
void HandleSigchld(int sig);

#endif
