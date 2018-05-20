#include "Game.h"

//Our simple main function
int main(int argc, int* argv[])
{
	Game game;

	//FrameLimit setting
	game.SetFrameRate(60);

	//Unless game is not done
	while (!game.GetWindow()->IsDone())
	{
		//Update & Draw
		game.Update();
		game.Draw();
		game.RestartClock();
	}
}