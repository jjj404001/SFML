#include "Game.h"

//Simple main
int main(int argc, int* argv[])
{
	Game game;

	//FrameLimit
	game.SetFrameRate(60);

	//Game Loop
	while (!game.GetWindow()->IsDone())
	{
		game.Update();
		game.Draw();
		game.RestartClock();
	}
}