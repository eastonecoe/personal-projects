/*
A basic implementation of the nLife cellular automaton. I originally wrote this early in my programming career as a quick way to test out the automaton.
This was my first use of graphics, as well as my first attempt at moving objects on an infinite grid.
*/

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <SDL.h>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

const int WIDTH = 200;
const int HEIGHT = 100;
const int PIXELSIZE = 9;
bool gridOn = true;

struct cell
{
	
	int state;
	int nextstate;
	int x;
	int y;
	cell* n;
	cell* ne;
	cell* e;
	cell* se;
	cell* s;
	cell* sw;
	cell* w;
	cell* nw;
	cell()
	{
		n = NULL;
		ne = NULL;
		e = NULL;
		se = NULL;
		s = NULL;
		sw = NULL;
		w = NULL;
		nw = NULL;
	}
};
struct anchor
{
	int x;
	int y;
};
struct color
{
	int r;
	int g;
	int b;
};

void initgrid(SDL_Surface* screenSurface, SDL_Window* window, vector<cell*> &grid, color colormap[])
{
	int tempsize = grid.size();
	for (int i = 0; i < tempsize; i++)
	{
		delete grid[0];
		grid.erase(grid.begin());
	}
	SDL_Rect currentCell;
	SDL_FillRect ( screenSurface, NULL, SDL_MapRGB( screenSurface->format, colormap[0].r, colormap[0].g, colormap[0].b ) );
	if (gridOn == true)
	{
		currentCell.h = HEIGHT * PIXELSIZE;
		currentCell.w = 1;
		currentCell.y = 0;
		for(int i = 0; i <= WIDTH; i++)
		{
			currentCell.x = i * PIXELSIZE;
			SDL_FillRect ( screenSurface, &currentCell, SDL_MapRGB( screenSurface->format, 75, 75, 75 ) );
		}
		currentCell.h = 1;
		currentCell.w = WIDTH * PIXELSIZE;
		currentCell.x = 0;
		for(int i = 0; i <= HEIGHT; i++)
		{
			currentCell.y = i * PIXELSIZE;
			SDL_FillRect ( screenSurface, &currentCell, SDL_MapRGB( screenSurface->format, 75, 75, 75 ) );
		}
	}
	SDL_UpdateWindowSurface (window);
}
void addgrid(vector<cell*> &grid, cell* newcell)
{
	cell* tempcell;
	int locality = grid.size();
	bool exists = false;
	for(int j = 0; j < grid.size(); j++)
	{
		if(grid[j]->x == newcell->x && grid[j]->y == newcell->y)
		{
			grid[j]->state = newcell->state;
			locality = j;
			exists = true;
		}
	}
	if(!exists)
	{
		grid.push_back(newcell);
		for(int i = 0; i < grid.size(); i++)
		{
			if(grid[i]->x == newcell->x && grid[i]->y == newcell->y + 1)
			{
				grid[i]->s = grid[locality];
				grid[locality]->n = grid[i];
			}
			if(grid[i]->x == newcell->x + 1 && grid[i]->y == newcell->y + 1)
			{
				grid[i]->sw = grid[locality];
				grid[locality]->ne = grid[i];
			}
			if(grid[i]->x == newcell->x + 1 && grid[i]->y == newcell->y)
			{
				grid[i]->w = grid[locality];
				grid[locality]->e = grid[i];
			}
			if(grid[i]->x == newcell->x + 1 && grid[i]->y == newcell->y - 1)
			{
				grid[i]->nw = grid[locality];
				grid[locality]->se = grid[i];
			}
			if(grid[i]->x == newcell->x && grid[i]->y == newcell->y - 1)
			{
				grid[i]->n = grid[locality];
				grid[locality]->s = grid[i];
			}
			if(grid[i]->x == newcell->x - 1 && grid[i]->y == newcell->y - 1)
			{
				grid[i]->ne = grid[locality];
				grid[locality]->sw = grid[i];
			}
			if(grid[i]->x == newcell->x - 1 && grid[i]->y == newcell->y)
			{
				grid[i]->e = grid[locality];
				grid[locality]->w = grid[i];
			}
			if(grid[i]->x == newcell->x - 1 && grid[i]->y == newcell->y + 1)
			{
				grid[i]->se = grid[locality];
				grid[locality]->nw = grid[i];
			}
		}
	}
	if(grid[locality]->n == NULL && grid[locality]->state != 0)
	{
		tempcell = new cell();
		tempcell->state = 0;
		tempcell->x = grid[locality]->x;
		tempcell->y = grid[locality]->y + 1;
		addgrid(grid, tempcell);
	}
	if(grid[locality]->ne == NULL && grid[locality]->state != 0)
	{
		tempcell = new cell();
		tempcell->state = 0;
		tempcell->x = grid[locality]->x + 1;
		tempcell->y = grid[locality]->y + 1;
		addgrid(grid, tempcell);
	}
	if(grid[locality]->e == NULL && grid[locality]->state != 0)
	{
		tempcell = new cell();
		tempcell->state = 0;
		tempcell->x = grid[locality]->x + 1;
		tempcell->y = grid[locality]->y;
		addgrid(grid, tempcell);
	}
	if(grid[locality]->se == NULL && grid[locality]->state != 0)
	{
		tempcell = new cell();
		tempcell->state = 0;
		tempcell->x = grid[locality]->x + 1;
		tempcell->y = grid[locality]->y - 1;
		addgrid(grid, tempcell);
	}
	if(grid[locality]->s == NULL && grid[locality]->state != 0)
	{
		tempcell = new cell();
		tempcell->state = 0;
		tempcell->x = grid[locality]->x;
		tempcell->y = grid[locality]->y - 1;
		addgrid(grid, tempcell);
	}
	if(grid[locality]->sw == NULL && grid[locality]->state != 0)
	{
		tempcell = new cell();
		tempcell->state = 0;
		tempcell->x = grid[locality]->x - 1;
		tempcell->y = grid[locality]->y - 1;
		addgrid(grid, tempcell);
	}
	if(grid[locality]->w == NULL && grid[locality]->state != 0)
	{
		tempcell = new cell();
		tempcell->state = 0;
		tempcell->x = grid[locality]->x - 1;
		tempcell->y = grid[locality]->y;
		addgrid(grid, tempcell);
	}
	if(grid[locality]->nw == NULL && grid[locality]->state != 0)
	{
		tempcell = new cell();
		tempcell->state = 0;
		tempcell->x = grid[locality]->x - 1;
		tempcell->y = grid[locality]->y + 1;
		addgrid(grid, tempcell);
	}
}
void loadgrid(vector<cell*> &grid, SDL_Surface* screenSurface, SDL_Window* window, anchor location, color colormap[])
{
	OPENFILENAME loader;
	char szFile[100];
	ZeroMemory( &loader, sizeof(loader));
	loader.lStructSize = sizeof(loader);
	loader.hwndOwner = NULL;
	loader.lpstrFile = szFile;
	loader.lpstrFile[0] = '\0';
	loader.nMaxFile = sizeof(szFile);
	loader.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	loader.nFilterIndex = 1;
	loader.lpstrFileTitle = NULL;
	loader.nMaxFileTitle = 0;
	loader.lpstrInitialDir = NULL;
	loader.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
	if(GetOpenFileName(&loader))
	{
		SDL_Rect currentCell;
		currentCell.h = PIXELSIZE - 1;
		currentCell.w = PIXELSIZE - 1;
		ifstream filevalues(loader.lpstrFile);
		initgrid(screenSurface, window, grid, colormap);
		cell* tempcell;
		for(int i = 0; !filevalues.eof(); i++)
		{
			tempcell = new cell();
			filevalues >> tempcell->x >> tempcell->y >> tempcell->state;
			if (tempcell->state < 0)
				break;

			addgrid(grid, tempcell);
			if(tempcell->x >= location.x && tempcell->x <= (location.x + WIDTH) && tempcell->y >= location.y && tempcell->y <= (location.y + HEIGHT))
			{
				currentCell.x = (tempcell->x - location.x) * PIXELSIZE + 1;
				currentCell.y = (HEIGHT - (tempcell->y - location.y)) * PIXELSIZE + 1;
				SDL_FillRect (screenSurface, &currentCell, SDL_MapRGB (screenSurface->format, colormap[tempcell->state].r, colormap[tempcell->state].g, colormap[tempcell->state].b) );
			}
		}
	}
}
void savegrid(vector<cell*> &grid)
{
	OPENFILENAME saver;
	char szFile[100];
	ZeroMemory( &saver, sizeof(saver));
	saver.lStructSize = sizeof(saver);
	saver.hwndOwner = NULL;
	saver.lpstrFile = szFile;
	saver.lpstrFile[0] = '\0';
	saver.nMaxFile = sizeof(szFile);
	saver.lpstrFilter = "GrowthFile\0*.gro\0";
	saver.nFilterIndex = 1;
	saver.lpstrFileTitle = NULL;
	saver.nMaxFileTitle = 0;
	saver.lpstrInitialDir = NULL;
	saver.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
	saver.lpstrDefExt = "gro";

	if(GetSaveFileName(&saver))
	{
		ofstream filevalues(saver.lpstrFile);
		for(int i = 0; i < grid.size(); i++)
		{
			if(grid[i]->state != 0)
			{
				filevalues << grid[i]->x << ' ' << grid[i]->y << ' ' << grid[i]->state << endl;
			}
		}
	}
}
void iterate(vector<cell*> &grid, SDL_Surface* screenSurface, anchor location, color colormap[])
{
	SDL_Rect currentCell;
	cell* tempcell;
	currentCell.w = PIXELSIZE - 1;
	currentCell.h = PIXELSIZE - 1;
	int k, p;
	for(int i = 0; i < grid.size(); i++)
	{
		k = 0;
		p = 0;
		if ( grid[i]->n != NULL && grid[i]->n->state >= grid[i]->state )
		{
			k++;
			if ( grid[i]->n->state > grid[i]->state )
				p++;
		}
		if ( grid[i]->ne != NULL && grid[i]->ne->state >= grid[i]->state )
		{
			k++;
			if ( grid[i]->ne->state > grid[i]->state )
				p++;
		}
		if ( grid[i]->e != NULL && grid[i]->e->state >= grid[i]->state )
		{
			k++;
			if ( grid[i]->e->state > grid[i]->state )
				p++;
		}
		if ( grid[i]->se != NULL && grid[i]->se->state >= grid[i]->state )
		{
			k++;
			if ( grid[i]->se->state > grid[i]->state )
				p++;
		}
		if ( grid[i]->s != NULL && grid[i]->s->state >= grid[i]->state )
		{
			k++;
			if ( grid[i]->s->state > grid[i]->state )
				p++;
		}
		if ( grid[i]->sw != NULL && grid[i]->sw->state >= grid[i]->state )
		{
			k++;
			if ( grid[i]->sw->state > grid[i]->state )
				p++;
		}
		if ( grid[i]->w != NULL && grid[i]->w->state >= grid[i]->state )
		{
			k++;
			if ( grid[i]->w->state > grid[i]->state )
				p++;
		}
		if ( grid[i]->nw != NULL && grid[i]->nw->state >= grid[i]->state )
		{
			k++;
			if ( grid[i]->nw->state > grid[i]->state )
				p++;
		}
		if ( grid[i]->state != 51 && p == 3 )
		{
			grid[i]->nextstate = grid[i]->state + 1;
			currentCell.x = (grid[i]->x - location.x) * PIXELSIZE + 1;
			currentCell.y = (HEIGHT - (grid[i]->y - location.y)) * PIXELSIZE + 1;
			SDL_FillRect (screenSurface, &currentCell, SDL_MapRGB (screenSurface->format, colormap[grid[i]->nextstate].r, colormap[grid[i]->nextstate].g, colormap[grid[i]->nextstate].b) );
		}
		else if ( grid[i]->state != 0 && k < 2 )
		{
			grid[i]->nextstate = grid[i]->state - 1;
			currentCell.x = (grid[i]->x - location.x) * PIXELSIZE + 1;
			currentCell.y = (HEIGHT - (grid[i]->y - location.y)) * PIXELSIZE + 1;
			SDL_FillRect (screenSurface, &currentCell, SDL_MapRGB (screenSurface->format, colormap[grid[i]->nextstate].r, colormap[grid[i]->nextstate].g, colormap[grid[i]->nextstate].b) );
		}
		else if ( grid[i]->state != 0 && k > 3 )
		{
			grid[i]->nextstate = grid[i]->state - 1;
			currentCell.x = (grid[i]->x - location.x) * PIXELSIZE + 1;
			currentCell.y = (HEIGHT - (grid[i]->y - location.y)) * PIXELSIZE + 1;
			SDL_FillRect (screenSurface, &currentCell, SDL_MapRGB (screenSurface->format, colormap[grid[i]->nextstate].r, colormap[grid[i]->nextstate].g, colormap[grid[i]->nextstate].b) );
			
		}
		else
		{
			grid[i]->nextstate = grid[i]->state;
		}
		
	}
	for(int i = 0; i < grid.size(); i++)
	{
		if(grid[i]->state == 0 && grid[i]->nextstate != 0)
		{
			if(grid[i]->n == NULL)
			{
				tempcell = new cell();
				tempcell->state = 0;
				tempcell->nextstate = 0;
				tempcell->x = grid[i]->x;
				tempcell->y = grid[i]->y + 1;
				addgrid(grid, tempcell);
			}
			if(grid[i]->ne == NULL)
			{
				tempcell = new cell();
				tempcell->state = 0;
				tempcell->nextstate = 0;
				tempcell->x = grid[i]->x + 1;
				tempcell->y = grid[i]->y + 1;
				addgrid(grid, tempcell);
			}
			if(grid[i]->e == NULL)
			{
				tempcell = new cell();
				tempcell->state = 0;
				tempcell->nextstate = 0;
				tempcell->x = grid[i]->x + 1;
				tempcell->y = grid[i]->y;
				addgrid(grid, tempcell);
			}
			if(grid[i]->se == NULL)
			{
				tempcell = new cell();
				tempcell->state = 0;
				tempcell->nextstate = 0;
				tempcell->x = grid[i]->x + 1;
				tempcell->y = grid[i]->y - 1;
				addgrid(grid, tempcell);
			}
			if(grid[i]->s == NULL)
			{
				tempcell = new cell();
				tempcell->state = 0;
				tempcell->nextstate = 0;
				tempcell->x = grid[i]->x;
				tempcell->y = grid[i]->y - 1;
				addgrid(grid, tempcell);
			}
			if(grid[i]->sw == NULL)
			{
				tempcell = new cell();
				tempcell->state = 0;
				tempcell->nextstate = 0;
				tempcell->x = grid[i]->x - 1;
				tempcell->y = grid[i]->y - 1;
				addgrid(grid, tempcell);
			}
			if(grid[i]->w == NULL)
			{
				tempcell = new cell();
				tempcell->state = 0;
				tempcell->nextstate = 0;
				tempcell->x = grid[i]->x - 1;
				tempcell->y = grid[i]->y;
				addgrid(grid, tempcell);
			}
			if(grid[i]->nw == NULL)
			{
				tempcell = new cell();
				tempcell->state = 0;
				tempcell->nextstate = 0;
				tempcell->x = grid[i]->x - 1;
				tempcell->y = grid[i]->y + 1;
				addgrid(grid, tempcell);
			}
		}
		grid[i]->state = grid[i]->nextstate;
	}
	vector<int> indices;
	for (int i = 0; i < grid.size(); i++)
	{
		if(grid[i]->state == 0 && (grid[i]->n == NULL || grid[i]->n->state == 0) && (grid[i]->ne == NULL || grid[i]->ne->state == 0) 
			&& (grid[i]->e == NULL || grid[i]->e->state == 0) && (grid[i]->se == NULL || grid[i]->se->state == 0) && (grid[i]->s == NULL || grid[i]->s->state == 0) 
			&& (grid[i]->sw == NULL || grid[i]->sw->state == 0) && (grid[i]->w == NULL || grid[i]->w->state == 0) && (grid[i]->nw == NULL || grid[i]->nw->state == 0))
		{
			if(grid[i]->n != NULL)
			grid[i]->n->s = NULL;
			if(grid[i]->ne != NULL)
			grid[i]->ne->sw = NULL;
			if(grid[i]->e != NULL)
			grid[i]->e->w = NULL;
			if(grid[i]->se != NULL)
			grid[i]->se->nw = NULL;
			if(grid[i]->s != NULL)
			grid[i]->s->n = NULL;
			if(grid[i]->sw != NULL)
			grid[i]->sw->ne = NULL;
			if(grid[i]->w != NULL)
			grid[i]->w->e = NULL;
			if(grid[i]->nw != NULL)
			grid[i]->nw->se = NULL;

			indices.push_back(i);
		}
	}
	int reduction = 0;
	for (int i = 0; i < indices.size(); i++)
	{
		delete grid[indices[i] - reduction];
		grid.erase(grid.begin() + (indices[i] - reduction));
		reduction++;
	}
}
void gridincrease(vector<cell*> &grid, SDL_Surface* screenSurface, int x, int y, int amountchanged, anchor location, color colormap[])
{
	SDL_Rect filler;
	int index = -1;
	int tempstate = 0;
	cell* tempcell;
	filler.h = PIXELSIZE - 1;
	filler.w = PIXELSIZE - 1;
	
	x = x/PIXELSIZE;
	y = y/PIXELSIZE;
	filler.x = x * PIXELSIZE + 1;
	filler.y = y * PIXELSIZE + 1;
	for(int i = 0; i < grid.size(); i++)
	{
		if(grid[i]->x == x + location.x && grid[i]->y == HEIGHT + location.y - y)
		{
			index = i;
			tempstate = grid[i]->state;
		}
	}
	tempcell = new cell();
	tempcell->x = x + location.x;
	tempcell->y = HEIGHT + location.y - y;
	if(amountchanged + tempstate >= 50)
		tempcell->state = 50;
	else
		tempcell->state = tempstate + amountchanged;
	if(index != -1)
	{
		if(grid[index]->n != NULL)
			grid[index]->n->s = NULL;
		if(grid[index]->ne != NULL)
			grid[index]->ne->sw = NULL;
		if(grid[index]->e != NULL)
			grid[index]->e->w = NULL;
		if(grid[index]->se != NULL)
			grid[index]->se->nw = NULL;
		if(grid[index]->s != NULL)
			grid[index]->s->n = NULL;
		if(grid[index]->sw != NULL)
			grid[index]->sw->ne = NULL;
		if(grid[index]->w != NULL)
			grid[index]->w->e = NULL;
		if(grid[index]->nw != NULL)
			grid[index]->nw->se = NULL;
		delete grid[index];
		grid.erase(grid.begin() + index);
	}
	addgrid(grid, tempcell);
	SDL_FillRect(screenSurface, &filler, SDL_MapRGB(screenSurface->format, colormap[tempcell->state].r, colormap[tempcell->state].g, colormap[tempcell->state].b) );
}

void griddecrease(vector<cell*> &grid, SDL_Surface* screenSurface, int x, int y, int amountchanged, anchor location, color colormap[])
{
	SDL_Rect filler;
	int index = -1;
	cell* tempcell;
	filler.h = PIXELSIZE - 1;
	filler.w = PIXELSIZE - 1;
	x = x/PIXELSIZE;
	y = y/PIXELSIZE;
	filler.x = x * PIXELSIZE + 1;
	filler.y = y * PIXELSIZE + 1;
	for(int i = 0; i < grid.size(); i++)
	{
		if(grid[i]->x == x + location.x && grid[i]->y == HEIGHT + location.y - y)
		{
			index = i;
		}
	}
	if (index == -1)
	{
		tempcell = new cell();
		tempcell->x = x + location.x;
		tempcell->y = HEIGHT + location.y - y;
		tempcell->state = 0;
		index = grid.size();
		addgrid(grid, tempcell);
	}
	if(grid[index]->state > amountchanged)
	{
		grid[index]->state = grid[index]->state - amountchanged;
		SDL_FillRect(screenSurface, &filler, SDL_MapRGB(screenSurface->format, colormap[grid[index]->state].r, colormap[grid[index]->state].g, colormap[grid[index]->state].b) );
	}
	else
	{
		grid[index]->state = 0;
		SDL_FillRect(screenSurface, &filler, SDL_MapRGB(screenSurface->format, colormap[0].r, colormap[0].g, colormap[0].b) );
	}
}

void movegrid(vector<cell*> &grid, SDL_Surface* screenSurface, anchor location, color colormap[])
{
	SDL_Rect currentCell;
	SDL_FillRect ( screenSurface, NULL, SDL_MapRGB( screenSurface->format, colormap[0].r, colormap[0].g, colormap[0].b ) );
	if (gridOn == true)
	{
		currentCell.h = HEIGHT * PIXELSIZE;
		currentCell.w = 1;
		currentCell.y = 0;
		for(int i = 0; i <= WIDTH; i++)
		{
			currentCell.x = i * PIXELSIZE;
			SDL_FillRect ( screenSurface, &currentCell, SDL_MapRGB( screenSurface->format, 75, 75, 75 ) );
		}
		currentCell.h = 1;
		currentCell.w = WIDTH * PIXELSIZE;
		currentCell.x = 0;
		for(int i = 0; i <= HEIGHT; i++)
		{
			currentCell.y = i * PIXELSIZE;
			SDL_FillRect ( screenSurface, &currentCell, SDL_MapRGB( screenSurface->format, 75, 75, 75 ) );
		}
	}
	currentCell.h = PIXELSIZE - 1;
	currentCell.w = PIXELSIZE - 1;
	for(int i = 0; i < grid.size(); i++)
	{
		if(grid[i]->x >= location.x && grid[i]->x <= (location.x + WIDTH) && grid[i]->y >= location.y && grid[i]->y <= (location.y + HEIGHT))
		{
			currentCell.x = (grid[i]->x - location.x) * PIXELSIZE + 1;
			currentCell.y = (HEIGHT - (grid[i]->y - location.y)) * PIXELSIZE + 1;
			SDL_FillRect (screenSurface, &currentCell, SDL_MapRGB (screenSurface->format, colormap[grid[i]->state].r, colormap[grid[i]->state].g, colormap[grid[i]->state].b) );
		}
	}
}

int main (int argc, char* args[] )
{
	SDL_Event period;
	int x, y;
	string buffer;
	int cstate;
	int dstate;
	string parameter;

	color colormap [51];
	for(int i = 0; i < 51; i++)
	{
		colormap[i].r = i * 5;
		colormap[i].g = i * 5;
		colormap[i].b = i * 5;
	}
	ifstream colorin;
	colorin.open("color.cfg");

	bool allparameters = false;
	bool rangecolor = false;
	int startr, startg, startb;
	int endr, endg, endb, statedifference, rdifference, gdifference, bdifference, tempdiff;
	while(!colorin.eof())
	{
		while(!colorin.eof() && !allparameters)
		{
			colorin >> parameter;
			if (parameter == "%")
			{
				allparameters = true;
			}
			if (parameter == "grid" && !allparameters)
			{
				colorin >> buffer;
				colorin >> parameter;
				if (parameter == "false")
				{
					gridOn = false;
				}
			}
		}
		colorin >> cstate;
		colorin >> buffer;
		if (strcmp(buffer.c_str(), "=") != 0)
		{
			rangecolor = true;
			dstate = atoi(buffer.c_str());
			colorin >> buffer;
		}
		if (!rangecolor)
		{
			colorin >> colormap[cstate].r >> colormap[cstate].g >> colormap[cstate].b;
		}
		else
		{
			colorin >> startr >> startg >> startb >> endr >> endg >> endb;
			statedifference = dstate - cstate;
			rdifference = endr - startr;
			gdifference = endg - startg;
			bdifference = endb - startb;
			for (int i = cstate; i <= dstate; i++)
			{
				tempdiff = i - cstate;
				colormap[i].r = int(startr + (double(tempdiff)/double(statedifference))*(rdifference));
				colormap[i].g = int(startg + (double(tempdiff)/double(statedifference))*(gdifference));
				colormap[i].b = int(startb + (double(tempdiff)/double(statedifference))*(bdifference));
			}
		}
		rangecolor = false;
	}
	anchor location;
	location.x = 0;
	location.y = 0;
	bool quit = false;
	bool paused = false;
	bool filing = false;
	int amountchanged = 1;
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

	SDL_Init (SDL_INIT_VIDEO);

	Uint32 oldtime = SDL_GetTicks();
	Uint32 newtime;
	char temptitle[40] = "";

	window = SDL_CreateWindow ("nLife 1.0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH * PIXELSIZE + 1, HEIGHT * PIXELSIZE + 1, SDL_WINDOW_SHOWN);
	screenSurface = SDL_GetWindowSurface (window);

	vector<cell*> grid;
	initgrid(screenSurface, window, grid, colormap);
	SDL_UpdateWindowSurface (window);
	paused = true;
	while(quit == false)
	{
		SDL_Delay(30);
		newtime = SDL_GetTicks();
		sprintf(temptitle, "nLife 1.0 FPS: %d", 1000/((long)newtime - (long)oldtime));
		SDL_SetWindowTitle(window, (const char*)temptitle);
		oldtime = newtime;
		if (paused == false)
		{
			iterate(grid, screenSurface, location, colormap );
			SDL_UpdateWindowSurface (window);
		}
		while (SDL_PollEvent (&period) )
		{
			if (period.type == SDL_QUIT)
			{
				quit = true;
			}
			if (period.type == SDL_WINDOWEVENT && period.window.event == SDL_WINDOWEVENT_RESTORED )
			{
				SDL_UpdateWindowSurface (window);
			}
			if (period.type == SDL_KEYDOWN && period.key.keysym.sym == SDLK_SPACE)
			{
				paused = !paused;
			}
			if (period.type == SDL_KEYDOWN && (period.key.keysym.sym == SDLK_RSHIFT || period.key.keysym.sym == SDLK_LSHIFT))
			{
				if (amountchanged == 1)
				{
					amountchanged = 5;
				}
				else if (amountchanged == 5)
				{
					amountchanged = 10;
				}
				else
				{
					amountchanged = 1;
				}
			}
			if (paused == true)
			{
				if (period.type == SDL_MOUSEBUTTONDOWN && period.button.button == SDL_BUTTON_LEFT && !filing)
				{
					x = period.button.x;
					y = period.button.y;
					gridincrease(grid, screenSurface, x, y, amountchanged, location, colormap);
					for(int z = 0; z < grid.size(); z++)
					{
						cout << grid[z]->x << ' ' << grid[z]->y << ' ' << grid[z]->state << endl;
					}
					SDL_UpdateWindowSurface (window);
				}
				if (period.type == SDL_MOUSEBUTTONDOWN && period.button.button == SDL_BUTTON_RIGHT && !filing)
				{
					x = period.button.x;
					y = period.button.y;
					griddecrease(grid, screenSurface, x, y, amountchanged, location, colormap);
					SDL_UpdateWindowSurface (window);
				}
				if (period.type == SDL_KEYDOWN && period.key.keysym.sym == SDLK_RETURN)
				{
					iterate(grid, screenSurface, location, colormap );
					SDL_UpdateWindowSurface (window);
				}
				if (period.type == SDL_KEYDOWN && period.key.keysym.sym == SDLK_ESCAPE)
				{
					initgrid(screenSurface, window, grid, colormap);
				}
				if (period.type == SDL_KEYDOWN && period.key.keysym.sym == SDLK_LEFT)
				{
					location.x = location.x - 1;
					movegrid(grid, screenSurface, location, colormap);
					SDL_UpdateWindowSurface (window);
				}
				if (period.type == SDL_KEYDOWN && period.key.keysym.sym == SDLK_RIGHT)
				{
					location.x = location.x + 1;
					movegrid(grid, screenSurface, location, colormap);
					SDL_UpdateWindowSurface (window);
				}
				if (period.type == SDL_KEYDOWN && period.key.keysym.sym == SDLK_UP)
				{
					location.y = location.y + 1;
					movegrid(grid, screenSurface, location, colormap);
					SDL_UpdateWindowSurface (window);
				}
				if (period.type == SDL_KEYDOWN && period.key.keysym.sym == SDLK_DOWN)
				{
					location.y = location.y - 1;
					movegrid(grid, screenSurface, location, colormap);
					SDL_UpdateWindowSurface (window);
				}
				filing = false;
				if (period.type == SDL_KEYDOWN && period.key.keysym.sym == SDLK_l)
				{
					SDL_MinimizeWindow(window);
					loadgrid(grid, screenSurface, window, location, colormap);
					SDL_RestoreWindow(window);
					SDL_UpdateWindowSurface (window);
				}
				if (period.type == SDL_KEYDOWN && period.key.keysym.sym == SDLK_s)
				{
					SDL_MinimizeWindow(window);
					savegrid(grid);
					SDL_RestoreWindow(window);
					SDL_UpdateWindowSurface (window);
				}
			}
		}
		
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}