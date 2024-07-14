#include <iostream>

using namespace std;

class Kiosk
{
    private:
        double adult_cost;
        double child_cost;
        double senior_cost;
        string * names;
        double total;
        int attendees;
        int index;

    public:
        Kiosk(double act, double cct, double sct)
        {
            adult_cost = act;
            child_cost = cct;
            senior_cost = sct;
            index = 0;
        }

        double get_total()
        {
            return total;
        }

        int set_counts(int adults, int children, int seniors)
        {
            attendees = adults + children + seniors;
            names = new string[attendees];
            total = (adult_cost * adults) + (child_cost * children) + (senior_cost * seniors);
            return attendees;
        }
        
        void add_name(string name)
        {
            names[index] = name;
            index++;
        }

        string get_names()
        {
            string name_list;

            for (int i = 0; i < index; i++)
            {
                name_list = name_list + " " +  names[i];               
            }

            return name_list; 
        }

        ~Kiosk()
        {
            delete [] names;
        }

};

int main()
{
    Kiosk * machine = new Kiosk(15, 10, 8);

    int act, cct, sct, count = 0;
    string name;

    cout << "How many adults? ";
    cin >> act;
    cout << "How many children? ";
    cin >> cct;
    cout << "How many seniors? ";
    cin >> sct;

    count = machine->set_counts(act, cct, sct);

    cout << endl;

    for (int i = 0; i < count; i++)
    {
        cout << "Name of attendee: ";
        cin >> name;
        machine->add_name(name);
    }

    cout << endl << "Total cost: " << machine->get_total() << endl;
    cout << "Enjoy the show," << machine->get_names() << "!\n";

    delete machine;
    
    return 0;
}
