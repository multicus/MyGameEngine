// MyGame123.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Game.h"
#include <thread>
//#include "DevTools.h"
#include <Windows.h>

//void task1()
//{
//	char hey;
//	std::cin.get(hey);
//	std::cout << hey << '\n';
//}

int main(int argc, char *argv[])
{
	//std::thread t1(task1);

	try
	{
		Game game;
		game.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}


	return 0;
}

