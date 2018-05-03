#include "Game.h"
  
int main(int argc, char* argv[])
{
	Game game;
	game.GetWindow()->SetFrame(60);

	while (!game.GetWindow()->IsDone())
	{
		game.Update();
		game.Render();
		game.RestartClock();
	}
}