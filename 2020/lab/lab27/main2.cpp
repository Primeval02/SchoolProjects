#include <list> // include the listlibraray 

#include <iostream> // include the library for input/output streams   (cout, endl and such)

#include <algorithm>

using namespace std;

void PrintBackwards(string val)
{
for (int x=val.length();x>=0;x--)
  cout <<val[x];
cout << endl;
}




int main()
  {
// create a list to store Strings
    cout << "creating a string list" << endl;
    list<string> l = {};
// add some Strings to the list we created
    cout << "adding values to the list" << endl;
    cout << "adding value one" << endl;
    l.push_back("one");
    cout << "adding value two" << endl;
    l.push_back("two");
    cout << "adding value three" << endl;
    l.push_back("three");
    cout << "adding value four" << endl;
    l.push_back("four");
    cout << "adding value five" << endl;
    l.push_back("five");
// lets create an Iterator so we can step through the items
    cout << "\n\ncreating an interator" << endl;
    cout << "setting the iterator = to vector.begin()" << endl;
    list<string>::iterator it = l.begin();
    cout << "our iterator points to the value " << *it << endl;
    cout << "incrementing the iterator" << endl;
    it++;
    cout << "our iterator now points to the value " << *it << endl;
// set the iterator to point to the first item in the vector
    cout << "\n\nprinting the items in the list front to back" << endl;
    it = l.begin();
// print out the item the iterator is pointing to
    cout << "the iterator now points to " << *it <<endl;
    cout << "increment the iterator" << endl;
    it++;
    cout << "the iterator now points to " << *it << endl;
    cout << "increment the iterator" << endl;
    it++;
    cout << "the iterator now points to " << *it << endl;
    cout << "increment the iterator" << endl;
    it++;
    cout << "the iterator now points to " << *it << endl;
    cout << "incrementing the iterator" << endl;
    it++;
    cout << "the iterator now points to " << *it << endl;
    cout << "incrementing the iterator" << endl;
    it++;
// lets move our iterator to the next item in the vector
// incrementing an iterator will make it move to the next item of in the container
// print out the item the iterator is pointing to
// use the * operator to "dereference" the iterator to get the value the iteratore points to
// ok lets try to print out all the items in the vector
// set the iterator back to the front of the vector
// print out the value
// set the iterator back to the front of the vector
// print out the value

    cout << "\n\ncreate a reverse_iterator" << endl;
    cout << "printing the items in the list back to front" << endl;
    cout << "set our iterator to list.rgegin() and use a while loop (!= list.rend())" << endl;
    list<string>::reverse_iterator rit = l.rbegin();
    while(rit != l.rend())
    {
        cout << "the iterator now points to " << *rit << endl;
        cout << "increment the iterator" << endl;
        rit++;
    }

    cout << "\nusing the provided function void PrintBackwards(string val)" << endl;
    cout << " use the for_each function to call it with" << endl;
    cout << " all the values in list" << endl;
    cout << "FORMAT: for_each(starting iterator, ending iterator, function name)\n" << endl;
    cout << for_each(l.begin(), l.end(), PrintBackwards);
  return 0;
  }


