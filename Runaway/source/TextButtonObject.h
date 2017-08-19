//
// Class made for interactive buttons
// Type of object
//

#pragma once
#include "TextObject.h"

enum class Function { Play, Options, Quit, Back, Nothing };

class TextButtonBody
{
private:
	float m_fade{ 0 };
	bool m_hover{ false };
	bool m_isClicked{ false };
	Function m_function{ Function::Nothing };

	sf::IntRect m_body;
	sf::RectangleShape m_bodyTexture;

public:
	void setBody(const sf::IntRect &bodyPos);
	void setFunction(const Function function);

	Function getFunction() const; // Returns corresponding function if clicked
	sf::IntRect const &getBody() const;
	sf::RectangleShape const &getBodyTexture() const;

	void draw(sf::RenderWindow &window);
	void logic(const float elapsedTime);
	void input(sf::RenderWindow &window);
};

class TextButtonObject :
	public TextObject
{
public:
	TextButtonBody m_textButtonBody;

	TextButtonObject(const bool isValid = false);
	TextButtonObject(const std::string &text, const bool isValid = false);
	TextButtonObject(const std::string &text, const Function function, const bool isValid = false);
	
	virtual void draw(sf::RenderWindow &window);
	virtual void logic(const float elapsedTime);
	virtual void input(sf::RenderWindow& window);
};