// Program Information ////////////////////////////////////////////////
/**
 * @file FileOps.c
*
* @brief Deals with file input
*
*
* @version 1.00
* Sim01
*
* @note Requires FileOps.h, Util.h, data.h
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileOps.h"
#include "Util.h"
#include "data.h"

int configRead( FILE* config, struct configInfo *configStruct )
{
  //70 since the longest inputBuffer in config is 34, add one and multiply by 2
  char inputBuffer[255];
  //output cant be bigger input, since it is shortened
  char extractOutput[255];

  const char DELIMITER = ':';

  int versionNumber, quantumTime, maxMemory, processTime, ioTime;

  if( config == NULL )
  {
    return CONFIG_FILE_ERROR;
  }

  if( checkMalformedConfig( config, DELIMITER ) == MALFORMED )
  {
    return MALFORMED_CONFIG_FILE;
  }

  //ignore first line
  //fgets(StrPointer/output, Chars to be read, file stream)
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  //resets output array to empty
  //memset(strPointer/output, value to set, number of bytes to set)
  memset( inputBuffer, 0, sizeof( inputBuffer ) );

  //version line
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  extractInfo( inputBuffer, extractOutput, DELIMITER );

  versionNumber = parseInt( extractOutput );

  if( versionNumber < 0 || versionNumber > 10 )
  {
      return INVALID_VERSION_NUMBER;
  }

  configStruct->versNum = versionNumber;

  memset( inputBuffer, 0, sizeof(inputBuffer));

  //file path
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  if( strCompareToDelim( inputBuffer, "File Path:", DELIMITER ) != 0 )
  {
    return MALFORMED_CONFIG_FILE;
  }

  extractInfo( inputBuffer, configStruct->filePath, DELIMITER );

  memset( inputBuffer, 0, sizeof(inputBuffer));


  //CPU Scheduling code
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  extractInfo( inputBuffer, extractOutput, DELIMITER );

  if( cpuCodeCheck( extractOutput, configStruct ) == CPUSCHEDULEERROR )
    {
      return INVALID_CPU_SCHEDULE;
    }


  memset( inputBuffer, 0, sizeof(inputBuffer));

  //quantum cycle time
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  extractInfo( inputBuffer, extractOutput, DELIMITER );

  quantumTime = parseInt( extractOutput );

  if( quantumTime < 0 || quantumTime > 100 )
  {
    return INVALID_QUANT_TIME;
  }

  configStruct->quantTime = quantumTime;

  memset( inputBuffer, 0, sizeof(inputBuffer));


  //Available Memory
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  extractInfo( inputBuffer, extractOutput, DELIMITER );

  //multiply by 1k to convert to bytes from kilobytes
  maxMemory = parseInt( extractOutput )*1000;

  if( quantumTime < 0 || maxMemory > 1048576 )
  {
    return INVALID_MEMORY_SIZE;
  }

  configStruct->memory = maxMemory;

  memset( inputBuffer, 0, sizeof(inputBuffer));

  //Processor Cycle Time
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  extractInfo( inputBuffer, extractOutput, DELIMITER );

  processTime = parseInt( extractOutput );

  if( processTime < 1 || processTime > 1000 )
  {
    return INVALID_PROCESS_TIME;
  }

  configStruct->procCycle = processTime;

  memset( inputBuffer, 0, sizeof(inputBuffer));


  //IO Cycle Time
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  extractInfo( inputBuffer, extractOutput, DELIMITER );

  ioTime = parseInt( extractOutput );

  if( ioTime < 1 || ioTime > 10000 )
  {
    return INVALID_IO_TIME;
  }

  configStruct->ioCycle = processTime;

  memset( inputBuffer, 0, sizeof(inputBuffer));


  //log To:
  fgets(inputBuffer, sizeof( inputBuffer ), config );

  extractInfo( inputBuffer, extractOutput, DELIMITER );

  if( strCompare( extractOutput, "Monitor") == 0 )
  {
    configStruct->logLocation = MONITOR_LOG;
  }

  else if( strCompare( extractOutput, "File") == 0 )
  {
    configStruct->logLocation = FILE_LOG;
  }

  else if( strCompare( extractOutput, "Both") == 0 )
  {
    configStruct->logLocation = BOTH_LOG;
  }
  else
  {
    return INVALID_LOG_LOCATION;
  }

  //file path
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  extractInfo( inputBuffer, configStruct->logPath, DELIMITER );

  memset( inputBuffer, 0, sizeof(inputBuffer));

  return 0;
}

int metaDataRead( FILE* mdf, struct process *processHead )
{
  int startOfOp, increment, lineLength, parseOutput, numOfProcess = 0;

  const char semiColon = ';', period = '.';

  char inputBuffer[255];

  struct process * current = processHead;

  //skip first line
  fgets( inputBuffer, sizeof( inputBuffer ), mdf );

  while( 1 )
  {
    startOfOp = 0;

    fgets( inputBuffer, sizeof( inputBuffer ), mdf );

    lineLength = strlength( inputBuffer );

    for( increment = 0; increment < lineLength; increment++ )
    {

      if( inputBuffer[ increment ] == semiColon ||
          inputBuffer[ increment ] == period)
      {
        if( numOfProcess == 0 )
        {
          numOfProcess++;

          parseOutput = extractProcess( inputBuffer, startOfOp,
                                        increment, current );
        }
        else
        {
          numOfProcess++;

          current->nextProcess = malloc(sizeof(struct process));

          parseOutput = extractProcess( inputBuffer, startOfOp,
                                        increment, current->nextProcess );

          current = current->nextProcess;
        }

        switch ( parseOutput )
        {
          case MALFORMED:

            return MALFORMED;

          case END_OF_METADATA:

            return END_OF_METADATA;

        }

        //current = current->nextProcess;

        //add one to skip space and semicolon
        startOfOp = increment + 2;
      }
    }
  }
  return 0;
}
