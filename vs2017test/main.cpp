/*
	A program to demonstrate Bidirectional BFS search from START point to TARGET point.
	The program will start a BFS algorithm from the START point and from the TARGET point and the same time.
	The output is the shortest path from the START point to the TARGET point.

	The program uses the OpenGL library; make sure to run as x86 platform.

	Based on AI Course basic BFS code.
	
	@Authors
	Tal Hagag - 313160921
	Omer Lev-Ron - 204573067
*/
#include "glut.h"
#include <time.h>
#include "Cell.h"
#include <vector>
#include <iostream>

using namespace std;

// Declare constant variables
const int W = 600;
const int H = 600;
const int WALL = 1; // Represents the Walls of the Maze
const int SPACE = 0; // Reperesents a blank space where we can "travel" in the maze
const int START = 2; // Represents the START point
const int TARGET = 3; // Represents the TARGET point
const int BLACK = 4; // visited
const int GRAY = 5; // Neighbours to visit
const int PATH = 6; // The path found from START to TARGET

const int MSZ = 100; // Maze size

// Declare global variables
int maze[MSZ][MSZ] = { 0 }; // Initialize the maze with SPACES 
bool runBFS = false; // Booleans to indicate if we use BFS 

vector <Cell*> graysFromStart; // grays queue from the START point
vector <Cell*> graysFromTarget; // grays queue form the TARGET point



void InitMaze();

void init()
{
	srand(time(0));
	glClearColor(1, 1, 1, 0);// color of window background
	glOrtho(-1, 1, -1, 1, 0, 1);
	InitMaze();
}

// The function will initialize the maze borders and also the cells
void InitMaze()
{
	int i, j;

	for (i = 0; i < MSZ; i++) // frame of WALLS
	{
		maze[0][i] = WALL;
		maze[MSZ - 1][i] = WALL;
		maze[i][0] = WALL;
		maze[i][MSZ - 1] = WALL;
	}

	// Setting cells which are not on the border of the maze to SPACE or WALL based on even/odd line number
	for (i = 1; i < MSZ - 1; i++)
		for (j = 1; j < MSZ - 1; j++)
		{
			if (i % 2 == 1) // Odd line
			{
				if (rand() % 100 > 20)
					maze[i][j] = SPACE;
				else
					maze[i][j] = WALL;
			}
			else // Even line
			{
				if (rand() % 100 > 35)
					maze[i][j] = WALL;
				else
					maze[i][j] = SPACE;
			}
		}
	// mark out the start and the target cells
	int target_row = rand() % MSZ, target_col = rand() % MSZ;
	maze[MSZ / 2][MSZ / 2] = START; // Start cell
	maze[target_row][target_col] = TARGET; // Target Cell

	// Create the START and TARGET cells and push to the queues
	Cell* start = new Cell(MSZ / 2, MSZ / 2, nullptr);
	Cell* target = new Cell(target_row, target_col, nullptr);
	graysFromStart.push_back(start); // the start cell is in grays
	graysFromTarget.push_back(target); // the target cell is in grays

}

// The function will draw the maze according to each cell value in 'maze'
void DrawMaze()
{
	int i, j;
	int xsz = W / MSZ;
	int ysz = H / MSZ;
	double x, y;
	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
		{
			switch (maze[i][j])   // set color
			{
			case SPACE:
				glColor3d(1, 1, 1); // White
				break;
			case WALL:
				glColor3d(0.2, 0, 0); // Brown
				break;
			case START:
				glColor3d(0.5, 0.5, 1); // Cyan
				break;
			case TARGET:
				glColor3d(1, 0, 0); // Red
				break;
			case BLACK:
				glColor3d(0.9, 0.9, 0.5);
				break;
			case GRAY:
				glColor3d(0.2, 0.6, 0.2);
				break;
			case PATH:
				glColor3d(1, 0.6, 1);
				break;
			}
			// Draw square -> maze[i][j]
			x = 2.0 * (j * xsz) / W - 1; // value in range [-1,1)
			y = 2.0 * (i * ysz) / H - 1; // value in range [-1,1)
			// Fill the sqaure 
			glBegin(GL_POLYGON);
			glVertex2d(x, y);
			glVertex2d(x, y + (2.0 * ysz) / H);
			glVertex2d(x + 2.0 * xsz / W, y + (2.0 * ysz) / H);
			glVertex2d(x + 2.0 * xsz / W, y);
			glEnd();
			// Draw the square borders (cleaner look)
			glColor3d(0, 0, 0);
			glBegin(GL_LINE_LOOP);
			glVertex2d(x, y);
			glVertex2d(x, y + (2.0 * ysz) / H);
			glVertex2d(x + 2.0 * xsz / W, y + (2.0 * ysz) / H);
			glVertex2d(x + 2.0 * xsz / W, y);
			glEnd();
		}
}

// Searches for a specific cell in the Cells queue and if exists return it
Cell* getCell(int row, int col, vector <Cell*>* grays)
{
	for (Cell* cell : *grays)
	{
		if (cell->GetRow() == row && cell->GetColumn() == col) { return cell; }
	}
	return nullptr;
}

// This function will build and restore the path from START to TARGET
void RestorePath(Cell* pcurrent)
{
	Cell* targetCell;
	Cell* intersectCell = pcurrent;

	while (pcurrent->GetParent() != nullptr)
	{
		maze[pcurrent->GetRow()][pcurrent->GetColumn()] = PATH;
		pcurrent = pcurrent->GetParent();
	}

	if (pcurrent->GetRow() == (MSZ / 2) && pcurrent->GetColumn() == (MSZ / 2)) // If the current cell is the START cell
	{
		pcurrent = getCell(intersectCell->GetRow(), intersectCell->GetColumn(), &graysFromTarget);
	}
	else
	{
		pcurrent = getCell(intersectCell->GetRow(), intersectCell->GetColumn(), &graysFromStart);
	}

	if (pcurrent)
	{
		while (pcurrent->GetParent() != nullptr)
		{
			maze[pcurrent->GetRow()][pcurrent->GetColumn()] = PATH;
			pcurrent = pcurrent->GetParent();
		}
	}
}

// The functions return 1 if the row and column values are in the the grays queue; else returns 0
int isIntersect(int row, int col, vector <Cell*>* grays)
{
	for (Cell* pcurrent : *grays)
	{
		if (pcurrent->GetRow() == row && pcurrent->GetColumn() == col) { return 1; } // The cell already exists in the grays queue (not an intersection)
	}
	return 0; 
}


// This function is pushing all the white neighbors to the queue
// if a neighbor is the TARGET point, the serach is done and it will restore the path
void CheckNeighbor(Cell* pcurrent, int row, int col, vector <Cell*>* grays)
{
	// Check the color of the neighbor cell
	Cell* nextCell;

	if (maze[row][col] == GRAY)
	{
		if (isIntersect(row, col, grays) == 0)
		{
			runBFS = false;
			nextCell = new Cell(row, col, pcurrent);
			RestorePath(nextCell);
		}
	}
	else if (maze[row][col] == SPACE)
	{
		nextCell = new Cell(row, col, pcurrent);
		maze[row][col] = GRAY;
		grays->push_back(nextCell);
	}
}


// BFS iteration on the Maze
void BFSIteration(vector <Cell*>* grays)
{
	Cell* pcurrent;

	if (grays->empty())
	{
		cout << "There is no solution.\n";
		runBFS = false;
	}
	else // there are gray cells
	{
		pcurrent = grays->front();  // save the FIRST element,
		grays->erase(grays->begin()); // remove it from the queue
		if (maze[pcurrent->GetRow()][pcurrent->GetColumn()] != START && maze[pcurrent->GetRow()][pcurrent->GetColumn()] != TARGET)
			maze[pcurrent->GetRow()][pcurrent->GetColumn()] = BLACK;   // and paint it black

		// Check the neighbors
		// Up
		CheckNeighbor(pcurrent, pcurrent->GetRow() + 1, pcurrent->GetColumn(), grays);
		// Down
		if (runBFS)
			CheckNeighbor(pcurrent, pcurrent->GetRow() - 1, pcurrent->GetColumn(), grays);
		// Right
		if (runBFS)
			CheckNeighbor(pcurrent, pcurrent->GetRow(), pcurrent->GetColumn() + 1, grays);
		// Left
		if (runBFS)
			CheckNeighbor(pcurrent, pcurrent->GetRow(), pcurrent->GetColumn() - 1, grays);
	}
}


// Display function implenmenation of the OpenGL
void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	DrawMaze();
	glutSwapBuffers(); // show all
}

// Runs the BFS algorithms from START and TARGET at the same time
void BidirectionalBFS()
{
	if (graysFromStart.empty() && graysFromTarget.empty())
	{
		runBFS = false;
	}
	else
	{
		BFSIteration(&graysFromStart);
		BFSIteration(&graysFromTarget);
	}
}

// Idle function implementation of the OpenGL
void idle()
{
	if (runBFS)
		BidirectionalBFS();
	glutPostRedisplay(); // go to display
}

// Options menu to choose the algorithm to run 
void menu(int choice)
{
	switch (choice)
	{
	case 0: // Bidirectional BFS
		runBFS = true;
		break;
	}
}


void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Bidirectional BFS Maze");

	glutDisplayFunc(display); // display is window refresh function
	glutIdleFunc(idle);  // idle is background function

	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Bidirectional BFS", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);



	init();

	glutMainLoop();
}