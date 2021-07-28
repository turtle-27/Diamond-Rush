#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>

// using namespace std;

#define UP 0     //-y
#define DOWN 1   //+y
#define LEFT 2   //-x
#define RIGHT 3  //+x

#define nodeadend//generate a maze without any dead ends! (consequently, many solutions to maze)
#define backtrack//enable this to generate mazes using depth-first search. Don't enable both.

long numin=1;     //Number of cells in the maze.
const int xsize=11;
const int ysize=11;

void initialize();
void generate();
std::string savebmp();
void removeDeadEnd(int xcur, int ycur);
std::string addCoins(std::string mapfile, int no_of_coins1, int no_of_coins2);

struct cell{
	bool interior_wall;
	bool in;  //Is this cell in the maze?
	bool up;  //Does the wall above this cell exist?
	bool left;//Does the wall to the left of this cell exist?
	int prevx, prevy; //The coordinates of the previous cell, used for backtracking.
};

cell MAZE[xsize][ysize];

void initialize(){
	//Initialize the maze!
	for(int x=0;x<xsize;x++){
		for(int y=0;y<ysize;y++){
			//The maze cells on the edges of the maze are "in" to provide padding. Otherwise, all maze cells are not in.
			MAZE[x][y].in   = (x==0||x==xsize-1||y==0||y==ysize-1)?1:0;
			//All maze cells have all walls existing by default, except the perimeter cells.
			MAZE[x][y].up   = (x==0||x==xsize-1||y==0)?0:1;
			MAZE[x][y].left = (x==0||y==0||y==ysize-1)?0:1;
		}
	}
	return;
}

void generate(){
	int xcur=1, ycur=1;//start growing from the corner. It could theoretically start growing from anywhere, doesn't matter.
	MAZE[xcur][ycur].in = 1;
	int whichway;
	bool success;
	do{
#ifdef nodeadend
	removeDeadEnd(xcur, ycur);
#endif
#ifdef backtrack
		while( MAZE[xcur][ycur-1].in&&MAZE[xcur][ycur+1].in&&
			   MAZE[xcur-1][ycur].in&&MAZE[xcur+1][ycur].in ){
				   //If all the neighbourhood cells are in, backtrack.
				int xcur2=MAZE[xcur][ycur].prevx;
				ycur=MAZE[xcur][ycur].prevy;
				xcur=xcur2;
		}
#endif
		do{
			//Randomly grow the maze if possible.
			success=0;
			whichway=rand()%4;
			switch(whichway){
			case UP:
				if(!MAZE[xcur][ycur-1].in){
					success=1;
					MAZE[xcur][ycur].up=0;
					MAZE[xcur][ycur-1].prevx=xcur;
					MAZE[xcur][ycur-1].prevy=ycur;
					ycur--;
				}
				break;
			case DOWN:
				if(!MAZE[xcur][ycur+1].in){
					success=1;
					MAZE[xcur][ycur+1].up=0;
					MAZE[xcur][ycur+1].prevx=xcur;
					MAZE[xcur][ycur+1].prevy=ycur;
					ycur++;
				}
				break;
			case LEFT:
				if(!MAZE[xcur-1][ycur].in){
					success=1;
					MAZE[xcur][ycur].left=0;
					MAZE[xcur-1][ycur].prevx=xcur;
					MAZE[xcur-1][ycur].prevy=ycur;
					xcur--;
				}
				break;
			case RIGHT:
				if(!MAZE[xcur+1][ycur].in){
					success=1;
					MAZE[xcur+1][ycur].left=0;
					MAZE[xcur+1][ycur].prevx=xcur;
					MAZE[xcur+1][ycur].prevy=ycur;
					xcur++;
				}
				break;
			}
		}while(!success);
		MAZE[xcur][ycur].in=1;
		numin++; //Every iteration of this loop, one maze cell is added to the maze.
	}while(numin<(xsize-2)*(ysize-2));
	
	removeDeadEnd(xcur, ycur);
	return;
}

void removeDeadEnd(int xcur, int ycur)
{
	int success;
	int whichway;
	if( MAZE[xcur][ycur-1].in&&MAZE[xcur][ycur+1].in&&MAZE[xcur-1][ycur].in&&MAZE[xcur+1][ycur].in){
				   //If at a dead end, randomly destroy a wall to make it not a dead end!
		do{
			success=0;
			whichway=rand()%4;
			switch(whichway){
			case UP:
				if(MAZE[xcur][ycur].up&&ycur!=1 ){
					success=1;
					MAZE[xcur][ycur].up=0;
				}
				break;
			case DOWN:
				if(MAZE[xcur][ycur+1].up&&ycur!=ysize-2){
					success=1;
					MAZE[xcur][ycur+1].up=0;
				}
				break;
			case LEFT:
				if(MAZE[xcur][ycur].left&&xcur!=1){
					success=1;
					MAZE[xcur][ycur].left=0;
				}
				break;
			case RIGHT:
				if(MAZE[xcur+1][ycur].left&&xcur!=xsize-2 ){
					success=1;
					MAZE[xcur+1][ycur].left=0;
				}
				break;
			}
		}while(!success);
	}
	return;
}

std::string savebmp(){
	std::string mazefile;
    // ofstream mazefile("mazefile.txt");
	int x, y, n;
	int width=(xsize-1)*2-1;
	int height=(ysize-1)*2-1;

	//Actual writing of data begins here:
	for(x = 0; x <= width - 1; x++){
		for(y = 0; y <= height - 1; y++){
			
			// Center(D)
			if(x == width/2 && y == height/2)
			{
				mazefile += 'D';
				// mazefile += ' ';
				continue;
			}

            // Gates
            if (x == width/2 && y == height/2 - 1)
            {
                mazefile += '5';
                continue;
            }

			// Wall around center
			if ((x == width/2 - 1 && (y >= height/2 - 1 && y <= height/2 + 1)) ||
				(x == width/2 + 1 && (y >= height/2 - 1 && y <= height/2 + 1)) ||
				(y == height/2 - 1 && (x >= width/2 - 1 && x <= width/2 + 1)) ||	
				(y == height/2 + 1 && (x >= width/2 - 1 && x <= width/2 + 1)))
			{
				mazefile += '2';
				// mazefile += ' ';
				continue;
			}

			if ((x == width/2 - 2 && y >= height/2 - 2 && y <= height/2 + 2) ||
				(x == width/2 + 2 && y >= height/2 - 2 && y <= height/2 + 2) ||
				(y == height/2 - 2 && x >= width/2 - 2 && x <= width/2 + 2) ||	
				(y == height/2 + 2 && x >= width/2 - 2 && x <= width/2 + 2))
			{
				mazefile += '0';
				// mazefile += ' ';
				continue;
			}

	
			if(x%2 == 1 && y%2 == 1){
				if(MAZE[x/2+1][y/2+1].in) 
				{
				 	
					if (MAZE[x/2+1][y/2+1].interior_wall)
					{
						mazefile += '1';
					}
					else
					{
						mazefile += '0';
					} 	
				}
				else 
				{
					mazefile += '1';
				}
			}else if(x%2 == 0 && y%2 == 0 ){
				
				mazefile += '1';
			}else if(x%2 == 0 && y%2 == 1){
				if(MAZE[x/2+1][y/2+1].left) 
				{
					mazefile += '1';
				}
				else 
				{
					if (MAZE[x/2+1][y/2+1].interior_wall)
					{
						mazefile += '1';
					}
					else
					{
						mazefile += '0';
					}
				}
			}else if(x%2 == 1 && y%2 == 0){
				if(MAZE[x/2+1][y/2+1].up) 
				{
					mazefile += '1';
				}
				else 
				{
					if (MAZE[x/2+1][y/2+1].interior_wall)
					{
						mazefile += '1';
					}
					else
					{
						mazefile += '0';
					}
				}
			}
			// mazefile += ' ';
			
		}
		// mazefile += '\n';
	}
	// mazefile.close();
	return mazefile;
}

std::string addCoins(std::string mapfile, int no_of_coins1, int no_of_coins2)
{
    int width = (xsize-1)*2-1;
	int height = (ysize-1)*2-1;

    // Gate
	mapfile[width + 1] = '2';
	mapfile[2*width - 2] = '2';
	
	while(no_of_coins1 != 0)
	{
		int r = rand()%361;
		if(r > 2*width && mapfile[r] == '0')
		{
			mapfile[r] = '3';
            no_of_coins1--;
		}
	}

    while(no_of_coins2 != 0)
	{
		int r = rand()%361;
		if(r > 2*width && mapfile[r] == '0')
		{
			mapfile[r] = '4';
            no_of_coins2--;
		}
	}

	// Gate
	mapfile[width + 1] = '0';
	mapfile[2*width - 2] = '0';
	
	//spawn location for enemy
	int enemy_count = 4;
	while(enemy_count != 0)
	{
		int r = rand()%361;
		if(r > 5*width && mapfile[r] == '0')
		{
			mapfile[r] = '6';
			enemy_count--;
		}
	}

    return mapfile;
}


