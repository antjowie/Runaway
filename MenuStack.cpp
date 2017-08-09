#include "MenuStack.h"

MenuStack::~MenuStack()
{
	while (!m_menuStack.empty())
		pop();
}

Menu * MenuStack::peek()
{
	return m_menuStack.empty() == true ? nullptr : m_menuStack.top();
}

void MenuStack::pop()
{
	if (m_menuStack.empty())
		return;

	delete m_menuStack.top();
	m_menuStack.top() = nullptr; // Probably unnecessary
	m_menuStack.pop();
}

void MenuStack::push(Menu * newMenu)
{
	m_menuStack.push(newMenu);
}
