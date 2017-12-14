//
// The screen the player sees when he dies
//

#include "Menu.h"
#include "TextButtonObject.h"

class DiedMenu : public Menu
{
private:
	sf::RectangleShape m_blackFade;
	// It's a percentage
	float m_fadeTimeline { 0.f };
	const float m_fadeTime{ 8.f };

	std::vector<TextButtonObject*> m_buttons;

public: 
	DiedMenu(MenuStack *const menuStack);
	virtual ~DiedMenu() override final;

	virtual void input(sf::RenderWindow &window) override final;
	virtual void update(const float elapsedTime) override final;
	virtual void draw(sf::RenderWindow &window) override final;
};