//One of the earlier programs I wrote for a basic programming class.
//A maze solver that uses a basic form of Dijkstra's algorithm (although I did not know it at the time).
//Not that complicated now, but I was the only person to come up with a solution that found the shortest path through any given maze.


#include <iostream>
#include <fstream>

using namespace std;



const int MAX_ROWS = 10;
const int MAX_COLS = 10;

struct ElementType {
	char value;
	bool solution;
	bool shortest;
};
typedef ElementType MazeType[MAX_COLS][MAX_ROWS];


void mazestart(MazeType, int, int);
void mazesearch(MazeType, int, int, int, int, char);
void print(MazeType, int, int);

int main () 
{
	int NR;
	int NC;
	MazeType grid;
	
	ifstream input;
	input.open("maze.in");
	input >> NR;
	input >> NC;
	for(int i = 0; i < NR; i++)
	{
		for(int j = 0; j < NC; j++)
		{
			input >> grid[j][i].value; 
			grid[j][i].shortest = false;
			grid[j][i].solution = false;
		}
	}
	mazestart (grid, NR, NC);
	print (grid, NR, NC);
}

void mazestart (MazeType grid, int numrows, int numcols)
{
	int previousx, previousy, sentx, senty;
	bool impossible = false;
	
	
	
	while(!grid[0][0].solution && !impossible)
	{
		for(int i = 0; i < numrows; i++)
		{
			for(int j = 0; j < numcols; j++)
			{
				if (grid[j][i].value >= 'a' && grid[j][i].value <= 'z')
				{
					grid[j][i].value = '.';
				}
			}
		}
		mazesearch (grid, numcols-1, numrows-1, 0, 0, 'a');
		if (grid[0][0].value != '.' && grid[0][0].value != 'a')
		{
			cout << "No solution found!" << endl;
			impossible = true;
		}
	}
		
	for(int i = 0; i < numrows; i++)
	{
		for(int j = 0; j < numcols; j++)
		{
			if (!grid[j][i].solution && grid[j][i].value != '*')
			{
				grid[j][i].value = '.';
			}
		}
	}
	
}

void mazesearch (MazeType grid, int finalx, int finaly, int currentx, int currenty, char step)
{
	if (step > 'z')
		step = 'a';

	if (currentx == finalx && currenty == finaly)
	{
		grid[currentx][currenty].solution = true;
		grid[currentx][currenty].value = step;
	}
	else if (grid[currentx][currenty + 1].value == '.' && currenty != finaly)
	{
		grid[currentx][currenty].value = step;
		mazesearch (grid, finalx, finaly, currentx, currenty + 1, step + 1);
		if (grid[currentx][currenty + 1].solution)
		{
			grid[currentx][currenty].solution = true;
		}
	}
	else if (grid[currentx + 1][currenty].value == '.' && currentx != finalx)
	{
		grid[currentx][currenty].value = step;
		mazesearch (grid, finalx, finaly, currentx + 1, currenty, step + 1);
		if (grid[currentx + 1][currenty].solution)
		{
			grid[currentx][currenty].solution = true;
		}
	}
	else if (grid[currentx][currenty - 1].value == '.' && currenty != 0)
	{
		grid[currentx][currenty].value = step;
		mazesearch (grid, finalx, finaly, currentx, currenty - 1, step + 1);
		if (grid[currentx][currenty - 1].solution)
		{
			grid[currentx][currenty].solution = true;
		}
	}
	else if (grid[currentx - 1][currenty].value == '.' && currentx != 0)
	{
		grid[currentx][currenty].value = step;
		mazesearch (grid, finalx, finaly, currentx - 1, currenty, step + 1);
		if (grid[currentx - 1][currenty].solution)
		{
			grid[currentx][currenty].solution = true;
		}
	}
	else
	{
		grid[currentx][currenty].value = '=';
	}
	
}

void print (MazeType grid, int numrows, int numcols)
{
	for(int i = 0; i < numrows; i++)
	{
		for(int j = 0; j < numcols; j++)
		{
			cout << grid[j][i].value;
		}
		cout << endl;
	}
}