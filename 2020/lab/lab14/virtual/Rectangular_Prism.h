/*
	 All rights reserved. No part of this document may be reproduced, distributed,   or transmitted in any form or by any means,    including photocopying,             recording, or other electronic or mechanical methods, without the prior         written    permission of the author, Michael Sarr
 */
#pragma once
#include "cmpslib19.h"
#include "Rectangle.h"

class Rectangular_Prism : public Rectangle
{
    protected:
        double depth;

    public:
        Rectangular_Prism()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            depth = 0;
            shape_type = 5;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }

        Rectangular_Prism(int len, int wid, int dep)
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            length = len;
            width = wid;
            depth = dep;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }

        ~Rectangular_Prism()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }

        void SetDepth(double input)
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            depth = input;
            if (input < 0)
            {
                depth = 0;
            }
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }

        string ToString()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            ostringstream temp;
            temp << "Rectangular_Prism: " << endl;
            temp << "Length: " << length << endl;
            temp << "Width: " << width << endl;
            temp << "Depth: " << depth << endl;
            temp << "Area: " << Area() << endl;
            temp << "Volume: " << Volume() << endl;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return temp.str();
        }

        double Area()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            double area = 2 * (length * width) + 2 * (length * depth) + 2 * (width * depth);
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return area;
        }

        double Volume()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            double volume = depth * Rectangle::Area();
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return volume;
        }

};

