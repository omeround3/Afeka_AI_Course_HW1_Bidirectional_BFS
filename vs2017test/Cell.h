#pragma once
class Cell
{
private:
	int row, column;
	Cell* parent;

public:
	Cell(int r, int c, Cell* p);
	int GetRow() { return row; }
	int GetColumn() { return column; }
	Cell* GetParent() { return parent; }
};

