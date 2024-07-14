/*
   All rights reserved. No part of this document may be reproduced, distributed,   or transmitted in any form or by any means,    including photocopying,             recording, or other electronic or mechanical methods, without the prior         written    permission of the author, Michael Sarr
 */
#pragma once
#include "cmpslib19.h"
#include "Square.h"

class Cube : public Square
{
    public:
        Cube()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            shape_type = 3;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }

        Cube(int input)
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            length = input;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }

        ~Cube()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        } 

        string ToString()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            ostringstream temp;
            temp << "Cube: " << endl;
            temp << "Length: " << length << endl;
            temp << "Area: " << Area() << endl;
            temp << "Volume: " << Volume() << endl;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return temp.str();
        }

        double Area()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            double area = Square::Area() * 6;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return area;
        }

        double Volume()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            double volume = Square::Area() * length;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return volume;
        }
};


