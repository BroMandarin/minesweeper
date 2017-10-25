#pragma once

enum BlockState
{
	NONE,
	FLAG,
	DIG
};

struct Block
{
	int x;
	int y;
	int number;
	bool mine;
	BlockState state;

	Block()
	{

	}

	Block(int x, int y)
	{
		this->x = x;
		this->y = y;
		this->number = 0;
		this->mine = false;
		this->state = NONE;
	}

	Block(int x, int y, int number, bool mine, BlockState state = NONE)
	{
		this->x = x;
		this->y = y;
		this->number = number;
		this->mine = mine;
		this->state = state;
	}

	Block(int x, int y, bool mine, BlockState state = NONE)
	{
		this->x = x;
		this->y = y;
		this->number = 0;
		this->mine = mine;
		this->state = state;
	}

	Block(int x, int y, int number, BlockState state = NONE)
	{
		this->x = x;
		this->y = y;
		this->number = number;
		this->mine = false;
		this->state = state;
	}

	bool operator==(const Block block)
	{
		if (x == block.x && y == block.y)
			return true;
		return false;
	}
};

void placeNumbers(Block **field, int size);

void placeMines(Block **field, int size, int level, int &mines);

void generateField(Block **field, int size, int level, int &mines);

void drawField(Block **field, int size, int mines, int flags);

void showField(Block **field, int size);

void dig(Block **field, int size, int xDig, int yDig);

int flag(Block **field, int size, int xFlag, int yFlag);

void revealMines(Block **field, int size, int xRed, int yRed);

bool checkForWin(Block **field, int size);

void cleanField(Block **field, int size);
