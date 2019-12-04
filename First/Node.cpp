#include "Node.h"
#include <stdio.h>



Node::Node()
{
	value = SPACE;
	parent = nullptr;
}


Node::Node( Node *other)
{
	value = other->GetValue();
	line = other->GetLine();
	column = other->GetColumn();

	if (other->GetParent() == nullptr)
		parent = nullptr;
	else
		parent = new Node(other->GetParent());

}


Node::~Node()
{
}


void Node::SetParent(Node* p)
{
	parent = p;
}


void Node::SetValue( int value)
{
	this->value = value;
}

void Node::SetLine(int line)
{
	this->line = line;
}


void Node::SetColumn(int col)
{
	this->column = col;
}

int Node::GetValue()
{
	return value;
}


int Node::GetLine()
{
	return line;
}


int Node::GetColumn()
{
	return column;
}

Node* Node::GetParent()
{
	return parent;
}