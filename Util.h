// Header File Information /////////////////////////////////////
/**
* @file Util.h
*
* @brief Header File for Util
*
* @version 1.00
* Sim01
*
*/

#ifndef UTIL_H
#include <stdio.h>
#include "data.h"

#define CPUSCHEDULEERROR 1
#define CPUSCHEDULECORRECT 0

#define CORRECT 0
#define MALFORMED 1
#define END_OF_METADATA 2

void extractInfo( char *input, char *output, char DELIMITER );

int extractProcess(char *line, int startOfProcess, int endOfProcess, struct process *node);

int checkMalformedConfig( FILE* config, char DELIMITER );

int strlength( char input[] );

void strCopy ( char *input, char *output );

int strCompare( char *oneStr, char *otherStr );

int strCompareToDelim( char *oneStr, char *otherStr, char DELIMITER );

int checkForSpace( char *str );

int parseInt( char *str );

int lenOfIntStr( char *str );

int powerOf( int base, int power );

int cpuCodeCheck( char *cpuCode, struct configInfo *configStruct );

void strConcat( char *str, char *concatStr );

void logFunc( struct configInfo *config, struct process *processList,
              double configTime, double processTime);

void configLog( struct configInfo *config, FILE *logFile, double configTime );

void processLog( struct configInfo *config, struct process *processList,
   FILE *logFile, double processTime );

void logLine( char * str, FILE *logFile, struct configInfo *config );

void deleteProcessList( struct process *processList );

#endif
