#include "libalex_avr.h"


e_command	identifyCommand(char* buffer, int bufferIndex, int* parsingIndex)
{
    char command[MAX_CMD_LEN + 1];
	bzeroStr(command, MAX_CMD_LEN);
	while (*parsingIndex < bufferIndex
			&& buffer[*parsingIndex] != ' '
			&& *parsingIndex < MAX_CMD_LEN)
	{
		command[*parsingIndex] = buffer[*parsingIndex];
		(*parsingIndex)++;
	}
	command[*parsingIndex] = '\0';

	if (ft_strcmp(command, "STATUS") == 0)
		return STATUS;
	if (ft_strcmp(command, "SET_ID") == 0)
		return SET_ID;
	if (ft_strcmp(command, "SET_PRIO") == 0)
		return SET_PRIO;
	if (ft_strcmp(command, "SET_TAG") == 0)
		return SET_TAG;
	if (ft_strcmp(command, "FACTORY_RESET") == 0)
		return FACTORY_RESET;
    if (ft_strcmp(command, "HEXDUMP") == 0)
		return HEXDUMP;
	return UNKNOWN;
}

void	parseNewTag(char* argumentStr, command_content_t* result)
{
	uint16_t len = ft_strlen(argumentStr);

	if (argumentStr[0] != '\"' || argumentStr[len - 2] != '\"')
	{
		uart_printstr("Tag argument is invalid\r\b");
		result->command = UNKNOWN;
		return;
	}

	ft_strlcpy(result->newTag, &argumentStr[1], len - 2);
}

void	parseNewNodeId(char* argumentStr, command_content_t* result)
{
	// vaguely guard against overflow
	if (ft_strlen(argumentStr) > 10)
	{
		uart_printstr("Node ID argument is invalid\r\b");
		result->command = UNKNOWN;
		return;
	}

	result->newId = ft_atou(argumentStr);
}

void	parseNewPriority(char* argumentStr, command_content_t* result)
{
	// vaguely guard against overflow
	if (ft_strlen(argumentStr) > 5)
	{
		uart_printstr("Priority argument is invalid\r\b");
		result->command = UNKNOWN;
		return;
	}

	result->newPriority = ft_atoi(argumentStr);
}

int ft_strcmp(char* str1, char* str2)
{
	while (*str1 && *str2 && *str1 == *str2)
    {
        str1++;
        str2++;
    }
    return (*str1 - *str2);
}

void	identifyArgument(char* buffer, int bufferIndex, int* parsingIndex, command_content_t* result)
{
	// uart_printstr("\r\nArgument:[");
	// uart_printstr(&buffer[*parsingIndex]);
	// uart_printstr("]\r\n");

    // skip space
	(*parsingIndex)++;

	if (bufferIndex - *parsingIndex > MAX_ARG_LEN)
	{
		result->command = UNKNOWN;
		return;
	}

	char argument[MAX_ARG_LEN + 1];
	bzeroStr(argument, MAX_ARG_LEN);
	int argIndex = 0;
	while (*parsingIndex <= bufferIndex)
	{
		argument[argIndex] = buffer[*parsingIndex];
		argIndex++;
		(*parsingIndex)++;
	}
	argument[argIndex] = '\0';

	// uart_printstr("Argument extracted as: [");
	// uart_printstr(argument);
	// uart_printstr("]\r\n");

	switch (result->command)
	{
		case SET_ID:
		{
			parseNewNodeId(argument, result);
			return;
		}
		case SET_TAG:
		{
			parseNewTag(argument, result);
			return;
		}
		case SET_PRIO:
		{
			parseNewPriority(argument, result);
			return;
		}
		default:
			return;
	}
}

void	clearCommand(command_content_t* command)
{
	command->command = UNKNOWN;
	command->newId = 0;
	command->newPriority = 0;
	bzeroStr(command->newTag, TAG_SIZE);
}

command_content_t	parseCommand(char* buffer, int bufferIndex)
{
	command_content_t	result;
	clearCommand(&result);
	int parsingIndex = 0;

	result.command = identifyCommand(buffer, bufferIndex, &parsingIndex);

	// uart_printstr("Command id as: ");
	// uart_printhex(result.command);
	// uart_printstr("\r\n");

	if (result.command == STATUS 
        || result.command == FACTORY_RESET 
        || result.command == HEXDUMP)
		return result;

	// uart_printstr("\r\nArgument should be:");
	// uart_printstr(&buffer[parsingIndex]);
	identifyArgument(buffer, bufferIndex, &parsingIndex, &result);

	return result;
}

void	printCommand(command_content_t* command)
{
	uart_printstr("\r\nCommand is: ");
	uart_itoa(command->command);
	uart_printstr("\r\nNew Tag is: ");
	uart_printstr(command->newTag);
	uart_printstr("\r\nNew ID is: ");
	uart_itoa(command->newId);
	uart_printstr("\r\nNew Priority is: ");
	uart_itoa(command->newPriority);
	uart_printstr("\r\n");
}