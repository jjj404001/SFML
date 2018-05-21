#include "Game.h"

// Improvement
// 1.Range base loop (unordered_map)
// 2.Code guarding (in memory allocation & file opening)
//   -> if a key is already exist, delete a binding
// 3.Window m_isFocus value
// 4.Structs & Classes that does not need default Ctor
//   -> Always Initialize data members
// 5.Recommend to set a pointer to null 
//	 after deleting that pointer
// 6.Please, close files when you are done with it

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