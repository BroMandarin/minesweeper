#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <vector>
#include <Windows.h>
#include "Mines.h"
using namespace std;

void placeNumbers(Block ** field, int size)
{
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			if (!field[y][x].mine)
			{
				int number = 0;

				for (int offsetY = -1; offsetY <= 1; offsetY++)
				{
					for (int offsetX = -1; offsetX <= 1; offsetX++)
					{
						if (x + offsetX >= 0 && x + offsetX < size &&
							y + offsetY >= 0 && y + offsetY < size)
						{
							if (field[y + offsetY][x + offsetX].mine)
								number++;
						}
					}
				}
				field[y][x] = Block(x, y, number);
			}
		}
	}
}

void placeMines(Block **field, int size, int level, int &mines)
{
	srand(time(0));

	int r;
	mines = 0;

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			r = rand() % 10 + 1;
			if ((level == 1 && r <= 1) || (level == 2 && r <= 2) || (level == 3 && r <= 3))
			{
				field[y][x] = Block(x, y, true);
				mines++;
			}
			else
				field[y][x] = Block(x, y, false);
		}
	}
}

void generateField(Block **field, int size, int level, int &mines)
{
	placeMines(field, size, level, mines);
	placeNumbers(field, size);
}

void drawField(Block **field, int size, int mines, int flags)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 135);

	cout << (char)218;
	for (int i = 0; i < size; i++)
		cout << (char)196;
	cout << (char)191;
	if (mines == flags)
		SetConsoleTextAttribute(hConsole, 138);
	else
		SetConsoleTextAttribute(hConsole, 140);
	cout << "\tMines: " << flags << "/" << mines << endl;
	SetConsoleTextAttribute(hConsole, 135);

	for (int y = 0; y < size; y++)
	{
		cout << (char)179;
		for (int x = 0; x < size; x++)
		{
			//SetConsoleTextAttribute(hConsole, 160);
			if (field[y][x].state == NONE)
			{
				SetConsoleTextAttribute(hConsole, 112);
				SetConsoleTextAttribute(hConsole, 135);
				cout << (char)219;
			}
			else if (field[y][x].state == FLAG)
			{
				SetConsoleTextAttribute(hConsole, 124);
				cout << (char)16;
			}
			else if (field[y][x].state == DIG)
			{
				SetConsoleTextAttribute(hConsole, 128);
				if (field[y][x].mine)
				{
					SetConsoleTextAttribute(hConsole, 192);
					cout << (char)15;
				}
				else if (field[y][x].number != 0)
				{
					switch (field[y][x].number)
					{
					case 1:
						SetConsoleTextAttribute(hConsole, 137);
						break;
					case 2:
						SetConsoleTextAttribute(hConsole, 138);
						break;
					case 3:
						SetConsoleTextAttribute(hConsole, 140);
						break;
					case 4:
						SetConsoleTextAttribute(hConsole, 129);
						break;
					case 5:
						SetConsoleTextAttribute(hConsole, 132);
						break;
					case 6:
						SetConsoleTextAttribute(hConsole, 139);
						break;
					case 7:
						SetConsoleTextAttribute(hConsole, 128);
						break;
					case 8:
						SetConsoleTextAttribute(hConsole, 135);
						break;
					}
					cout << field[y][x].number;
				}
				else
					cout << '.';
			}
		}
		SetConsoleTextAttribute(hConsole, 135);
		cout << (char)179 << endl;
	}

	cout << (char)192;
	for (int i = 0; i < size; i++)
		cout << (char)196;
	cout << (char)217 << endl;
}

void showField(Block **field, int size)
{
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			if (field[y][x].mine)
				cout << '*';
			else
				cout << field[y][x].number;
		}
		cout << endl;
	}
}

void dig(Block **field, int size, int xDig, int yDig)
{
	field[yDig][xDig].state = DIG;

	if (!field[yDig][xDig].mine && field[yDig][xDig].number == 0)
	{
		vector<Block> frontier;
		vector<Block> visited;
		Block current = field[yDig][xDig];

		frontier.push_back(current);
		visited.push_back(current);

		while (frontier.size() > 0)
		{
			current = frontier[frontier.size() - 1];
			frontier.resize(frontier.size() - 1);

			for (int y = -1; y <= 1; y++)
			{
				for (int x = -1; x <= 1; x++)
				{
					if (current.x + x >= 0 && current.x + x < size &&
						current.y + y >= 0 && current.y + y < size)
					{
						Block next = field[current.y + y][current.x + x];

						bool newBlock = true;
						for (int i = 0; i < visited.size(); i++)
						{
							if (visited[i] == next)
							{
								newBlock = false;
								break;
							}
						}
						if (newBlock)
						{
							if (next.number == 0)
								frontier.push_back(next);
							visited.push_back(next);
							field[next.y][next.x].state = DIG;
						}
					}
				}
			}
		}
	}
}

int flag(Block **field, int size, int xFlag, int yFlag)
{
	if (field[yFlag][xFlag].state == NONE)
	{
		field[yFlag][xFlag].state = FLAG;
		return 1;
	}
	else if (field[yFlag][xFlag].state == FLAG)
	{
		field[yFlag][xFlag].state = NONE;
		return -1;
	}
	return 0;
}

void revealMines(Block **field, int size, int xRed, int yRed)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			if (x == xRed && y == yRed)
				SetConsoleTextAttribute(hConsole, 192);
			else
				SetConsoleTextAttribute(hConsole, 128);
			if (field[y][x].mine)
				field[y][x].state = DIG;
		}
	}
}

bool checkForWin(Block **field, int size)
{
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			if (field[y][x].mine)
			{
				if (field[y][x].state == FLAG)
					continue;
				return false;
			}
		}
	}
	return true;
}

void cleanField(Block **field, int size)
{
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			if (!field[y][x].mine)
				field[y][x].state = DIG;
		}
	}
}
