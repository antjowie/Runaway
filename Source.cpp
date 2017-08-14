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
	MenuStack m_menuStack;
	
	window.setFramerateLimit(60);
	m_menuStack.push(new MainMenu(&m_menuStack));
	time.restart();

	while (window.isOpen())
	{
		// Used for animation and consistent movement over all fps values
		elapsedTime = time.restart().asSeconds();

		// Failsave
		if (m_menuStack.peek() == nullptr) continue;

		// Logic
		m_menuStack.peek()->input(window);
		m_menuStack.peek()->update(elapsedTime);

		// Draw
		window.clear(sf::Color::Black);
		m_menuStack.peek()->draw(window);
		window.display();

		if (m_menuStack.peek()->isPop())
		{
			m_menuStack.pop();
		}
	}

	return 0;
}