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

# define MAGIC_NUMER 0xF0

# define NODE_SIZE sizeof(node_t)

const uint16_t c_slot_address[] = {0, 64, 128, 192};

typedef enum
{
	NODE_SLOT_0,
	NODE_SLOT_1,
	NODE_SLOT_2,
	NODE_SLOT_3,
	NONE 
} e_node_slots;


e_node_slots	findNode()
{
	if (EEPROM_read(c_slot_address[NODE_SLOT_0]) == MAGIC_NUMER)
		return NODE_SLOT_0;
	if (EEPROM_read(c_slot_address[NODE_SLOT_1]) == MAGIC_NUMER)
		return NODE_SLOT_1;
	if (EEPROM_read(c_slot_address[NODE_SLOT_2]) == MAGIC_NUMER)
		return NODE_SLOT_2;
	if (EEPROM_read(c_slot_address[NODE_SLOT_3]) == MAGIC_NUMER)
		return NODE_SLOT_3;
	return NONE;
}

node_t	readNode(e_node_slots nodeSlot)
{
    node_t	node;
    uint8_t	*bytes = (uint8_t *)&node;

    for (uint8_t i = 0; i < NODE_SIZE; i++)
    {
        bytes[i] = EEPROM_read(c_slot_address[nodeSlot] + i);
    }
    return (node);
}

void	printNode(e_node_slots nodeSlot)
{
	node_t node = readNode(nodeSlot);	

	uart_printstr("Node ID: ");
	uart_itoa(node.nodeId);
	uart_printstr("\r\n");

	uart_printstr("Priority: ");
	uart_itoa(node.priority);
	uart_printstr("\r\n");

	uart_printstr("Slot: ");
	if (nodeSlot == NODE_SLOT_0)
		uart_tx('1');
	if (nodeSlot == NODE_SLOT_1)
		uart_tx('2');
	if (nodeSlot == NODE_SLOT_2)
		uart_tx('3');
	if (nodeSlot == NODE_SLOT_3)
		uart_tx('4');
	uart_printstr("\r\n");

	uart_printstr("Tag: ");
	uart_printstr(node.tag);
	uart_printstr("\r\n");
}

int	writeNode(node_t* node, e_node_slots nodeSlot)
{
	uint8_t* bytes = (uint8_t *)node;

    for (uint8_t i = 0; i < NODE_SIZE; i++)
    {
        uint16_t addr = c_slot_address[nodeSlot] + i;

        if (EEPROM_read(addr) != bytes[i])
        {
            EEPROM_write(addr, bytes[i]);

            if (EEPROM_read(addr) != bytes[i])
            {
				uart_printstr("Corruption detected.");
                return (1);
            }
        }
    }
	return (0);
}

node_t	updateNode(command_content_t* command, e_node_slots position)
{
	node_t	updatedNode = readNode(position);

	if (command->command == SET_ID)
		updatedNode.nodeId = command->newId;

	else if (command->command == SET_PRIO)
		updatedNode.priority = command->newPriority;

	else if (command->command == SET_TAG)
		ft_strlcpy(updatedNode.tag, command->newTag, TAG_SIZE);

	return (updatedNode);
}

node_t	createNode(command_content_t* command)
{
	node_t	newNode;
	newNode.magicNumber = MAGIC_NUMER;
	newNode.nodeId = command->command == SET_ID ? command->newId : 0;
	newNode.priority = command->command == SET_PRIO ? command->newPriority : 0;
	if (command->command == SET_TAG)
		ft_strlcpy(newNode.tag, command->newTag, TAG_SIZE);
	else
	{
		bzeroStr(newNode.tag, TAG_SIZE);
		ft_strlcpy(newNode.tag, "Unconfigured", 13);
	}
	
	return newNode;
}

void	modifyNode(command_content_t* command)
{
	node_t updatedNode; 
	
	e_node_slots nodeSlot = findNode();
	if (nodeSlot == NONE)
		updatedNode = createNode(command);
	else
		updatedNode = updateNode(command, nodeSlot);

	int writeStatus = writeNode(&updatedNode, nodeSlot);
	while (writeStatus != 0 && nodeSlot < NONE)
	{
		nodeSlot++;
		
		uart_printstr("Relocating config to slot");
		uart_itoa(nodeSlot);
		uart_printstr("... ");

		writeStatus = writeNode(&updatedNode, nodeSlot);
		
		if (writeStatus != 0)
			uart_printstr("Fail\r\n");
		else
			uart_printstr("Success\r\n");
	}

	if (writeStatus != 0)
		uart_printstr("CRITICAL EEPROM FAILURE\r\n");
	else
		uart_printstr("Done.")
}

void	printStatus()
{
	e_node_slots nodeSlot = findNode();
	if (nodeSlot == NONE)
		uart_printstr("Node unconfigured\r\n");
	else
		printNode(nodeSlot);
}


void	executeCommand(command_content_t* command)
{
	switch (command->command)
	{
		case STATUS:
		{
			printStatus();
			return;
		}
		case FACTORY_RESET:
		{
			return;
		}
		default:
		{
			modifyNode(command);
		}
	}
}


int main()
{
	uart_init();

	e_state state = PROMPT;
	command_content_t command;

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
				printCommand(&command);
				uart_printstr("\r\n");
				executeCommand(&command);
				state = PROMPT;
			}
			default:
			{
				break;
			}
		}

	}
}


