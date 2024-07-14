#pragma once
#include "easylogging++.h"
#include "cmpslib19.h"

class Square
    {
    // the Square class will contain


    // data members

  protected:
    double length;


    // functions

  public:
    Square();
    /* set lenght to 0 */


    Square(double);
    /* set length to parameter, use your function to SetLength so it has the logic to ensure it is 
       a legitimate value  ( you dont want the same logic in more than one place) */


    ~Square();
    /* just log start & end */


    void SetLength(double);
    /* set length to value passed in , if value is less than zero set length to 0 */


    double Area();
    /* return the area,  (length * length) */


    string ToString();
    /* return a formatted string like the example */


  };





