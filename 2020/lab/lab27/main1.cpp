#include <vector> // include the vector libraray 

#include <iostream> // include the library for input/output streams   (cout, endl and such)

#include <algorithm> // for the random_shuffle and sort

#include <ctime> // to seed the random number generator



using namespace std;

int main()
  {

  // create a vector to store Integers
  
  cout  << "creating an integer vector" << endl;
  vector<int> v = {};
  // add values to the vector using the push_back function
  cout << "adding values to the vector" << endl;
  cout << "adding value 10" << endl;
  v.push_back(10);
  cout << "adding value 20" << endl;
  v.push_back(20);
  cout << "adding value 30" << endl;
  v.push_back(30);
  cout << "adding value 40" << endl;
  v.push_back(40);
  cout << "adding value 50" << endl;
  v.push_back(50);
// test the size function
  cout << "call the size function" << endl;
  int size = v.size();
  cout << "size returns " << size << endl;
// create and iterator
  cout << "\n\ncreating a standard vector::iterator" << endl;
  cout << "setting the iterator = to vector.begin()" << endl;
  vector<int>::iterator it = v.begin();
  cout << "our iterator points to the value 10" << endl;
  cout << "incrementing the iterator" << endl;
  it++;
  cout << "our iterator now points to the value 20" << endl;
// print out the value
  cout << "\n\nprinting the items in the vector front to back" << endl;
  cout << "set our iterator to vector.begin() and use a while loop (!= vector.end()))" << endl;
  it = v.begin();
  while(it != v.end())
  {
      cout << "the iterator now points to " << *it << endl;
      cout << "increment the iterator" << endl;
      it++;
  }
// test the [ ] operator
  cout << "\nVector also has an overloaded [] operator" << endl;
  cout << "use a for loop and print out the elements of the vector via the index operator" << endl;
  for(int i = 0; i < size; i++)
  {
      cout << "myIntegerVector[" << i << "] = " << v[i] << endl;
  }
// shuffle and print  
  cout << "use the random_shuffle algorithm to randomize the values in the vector (be sure to seed the random number generator)" << endl;
  srand(time(0));
  random_shuffle(v.begin(), v.end());
  cout << "use a for loop and print out the elements of the vector via the index operator" << endl;
  for(int i = 0; i < size; i++)
  {
      cout << "myIntegerVector[" << i << "] = " << v[i] << endl;
  }
// sort and print
  cout << "use the sort algorithm to sort the vector" << endl;
  sort(v.begin(), v.end());
  cout << "use a for loop and print out the elements of the vector via the index operator" << endl;
  for (int i = 0; i < size; i++)
  {
      cout << "myIntegerVector[" << i << "] = " << v[i] << endl;
  }
  }
