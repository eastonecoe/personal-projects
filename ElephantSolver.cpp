/*
A program that solves Elephant SpinOut puzzles (http://www.puzzles.com/products/ElephantSpinOut/ElephantHowToPlay.htm).
*/

#include <iostream>
#include <vector>

using namespace std;

void solvegame (vector<bool>& elephants, int& turnCount)
{
	int elephantCount = 0;
	int turnSlot = 0;
	
	for (int i = 0; i < elephants.size(); i++)
	{
		if (elephants[i] == false)
			elephantCount++;
	}
	
	if (elephantCount == 0)
	{
		return;
	}
	
	else if (elephantCount % 2 == 1)
	{
		cout << "Turn the elephant." << endl;
		turnCount++;
		elephants[0].flip();

		solvegame (elephants, turnCount);
		return;
	}
	
	else if (elephantCount % 2 == 0)
	{
		for (int i = 0;; i++)
		{
			if (elephants[i] == false)
			{
				elephants[i+1].flip();
				turnSlot = i + 1;
				break;
			}
		}
		
		cout << "Slide the elephants toward the exit " << turnSlot << " times." << endl;
		cout << "Turn the elephant." << endl;
		cout << "Slide the elephants inward " << turnSlot << " times." << endl;
		turnCount += 3;
		
		solvegame (elephants, turnCount);
		return;
	}
}

void main ()
{
	int gameSize, turnCount;
	turnCount = 0;
	cout << "Number of elephants?" << endl;
	cin >> gameSize;
	
	cout << "The instructions assume a starting position of the elephants slid inward as far as possible.  Multiple slides in a row count as one move." << endl << endl;
	vector<bool> game (gameSize, false);
	solvegame(game, turnCount);

	cout << endl << "The number of moves was: " << turnCount << endl;
	
}