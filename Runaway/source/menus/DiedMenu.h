//
// The screen the player sees when he dies
//

#include "Menu.h"
#include "TextButtonObject.h"

class DiedMenu : public Menu
{
private:
	std::vector<TextButtonObject*> m_buttons;

public: 
	DiedMenu(MenuStack *const menuStack);
	virtual ~DiedMenu() override final;

	virtual void input(sf::RenderWindow &window) override final;
	virtual void update(const float elapsedTime) override final;
	virtual void draw(sf::RenderWindow &window) override final;
};