#include "cmpslib19.h"
#include "easylogging++.h"



class Computer
  {
  private:
      int cores;
      double processor_speed;
      int drive_size;
      int ram;
      string processor_type;
      static int count;

  public:
    
    Computer();

    Computer(int _cores, double _processor_speed, int _drive_size, int _ram, string _processor_type);

    Computer(const Computer & input);

    string ToString();

    ~Computer();

    static int GetCount();

    friend bool Greater(const Computer & x, const Computer & y);
    friend bool Less(const Computer & x, const Computer & y);
    friend bool Equal(const Computer & x, const Computer & y);
  };



