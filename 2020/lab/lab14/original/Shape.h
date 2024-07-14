/*
	 All rights reserved. No part of this document may be reproduced, distributed,   or transmitted in any form or by any means,    including photocopying,             recording, or other electronic or mechanical methods, without the prior         written    permission of the author, Michael Sarr
 */
#pragma once
#include "cmpslib19.h"

class Shape
{
    public:
        int shape_type;

        Shape()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            shape_type = 0;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }

        ~Shape()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }

        double Area()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return 0;
        }

        double Volume()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return 0;
        }

        string ToString()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            ostringstream temp;
            temp << "Shape: " << endl;
            temp << "Area: " << Area() << endl;
            temp << "Volume: " << Volume() << endl;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return temp.str();
        }
};


