#include "cmpslib19.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP



#include "functions.h"


void PrintArray(int*v,int size)
  {
  std::cout << "Array: ";
  for (int loop =0; loop<size; loop++)
    {
    std::cout << v[loop] << ",";
    }
  std::cout << endl;
  }

int main()
  {
  InitializeLogger("main1.log");



  int testarray1[]= { 10,95,20,85,30,75,40,65,50,55,60,45,70,35,80,25,5,15,90,100};
  int testarray2[]= { 10,95,20,85,30,75,40,65,50,55,60,45,70,35,80,25,5,15,90,100};
  int testarray3[]= { 10,95,20,85,30,75,40,65,50,55,60,45,70,35,80,25,5,15,90,100};


  int Sort1_comparisons=0;
  int Sort2_comparisons=0;
  int Sort3_comparisons=0;

  Sort1_comparisons = Sort1(testarray1,20);
  Sort2_comparisons = Sort2(testarray2,20);
  Sort3_comparisons = Sort3(testarray3,20);

  cout << "Is testarray1 sorted:" << IsSortedAscending(testarray1,20) << endl;
  PrintArray(testarray1,20);
  cout << "Is testarray2 sorted:" << IsSortedAscending(testarray2,20) << endl;
  PrintArray(testarray2,20);
  cout << "Is testarray3 sorted:" << IsSortedAscending(testarray3,20) << endl;
  PrintArray(testarray3,20);


  cout << "Sort1 required " << Sort1_comparisons << " comparisons to complete the sort"<< endl;
  cout << "Sort2 required " << Sort2_comparisons << " comparisons to complete the sort"<< endl;
  cout << "Sort3 required " << Sort3_comparisons << " comparisons to complete the sort"<< endl;


  return 0;
  }




