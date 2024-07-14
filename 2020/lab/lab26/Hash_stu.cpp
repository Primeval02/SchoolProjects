#include "Hash.h"



// Calculates the primary key using modulo arithmetic

// Primary key will be in the range 0 to MAX_CAPACITY - 1

int HashTable::hash1(int value)
{
    return (value % MAX_CAPACITY);
}

// for the 2nd hash we want to have an alternate method of calculating a hash

// MAX_CAPACITY - ( value % MAX_CAPACITY)

int HashTable::hash2(int value)
{
    return (MAX_CAPACITY - (value % MAX_CAPACITY));
}


// create a string representation of the HashTable class

// make sure your output from the ToString function matches the example

// before AND AFTER you delete some items

string HashTable::ToString()
{
    ostringstream print;
    print << "Hashtable: " << endl;
    print << "Hash Size: " << MAX_CAPACITY << endl;
    print << "Count: " << count << endl;
    for(int i = 0; i < MAX_CAPACITY; i++)
    {
        if (hashtable[i] != EMPTY_VALUE)
        {
            if (hashtable[i] != DELETED_VALUE)
            {
                print << "hashtable[  " << i << "] contains:     " << hashtable[i] << " hash1 =      " << hash1(hashtable[i]) << "  hash2 =     " << hash2(hashtable[i]) << endl;     
            }

            else
            {
                print << "hashtable[  " << i << "] contains:     " << "DELETED_VALUE" << endl;
            }
        }
    }
    return print.str();
}









