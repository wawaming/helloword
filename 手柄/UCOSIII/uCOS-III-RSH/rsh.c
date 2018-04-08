/*
     raw os - Copyright (C)  Lingjun Chen(jorya_txj).

    This file is part of raw os.

    raw os is free software; you can redistribute it it under the terms of the 
    GNU General Public License as published by the Free Software Foundation; 
    either version 3 of the License, or  (at your option) any later version.

    raw os is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
    without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program. if not, write email to jorya.txj@gmail.com
                                      ---

    A special exception to the LGPL can be applied should you wish to distribute
    a combined work that includes raw os, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/


/*   2012-8  Created by jorya_txj
  *  xxxxxx   please added here
  */

#include <cpu.h>
#include <rsh.h>

/*
 * The callback function that is executed when "help" is entered.  This is the
 * only default command that is always present.
 */
 



static CPU_INT32S rsh_help_command(CPU_INT08S *pcWriteBuffer, CPU_INT32U xWriteBufferLen, const CPU_INT08S *pcCommandString );

/*
 * Return the number of parameters that follow the command name.
 */

/* The definition of the "help" command.  This command is always at the front
of the list of registered commands. */
static const xCommandLineInput xHelpCommand = 
{
  "help",
  "help: Lists all the registered commands\r\n",
  rsh_help_command,
  0
};

/* The definition of the list of commands.  Commands that are registered are
added to this list. */
static xCommandLineInputListItem xRegisteredCommands =
{  
  &xHelpCommand,  /* The first command in the list is always the help command, defined in this file. */
  0      /* The next pointer is initialised to NULL, as there are no other registered commands yet. */
};


static xCommandLineInputListItem *current_command_list_point = &xRegisteredCommands;
static xCommandLineInputListItem *current_active_command = (void *)0;


static CPU_INT08U command_length_get(CPU_INT08S *command)
{
  CPU_INT08S *sc;
  // Ivan.Xie 修改: 回车换行符同样作为字符串的结束符
  for (sc = command; (*sc != '\0') && (*sc != ' ') && (*sc != '\n') && (*sc != '\r'); ++sc)
    /* nothing */;
  return sc - command;

}

/*********************************************************所用到的支持的string处理函数******************************/
/**
 * strncmp - Compare two length-limited strings
 * @cs: One string
 * @ct: Another string
 * @count: The maximum number of bytes to compare
 */
int raw_strncmp(const char *cs, const char *ct, CPU_INT32U count)
{
	unsigned char c1, c2;

	while (count) {
		c1 = *cs++;
		c2 = *ct++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
		count--;
	}
	return 0;
}



char * raw_strncpy(char *dest, const char *src, CPU_INT32U count)
{
	char *tmp = dest;

	while (count) {
		if ((*tmp = *src) != 0)
			src++;
		tmp++;
		count--;
	}
	return dest;
}


void * raw_memset(void *src, CPU_INT08U byte, CPU_INT32U count)
{
	CPU_INT32U *aligned_temp_addr;
	CPU_INT08U *temp_addr;
	CPU_INT32U temp_word;

	temp_addr = src;

	if ((count >= 4u) && (((CPU_INT32U)src & 3u) == 0u)) {
	
		aligned_temp_addr = (CPU_INT32U *)src;

		/*make a 4 bytes word*/
		temp_word  =  (byte << 8u) | byte;
		temp_word |= (temp_word << 16u);
	
		while (count >= 16u) {
	
			*aligned_temp_addr++ = temp_word;
			*aligned_temp_addr++ = temp_word;
			*aligned_temp_addr++ = temp_word;
			*aligned_temp_addr++ = temp_word;
			count -= 16u;
		}

		while (count >= 4u) {
		
			*aligned_temp_addr++ = temp_word;
			count -= 4u;
		}

	
		temp_addr = (CPU_INT08U *)aligned_temp_addr;
	}

	while (count--) {
		
		*temp_addr++ = byte;
	}

	return src;

}
/*-----------------------------------------------------------*//*-----------------------------------------------------------*//*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

void rsh_register_command(const xCommandLineInput * const pxCommandToRegister, xCommandLineInputListItem *pxNewListItem)
{

  /* Check the parameter is not NULL. */
  if (pxCommandToRegister == 0) {

    RAW_ASSERT(0);
  }

  
  if (pxNewListItem == 0) {
    
    RAW_ASSERT(0);
  }

  
  /* Reference the command being registered from the newly created 
  list item. */
  pxNewListItem->pxCommandLineDefinition = pxCommandToRegister;

  /* The new list item will get added to the end of the list, so 
  pxNext has nowhere to point. */
  pxNewListItem->pxNext = 0;

  /* Add the newly created list item to the end of the already existing
  list. */
  current_command_list_point->pxNext = pxNewListItem;

  /* Set the end of list marker to the new list item. */
  current_command_list_point = pxNewListItem;
  
}


static const xCommandLineInputListItem *current_process_command_point;


CPU_INT32S rsh_process_command( const CPU_INT08S * const pcCommandInput, CPU_INT08S *pcWriteBuffer, CPU_INT32U xWriteBufferLen  )
{
  CPU_INT32S xReturn = 0;
  const CPU_INT08S *pcRegisteredCommandString;
  CPU_INT08U s_input_length;
  CPU_INT08U s_command_length;
  CPU_INT08U s_commpare_length;
  
  /* Check the parameter is not NULL. */
  if (pcCommandInput == 0) {
    RAW_ASSERT(0);
  }

  if (pcWriteBuffer == 0) {
    RAW_ASSERT(0);
  }

  if (xWriteBufferLen == 0) {
    RAW_ASSERT(0);
  }
  
  /* Note:  This function is not re-entrant.  It must not be called from more
  thank one task. */

  if (current_process_command_point == 0) {

    /* Search for the command string in the list of registered commands. */
    for (current_process_command_point = &xRegisteredCommands; current_process_command_point != 0; current_process_command_point = current_process_command_point->pxNext) {
    
      pcRegisteredCommandString = current_process_command_point->pxCommandLineDefinition->pcCommand;
      s_command_length = command_length_get((CPU_INT08S *)pcRegisteredCommandString);
      s_input_length = command_length_get((CPU_INT08S *)pcCommandInput);
      
      if (s_input_length > s_command_length) {
        s_commpare_length = s_input_length;
      }
      else {
        s_commpare_length = s_command_length;
      }
      
      if (raw_strncmp((const char *)pcCommandInput, (const char *)pcRegisteredCommandString, s_commpare_length) == 0 ) {
        /* The command has been found.  Check it has the expected
        number of parameters.  If cExpectedNumberOfParameters is -1,
        then there could be a variable number of parameters and no
        check is made. */
        if (current_process_command_point->pxCommandLineDefinition->cExpectedNumberOfParameters >= 0) {
          // Ivan.Xie 修改: 参数个数不等于 改为 大于
          if (rsh_get_parameters_numbers( pcCommandInput ) > current_process_command_point->pxCommandLineDefinition->cExpectedNumberOfParameters) {
            xReturn = 1;
          }
        }

        break;
      }
    }
  }

  if (current_process_command_point && (xReturn == 1)) {
  
    /* The command was found, but the number of parameters with the command
    was incorrect. */
    raw_strncpy( ( char * ) pcWriteBuffer, "\rIncorrect command parameter(s).  Enter \"?\" or \"help\" to view a list of available commands.\r\n", xWriteBufferLen );
    current_process_command_point = 0;
  }
  
  else if (current_process_command_point != 0) {
  
    raw_memset(pcWriteBuffer, 0, xWriteBufferLen);
    /* Call the callback function that is registered to this command. */
    xReturn = current_process_command_point->pxCommandLineDefinition->pxCommandInterpreter(pcWriteBuffer, xWriteBufferLen, pcCommandInput);
    
    /* If xReturn is 1, then no further strings will be returned
    after this one, and  pxCommand can be reset to NULL ready to search 
    for the next entered command. */
    if (xReturn == 1) {
      current_active_command = (xCommandLineInputListItem *)current_process_command_point;
      current_process_command_point = 0;
    }
  }
  
  else {
  
    /* pxCommand was NULL, the command was not found. */
    raw_strncpy( ( char * ) pcWriteBuffer, (const char *) "\rCommand not recognised.  Enter \"?\" or \"help\" to view a list of available commands.\r\n", xWriteBufferLen );
    xReturn = 1;
  }
  
  return xReturn;
}

CPU_INT32S rsh_process_key(CPU_INT08S key_code)
{
  CPU_INT32S xReturn = 1;
  if (current_active_command != (void *)0)
  {
    CPU_INT08S key = key_code;
    xReturn = current_active_command->pxCommandLineDefinition->pxCommandInterpreter(&key, 1, (void *)0);
  }
  return xReturn;
}

CPU_INT08S *rsh_get_parameter(const CPU_INT08S *pcCommandString, CPU_INT32S uxWantedParameter, CPU_INT32S *pxParameterStringLength )
{
  CPU_INT32S uxParametersFound = 0;
  CPU_INT08S *pcReturn = 0;
  
  *pxParameterStringLength = 0;

  while( uxParametersFound < uxWantedParameter )
  {
    /* Index the character pointer past the current word.  If this is the start
    of the command string then the first word is the command itself. */
    while( ( ( *pcCommandString ) != 0x00 ) && ( ( *pcCommandString ) != ' ' ) )
    {
      pcCommandString++;
    }

    /* Find the start of the next string. */
    while( ( ( *pcCommandString ) != 0x00 ) && ( ( *pcCommandString ) == ' ' ) )
    {
      pcCommandString++;
    }

    /* Was a string found? */
    if( *pcCommandString != 0x00 )
    {
      /* Is this the start of the required parameter? */
      uxParametersFound++;

      if( uxParametersFound == uxWantedParameter )
      {
        /* How long is the parameter? */
        pcReturn = (CPU_INT08S *)pcCommandString;
        while( ( ( *pcCommandString ) != 0x00 ) && ( ( *pcCommandString ) != ' ' ) )
        {
          ( *pxParameterStringLength )++;
          pcCommandString++;
        }

        break;
      }
    }
    else
    {
      break;
    }
  }

  return pcReturn;
}
/*-----------------------------------------------------------*/


static const xCommandLineInputListItem *current_help_command_point;

static CPU_INT32S rsh_help_command( CPU_INT08S *pcWriteBuffer, CPU_INT32U xWriteBufferLen, const CPU_INT08S *pcCommandString )
{
  
  CPU_INT32S xReturn;

  ( void ) pcCommandString;

  if (current_help_command_point == 0) {
  
    /* Reset the pxCommand pointer back to the start of the list. */
    current_help_command_point = &xRegisteredCommands;
  }

  /* Return the next command help string, before moving the pointer on to
  the next command in the list. */
	
  raw_strncpy( ( char * ) pcWriteBuffer, ( const char * )current_help_command_point->pxCommandLineDefinition->pcHelpString, xWriteBufferLen );
  current_help_command_point = current_help_command_point->pxNext;

  if (current_help_command_point == 0) {

    /* There are no more commands in the list, so there will be no more
    strings to return after this one and 1 should be returned. */
    xReturn = 1;
  }
  else {

    xReturn = 0;
  }

  return xReturn;
}


CPU_INT08S rsh_get_parameters_numbers( const CPU_INT08S * pcCommandString )
{
  CPU_INT08S cParameters = 0;
  CPU_INT32S xLastCharacterWasSpace = 1;

  /* Count the number of space delimited words in pcCommandString. */
  while( *pcCommandString != 0x00 )
  {
    if( ( *pcCommandString ) == ' ' )
    {
      if( xLastCharacterWasSpace != 0 )
      {
        cParameters++;
        xLastCharacterWasSpace = 0;
      }
    }
    else
    {
      xLastCharacterWasSpace = 1;
    }

    pcCommandString++;
  }

  /* The value returned is one less than the number of space delimited words,
  as the first word should be the command itself. */
  return cParameters;
}

