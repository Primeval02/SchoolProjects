#include "Square.h"

Square::Square()
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    SetLength(0);
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
}

Square::Square(double _length)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    SetLength(_length);
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
}

Square::~Square()
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
}

void Square::SetLength(double value)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    length = value;
    if (value < 0)
    {
        length = 0;
    }
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
}

double Square::Area()
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    double area = (length * length);
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
    return area;
}

string Square::ToString()
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    ostringstream temp;
    temp << "ToString() returns: Square: Length=" << length << " area=" << Area() << endl;
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
    return temp.str();
}
