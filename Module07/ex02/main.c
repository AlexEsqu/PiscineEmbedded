#include "libalex_avr.h"

// You are developing a "Network Node" device. Each node must store its unique opera-
// tional parameters in its non-volatile EEPROM. Because these nodes are deployed in the
// field, they must be able to detect data corruption and handle the physical wear of the
// memory. You must define a struct representing the device configuration:
// • A Magic Number: To identify your data block.
// • A Node ID: A 32-bit unsigned integer.
// • A Priority: A 16-bit signed integer.
// • A Tag: A string of 32 characters maximum (not including the null-terminator). It
// must only contain alphanumeric characters, dashes (-), or underscores (_).
// • An Integrity Check: A field of at least 16 bits. The Integrity Check must be
// calculated dynamically based on the Node ID, Priority, and Tag. It must be robust
// enough to detect a single-bit change in any of the fields. You are free to choose
// your algorithm; common industrial choices include a 16-bit Additive Checksum,
// Fletcher’s Checksum, or CRC-16 (available in util/crc16.h). A simple 8-bit
// XOR sum or a fixed value is not sufficient for this exercise.

// # Hardware is unreliable. Between power brownouts and cosmic rays,
// your device’s configuration will eventually face bit-flips. In
// production, engineers also include a "version" field in their structs
// to manage migrations.

// # Wear Leveling and Reliability
// EEPROM has a limited number of write cycles. To maximize the longevity of your node,
// you must implement the following safety measures:
// • Write Economy: Do not overwrite a byte if the value already stored in memory
// is identical to the new value.
// • Verification: Every write operation must be verified by reading the byte back
// immediately to ensure it was stored correctly.
// • Relocation: If a write verification fails (the read value does not match the intended
// value), you must relocate the entire configuration structure to a new address.
// • Your program must support at least 4 predefined slots in the EEPROM. On
// startup, it must scan these locations to find the one containing the valid configura-
// tion.
// Implement the following UART interface:
// > STATUS
// Node unconfigured
// > SET_ID 1024
// > STATUS
// Node ID: 1024
// Priority: 0
// Slot: 4
// Tag: "Unconfigured"
// # (If a write failure occurs during a command)
// > SET_TAG "Relay_01"
// Corruption detected.
// Relocating config to slot 1... Fail
// Relocating config to slot 2... Success
// Done.
// • STATUS :
// ◦ If no valid configuration is found in any slot: Node unconfigured.
// ◦ If a magic number is found but the integrity check fails: CRITICAL: Data
// corruption detected!.
// ◦ If valid: Display the ID, Priority, and Tag.
// • SET_ID, SET_PRIO, SET_TAG : Updates the specific field, recalculates the integrity,
// and saves to EEPROM. If a write fails and all slots are exhausted after a full slot
// loop, print CRITICAL EEPROM FAILURE.
// • FACTORY_RESET : Invalidates the current configuration.

// Preparation for Evaluation
// Since physical EEPROM failure is difficult to trigger on demand, you must be prepared
// to demonstrate your relocation logic to the evaluator.
// Plan a "hook" in your code (such as a conditional branch or a specific debug macro)
// that allows you to simulate a verification failure. During the defense, you will be asked
// to trigger this failure to prove that your program correctly detects the error, relocates
// the configuration to the next available slot, and successfully retrieves the data from the
// new location after a reboot.



e_command	identifyCommand(char* buffer, int* bufferIndex, int* parsingIndex)
{
	char command[MAX_CMD_LEN + 1];
	bzeroStr(command, MAX_CMD_LEN);
	while (*parsingIndex < *bufferIndex
			&& buffer[*parsingIndex] != ' ' 
			&& *parsingIndex < MAX_CMD_LEN)
	{
		command[*parsingIndex] = buffer[*parsingIndex];
		(*parsingIndex)++;
	}
	command[*parsingIndex] = '\0';
	
	if (ft_strcmp(buffer, "STATUS") == 0)
		return STATUS;
	if (ft_strcmp(buffer, "SET_ID") == 0)
		return SET_ID;
	if (ft_strcmp(buffer, "SET_PRIO") == 0)
		return SET_PRIO;
	if (ft_strcmp(buffer, "SET_TAG") == 0)
		return SET_TAG;
	if (ft_strcmp(buffer, "FACTORY_RESET") == 0)
		return FACTORY_RESET;
	return UNKNOWN;
}

void	parseNewTag(char* argumentStr, command_content_t* result)
{
	uint16_t len = ft_strlen(argumentStr);
	
	if (argumentStr[0] != '\"' || argumentStr[len] != '\"')
	{
		result->command = UNKNOWN;
		return;
	}

	ft_memmove(result->newTag, &argumentStr[1], len - 2);
}

void	parseNewNodeId(char* argumentStr, command_content_t* result)
{
	// vaguely guard against overflow
	if (ft_strlen(argumentStr) > 10)
	{
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
		result->command = UNKNOWN;
		return;
	}

	result->newId = ft_atoi(argumentStr);
}

void	identifyArgument(char* buffer, int* bufferIndex, int* parsingIndex, command_content_t* result)
{
	if (bufferIndex - parsingIndex > MAX_ARG_LEN + 1)
	{
		result->command = UNKNOWN;
		return;
	}
	
	char argument[MAX_ARG_LEN + 1];
	bzeroStr(argument, MAX_ARG_LEN);
	while (*parsingIndex < *bufferIndex
			&& buffer[*parsingIndex] != ' ' 
			&& *parsingIndex < MAX_ARG_LEN)
	{
		argument[*parsingIndex] = buffer[*parsingIndex];
		(*parsingIndex)++;
	}
	argument[*parsingIndex] = '\0';

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

command_content_t	parseCommand(char* buffer, int bufferIndex)
{
	command_content_t	result;
	int parsingIndex = 0;

	result.command = identifyCommand(buffer, &bufferIndex, &parsingIndex);

	uart_printstr("Command id as: ");
	uart_printhex(result.command);
	uart_printstr("\r\n");

	if (result.command == STATUS || result.command == FACTORY_RESET)
		return result;

	identifyArgument(buffer, &bufferIndex, &parsingIndex, &result);

	return result;
}


void	printCommand(command_content_t command)
{
	uart_printstr("\r\nCommand is: ");
	uart_itoa(command.command);
	uart_printstr("\r\nNew Tag is: ");
	uart_printstr(command.newTag);
	uart_printstr("\r\nNew ID is: ");
	uart_itoa(command.newId);
	uart_printstr("\r\nNew Priority is: ");
	uart_itoa(command.newPriority);
	uart_printstr("\r\n");
}



int main()
{
	uart_init();

	e_state state = PROMPT;
	command_content_t command;
	ft_memset(&command, 0, sizeof(command));

	char	buffer[BUFFER_SIZE];
	int		bufferIndex = 0;
	bzeroStr(buffer, BUFFER_SIZE);

	while (1)
	{
		switch (state)
		{
			case PROMPT:
			{
				bzeroStr(buffer, BUFFER_SIZE);
				bufferIndex = 0;
				ft_memset(&command, 0, sizeof(command));
				uart_printstr("> ");
				state = RECEIVE_COMMAND;
				break;
			}

			case RECEIVE_COMMAND:
			{
				handleUserTyping(buffer, &bufferIndex, &state);
				break;
			}

			case VALIDATE_EXECUTE:
			{
				command = parseCommand(buffer, bufferIndex);
				printCommand(command);
				uart_printstr("\r\n");
				state = PROMPT;
			}

		}
		
	}
}



// EEARH =  EEPROM Address Register High
// 	–			–			–			–			–			–			EEAR9		EEAR8
// [																	]	[EEPROM address			]

// EEARH =  EEPROM Address Register Low
//	EEAR7		EEAR6		EEAR5		EEAR4		EEAR3		EEAR2		EEAR1		EEAR0
// [	EEPROM address																				]

// EEDR – The EEPROM Data Register
// MSB			-			-			-			-			-			-			LSB
// [	Data to be written / Data read out															]


// EECR – The EEPROM Control Registe
// –			–			EEPM1		EEPM0		EERIE		EEMPE		EEPE		EERE
// [					  ]	[Programming Mode	  ]	[Interrupt]	[MasterWri]	[WriteEnab]	[ReadEnabl]
