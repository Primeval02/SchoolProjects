#include "computer.h"



// when putting your funcion bodies outside the class.. in a cpp file 
// ReturnType Classname::FunctionName(parameters)
string Computer::ToString()
{
	LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
	ostringstream out; // a temporary stream, use just like you would cout but it stays in memory
	out << processor_speed	<< " Ghz "		<< processor_type << " "
		<<  cores	<< " Cores"				<< endl
		<< drive_size		<< " Gb disk"	<< endl
		<< ram				<< " Gb ram"	<< endl;

  string temp = out.str(); // put all the text in the stream into a single string
	LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
	return(temp);
}

Computer::Computer(int _cores, double _processor_speed, int _drive_size, int _ram, string _processor_type)
{
    LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
    cores = _cores;
    processor_speed = _processor_speed;
    drive_size = _drive_size;
    ram = _ram;
    processor_type = _processor_type;
    LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
}

Computer::Computer(const Computer & input)
{
    LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
    cores = input.cores;
    processor_speed = input.processor_speed;
    drive_size = input.drive_size;
    ram = input.ram;
    processor_type = input.processor_type;
    LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
}

Computer::Computer()
{
    LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
    cores = 0;
    processor_speed = 0;
    drive_size = 0;
    ram = 0;
    processor_type = "none";
    LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
}

Computer::~Computer()
{
    LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
    LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
}
