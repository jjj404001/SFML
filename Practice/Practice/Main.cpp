#include "Game.h"

//Our simple main
int main(int argc, int* argv[])
{
	Game game;

	//Unless game is not done
	while (!game.GetWindow()->IsDone())
	{
		//Update & Draw
		game.Update();
		game.Draw();
	}
}