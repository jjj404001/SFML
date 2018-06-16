#ifndef ECS_TYPES_H
#define ECS_TYPES_H

using ComponentType = unsigned int;
#define N_COMPONENT_TYPES 32

enum class Component {
	Position = 0, SpriteSheet, State, Movable, Controller, Collidable
};

enum class System {
	Renderer = 0, Movement, Collision, Control, State, SheetAnimation
};

#endif