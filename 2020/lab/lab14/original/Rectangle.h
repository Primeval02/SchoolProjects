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
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            width = 0;
            shape_type = 4;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }

        Rectangle(int len, int wid)
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            length = len;
            width = wid;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }

        ~Rectangle()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
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

