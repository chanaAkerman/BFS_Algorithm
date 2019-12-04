#pragma once

const int SPACE = 0;
const int WALL = 1;
const int START = 2;
const int TARGET = 3;
const int PATH = 4; // belongs to the path to target
const int GRAY = 5; // Fringe
const int BLACK = 6; // VISITED


class Node
{
public:
	Node();
	Node( Node *other);
	~Node();

private:
	int value;
	Node* parent;
	int line, column;

public:
	void SetParent(Node * p);
	void SetValue(int value);
	void SetLine(int line);
	void SetColumn(int col);
	int GetValue();
	int GetLine();
	int GetColumn();
	Node * GetParent();
};

