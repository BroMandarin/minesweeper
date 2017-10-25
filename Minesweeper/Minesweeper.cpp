#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Mines.h"
using namespace std;

void endGame(Block **field, int size)
{
	for (int i = 0; i < size; i++)
		delete[] field[i];
	delete[] field;
}

void moveCursor(int x, int y)
{
	COORD p = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

void menu(int &size, int &level)
{
	int menu;

	cout << "Select field size:" << endl;
	cout << "1. 10x10" << endl;
	cout << "2. 20x20" << endl;

	cin >> menu;

	if (menu >= 1 && menu <= 2)
		size = menu * 10;
	else
	{
		cout << "Error!" << endl;
		exit(0);
	}

	cout << "Select difficulty level:" << endl;
	cout << "1. Easy" << endl;
	cout << "2. Normal" << endl;
	cout << "3. Hard" << endl;

	cin >> menu;

	if (menu >= 1 && menu <= 3)
		level = menu;
	else
	{
		cout << "Error" << endl;
		exit(0);
	}
		
}

int main()
{
	int size, level, mines, flags = 0;

	menu(size, level);

	Block **field = new Block*[size];
	for (int i = 0; i < size; i++)
		field[i] = new Block[size];

	generateField(field, size, level, mines);

	bool end = false, win = false;
	int cursorX = 1, cursorY = 1;
	char input;

	system("cls");
	drawField(field, size, mines, flags);

	bool redraw = true;

	while (!end)
	{
		if (_kbhit())
		{
			input = _getch();
			switch (input)
			{
			case 72:
				if (cursorY > 1)
					cursorY--;
				break;
			case 77:
				if (cursorX < size)
					cursorX++;
				break;
			case 80:
				if (cursorY < size)
					cursorY++;
				break;
			case 75:
				if (cursorX > 1)
					cursorX--;
				break;
			case ' ':
				dig(field, size, cursorX - 1, cursorY - 1);
				if (field[cursorY - 1][cursorX - 1].mine)
				{
					revealMines(field, size, cursorX - 1, cursorY - 1);
					end = true;
				}
				redraw = true;
				break;
			case 'z':
				flags += flag(field, size, cursorX - 1, cursorY - 1);
				redraw = true;
				break;
			case 27: case 'q':
				end = true;
				break;
			}
			moveCursor(cursorX, cursorY);
		}

		if (redraw)
		{
			system("cls");
			drawField(field, size, mines, flags);
			moveCursor(cursorX, cursorY);
		}
		redraw = false;

		if (mines == flags)
		{
			if (checkForWin(field, size))
			{
				win = true;
				end = true;
			}
		}
	}

	if (win)
	{
		cleanField(field, size);
		system("cls");
		drawField(field, size, mines, flags);
		cout << "YOU WIN!" << endl;
	}
	else
	{
		cout << "BOOM!" << endl;
	}

	endGame(field, size);

	system("pause");

    return 0;
}
