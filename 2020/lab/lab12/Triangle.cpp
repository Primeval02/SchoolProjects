#include "Triangle.h"

Triangle::Triangle()
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    SetHeight(0);
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
}

Triangle::Triangle(double _length, double _height)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    SetLength(_length);
    SetHeight(_height);
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
}

Triangle::~Triangle()
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;    
}

void Triangle::SetHeight(double value)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    height = value;
    if (value < 0)
    {
        height = 0;
    }
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
}

double Triangle::Area()
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    double area = (0.5) * (length * height);
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
    return area;
}

string Triangle::ToString()
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    ostringstream temp;
    temp << "ToString() returns: Triangle: Length=" << length << " Height=" << height << " area=" << Area() << endl;
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
    return temp.str();
}
