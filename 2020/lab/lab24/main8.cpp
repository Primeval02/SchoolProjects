#include <iostream>
#include <random>
#include <array>
#include <algorithm>
#include <ctime>
#include "cmpslib19.h"


#include "AVL.h"
#ifndef MAX_VALUES
#define MAX_VALUES 1000
#endif


int main()
  {
  /*
     lets create an array of test values
     populate the different containters fromt he same array to keep the tests constistent
   */

  std::array<int,MAX_VALUES> sample_data;
  for ( int loop2=0; loop2< MAX_VALUES; loop2++)
    {
    sample_data[loop2] = loop2;
    }

  // this part will shuffle the array
  //std::srand ( unsigned ( std::time(0) ) );
  //random_shuffle(sample_data.begin(),sample_data.end());

  int search_value = sample_data[(int) (MAX_VALUES  * .5)  ]; // .5 is halfway

  clock_t start,med,end;

  std::cout << "AVL (inserting accending values) " << std::endl;
  start = clock();
  AVL temp2;
  for (int loop2=0; loop2< MAX_VALUES; loop2++)
    {
    temp2.insert(sample_data[loop2]);
    }
  med = clock();

  TreeNode *  ptr2 = temp2.search(search_value);
  end = clock();
  std::cout << "insert time:"<< std::fixed  << (double) (med-start) << std::endl;
  std::cout << "search time:"<< std::fixed  << (double) (end - med) << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;


  return 0;
  }


