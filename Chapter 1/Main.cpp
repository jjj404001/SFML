#include <SFML/Graphics.hpp>

void main(int argc, char** argv[])
{
	//RenderWindow Constructor 
	sf::RenderWindow window(sf::VideoMode(640, 480), 
							"Rendering the rectangle");

	/*
	//RectangleShape Constructor                  (size)   
	sf::RectangleShape rectangle(sf::Vector2f(128.0f, 128.0f));
	rectangle.setFillColor(sf::Color::Red);
	rectangle.setPosition(320, 240);
	rectangle.setOrigin(rectangle.getSize().x / 2,
						rectangle.getSize().y / 2);
	*/

	//Mushroom
	sf::Texture mushroomTexture;
	mushroomTexture.loadFromFile("Mushroom.png");
	sf::Sprite mushroom(mushroomTexture);
	sf::Vector2u size = mushroomTexture.getSize();
	mushroom.setOrigin(size.x / 2, size.y / 2);
	sf::Vector2f increment(0.2f, 0.2f);

	//Game Loop Begins
	while (window.isOpen())
	{
		//Event Constructor
		sf::Event event;

		while (window.pollEvent(event)) //Catch event?
		{
			if (event.type == sf::Event::Closed)
			{
				//Close window button clicked
				window.close();
			}
		}

		if ((mushroom.getPosition().x + (size.x / 2) >
			window.getSize().x && increment.x > 0) ||
			(mushroom.getPosition().x - (size.x / 2) < 0 &&
				increment.x < 0))
		{
			//Reverse the direction on X axis
			increment.x = -increment.x;
		}

		if ((mushroom.getPosition().y + (size.y / 2) >
			window.getSize().y && increment.y > 0) ||
			(mushroom.getPosition().y - (size.y / 2) < 0 &&
				increment.y < 0))
		{
			//Reverse the direction on Y axis
			increment.y = -increment.y;
		}

		mushroom.setPosition(mushroom.getPosition() + increment);
		mushroom.setColor(sf::Color(255, 0, 0, 255));

		//default is black: window.clear() -> black!!
		//window.clear(sf::Color(0, 0, 0, 255));
		window.clear(sf::Color(16, 16, 16, 255)); // Dark grey

		// Drawing our sprite
		window.draw(mushroom);

		//render our shape
		//window.draw(rectangle);
		window.display();
	}
}