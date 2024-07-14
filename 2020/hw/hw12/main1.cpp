// Purpose: HW 7 Hash Tables

//          This implements a static sized array-based hash table that uses

//          double hashing for collision resolution. The double hashing probe

//          sequence is hash1 , hash 2



#include <iostream>
#include <limits>
#include <string.h>
using namespace std;
#include "cmpslib19.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP


#include "Hash.h"

char menu();
int main()
  {
  // set up the logger

  el::Configurations LGR;
  LGR.setToDefault();// load current configuration

  #if LOGGING ==ON
    LGR.setGlobally(el::ConfigurationType::Filename,"example.log");// set logname

    LGR.set(el::Level::Info,el::ConfigurationType::ToStandardOutput,"false");// dont push LOG (INFO) to console

  #else
    LGR.setGlobally(el::ConfigurationType::Enabled,"false");// disable all the logging

  #endif
  el::Loggers::reconfigureLogger("default", LGR);// store new configuration







  HashTable h;  // Declare the table.

  char sel;
  int  num;

  do
    {
    try
      {
      sel = menu();
      switch(sel)
        {
        case 'x':
          cout << "Good-bye!\n";
          break;

        case 'i':
          Prompt("Enter an integer: ", num);
          cout << "Inserting " << num << " into the hash table." << endl;
          if (h.insert(num))
          {
              cout << num << " inserted successfully." << endl;
          }
          else
          {
              cout << num << " NOT inserted successfully." << endl;
          }
          break;

        case 'd':
          Prompt("Enter an integer: ", num);
          cout << "Deleting " << num << " from the hash table." << endl;
          if (h.remove(num))
          {
              cout << num << " removed successfully." << endl;
          }
          else
          {
              cout << num << " NOT removed successfully." << endl;
          }
          break;

        case 's':
          Prompt("Enter an integer: ", num);
          cout << "Searching for " << num << " in the hash table." << endl;
          if (h.search(num))
          {
              cout << num << " was found." << endl;
          }
          else
          {
              cout << num << " was NOT found." << endl;
          }
          break;

        case 'k':
          Prompt("Enter an integer: ", num);
          cout << "Primary key for " << num << " is " << h.hash1(num) << endl;
          cout << "Secondary Key for " << num << " is " << h.hash2(num) << endl;
          break;

        case 'p':
          cout << h.ToString() << endl;
          break;
        
        default:
          cout << "Invalid selection!\n";
        }
      }
      catch(string val)
      {
        cout <<"Exception: " << val << endl;
      }
      catch(char * const val)
      {
        cout << "Exception: " << val << endl;
      }



    }
  while(sel != 'x');

  return 0;
  }

char menu()
  {
  char sel;

  cout << endl;
  cout << "          Hash Table Menu\n";
  cout << "  ===================================\n";
  cout << "  i. Insert an integer into the table\n";
  cout << "  d. Delete an integer from the table\n";
  cout << "  s. Search the table for an integer \n";
  cout << "  k. Print the primary and secondary \n";
  cout << "     keys for an integer             \n";
  cout << "  p. Print HashTable                 \n";
  cout << "                                     \n";
  cout << "  x. Exit                            \n";
  cout << "  ===================================\n";
  Prompt("  Enter selection: ",sel);
  cout << endl;
  return sel;
  }






