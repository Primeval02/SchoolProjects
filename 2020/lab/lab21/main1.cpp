#include <iostream>


#include <stdlib.h>
#include <string.h>
#include "cmpslib19.h"


using namespace std;
int speed = 100;

#define  ROWS 50 
#define  COLUMNS 122


#define StartingX 1
#define StartingY 1


char MAZE[][COLUMNS] =	
{
	"+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+",
	"|       |         |   |   |       |   |   |     |     |         |   |     |     | |     | |         |     | |   |   |   |",
	"+-+-+-+ + +-+ + +-+ + + + + +-+-+ + + + + +-+ + + +-+ + +-+-+-+ + + +-+ + + +-+ + + +-+ + + +-+-+ + + +-+ + + + + + + + +",
	"|       | |   |     |   | |   |   | |   | |   |   | | | |   | | | |     | | |   |   |   |   |     | | |   |   | | |   | |",
	"+ +-+-+-+ +-+-+-+-+-+-+-+ +-+ + + + +-+-+ + +-+-+-+ + + +-+ + + +-+-+-+-+ +-+ +-+ + + +-+-+ + +-+ + + + +-+-+-+ + +-+-+ +",
	"| |     |   |         | |   | | | | | |   |         | |     |     |   |   |   | | | |     | | |   | | |         |   |   |",
	"+ + + + +-+ + +-+ +-+ + + +-+ + +-+ + + +-+-+-+ +-+-+ +-+-+-+ +-+ + +-+ +-+ +-+ + + +-+-+ +-+ +-+-+ + + +-+-+-+-+-+ + +-+",
	"| | | |   | | |   |   | |   | |     | | |   |   | |   |       |   |       |   |   | |       |     |   | |           | | |",
	"+ + + +-+ + + + +-+ +-+ +-+ + +-+-+-+ + + + +-+-+ + +-+-+-+-+-+ +-+-+-+ + +-+ +-+-+ +-+-+-+ +-+-+ +-+-+ + +-+-+-+ +-+ + +",
	"| | | |   | | |   |       |   |         | | |     |           | |   |   |         | |     | |   |   |     |     | |   | |",
	"+ + + + +-+ + + + +-+-+-+ +-+-+ +-+ +-+-+ + + +-+ + +-+-+-+-+ + + + +-+ +-+-+ +-+ +-+ +-+ + + +-+-+ + +-+-+ +-+-+ + +-+ +",
	"| | | |     | | |   | |   |   | | | |   | |   | | |   |   | | |   |   |   |   | |     |   |   |     |   |     |   |     |",
	"+ + + +-+-+-+ + +-+ + + +-+-+ + + + +-+ + +-+-+ + +-+-+ + + + + +-+-+ +-+ +-+ + +-+-+-+ +-+-+ + +-+-+-+ + + +-+ +-+-+-+ +",
	"|   |     |   |   | | |       | | |     |   |   |     | |   | |     |   |   |   |   |   |   | | |     |   | |   |   |   |",
	"+-+-+-+ + + +-+-+ + + +-+-+-+-+ + +-+-+-+-+ + +-+-+-+ + +-+ + +-+-+-+-+ + + +-+-+ + + +-+ +-+ + + +-+ +-+-+ + +-+ + + +-+",
	"|   |   |   |   | | |       | | | |   |   |     |       |   |   |     | | |     | | |   |   | | |   |   |   |     | |   |",
	"+ + + +-+-+-+ + + + +-+-+-+ + + + + + +-+ +-+-+-+ +-+ +-+ +-+-+ + +-+ + +-+-+-+ + +-+-+ +-+ + + +-+-+-+ +-+-+-+-+-+ +-+-+",
	"| |   |   |   | | | |   | | |       | |   |   |   | | |   |   | |   | |   |   | |   |       |       | |   |       |     |",
	"+ +-+-+ + + +-+ + + + + + + +-+-+ +-+ + +-+ + + +-+ + + +-+ + + +-+ + +-+ + + + +-+ + +-+-+-+-+-+-+ + +-+ + +-+-+ +-+-+ +",
	"| |   | | |   | | | | |     |     | | |   | |   |     | |   | | |   | |   | |   | | | |         | | |   | | | |   |     |",
	"+ + + + +-+-+ + + + +-+ +-+-+ +-+-+ + +-+ + +-+-+ +-+-+ + +-+ + + + +-+ +-+-+ + + + + + +-+-+-+ + + +-+ + + + + +-+ +-+-+",
	"| | | |       |   |     |   | |     | | |   |   | |     |   |   | | |   |     | |     | | |         |   | | |       |   |",
	"+ +-+ +-+-+-+-+-+-+-+-+-+-+ + + + +-+ + +-+-+ + + + +-+-+-+-+-+ + + + +-+ +-+-+ +-+ +-+ + + +-+ +-+-+ +-+ + +-+-+-+-+ + +",
	"|   |   |               |   | | |     | |   | | | |   |     |     | |     | |   | |     |   | |   |     | | |   |   | | |",
	"+-+ +-+ + +-+-+-+ +-+ +-+ +-+ + +-+-+-+ + + + + + +-+ + +-+ + + +-+ +-+ +-+ + +-+ +-+ +-+-+-+ +-+ + + + + +-+ + + + + + +",
	"|   | | | |       | | |   |   | |     | | |   | | |   |   |   | |   | | |   | |   |   | |   |     | | | |     | | | | | |",
	"+ +-+ + + + +-+-+-+ + + +-+ +-+ + +-+ + +-+-+ + + + +-+ +-+-+-+ + +-+ + + + + + +-+-+-+ + +-+-+-+-+ +-+ + +-+ + +-+ +-+ +",
	"| |   |   |         |       | |   | |         |   |     |     | |     | | | | | |   |   | |           | | |   | |   |   |",
	"+ + +-+ +-+-+ +-+-+-+-+ +-+-+ +-+ + +-+ +-+-+-+-+-+-+-+-+ +-+ + +-+-+-+ +-+ + + + + + +-+ + +-+-+-+ + + + + +-+ + +-+ +-+",
	"| | |   | |     |     | |   |     |   | |           |   | |     | |   |   | |   | | |   |   |       | |   |   | | | | | |",
	"+ + + +-+ + + + + +-+ + + + +-+-+-+-+ + + +-+-+-+ + +-+ + + +-+-+ + + +-+ + +-+ + + + + + +-+ +-+-+-+ +-+-+-+ + + + + + +",
	"| | |     | | | |   |     |     |   |   | |       |     | |   |     |   | |     | |   |   |     |   | |       |   |   | |",
	"+ + +-+-+-+ + + +-+-+-+-+-+-+-+ + + +-+-+ + +-+-+-+-+-+-+ +-+ + +-+-+-+ + +-+-+-+ +-+-+-+-+-+-+ + +-+ + +-+-+-+-+-+ +-+ +",
	"| | |   |   | | |     |   |     | |       |   |     |   | |   |   |   | | |   |               | | |   | |     |   |   | |",
	"+ + + + + +-+ + + +-+ + +-+ +-+-+ +-+-+-+-+-+ +-+-+ + +-+ +-+-+-+ + + + + + +-+ +-+-+-+-+-+-+ + + + + + +-+-+ + + + +-+ +",
	"| |   |   | | | | | | |   | |   |         |   |     |   |     |   | | |   |     |     |       | |   | | |   |   | | |   |",
	"+ +-+-+-+-+ + + + + + +-+ + +-+ +-+-+-+-+ + +-+ +-+-+-+ +-+-+ + +-+ + +-+-+-+-+ + +-+ +-+-+-+-+ +-+-+ + + + + +-+ + + +-+",
	"| |       | | | |   | |   |     | |     | |       |   | |           | |   |   | | | |     |   |   |   |   | | | | | |   |",
	"+ + +-+-+ + + + +-+-+ + +-+-+-+ + + +-+ + +-+-+-+-+ + + +-+-+-+ +-+-+ +-+ + + +-+ + +-+-+ + + +-+ + +-+-+-+ + + + + +-+ +",
	"|   | |   | | | |       |   |   |     | | |         | |   |     |   |   | | |     |     | | | |   | | |     | |   |     |",
	"+ +-+ + +-+ + +-+ +-+ +-+-+ + +-+-+ + + + + + +-+-+-+ +-+ + +-+-+ + +-+ + + +-+-+-+ +-+ + + + + +-+ + + + +-+ +-+-+-+ +-+",
	"| | |   |   |     |         | |     | | |   | |     |       |   | |     | |         |   | | | | |   |   | |   |     |   |",
	"+ + +-+-+ +-+-+-+-+-+ +-+-+-+ + +-+-+-+ +-+-+-+ +-+ +-+-+ +-+-+ + +-+-+ + +-+ +-+-+-+-+-+ + + +-+ +-+-+ + + +-+ +-+ +-+ +",
	"| |   |   |     |     |       |   |   |   |       | |     |     | | |   | |   |     |     | |     |   | | | |     |   | |",
	"+ +-+ + +-+ +-+ + +-+-+-+-+-+-+-+ +-+ +-+ + + + +-+ + +-+-+ +-+-+ + + +-+ + + + +-+-+ +-+ + +-+-+-+ + +-+ + +-+ +-+-+ + +",
	"|       |     |     |     |   |   |   | |   | |   | | |     | |   |       | |   |   | |   | |       |     |       |   | |",
	"+-+-+ +-+-+-+ + +-+-+ +-+ +-+ + +-+ +-+ +-+-+-+-+ + + + +-+-+ + +-+-+ +-+-+ +-+-+ + + + +-+ +-+ +-+-+-+-+-+-+-+ + +-+-+ +",
	"|   | |       |     |   |   | |     |   |         | |   |   |   |     |     |   | | | |       | |   |     |     |   |   |",
	"+ + + + +-+-+-+-+-+ +-+-+ + + +-+-+ + + + +-+-+-+-+ + +-+ + +-+-+ + + +-+-+-+-+ + + + +-+-+-+ + + + +-+ + + +-+-+-+ + +-+",
	"| |   |           |       |   |       | |         |   |   |       | |             |   |       |   |     |         |     F",
	"+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+"
};

//--------------------------------------------------------------------

void printMaze()
{
	// we want to redraw the maze so move the cursor back so we print in the same place

	PositionCursor(0,0);

	cout << ChangeToGreen() ;
	for(int rows=0;rows <= ROWS  ;rows++)
	{
		for (int columns=0;columns < COLUMNS;columns++)
		{
			PositionCursor(rows,columns);
			cout <<  MAZE[rows][columns];
		}
		cout <<"\n";
	}
}

//---------------------------------------------------------------------


bool Step(int x, int y, int level)
{ 
	// is the point x, y the finish? 'F'  if so cout "Finish Fournd at.... "
// and return true
    if (MAZE[x][y] == 'F')
    {
        cout << "Finish found at " << x << "," << y << endl;
        return true;
    }

// if the point x,y is not an empty space ' ' return false
    if (MAZE[x][y] != ' ')
    {
        return false;
    }
// so now we know that point x,y  is a space
// set  MAZE[x][y] to the char '@'
// set the cursor postition to x,y   , use function PositionCursor
// print the char '@'
    MAZE[x][y] = '@';
    PositionCursor(x, y);
    cout << RED("@");

// print some debugging info, mine prints current location
// and how many levels deep we are */

	PositionCursor(ROWS +1,0);
	cout << ChangeToGreen()<< "currently at "<<x<<","<<y<< "    " << endl;
	cout << level << " levels deep\n";
	// pause for a while

	WaitHundredth(speed); 
	// call Step and pass in the coordinates of one space to the left ( x-1 ) 
// if it returs true you found the end and return true as well
    if (Step(x-1, y, level+1))
    {
        return true;
    }
// call Step and pass in the coordinates of one space down ( y+1 ) 
// if it returs true you found the end and return true as well
    if (Step(x, y+1, level+1))
    {
        return true;
    }
// call Step and pass in the coordinates of one space to the right ( x+1 ) 
// if it returs true you found the end and return true as well
    if (Step(x+1, y, level+1))
    {
        return true;
    }
// call Step and pass in the coordinates of one space up ( y-1 ) 
// if it returs true you found the end and return true as well
    if (Step(x, y-1, level+1))
    {
        return true;
    }
// if we are here we have not found the end... we must be at a dead end so we want to delte the '@' 
// from the 2d array and draw a space back to erase our path as we go back and try another direction
// if you have not returned true by now go ahead and set this postion in the 2d array back to a space
// set the cursor position to x and y
// and print a space at this position
// return false
    MAZE[x][y] = ' ';
    PositionCursor(x,y);
    cout << ' ';

	return false;
}



//-----------------------------------------------------------------

int main()
{

	Prompt("What delay do you want between moves... 1-99:",speed,1,99);

	system("clear");
	printMaze();

	Step(StartingX ,StartingY,1);

	return 0;
}




