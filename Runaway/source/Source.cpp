//
// Start of the program
// Initiates the stack and game loop
//

#include "MenuStack.h"
#include "MainMenu.h"

int main() 
{
	sf::RenderWindow window(sf::VideoMode(1280,720),"Runaway",sf::Style::Default);
	sf::Clock time;
	float elapsedTime;
	MenuStack menuStack;
	
	window.setFramerateLimit(60);
	menuStack.push(new MainMenu(&menuStack));
	time.restart();

	while (window.isOpen())
	{
		// Used for animation and consistent movement over all fps values
		elapsedTime = time.restart().asSeconds();

		// Failsave
		if (menuStack.peek() == nullptr) continue;

		// Logic
		menuStack.peek()->input(window);
		menuStack.peek()->update(elapsedTime);

		// Draw
		window.clear(sf::Color::Black);
		menuStack.peek()->draw(window);
		window.display();

		if (menuStack.peek()->isPop())
		{
			menuStack.pop();
		}
	}

	return 0;
}