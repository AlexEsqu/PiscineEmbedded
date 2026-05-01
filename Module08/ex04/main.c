#include "libalex_avr.h"

// • You must create a program that listens on the serial port.
// • This program should modify the color of the LEDs D6, D7 or D8 based on the value
// 	 sent by the standard input.
// • You should display an RGB HEX color in the format #RRGGBBDX.
// • DX will correspond to the value of the LED you wish to modify (D6/D7/D8).
// • If the program reads #FULLRAINBOW on the standard input, the LEDs should light
// 	 up according to a wheel() function that you will have created during exercise 2 of
// 	 Module04.
// • The program should display an error message if the parameter sent on the standard
// 	 input is not readable with the usage.
//
// #000000D6
// #35aaaaD7
// #221299D8
// ...
// #FULLRAINBOW


spi_led_config_t g_ledConfig = {0};


void wheel(uint8_t pos)
{
	uint32_t color = 0;
	pos = 255 - pos;

	if (pos < 85)
	{
		color = getRGB(255 - pos * 3, 0, pos * 3);
	}
	else if (pos < 170)
	{
		pos = pos - 85;
		color = getRGB(0, pos * 3, 255 - pos * 3);
	}
	else
	{
		pos = pos - 170;
		color = getRGB(pos * 3, 255 - pos * 3, 0);
	}
	g_ledConfig.colorD6 = color;
	g_ledConfig.colorD7 = color;
	g_ledConfig.colorD8 = color;
}

char	isValidCmdFormat(char* buffer, int* bufferIndex)
{
	if (*bufferIndex != 9 || buffer[0] != '#')
		return 0;

	for (int i = 1; i < SIZE_OF_COLOR_FORMAT + 1; i++)
	{
		if (!isHexDigit(buffer[i]))
			return 0;
	}

	if (buffer[7] != 'D')
		return 0;

	if (buffer[8] < '6' || buffer[8] > '8')
		return 0;

	return 1;
}

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

// INTERRUPT ON TIMER1 A EVERY 10ms
// Using timer1 since biggest and can count per seconds
void	__attribute__((signal)) __vector_timer1_compa(void)
{
	if (g_ledConfig.isRainbow)
	{
		g_ledConfig.rainbowPos++;
		wheel(g_ledConfig.rainbowPos);
	}
}

void	setGeneralTimer()
{
	// Set up timer1
	timer1_init(TIMER_MODE_CTC, TOP_OCRA, CMP_DISCONNECT, CMP_DISCONNECT);

	// set timer1 TOP to 100 ms
	OCR1A = (F_CPU / (1024UL * 10UL)) - 1;

	// set timer1 to compare to the TOP value in OCR1A
	TIMSK1 |= (1 << OCIE1A);

	// launch by setting up the timer1 prediviser
	timer1_launch(CLK_DIV1024);
}

int main()
{
	spi_init();
	uart_init();
	// setGeneralTimer();

	// SREG |= (1 << 7);
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


