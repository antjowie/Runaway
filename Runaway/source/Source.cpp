//
// Start of the program
// Initiates the stack and game loop
//

#include "MenuStack.h"
#include "MainMenu.h"
#include "Config.h"

int main() 
{
	Config::getInstance().loadConfig();
	sf::RenderWindow window;
	window.create(sf::VideoMode(1280, 720), "Runaway", sf::Style::Default);
	
	sf::Clock time;
	float elapsedTime;
	float elapsedRenderTime{ 0 };
	MenuStack menuStack;
	
	const float frameLimit{ static_cast<float>(Config::getInstance().getConfig("frameLimit").integer) }; // So that our keyboard pollrate will not be tied to loop
	window.setFramerateLimit(static_cast<unsigned>(frameLimit));
	menuStack.push(new MainMenu(&menuStack));
	time.restart();

	while (window.isOpen())
	{
		// Used for animation and consistent movement over all fps values
		elapsedTime = time.restart().asSeconds();

		// If user pauses the program
		if (elapsedTime > 0.2f)	elapsedTime = 1.f / frameLimit;

		elapsedRenderTime += elapsedTime;

		// Failsave
		if (menuStack.peek() == nullptr) break;
		


		// Logic
		menuStack.peek()->input(window);
		menuStack.peek()->update(elapsedTime);

		// Draw
		if (elapsedRenderTime > (1 / frameLimit))
		{
			elapsedRenderTime = fmod(elapsedRenderTime,(1 / frameLimit));
			window.clear(sf::Color::Red);
			menuStack.peek()->draw(window);
			window.display();
		}

		if (menuStack.peek()->isPop())
		{
			menuStack.pop();
		}
	}

	Config::getInstance().saveConfig();
	
	return 0;
}