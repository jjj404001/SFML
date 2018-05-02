#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <vector>
#include <SFML/Graphics.hpp>

using MessageContainer = std::vector<std::string>;

class Textbox
{
public:
	Textbox();
	Textbox(int visible, int charSize,
		int width, sf::Vector2f screenPos);

	~Textbox();
	void Setup(int visible, int charSize,
		int width, sf::Vector2f screenPos);

	void Add(std::string message);
	void Clear();

	void Render(sf::RenderWindow & wind);

private:
	MessageContainer m_messages;
	int m_numVisible;

	sf::RectangleShape m_backdrop;
	sf::Font m_font;
	sf::Text m_content;
};

#endif