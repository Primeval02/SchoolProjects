#pragma once 

#include <cxxabi.h>
#include <bitset>
#include <time.h>
#include <string>   // string class
#include <string.h> // c-string library
#include <sstream>  // ostringstream
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>  // c file output
#include <stdarg.h>
#include <typeinfo> // find names of ADTs at runtime
#include <limits.h> // min and max values for dataytypes
#include <limits>
#include <ios>
#include <float.h>
#include <math.h>
#include <iomanip>
#include <stdexcept>  // exception handling .. 
#include <signal.h>   // for catching the segfaults
#include <random>

using std::string;
using std::cout;
using std::cin;
using std::ostringstream;
using std::setfill;
using std::setw;
using std::endl;
using std::boolalpha;


// this one is used by the prompt functions you wont need it
template <typename CharT>
std::streamsize ignore_line (std::basic_istream<CharT>& in, bool always_discard = false );



/**************************************
  user input
 ***************************************/


/* use std::cout  to send the first parameter "text" to the console
   take in input from the user and store it in the second parameter */



// a version specific for when parmam 2 is a string, read in a whole line
inline void Prompt (string prompt, string & val)
{
	std::cout  << prompt;
	ignore_line( std::cin );// clear the buffer if needed
	getline(std::cin,  val); // this 
}

// a generic verison for all other data types
	template <class T>
inline void Prompt (string prompt, T & val)
{

	std::cout  << prompt;
	std::cin >> val;
	while(std::cin.fail())  // if it fails... ie user input does not match the datatype
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout  <<  prompt;
		std::cin >> val;
	}
}


/* use std::cout  to send the first parameter "text" to the console
   take in input from the user until they enter a valid integer
   value between min and max , the 2nd and 3rd parameters */
	template <class T>
inline void Prompt( string prompt,T& val, T minval, T maxval)
{
	Prompt(prompt,val);
	while ( val < minval  || val > maxval)
	{
		std::cout  << "The entered value must be with the range " << minval << " to " << maxval << std::endl;
		Prompt(prompt,val);
	}
}





/*
   bool PromptYN(string input)
   display the string 'input'
   give the user a prompt
   if they type Y or y return true
   if they type N or n return false
   if they type anything else
   keep repeating till they type Y,y,N or n

usage:
if ( PromptYN("Do you like SPAM & eggs?")
std::cout  << "you answered Y or y" << std::endl;
else
std::cout  << "you answered N or n" << std::endl;
*/


inline bool PromptYN(string input)
{
	while (true)
	{
		std::cout  << input << " (Y/N)"<< std::endl;
		ignore_line( std::cin ); // clear the buffer if needed
		string user_input;
		std::cin >> user_input;
		if ( user_input == "Y" || user_input =="y")
		{
			return true;
		}
		if (user_input == "N" || user_input =="n")
		{
			return false;
		}
	}
}






/*********************************************
  system stuff
 *********************************************/

// Do nothing for a period of time
// // used to slow the operation of programs
// // the parameter passed in will be approximatley in 100ths of a second
// // 25 -> quarter of a second, 50 -> half a second
inline void WaitHundredth(int time)
{
	struct timespec tim, tim2;

	while (time > 0)
	{
		tim.tv_sec = 0;
		if (time > 99)
			tim.tv_nsec = 99*10000000;
		else
			tim.tv_nsec = time*10000000;
		nanosleep(&tim , &tim2);
		time -= 99;
	}
}


// clear the screen
#define ClearScreen() system("clear")


/*
   GetTime()
   returns the current time as a formatted char array
   The returned string has the following format:
   Www Mmm dd hh:mm:ss yyyy
   Where Www is the weekday, Mmm the month in letters, dd the day of the month, hh:mm:ss the time, and yyyy the year.
   The string is followed by a new-line character ('\n') and the terminating null-character.
   */
inline char * GetTime()
{
	time_t rawtime = time(0);
	struct tm * timeinfo;

	timeinfo = localtime ( &rawtime );
	return asctime (timeinfo);
}


inline string GetDateTime()
{
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	ostringstream tm;
	tm << (now->tm_year + 1900) << '-' 
		<< setfill('0') << setw(2) <<(now->tm_mon + 1) << '-'
		<< setfill('0') << setw(2) << now->tm_mday << "-"
		<< setfill('0') << setw(2) << now->tm_hour << ":"
		<< setfill('0') << setw(2) << now->tm_min << ":"
		<< setfill('0') << setw(2) << now->tm_sec ;
	return tm.str();
}



/****************************************
  Numberic fuctions

 *****************************************/

/*
   this will try to convert a string to in integer
   may throw and exception if it cant do it
   */

inline int StringToInteger( string input){return stoi(input);}
inline double  StringToDouble( string input ){return stod(input);}


inline bool StringToBool(string input)
{
	if(input=="true" || input =="1"){return true;}
	if(input=="false" || input =="0"){return false;}
	return false;
}



/*
   compare some floating point numbers
   this should work for type double as well

   it is difficult to compare for equality so this will tell us if they are within
   a distance of each other the default for that value is .0005

*/
inline bool VeryClose(float A, float B, float epsilon = 0.0005f)
{
	return (fabs(A - B) < epsilon);
}




/*
   this will try to convert a string to in integer
   may throw and exception if it cant do it
   */
	template <typename T>
inline	string NumberToString ( T Number )
{
	ostringstream ss;
	ss << Number;
	return ss.str();
}


inline int CreateRandomNumber(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min,max);
	return (dis(gen));
}

inline bool IsNumber(string str)
{
	for (unsigned int i = 0; i < str.length(); i++)
		if (! isdigit(str[i])){return false;}
	return true;
}

inline bool IsInteger(string input)
{
	try
	{
		int temp = stoi(input);
		temp++; // to avoid unused var warning
	}
	catch(...){return false;}
	return true;
}


inline bool IsDouble(string input)
{

	try
	{
		double temp = stod(input);
		temp++; // to avoid unused var warning
	}
	catch(...){return false;}
	return true;
}


inline string PassFail(int in)
{
	/* the parameter to this function should be bool but since 
	   some people cant understand the  basic idea of an expression
	   i had to change it to take int
	   */
	if ( in > 1) 
	{  
		std::cout << "The PassFail function is designed to take an expresson as its parameter\n something that evaluates to true or false\n you are not using the fuction properly stopping execution of your program. line number " <<__LINE__ << endl;
		exit(9);
	}
	return (in) ? "Pass":"Fail";

}
inline string PF(int in) {return PassFail(in);}

/*
   inline string PF(int  in){return (in) ? "Pass":"Fail";}

   inline string PassFail(bool in){return (in) ? "Pass":"Fail";}
   inline string PF(bool in){return (in) ? "Pass":"Fail";}
   */
/*
 ****************************************************
 *  Array based functions                           *
 ****************************************************/

/* a function that will tell you if an array is sorted
params : pointer to the array, int array size  */
	template<class T>
inline bool IsSortedAscending(T* data,unsigned int size)
{
	for(unsigned int loop=0;loop<size-1;loop++)
		if(data[loop]>data[loop+1])
			return false;
	return true;
}

/* this is a kinda cool way to do it, pass in a reference to the array
   unfortunatly it isnt very compatable with most exisitng progams as 
   it implememts some new features to c++ 
   but you dont have to pass in the size as a 2nd parameter        */
	template<class T,size_t N>
inline bool IsSortedAscending(T(&data )[N])
{
	// data is a reference to the array and N is the size of the array
	for(unsigned int loop=0;loop<N -1;loop++)
		if(data[loop]>data[loop+1])
			return false;
	return true;
}


	template<class T>
inline bool IsSortedDescending(T* data,unsigned int size)
{
	for(unsigned int loop=0;loop<size-1;loop++)
		if(data[loop]<data[loop+1])
			return false;
	return true;
}

/* this is a kinda cool way to do it, pass in a reference to the array
   unfortunatly it isnt very compatable with most exisitng progams as 
   it implememts some new features to c++ 
   but you dont have to pass in the size as a 2nd parameter        */
	template<class T,size_t N>
inline bool IsSortedDescending(T(&data )[N])
{
	// data is a reference to the array and N is the size of the array
	for(unsigned int loop=0;loop<N -1;loop++)
		if(data[loop]<data[loop+1])
			return false;
	return true;
}

	template<class T>
inline void ResizeArray(T*& data,size_t newsize,size_t copy =0)
{
/* data is a T* reference so we can change the value of the pointer
   and make it point to someplace else */

T* temp = new T[newsize];
// copy over old values if we need to 
for(size_t loop=0;loop < copy;loop++)
 temp[loop]=data[loop];

delete[] data;
data = temp;// this is why we passed it in by reffernce... T*& NOT T* .. so we can change it
}
/***************************************
file functions
****************************************/
//TODO

// void FromFile(string filename, string & output)
// void FromFile(string filename, vector<string> &output)





/***************************************
  console functions
 ****************************************/


/*
   returns the input string wrapped in the commands to change color , display the text and change back to white
   std::cout  << "the cat is " << RED("named felix" << " the cat" << std::endl;
   only "named felix" will be red
   */

inline string RED(string input){return string("\033[1;31m"+input+"\033[0m");}
inline string GREEN(string input){return string("\033[1;32m"+input+"\033[0m");}
inline string YELLOW(string input){return string("\033[1;33m"+input+"\033[0m");}
inline string BLUE(string input){return string("\033[1;34m"+input+"\033[0m");}
inline string MAGENTA(string input){return string("\033[1;35m"+input+"\033[0m");}
inline string CYAN(string input){return string("\033[1;36m"+input+"\033[0m");}

/* returns a string that when pushed to std::cout  will change the console output color
   you will probably want to set it back white at some point */

inline string ChangeToRed(){return string("\033[1;31m");}
inline string ChangeToGreen(){return string("\033[1;32m");}
inline string ChangeToYellow(){return string("\033[1;33m");}
inline string ChangeToBlue(){return string("\033[1;34m");}
inline string ChangeToMagenta(){return string("\033[1;35m");}
inline string ChangeToCyan(){return string("\033[1;36m");}
inline string ChangeToWhite(){return string("\033[0m");}



/* move the cursor to the position indcated by the row and column.. Note the offset is 1 not 0 */
inline void PositionCursor(int row,int col){printf("\033[%i;%iH",row+1,col+1);}


	template <typename T>
inline std::string GetClassName(const T & a)
{
	int itemp=0;
	char * ptemp = abi::__cxa_demangle( typeid(T).name(),0,0,&itemp);
	std::string temp( ptemp);
	free (ptemp);
	return temp;
}


  template <typename T>
inline  string PDS ( T val )
{
  ostringstream ss;
  ss <<"type: " << GetClassName( val ) << " size:" << sizeof(T) << " value:" << val;
  return ss.str();
}

  template <typename T>
inline  string PDSXML ( T val ,string name)
{
  ostringstream ss;
  ss <<"<value name='" << name << "'>" << val << "</value>";
  return ss.str();
}





/*********************************************************************************************************************
 **********************************************************************************************************************
 below are functions that you wont be calling they are
 for use by other functions f20
 **********************************************************************************************************************
 **********************************************************************************************************************/

	template <typename CharT>
std::streamsize ignore_line (
		std::basic_istream<CharT>& in, bool always_discard )
{
	std::streamsize nread = 0;
	if ( always_discard
			|| ( in.rdbuf()->sungetc() != std::char_traits<CharT>::eof()
				&& in.get() != in.widen ( '\n' ) ) )
	{
		// The stream is good, and we haven't
		// read a full line yet, so clear it out
		in.ignore ( std::numeric_limits<std::streamsize>::max(), in.widen ( '\n' ) );
		nread = in.gcount();
	}
	return nread;
}


