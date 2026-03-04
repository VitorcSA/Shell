#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdbool.h>

char **SliceStr(char *str,char *tag);
void Trim(char *str);
int ChangeStdout(const char *newStdout,int flags);
int ChangeStdin(const char *newStdin);
void RestoreStdout(int originalStdout);
void RestoreStdin(int originalStdin);

#endif
