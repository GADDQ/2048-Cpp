/*
* Simple 2048 game wrote with C++.
* 
* Best with Visual Studio 2022 to compile.
* Or g++ complier if you are using Linux.
*/




#include <iostream>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

bool isGameOver{false};
int gameData[4][4];
int score{0};

void clear();

int randomNumber(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

void initGameData()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			gameData[i][j] = 0;
		}
	}
}

int getNextDirection()
{
	/* get input from user immediately, and turn it into directionID.
	  *here are the orgin ascii number:
	  *a=97
       w=119
	   s=115
       d=100
       *
       *and this is directionID:
       up:1
       left:2
       down:3
       right:4
	*/

	while (true) //this will exit when meet a vaild key.
	{
		switch (_getch())
		{
		case 119:
			return 1;
		case 97:
			return 2;
		case 115:
			return 3;
		case 100:
			return 4;
		}
	}
}

int whichNumber()
{
	if (randomNumber(1, 100) <= 10)
		return 4;
	else
		return 2;
}

void showGameInfo();

int getSize(int value)
{
	if (value == 0)
		return 1;
	int result{0};
	while (value != 0)
	{
		value /= 10;
		result++;
	}
	return result;
}

std::string drawColor(int number)
{
	int colorID;
	switch (number)
	{
	case 2:
		colorID = 15;
	case 4:
		colorID = 180;
		break;
	case 8:
		colorID = 178;
		break;
	case 16:
		colorID = 172;
		break;
	case 32:
		colorID = 208;
		break;
	case 64:
		colorID = 202;
		break;
	case 128:
		colorID = 228;
		break;
	case 256:
		colorID = 227;
		break;
	case 512:
	case 1024:
	case 2048:
		colorID = 226;
		break;
	case 4096:
		colorID = 196;
		break;
	case 8192:
		colorID = 160;
		break;
	case 16384:
		colorID = 124;
		break;
	case 32768:
		colorID = 45;
		break;
	case 65536:
		colorID = 39;
		break;
	case 131072:
		colorID = 33;
		break;
	default:
		colorID = 7;
		break;
	}
	return "\u001b[38;5;" + std::to_string(colorID) + "m" + std::to_string(number) + "\u001b[0m";
}

void renderGraphic(int data[4][4])
{
	/*
	* render game graphic.
	*/
	clear();

	std::cout << "  /$$$$$$   /$$$$$$  /$$   /$$  /$$$$$$ " << '\n';
	std::cout << " /$$__  $$ /$$$_  $$| $$  | $$ /$$__  $$" << '\n';
	std::cout << "|__/  \\ $$| $$$$\\ $$| $$  | $$| $$  \\ $$" << '\n';
	std::cout << "  /$$$$$$/| $$ $$ $$| $$$$$$$$|  $$$$$$/" << '\n';
	std::cout << " /$$____/ | $$\\ $$$$|_____  $$ >$$__  $$" << '\n';
	std::cout << "| $$      | $$ \\ $$$      | $$| $$  \\ $$" << '\n';
	std::cout << "| $$$$$$$$|  $$$$$$/      | $$|  $$$$$$/" << '\n';
	std::cout << "|________/ \\______/       |__/ \\______/ " << '\n';
	std::cout << '\n'<<'\n';
	std::cout << "Score: " << score;
	std::cout << '\n'<< '\n'<<'\n';

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << drawColor(data[i][j]);
			switch (getSize(data[i][j]))
			{
			//auto format
			case 1:
				std::cout << "      ";
				break;
			case 2:
				std::cout << "     ";
				break;
			case 3:
				std::cout << "    ";
				break;
			case 4:
				std::cout << "   ";
				break;
			case 5:
				std::cout << "  ";
				break;
			case 6:
				std::cout << " ";
				break;
			}
		}
		std::cout << '\n'<< '\n';
	}
}

bool findEmptyBlock(int data[4][4], bool setNewNumber)
{
	/*
	* find a empty block.
	* if setNewNumber set to true, then will generate a new number after find a empty block.
	* true for sucess and false for fail.
	*/

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (data[i][j] == 0)
			{
				if (setNewNumber)
					data[i][j] = whichNumber();
				return true;
			}
		}
	}
	return false;
}

bool generateNewNumber(int data[4][4])
{
	/*
	* generate a new number block in the game
	* need a gameData to identify which game will generate in.
	* if sucess, return true. if not, return false
	*/
	bool isSucess{false};
	int x;
	int y;
	//choose random number to spawn
	for (int i = 0; i < 5; i++)
	{
		x = randomNumber(0, 3);
		y = randomNumber(0, 3);
		if (data[x][y] == 0)
		{
			data[x][y] = whichNumber();
			isSucess = true;
			break;
		}
	}

	//maybe just bad luck... try all position
	if (!isSucess)
		if (findEmptyBlock(data, true))
			isSucess = true;
	//still not? gg...
	return isSucess;
}

bool checkAndMoveBlock(int data[4][4], int direction)
{
	bool isAlreadyCombineOnce{false};
	bool isMoveSucess{false};
	if (!isGameOver)
	{
		switch (direction)
		{
		case 0:
			for (int j = 0; j < 4; j++)
			{
				if (isMoveSucess)
					break;

				for (int i = 1; i < 4; i++)
				{
					if (data[i][j] == 0 || i == 0)
						continue;
					if (data[i - 1][j] == 0)
					{
						isMoveSucess = true;
						break;
					}
					else if (data[i - 1][j] == data[i][j])
					{
						if (!isAlreadyCombineOnce)
						{
							isMoveSucess = true;
							break;
						}
					}
				}
			}
			if (isMoveSucess)
				break;

			for (int i = 0; i < 4; i++)
			{
				if (isMoveSucess)
					break;

				for (int j = 1; j < 4; j++)
				{
					if (data[i][j] == 0 || j == 0)
						continue;
					if (data[i][j - 1] == 0)
					{
						isMoveSucess = true;
						break;
					}
					else if (data[i][j - 1] == data[i][j])
					{
						if (!isAlreadyCombineOnce)
						{
							isMoveSucess = true;
							break;
						}
					}
				}
			}
			if (isMoveSucess)
				break;

			for (int j = 0; j < 4; j++)
			{
				if (isMoveSucess)
					break;

				for (int i = 2; i >= 0; i--)
				{
					if (data[i][j] == 0 || i == 3)
						continue;
					if (data[i + 1][j] == 0)
					{
						isMoveSucess = true;
						break;
					}
					else if (data[i + 1][j] == data[i][j])
					{
						if (!isAlreadyCombineOnce)
						{
							isMoveSucess = true;
							break;
						}
					}
				}
			}
			if (isMoveSucess)
				break;

			for (int i = 0; i < 4; i++)
			{
				for (int j = 2; j >= 0; j--)
				{
					if (data[i][j] == 0 || j == 3)
						continue;
					if (data[i][j + 1] == 0)
					{
						isMoveSucess = true;
						break;
					}
					else if (data[i][j + 1] == data[i][j])
					{
						if (!isAlreadyCombineOnce)
						{
							isMoveSucess = true;
							break;
						}
					}
				}
			}
			break;

		case 1:
			for (int j = 0; j < 4; j++)
			{
				for (int i = 1; i < 4; i++)
				{
					if (data[i][j] == 0 || i == 0)
						continue;
					if (data[i - 1][j] == 0)
					{
						isMoveSucess = true;
						data[i - 1][j] = data[i][j];
						data[i][j] = 0;
						i -= 2;
					}
					else if (data[i - 1][j] == data[i][j])
					{
						if (!isAlreadyCombineOnce)
						{
							isMoveSucess = true;
							data[i][j] = 0;
							data[i - 1][j] *= 2;
							score += data[i - 1][j];
							isAlreadyCombineOnce = true;
						}
						else
							isAlreadyCombineOnce = false;
					}
					else
						isAlreadyCombineOnce = false;
				}
				isAlreadyCombineOnce = false;
			}
			break;
			
		case 2:
			for (int i = 0; i < 4; i++)
			{
				for (int j = 1; j < 4; j++)
				{
					if (data[i][j] == 0 || j == 0)
						continue;
					if (data[i][j - 1] == 0)
					{
						isMoveSucess = true;
						data[i][j - 1] = data[i][j];
						data[i][j] = 0;
						j -= 2;
					}
					else if (data[i][j - 1] == data[i][j])
					{
						if (!isAlreadyCombineOnce)
						{
							isMoveSucess = true;
							data[i][j] = 0;
							data[i][j - 1] *= 2;
							score += data[i][j - 1];
							isAlreadyCombineOnce = true;
						}
						else
							isAlreadyCombineOnce = false;
					}
					else
						isAlreadyCombineOnce = false;
				}
				isAlreadyCombineOnce = false;
			}
			break;
			
		case 3:
			for (int j = 0; j < 4; j++)
			{
				for (int i = 2; i >= 0; i--)
				{
					if (data[i][j] == 0 || i == 3)
						continue;
					if (data[i + 1][j] == 0)
					{
						isMoveSucess = true;
						data[i + 1][j] = data[i][j];
						data[i][j] = 0;
						i += 2;
					}
					else if (data[i + 1][j] == data[i][j])
					{
						if (!isAlreadyCombineOnce)
						{
							isMoveSucess = true;
							data[i][j] = 0;
							data[i + 1][j] *= 2;
							score += data[i + 1][j];
							isAlreadyCombineOnce = true;
						}
						else
							isAlreadyCombineOnce = false;
					}
					else
						isAlreadyCombineOnce = false;
				}
				isAlreadyCombineOnce = false;
			}
			break;
			
		case 4:
			for (int i = 0; i < 4; i++)
			{
				for (int j = 2; j >= 0; j--)
				{
					if (data[i][j] == 0 || j == 3)
						continue;
					if (data[i][j + 1] == 0)
					{
						isMoveSucess = true;
						data[i][j + 1] = data[i][j];
						data[i][j] = 0;
						j += 2;
					}
					else if (data[i][j + 1] == data[i][j])
					{
						if (!isAlreadyCombineOnce)
						{
							isMoveSucess = true;
							data[i][j] = 0;
							data[i][j + 1] *= 2;
							score += data[i][j + 1];
							isAlreadyCombineOnce = true;
						}
						else
							isAlreadyCombineOnce = false;
					}
					else
						isAlreadyCombineOnce = false;
				}
				isAlreadyCombineOnce = false;
			}
			break;
		}
	}
	return isMoveSucess;
}

int main()
{
	srand(time(0));
	initGameData();
	showGameInfo();
	_getch();
	clear(); //press any key to start,and clear screen
	generateNewNumber(gameData);
	while (!isGameOver)
	{
		if (!generateNewNumber(gameData))
			if (!checkAndMoveBlock(gameData, 0))
				isGameOver = true;
		renderGraphic(gameData);
	move:
		if (!checkAndMoveBlock(gameData, getNextDirection()))
			if (findEmptyBlock(gameData, false))
				goto move;
	}
	clear();
	std::cout << "Game Over!";
	return 0;
}

void showGameInfo()
{
	std::cout << "  /$$$$$$   /$$$$$$  /$$   /$$  /$$$$$$ " << '\n';
	std::cout << " /$$__  $$ /$$$_  $$| $$  | $$ /$$__  $$" << '\n';
	std::cout << "|__/  \\ $$| $$$$\\ $$| $$  | $$| $$  \\ $$" << '\n';
	std::cout << "  /$$$$$$/| $$ $$ $$| $$$$$$$$|  $$$$$$/" << '\n';
	std::cout << " /$$____/ | $$\\ $$$$|_____  $$ >$$__  $$" << '\n';
	std::cout << "| $$      | $$ \\ $$$      | $$| $$  \\ $$" << '\n';
	std::cout << "| $$$$$$$$|  $$$$$$/      | $$|  $$$$$$/" << '\n';
	std::cout << "|________/ \\______/       |__/ \\______/ " << '\n';
	std::cout << '\n'<<'\n';
	std::cout << "A simple 2048 game wrote with C++.\nUse W, A, S, D to control.\nPress any key to start...";
	std::cout<<'\n'<<'\n';
	std::cout<<"Please notice, if you are using Windows, you have to use PowerShell 7+ to make sure game can render color correctly.";
}

void clear()
{
#ifdef _WIN32
#ifdef _WIN64
	system("cls");
#else
	system("cls");
#endif
#else
	system("clear");
#endif
}