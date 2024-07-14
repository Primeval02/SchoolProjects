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
	Prompt("Enter a value 1-3 to choose the file to open: ",choice,1,3);

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

	std::ifstream in;// Create an input file stream.
	in.open(filename);

	if (in.fail())
	{
		cout << "Could not open file \""<< filename <<endl;
		return 9;
	}

    cout << "Creating dynamic array named \"data\" of size " << 5 << endl;
    int * data;
    data = new int[5];

    cout << "Creating int index = 0 and int capacity = 5 " << endl;
    int index = 0;
    int capacity = 5;

    cout << "Adding count and incrementing each time " << endl;
    cout << "Readng values into my dynamic array " << endl;
    cout << "Opening file for the first and only time " << filename << endl;
    cout << "Reading the values in and storing them into the array not printing them " << endl;

    int temp;
	while ( in >> temp  ) // reads in a single value into temp
	{
        data[index] = temp;
        cout << "Storing " << data[index] << " into data[" << index << "]" << endl;
        index++;

        if (index == capacity)
        {
            cout << "       The array is full, must resize" << endl;
            cout << "       Current address of \"data\" is " << data << endl;
            cout << "       Capacity = " << capacity << endl;
            cout << "       Index = " << index << endl;
            cout << "       data[" << index << "] is off the end of our array, array[" << index - 1 << "] is the last element, so lets resize" << endl;
            cout << "       create a temp * and set it to a new array of size " << capacity * 2 << endl; 
            int doubled = capacity * 2;
            int * largerArray;
            largerArray = new int [doubled];
            cout << "       copy elements from old array to new array using a for loop " << endl;
            for(int i = 0; i < index; i++)
            {
                cout << "               Copying data[" << i << "]  (" << data[i] << ") to the new array" << endl;
                largerArray[i] = data[i];
            }
            cout << "       All the values are copied to the new array " << endl;
            cout << "       Delete the old array " << endl;
            delete[] data;
            cout << "       Set our array pointer to point to the new array (our temp *) " << endl;
            data = largerArray;
            cout << "       Set capacty to be duble of what it already was " << endl;
            capacity = doubled;
            cout << "       Current address of \"data\" is " << data << endl;
            cout << "       Capacity = " << capacity << endl;
            cout << "       Index = " << index << endl;
            cout << "       We have room now so continue on " << endl;
        }

	}
    cout << "Closing file " << filename << endl;
    cout << "Calling PrintArray to print the values in my array " << endl;
    PrintArray(data, index);
    cout << "Deleting dynamic array " << endl;
    delete[] data;

	in.close();

	return 0;
}






