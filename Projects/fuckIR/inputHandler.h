#ifndef INPUTHANDLER
#define INPUTHANDLER
#include <string.h>
#include <IRCodes.h>
#include <math.h>

typedef unsigned int boolean;

#define True 1
#define False 0

int irctoi(char* irc);

boolean checkCode(int userC, int inputC[4]);


#endif