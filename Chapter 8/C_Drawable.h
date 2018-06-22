#ifndef C_DRAWABLE_H
#define C_DRAWABLE_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "C_Base.h"

class C_Drawable : public C_Base
{
public:
	C_Drawable(const Component & type) : C_Base(type) {}
	virtual ~C_Drawable() {}

	virtual void UpdatePosition(const sf::Vector2f & vec) = 0;
	virtual const sf::Vector2i & GetSize() = 0;
	virtual void Draw(sf::RenderWindow * wind) = 0;

private:

};

#endif