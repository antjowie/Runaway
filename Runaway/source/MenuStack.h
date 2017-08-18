//
// MenuStack class file
//

#pragma once
#include "Menu.h"

#include <stack>

class MenuStack
{
private:
	std::stack<Menu*> m_menuStack;

public:
	~MenuStack();

	Menu* peek();
	void pop();
	void push(Menu *newMenu);
};

