#include <iostream>
using namespace std;
#include "cmpslib19.h"
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP



#define ELEMTYPE string 
#define QUEUESIZE 10
#include "Queue.h"

int main()
{
 InitializeLogger("main4.log");


	Queue Queue1;
	cout << Queue1.ToString() << endl;
	Queue1.enQueue("The");	
	Queue1.enQueue("queue");	
	Queue1.enQueue("is");	
	Queue1.enQueue("FIFO");	
	Queue1.enQueue("first");	
	Queue1.enQueue("in");	
	Queue1.enQueue("first");	
	Queue1.enQueue("out");	
	cout << Queue1.ToString() << endl;
	Queue1.enQueue("of");
	Queue1.enQueue("sight");
	Queue1.enQueue("dont");
	Queue1.enQueue("add");
	Queue1.enQueue("these");
	Queue1.deQueue();
	Queue1.deQueue();
	Queue1.enQueue("It");
	Queue1.enQueue("works");
	Queue1.deQueue();
	Queue1.deQueue();
	Queue1.deQueue();
	Queue1.deQueue();
	Queue1.deQueue();
	cout << Queue1.ToString() << endl;
	return 0;
}


