#include <avr/io.h>
#include <avr/interrupt.h>
#include <utils/delay.h>

// • Create 2 strings, a username and a password.
// • Display a prompt on the serial port that asks for the username and password.
// • When typing the username, there should be an echo.
// • same for the password but with ’*’ instead of characters.
// • The Backspace key deletes a character.
// • The Enter key validates the input.
// • If the username and password are correct, the program displays the welcome text
//   and makes the LEDs blink.
//          Bonus points if you add a dramatic effect. ;)
// • Otherwise, the program displays the error message.

// ex:
// Enter your login:
// username: spectre
// password: ******
// Bad combination username/password
// Enter your login:
// username: spectre
// password: ******
// Hello spectre!
// Shall we play a game?

int main()
{
	

}
