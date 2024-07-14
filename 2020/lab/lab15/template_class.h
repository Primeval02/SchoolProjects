#include "cmpslib19.h"
#include "easylogging++.h"

template <class T>
class  demo
{
    private:
        T data;

    public:
        T getData()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return data;
        }

        void setData(T input)
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            data = input;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }

        friend std::ostream& operator << (std::ostream& os, demo& s)
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            os << "  size of (data) = " << sizeof(s.data) << endl;
            os << "  data is of type " << GetClassName(s.data) << endl;
            os << "  data contains: " << s.data << endl; 
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return os;
        }

};

