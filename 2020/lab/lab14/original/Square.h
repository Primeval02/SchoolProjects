/*
   All rights reserved. No part of this document may be reproduced, distributed,   or transmitted in any form or by any means,    including photocopying,             recording, or other electronic or mechanical methods, without the prior         written    permission of the author, Michael Sarr
 */
#pragma once
#include "cmpslib19.h"
#include "Line.h"

class Square : public Line
{
    public:

        Square()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            shape_type = 2;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }

        Square(int input)
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            length = input;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }

        ~Square()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }

        string ToString()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            ostringstream temp;
            temp << "Square: " << endl;
            temp << "Length: " << length << endl;
            temp << "Area: " << Area() << endl;
            temp << "Volume: " << Line::Volume() << endl;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return temp.str();
        }

        double Area()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            double area = length * length;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return area;
        }

};
