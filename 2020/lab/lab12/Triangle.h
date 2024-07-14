#pragma once
#include "easylogging++.h"
#include "cmpslib19.h"
#include "Square.h"

class Triangle : public Square
{
    // the Triangle class will contain


    //data members

  protected:
    double height;

    //functions

  public:/**/
    Triangle();
    /* set height to 0 ( use SetHeight  ), length should be set in Square constructor */


    Triangle(double, double);
    /*
    set length to value one passed in
    set height to value two passed in
    use SetLength (inherited from Square)  and SetHeight
    */


    ~Triangle();
    /* just log start & end */

    void SetHeight(double );
    /*
      this is a an additional function that the square class did not have 
			set height to value passed in , if value is less than zero set height to 0 */


    double Area();
    /* return the area,  (-5. (length * height)) 
       we are overriding the Area function of the Square class as we need it to be different as
       the formula to calculate the area of a Triangle is different from that of a Square
    */

    string ToString();
    /* return a formatted string like the example */

  };


