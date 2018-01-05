#include "CreditsMenu.h"
#include "Config.h"


void CreditsMenu::input(sf::RenderWindow & window)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
				window.close();
			break;
		}
	}
}

void CreditsMenu::update(const float elapsedTime)
{
}

void CreditsMenu::draw(sf::RenderWindow & window)
{
	window.setView(window.getDefaultView());
	window.clear(sf::Color::Black);
	for (const auto &iter : m_text)
		window.draw(iter);
}

CreditsMenu::CreditsMenu(MenuStack * const menuStack):
	Menu(menuStack,std::string("Runaway - Credits"))
{
	Config::getInstance().setConfig("coin7", Item(true));

	m_text.push_back(TextObject("WHEN THE CORE GOT DEFEATED, LIGHT RETURNED AROUND YOU",true));
	m_text.back().setPos(sf::Vector2f(42, 37));
	m_text.push_back(TextObject("THE DARKNESS GOT DRIVEN AWAY, THE LAND FINALLY", true));
	m_text.back().setPos(sf::Vector2f(126, 94));	
	m_text.push_back(TextObject("WOKE UP", true));
	m_text.back().setPos(sf::Vector2f(534, 148));

	m_text.push_back(TextObject("THANK YOU FOR PLAYING MY GAME!", true));
	m_text.back().setPos(sf::Vector2f(27,273));
	m_text.push_back(TextObject("IT IS MY FIRST GAME AND I WANTED TO DO MANY THINGS", true));
	m_text.back().setPos(sf::Vector2f(27, 320));
	m_text.push_back(TextObject("AT THE SAME TIME, THAT'S WHY IT TURNED FROM A PUZZLE", true));
	m_text.back().setPos(sf::Vector2f(27, 365));
	m_text.push_back(TextObject("GAME TO A PLATFROMER. I ORIGINALLY STARTED THIS PROJECT", true));
	m_text.back().setPos(sf::Vector2f(27, 411));
	m_text.push_back(TextObject("WITH NO GAMEPLAY IN MIND. SO THAT'S WHY THE STORY, IF THERE", true));
	m_text.back().setPos(sf::Vector2f(27, 460));
	m_text.push_back(TextObject("REALLY WAS ANY, FEELS SO CHEAP. AS DO THE GAMEPLAY MECHANICS", true));
	m_text.back().setPos(sf::Vector2f(27, 505));
	m_text.push_back(TextObject("BUT I AM HAPPY THAT TO BE ABLE TO SAY THAT I HAVE MADE A", true));
	m_text.back().setPos(sf::Vector2f(27, 550));
	m_text.push_back(TextObject("PLAYABLE GAME FROM SCRATCH, EVENT IF IT'S NOT ANY FUN", true));
	m_text.back().setPos(sf::Vector2f(27, 600));
	m_text.push_back(TextObject("FOR ME, IT IS A GREAT ACHIEVEMENT, THANK YOU", true));
	m_text.back().setPos(sf::Vector2f(27, 640));

	for (auto &iter : m_text)
		iter.setTextSize(20);
}