#include "libalex_avr.h"

// • Write a program that will turn on the 3 LEDs D6, D7, and D8 and address the
// 	following issues:
// 		◦ The potentiometer RV1 is used to change the value of one of the 3 RGB
// 			primary colors.
// 		◦ The button SW1 is used to validate the value of the potentiometer and switch
// 			to the next primary color.
// 		◦ The button SW2 is used to switch from one LED to the next


spi_led_config_t g_ledConfig = {0};

uint8_t hexCharToValue(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return 0;
}

uint32_t extractColor(char* buffer)
{
	uint8_t r, g, b;

	// Extract hex values from positions 1-2 (R), 3-4 (G), 5-6 (B)
	r = (hexCharToValue(buffer[1]) << 4) | hexCharToValue(buffer[2]);
	g = (hexCharToValue(buffer[3]) << 4) | hexCharToValue(buffer[4]);
	b = (hexCharToValue(buffer[5]) << 4) | hexCharToValue(buffer[6]);

	return getRGB(r, g, b);
}

void	parseAndExecuteCommand(char* buffer, int* bufferIndex)
{
	if (ft_strcmp(buffer, "#FULLRAINBOW") == 0)
	{
		g_ledConfig.isRainbow = 1;
	}

	else if (!isValidCmdFormat(buffer, bufferIndex))
		uart_printstr("Invalid format, try again.\r\n");

	else
	{
		g_ledConfig.isRainbow = 0;
		uint32_t color = extractColor(buffer);
		if (buffer[8] == '6')
			g_ledConfig.colorD6 = color;
		if (buffer[8] == '7')
			g_ledConfig.colorD7 = color;
		if (buffer[8] == '8')
			g_ledConfig.colorD8 = color;
	}
}

int main()
{
	spi_init();
	uart_init();

	e_state state = PROMPT;
	char	buffer[BUFFER_SIZE];
	int	bufferIndex = 0;

	while (1)
	{
		switch (state)
		{
			case PROMPT:
			{
				ft_memset(buffer, '\0', BUFFER_SIZE);
				bufferIndex = 0;
				uart_printstr("> ");
				state = AWAIT_COMMAND;
				break;
			}
			case AWAIT_COMMAND:
			{
				handleUserTyping(buffer, &bufferIndex, &state);
				break;
			}
			case EXECUTE_COMMAND:
			{
				parseAndExecuteCommand(buffer, &bufferIndex);
				state = PROMPT;
				break;
			}
		}
		if (g_ledConfig.isRainbow)
		{
			wheel(g_ledConfig.rainbowPos++);
			_delay_ms(20);
		}
		spi_send_all_led_frames(g_ledConfig.colorD6, g_ledConfig.colorD7, g_ledConfig.colorD8);
	}
}


