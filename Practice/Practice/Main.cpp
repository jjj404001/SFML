#include "Game.h"

int main(int argc, int* argv[])
{
	Game game;

	while (!game.GetWindow()->IsDone())
	{
		game.Update();
		game.Draw();
	}
}