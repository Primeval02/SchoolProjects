/*
	 All rights reserved. No part of this document may be reproduced, distributed,   or transmitted in any form or by any       means, including                     photocopying,             recording, or other electronic or mechanical methods, without the prior         written permission of the author, Michael Sarr
 */

#include "cmpslib19.h"

void PrintArray(int*array,int size)
{
 cout << "Array Address" << array << endl;
	for(int loop =0; loop<size; loop++)
	{
		if(loop)
		{
			cout << ",";
		}
		cout << array[loop];
	}
	cout << endl;
}

int main()
{

	char filename[50];
	int choice ;
	Prompt("Enter a value 1-3 to chose the file to open: ",choice,1,3);

	if (choice ==1 )
	{
		strncpy(filename,"test_input9.txt",50);
	}

	if (choice ==2 )
	{
		strncpy(filename,"test_input50.txt",50);
	}

	if (choice ==3 )
	{
		strncpy(filename,"test_input100.txt",50);
	}

	cout << "opening file " << filename << ", and reading line by line to determine number of elements " << endl;

	std::ifstream in;// Create an input file stream.
	in.open(filename);

	if (in.fail())
	{
		cout << "Could not open file \""<< filename <<endl;
		return 9;
	}

	int temp;
	int count=0;
	while ( in >> temp  ) // reads in a single value into temp
	{
		// Continue if the line was sucessfully read.
		//cout << temp << endl;
		count++;
	}
	in.close();



	cout << "The file contains " << count << " values\n";

	/*
		 now
		 make a dynamic array of the size that you just calculated
		 by reading in the array the first time
		 open the file again and read the values into the file
		 (use the code above as an example of how to open and read a file line by line)
		 print them out
		 delete your dynamic array
	 */
    cout << "Creating dynamic array of size " << count << endl;
    int * array;
    array = new int [count];

    cout << "Opening file " << filename << " again " << endl;
    in.open(filename);

    if (in.fail())
    {
        cout << "Could not open file \"" << filename << endl;
    }

    cout << "Reading the values one at a time and storing them into new array " << endl;
    count = 0;
    while (in >> temp)
    {
        array[count] = temp;
        count++;
    }

    cout << "Closing file " << filename << endl;
    in.close();

    cout << "Calling the PrintArray function to print the values from my array " << endl;
    PrintArray(array, count); 

    cout << "Deleting dynamic array " << endl;
    delete[] array;

	return 0;
}






