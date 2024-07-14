/*
	 All rights reserved. No part of this document may be reproduced, distributed,   or transmitted in any form or by any means,    including photocopying,             recording, or other electronic or mechanical methods, without the prior         written    permission of the author, Michael Sarr
 */
#pragma once
#include "cmpslib19.h"
#include "Line.h"

class Rectangle : public Line
{
    protected:
        double width;

    public:
        Rectangle()
        {
            width = 0;
        }

        Rectangle(int len, int wid)
        {
            length = len;
            width = wid;
        }
        void SetWidth(double input)
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            width = input;
            if (input < 0)
            {
                width = 0;
            }
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }

        string ToString()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            ostringstream temp;
            temp << "Rectangle: " << endl;
            temp << "Length: " << length << endl;
            temp << "Width: " << width << endl;
            temp << "Area: " << Area() << endl;
            temp << "Volume: " << Line::Volume() << endl;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return temp.str();
        }

        double Area()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            double area = length * width;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return area;
        }

};

