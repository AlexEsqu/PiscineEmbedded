// PC5 : SDA / ADC4, data of the Two Wire
// PC6 : DCL / ADC5, clock of the Two Wire


// PROGRAM STARTS

typedef enum
{
	START,					// program started, nothing received from other board
	FIRST_READY,			// has sent first comm, awaiting response from other player
	LAST_READY,				// has received first comm from other board
	COUNTDOWN,				// Start led countdown
	RACE,					// Await player button push
	PODIUM,					// Display which player won and which lost
	ERROR,					// Problem, display red led
} e_state;



// int main(void)
// {
// 	e_state state = START;

// 	switch state
// 	{
// 		case START:
// 		{
// 			// Open receive and transmit port
// 			// Allow button to send message
// 			// Await a message from other board

// 			// if (receive message)
// 			// case = LAST_READY
// 			// if (message sent is acknowledged)
// 			// case = FIRST_READY
// 			break;
// 		}
// 		case FIRST_READY:
// 		{
// 			// set up as controller / master
// 			break;
// 		}
// 		case LAST_READY:
// 		{
// 			// set up as target / slave
// 			break;
// 		}
// 		case COUNTDOWN:
// 		{
// 			// timer for race start
// 			// led display countdown
// 			break;
// 		}
// 		case RACE:
// 		{
// 			// while (1)
// 			// await first button push (acknowledgement ?)
// 			[[fallthrough]]
// 		}
// 		case PODIUM:
// 		{
// 			// display led RED for loser, GREEN / GAYMER for winner
// 			break;
// 		}
// 		default:
// 		{
// 			// display orange led
// 		}
// 	}
// }


int main()
{

}
