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

HashTable::HashTable(int cap)
{
    hashtable = new int[cap];
    capacity = MAX_CAPACITY;
    count = 0;
    for (int i = 0; i < MAX_CAPACITY; i++)
    {
        hashtable[i] = EMPTY_VALUE;
    }
}

HashTable::~HashTable()
{
    delete hashtable;
}

bool HashTable::empty()
{
    if (count == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool HashTable::full()
{
    if (count == capacity)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool HashTable::search(int value)
{
    int position = hash1(value);

    if (hashtable[position] == value)
    {
        return true;
    }
    if (hashtable[position] == EMPTY_VALUE)
    {
        return false;
    }

    position = hash2(value);

    for(int i = position; i < MAX_CAPACITY; i++)
    {
        if (hashtable[i] == value)
        {
            return true;
        }
        if (hashtable[i] == EMPTY_VALUE)
        {
            return false;
        }
        i += 5;
    }
    return false;
}

bool HashTable::insert(int value)
{
    int position = hash1(value);
    if (full() == true)
    {
        throw "HashTable is full, cannot add another"s;
    }

     if (value == DELETED_VALUE || value == EMPTY_VALUE)
    {
        throw "Value is either DELETED_VALUE or EMPTY_VALUE"s;
    }
   

    if (hashtable[position] == EMPTY_VALUE || hashtable[position] == DELETED_VALUE)
    {
        hashtable[position] = value;
        count++;
        return true;
    }

    position = hash2(value);

    for(int i = position; i < MAX_CAPACITY; i++)
    {
        if (hashtable[i] == EMPTY_VALUE || hashtable[i] == DELETED_VALUE)
        {
            hashtable[i] = value;
            count++;
            return true;
        }
        i += 5;
    }
    return false;

}

bool HashTable::remove(int value)
{
    int position = hash1(value);

    if (empty() == true)
    {
        throw "Cannot delete a value in an empty table"s;
    }

    if (value == DELETED_VALUE || value == EMPTY_VALUE)
    {
        throw "Value is either DELETED_VALUE or EMPTY_VALUE"s;
    }

    if (hashtable[position] == value)
    {
        hashtable[position] = DELETED_VALUE;
        count--;
        return true;
    }

    position = hash2(value);

    for(int i = position; i < MAX_CAPACITY; i++)
    {
        if (hashtable[i] == value)
        {
            hashtable[i] = DELETED_VALUE;
            count--;
            return true;
        }
        if (hashtable[i] == EMPTY_VALUE)
        {
            return false;
        }
        i += 5;
    }
    return false;


}
