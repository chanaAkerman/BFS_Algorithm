#include "GLUT.h"
#include <math.h>
#include <time.h>
#include "Node.h"
#include <vector>

using namespace std;

const int W = 600; // window width
const int H = 600; // window height
const int MSZ = 100;

bool run_bfs = false;

Node* maze[MSZ][MSZ];

vector <Node*> gray; // gray nodes

void SetupMaze();


void init()
{
	int i, j;

	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
			maze[i][j] = new Node();

	srand(time(0)); // pseudo randomizer

	glClearColor(0.7, 0.7, 0.7, 0);

	SetupMaze();

	glOrtho(-1, 1, -1, 1, -1, 1);

}

void SetupMaze()
{
	int i, j;

	// setup borders
	for (i = 0; i < MSZ; i++)
	{
		maze[0][i]->SetValue(WALL);
		maze[MSZ - 1][i]->SetValue(WALL);
		maze[i][0]->SetValue(WALL);
		maze[i][MSZ - 1]->SetValue(WALL);
	}
	// add inner space
	for (i = 1; i < MSZ - 1; i++)
		for (j = 1; j < MSZ - 1; j++)
		{
			maze[i][j]->SetLine(i);
			maze[i][j]->SetColumn(j);
			if (i % 2 == 0) // more walls
			{
				if (rand() % 20 > 10)
					maze[i][j]->SetValue(WALL);
			}
			else // more spaces
				if (rand() % 20 > 13)
					maze[i][j]->SetValue(WALL);
		}
	// add start and target
	maze[MSZ / 2][MSZ / 2]->SetValue(START);
	maze[rand() % MSZ][rand() % MSZ]->SetValue(TARGET);

	// init gray
	gray.push_back(maze[MSZ / 2][MSZ / 2]);
}

void DrawMaze()
{
	int i, j;
	double sz, x, y;

	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
		{
			// set color
			switch (maze[i][j]->GetValue())
			{
			case SPACE:
				glColor3d(1, 1, 1); // white
				break;
			case WALL:
				glColor3d(0.4, 0, 0); // dark red
				break;
			case START:
				glColor3d(0.4, 0.8, 1); // light blue
				break;
			case TARGET:
				glColor3d(1, 0, 0); // red
				break;
			case GRAY:
				glColor3d(0, 1, 0); // light green
				break;
			case BLACK:
				glColor3d(0, 0.7, 0.3); // dark green
				break;
			case PATH:
				glColor3d(1, 0, 1); // magenta
				break;
			}
			// draw rectangle
			sz = 2.0 / MSZ;
			x = j * sz - 1;
			y = i * sz - 1;

			glBegin(GL_POLYGON);
			glVertex2d(x, y);
			glVertex2d(x + sz, y);
			glVertex2d(x + sz, y + sz);
			glVertex2d(x, y + sz);

			glEnd();
		}
}


void RestorePath(Node* first)
{
	Node* current = new Node(first);

	while (current->GetValue() != START)
	{
		maze[current->GetLine()][current->GetColumn()]->SetValue(PATH);
		current = current->GetParent();
	}
}

void BfsIteration()
{
	Node* current;

	if (gray.empty()) // there is no way to target
	{
		run_bfs = false;
	}
	else
	{
		// kind of pop
		current = new Node(gray[0]);
		gray.erase(gray.begin());
		// check down 
		if (maze[current->GetLine()][current->GetColumn()]->GetValue() != START)
			maze[current->GetLine()][current->GetColumn()]->SetValue(BLACK); // visited
		if (maze[current->GetLine() - 1][current->GetColumn()]->GetValue() == TARGET) // found
		{
			run_bfs = false;
			// restore path
			RestorePath(current);

		}
		else if (maze[current->GetLine() - 1][current->GetColumn()]->GetValue() == SPACE)
		{
			maze[current->GetLine() - 1][current->GetColumn()]->SetValue(GRAY); // paint it gray
			maze[current->GetLine() - 1][current->GetColumn()]->SetParent(current);
			gray.push_back(maze[current->GetLine() - 1][current->GetColumn()]);
		}
		if (run_bfs) // the target wasn't found yet
		{
			// check UP
			if (maze[current->GetLine() + 1][current->GetColumn()]->GetValue() == TARGET) // found
			{
				run_bfs = false;
				// restore path
				RestorePath(current);
			}
			else if (maze[current->GetLine() + 1][current->GetColumn()]->GetValue() == SPACE)
			{
				maze[current->GetLine() + 1][current->GetColumn()]->SetValue(GRAY); // paint it gray
				maze[current->GetLine() + 1][current->GetColumn()]->SetParent(current);
				gray.push_back(maze[current->GetLine() + 1][current->GetColumn()]);
			}
		}
		if (run_bfs) // the target wasn't found yet
		{
			// check LEFT
			if (maze[current->GetLine()][current->GetColumn() - 1]->GetValue() == TARGET) // found
			{
				run_bfs = false;
				// restore path
				RestorePath(current);
			}
			else if (maze[current->GetLine()][current->GetColumn() - 1]->GetValue() == SPACE)
			{
				maze[current->GetLine()][current->GetColumn() - 1]->SetValue(GRAY); // paint it gray
				maze[current->GetLine()][current->GetColumn() - 1]->SetParent(current);
				gray.push_back(maze[current->GetLine()][current->GetColumn() - 1]);
			}
		}
		if (run_bfs) // the target wasn't found yet
		{
			// check RIGHT
			if (maze[current->GetLine()][current->GetColumn() + 1]->GetValue() == TARGET) // found
			{
				run_bfs = false;
				// restore path
				RestorePath(current);

			}
			else if (maze[current->GetLine()][current->GetColumn() + 1]->GetValue() == SPACE)
			{
				maze[current->GetLine()][current->GetColumn() + 1]->SetValue(GRAY); // paint it gray
				maze[current->GetLine()][current->GetColumn() + 1]->SetParent(current);
				gray.push_back(maze[current->GetLine()][current->GetColumn() + 1]);
			}
		}
	}
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawMaze();

	glutSwapBuffers();// show what was drawn in "frame buffer"
}

void idle()
{
	if (run_bfs)
		BfsIteration();
	glutPostRedisplay();// calls indirectly to display
}

void Menu(int choice)
{
	switch (choice) {
	case 1: // bfs
		run_bfs = true;
		break;
	}
}
void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("AI Example ");

	glutDisplayFunc(display); // refresh function
	glutIdleFunc(idle); // idle: when nothing happens

	// menu
	glutCreateMenu(Menu);
	glutAddMenuEntry("BFS", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	init();

	glutMainLoop();
}