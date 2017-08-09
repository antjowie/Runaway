//
// Start of the program
// Initiates the stack
//

#include "MenuStack.h"

int main() 
{
	sf::RenderWindow window(sf::VideoMode(1280,720),"Runaway",sf::Style::Resize);
	sf::Clock time;
	sf::Time elapsedTime;
	MenuStack menuStack;
	
	menuStack.push();
	time.restart();

	while (window.isOpen())
	{
		// Used for animation and consistent movement over all fps values
		elapsedTime = time.restart();

		// Logic
		menuStack.peek()->input(window);
		menuStack.peek()->update(elapsedTime);

		// Draw
		window.clear(sf::Color::Black);
		menuStack.peek()->draw(window);
		window.display();
	}

	return 0;
}