/*
   All rights reserved. No part of this document may be reproduced, distributed,   or transmitted in any form or by any means,    including photocopying,             recording, or other electronic or mechanical methods, without the prior         written    permission of the author, Michael Sarr
 */
#pragma once
#include "cmpslib19.h"
#include "Shape.h"

class Line : public Shape
{
    protected:
        double length;

    public:
        Line()
        {
            length = 0;
        }

        Line(int input)
        {
            SetLength(input);
        }

        string ToString()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            ostringstream temp;
            temp << "Line: " << endl;
            temp << "Length: " << length << endl;
            temp << "Area: " << Shape::Area() << endl;
            temp << "Volume: " << Shape::Volume() << endl;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return temp.str();
        }

        void SetLength(double input)
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            length = input;
            if (input < 0)
            {
                length = 0;
            }
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }
};
