#include "DiedMenu.h"

DiedMenu::DiedMenu(MenuStack *const menuStack):
	Menu::Menu(menuStack,"Runaway - you died")
{
	// Constants for the buttons
	const int height{ 200 };
	const int textOffset{ 20 };
	const int textSize{ 50 };

	TextObject *title = new TextObject("YOU DIED", true);
	title->setTextSize(80);
	title->setPos(sf::Vector2f(1280/2-300, 720 - height * 3 + 40));
	pushObject(title);

	m_buttons.push_back(new TextButtonObject("CONTINUE", Function::Continue, true));
	m_buttons.push_back(new TextButtonObject("RETURN TO MAIN MENU", Function::Back, true));

	for (size_t i{ 0 }; i < m_buttons.size(); i++) 
	{
		sf::Vector2f pos{ 0,720.f - height * 2.f + static_cast<float>(i * height) };
		TextButtonObject *iter{ static_cast<TextButtonObject*>(m_buttons[i]) };
		
		// Rect
		iter->setBody(sf::IntRect(pos.x,pos.y, 1280, height));
		
		// Background
		iter->m_bodyTexture.setPosition(pos);
		iter->m_bodyTexture.setSize(sf::Vector2f(1280, height));
	
		// Text
		iter->setTextSize(50);
		iter->setPos(sf::Vector2f(textOffset, pos.y + height/2 - textSize / 2));
	}
}

DiedMenu::~DiedMenu()
{
	Menu::~Menu();
	for (auto &deleter : m_buttons)
		delete deleter;
	m_buttons.clear();
}

void DiedMenu::input(sf::RenderWindow & window)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;

		}
	}

	for (const auto &iter : m_buttons)
		iter->input(window);
}
void DiedMenu::update(const float elapsedTime)
{
	for (const auto &iter : m_buttons)
	{
		iter->logic(elapsedTime);
		if(iter->getFunction() != Function::Nothing)
			switch (iter->getFunction())
			{
				case Function::Continue:
				break;

				case Function::Back:
				break;
			}
	}
}

void DiedMenu::draw(sf::RenderWindow & window)
{
	// To update title and display title
	// Note to self
	// The way objects are managed takes away a lot of freedom, the object manager was meant as a way
	// to stop having to make class variables to keep track of our 'objects'. But in the end this was a way easier approach.
	// Because of this many objects got their own little storage handlers (mostly vectors or lists). 
	// In my next project I have a way better way to manage it, but if I change it now, I have to change everything that uses and doesn't
	// use the object manager. And that didn't seem to be worth it to me.
	Menu::draw(window);

	window.setView(window.getDefaultView());
	for (const auto &iter : m_buttons)
		window.draw(*iter);
}
