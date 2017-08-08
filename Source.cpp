//
// Start of the program.
// Initiates the stack.
//

#include "MenuStack.h"

int main() 
{
	MenuStack menuStack;
	menuStack.push();

	sf::RenderWindow window(sf::VideoMode(1280,720),"Runaway",sf::Style::Resize);
	while (window.isOpen())
	{
		menuStack.peek()->run(window);
	}

	return 0;
}