#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define UP 0     //-y
#define DOWN 1   //+y
#define LEFT 2   //-x
#define RIGHT 3  //+x
#define OUTFILE "MAZE"
#define WHITE fprintf(outfile, "%c%c%c", 255,255,255)
#define BLACK fprintf(outfile, "%c%c%c", 0,0,0)
#define RED   fprintf(outfile, "%c%c%c", 0,0,255)

#define nodeadend//generate a maze without any dead ends! (consequently, many solutions to maze)
//#define prim    //enable this to generate mazes using prim's algorithm.
#define backtrack//enable this to generate mazes using depth-first search. Don't enable both.
//#define movie   //this option spams bitmaps to illustrate each step of generation.

long numin=1;     //Number of cells in the maze.
const int xsize=20;
const int ysize=20;

void initialize();
void generate();
void savebmp(int xspecial, int yspecial);
void removeDeadEnd(int xcur, int ycur);
void addFood();

struct cell{
	bool interior_wall;
	bool in;  //Is this cell in the maze?
	bool up;  //Does the wall above this cell exist?
	bool left;//Does the wall to the left of this cell exist?
	int prevx, prevy; //The coordinates of the previous cell, used for backtracking.
};

cell MAZE[xsize][ysize];

int main(){
	srand((unsigned int)time(NULL)); //seed random number generator with system time
	initialize();      //initialize the maze
	generate();      //generate the maze
#ifdef movie
	for(int i=1;i<10;i++){
		numin++;
		savebmp(0,0);      //output the bitmap
	}
#else movie
	savebmp(0,0);
#endif
	addFood();
	return 0;
}

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
#ifdef movie
		savebmp(xcur,ycur);
#endif
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
#ifdef prim
		do{
			//randomly find a cell that's in the maze
			xcur=rand()%(xsize-2)+1;
			ycur=rand()%(ysize-2)+1;
		}while(!MAZE[xcur][ycur].in ||
			MAZE[xcur][ycur-1].in&&MAZE[xcur][ycur+1].in&&
			MAZE[xcur-1][ycur].in&&MAZE[xcur+1][ycur].in);
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
	
	for(int x = xsize/2 - 4; x <= xsize/2 + 4; x++)
	{
		for(int y = ysize/2 - 4 ; y <= ysize/2 + 4; y++)
		{
			
			// MAZE[x][y].interior_wall = (x==xsize/2 - 4 ||x==xsize/2 + 4||y==ysize/2 - 4||y==ysize/2 + 4)?1:0;
			// MAZE[x][y].in   = (x==xsize/2 - 4 ||x==xsize/2 + 4||y==ysize/2 - 4||y==ysize/2 + 4)?1:0;
			// //All maze cells have all walls existing by default, except the perimeter cells.
			// MAZE[x][y].up   = (x==xsize/2 - 4||x==xsize/2 + 4||y==ysize/2 + 4)?0:1;
			// MAZE[x][y].left = (x==xsize/2 - 4||y==ysize/2 - 4||y==ysize/2 + 4)?0:1;
			
			if (x >= xsize/2 - 4 && x <= xsize/2 + 4 && y >= ysize/2 - 4 && y <= ysize/2 + 4)
			{MAZE[x][y].interior_wall = 1; MAZE[x][y].in = 1;}

			if (x >= xsize/2 - 4 && x <= xsize/2 + 4 && y >= ysize/2 - 4 && y <= ysize/2 + 4)
			{MAZE[x][y].up = 1;}

			if (x >= xsize/2 - 4 && x <= xsize/2 + 4 && y >= ysize/2 - 4 && y <= ysize/2 + 4)
			{MAZE[x][y].left = 1;}	

			if ((x >= xsize/2 - 3 && x <= xsize/2 + 3) && y >= ysize/2 - 3 && y <= ysize/2 + 3)
			{
				MAZE[x][y].in = 0; MAZE[x][y].interior_wall = 0;
			}
		}
	}


	xcur=xsize/2 - 3; 
	ycur=ysize/2 - 3;
	MAZE[xcur][ycur].in = 1;
	numin = 1;
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
	}while(numin<49);

	// for (int i = 1; i < xsize - 1; i++)
	// {
	// 	for (int j = 1; j < ysize - 1; j++)
	// 	{
	// 		if (i >= xsize/2 - 5 && i <= xsize/2 + 5 && j <= ysize/2 + 5 && j >= ysize/2 - 5)
	// 		{
	// 			continue;
	// 		}
	// 		else
	// 		{
	// 			removeDeadEnd(i,j);
	// 		}
	// 	}
	// }
	

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

void savebmp(int xspecial, int yspecial){
	//save a bitmap file! the xspecial, yspecial pixel is coloured red.
	FILE * outfile;
	ofstream mazefile("mazefile.txt");
	int extrabytes, paddedsize;
	int x, y, n;
	int width=(xsize-1)*2-1;
	int height=(ysize-1)*2-1;

	extrabytes = (4 - ((width * 3) % 4))%4; 

	char filename[200];
	
	sprintf(filename, "%s_%dx%d_n%d.bmp", OUTFILE, xsize, ysize, numin);
	paddedsize = ((width * 3) + extrabytes) * height;

	unsigned int headers[13] = {paddedsize + 54, 0, 54, 40, width, height, 0, 0, paddedsize, 0, 0, 0, 0};

	outfile = fopen(filename, "wb");
	fprintf(outfile, "BM");

	for (n = 0; n <= 5; n++){
	   fprintf(outfile, "%c", headers[n] & 0x000000FF);
	   fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
	   fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
	   fprintf(outfile, "%c", (headers[n] & (unsigned int) 0xFF000000) >> 24);
	}

	fprintf(outfile, "%c", 1);fprintf(outfile, "%c", 0);
	fprintf(outfile, "%c", 24);fprintf(outfile, "%c", 0);

	for (n = 7; n <= 12; n++){
	   fprintf(outfile, "%c", headers[n] & 0x000000FF);
	   fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
	   fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
	   fprintf(outfile, "%c", (headers[n] & (unsigned int) 0xFF000000) >> 24);
	}

	//Actual writing of data begins here:
	for(x = 0; x <= width - 1; x++){
		for(y = 0; y <= height - 1; y++){
			
			if ((x == width/2 + 1 && (y == height/2 - 7 || y == height/2 + 9 || y == height/2 - 6 || y == height/2 + 8)) || (y == height/2 + 1 && (x == width/2 - 7 || x == width/2 + 9 || x == width/2 - 6 || x == width/2 + 8)))
			{
				WHITE;
				mazefile << '.';
				continue;
				
			}
			
			if ((x == width/2 - 8 || x == width/2 + 10) && ( height/2 - 8 <= y && y <= height/2 + 10))
			{
				WHITE;
				mazefile << '.';
				continue;
			}

			if ((y == height/2 - 8 || y == height/2 + 10) && ( width/2 - 8 <= x && x <= width/2 + 10))
			{
				WHITE;
				mazefile << '.';
				continue;
			}
			
			
			if(x%2 == 1 && y%2 == 1){
				if(x/2+1 == xspecial && y/2+1 == yspecial) BLACK;
				else{
					if(MAZE[x/2+1][y/2+1].in) 
					{
					 	
						if (MAZE[x/2+1][y/2+1].interior_wall)
						{
							BLACK;
							mazefile << '#';
						}
						else
						{
							WHITE; 
							mazefile << '.';
						} 	
					}
					else 
					{
						if (MAZE[x/2+1][y/2+1].interior_wall)
						{
							BLACK;
							mazefile << '#';
						}
						else
						{
							BLACK; 
							mazefile << '#';
						}
					}
				}
			}else if(x%2 == 0 && y%2 == 0 ){
				
				if (MAZE[x/2+1][y/2+1].interior_wall)
				{
					BLACK;
					mazefile << '#';
				}
				else
				{
					BLACK; 
					mazefile << '#';
				}
			}else if(x%2 == 0 && y%2 == 1){
				if(MAZE[x/2+1][y/2+1].left) 
				{
					if (MAZE[x/2+1][y/2+1].interior_wall)
					{
						BLACK;
						mazefile << '#';
					}
					else
					{
						BLACK; 
						mazefile << '#';
					}
				}
				else 
				{
					if (MAZE[x/2+1][y/2+1].interior_wall)
					{
						BLACK;
						mazefile << '#';
					}
					else
					{
						WHITE; 
						mazefile << '.';
					}
				}
			}else if(x%2 == 1 && y%2 == 0){
				if(MAZE[x/2+1][y/2+1].up) 
				{
					if (MAZE[x/2+1][y/2+1].interior_wall)
					{
						BLACK;
						mazefile << '#';
					}
					else
					{
						BLACK; 
						mazefile << '#';
					}
				}
				else 
				{
					if (MAZE[x/2+1][y/2+1].interior_wall)
					{
						BLACK;
						mazefile << '#';
					}
					else
					{
						WHITE; 
						mazefile << '.';
					}
				}
			}

			
		}
		mazefile << endl;
		if (extrabytes){     // See above - BMP lines must be of lengths divisible by 4.
			for (n = 1; n <= extrabytes; n++){
				fprintf(outfile, "%c", 0);
			}
		}
	}
	printf("file printed: %s\n", filename); 
	fclose(outfile);
	mazefile.close();
	return;
}
void addFood()
{
	fstream newfile;
    
	vector<string> svec;
	newfile.open("mazefile.txt",ios::in); //open a file to perform read operation using file object
    if (newfile.is_open()){ //checking whether the file is open
		string tp;
		while(getline(newfile, tp))
		{ //read data from file object and put it into string.
			svec.push_back(tp);
		}		
		newfile.close(); //close the file object.
   	}

	// Special food
	int count = 5;
	while(count != 0)
	{
		int r1 = rand()%37;
	    int r2 = rand()%37;
		string str = svec[r1];
		if(str[r2] == '.')
		{
			str[r2] = 'C';
			svec[r1] = str;
			count--;
		}
	}

	// Gate 
	count = 2;
	while(count != 0)
	{
		int r1 = rand()%(5);
		int r2 = rand()%37;
		string str = svec[r1];
		if(str[r2] == '.')
		{
			str[r2] = 'G';
			svec[r1] = str;
			count--;
		}
	}
	//spawn location for player1 & player2
	count = 1;
	while(count != 0)
	{
		int r1 = rand()%(37-30) + 30;
		int r2 = rand()%(37-30) + 30;
		string str = svec[r1];
		if(str[r2] == '.')
		{
			str[r2] = 'P';
			svec[r1] = str;
			count--;
		}

	}

	//spawn location for enemy
	count = 1;
	while(count != 0)
	{
		int r1 = rand()%(28-12) + 12;
		int r2 = rand()%(28-12) + 12;
		string str = svec[r1];
		if(str[r2] == '.')
		{
			str[r2] = 'E';
			svec[r1] = str;
			count--;
		}
	}

	newfile.open("maze.txt",ios::out);
	for(int i = 0; i< svec.size(); i++)
	{
		newfile << svec[i] << endl;
	}

}


