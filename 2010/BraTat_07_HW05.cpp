#include <iostream>
#include <string>

using namespace std;

class Card
{
    private:
        int suit;
        int value;

    public:
        Card(int s, int v)
        {
            suit = s;
            value = v;
        }

        bool operator <= (Card rhs)
        {
            if (value == 1)
            {
                value = 14;
            }

            if (value <= rhs.value)
            {
                return false;
            }

            else
            {
                return true;
            }
        }
        
        friend ostream & operator << (ostream & out, Card & rhs);
};

string as_string(int value, int suit);

int main()
{
    Card test1(1, 1);
    Card test2(1, 10);

    cout << test1 << endl;

    if (test2 <= test1)
    {
        cout << test1 << " beats " << test2 << endl;
    }

    else
    {
        cout << test2 << " beats " << test1 << endl;
    }

    return 0;
}

ostream & operator << (ostream & out, Card & rhs)
{
    out << as_string(rhs.value, rhs.suit);
    return out; 
}


        string as_string(int value, int suit)
        {
            string full_card;

            if (value == 1)
            {
                full_card = "Ace ";   
            }

            else if (value == 11)
            {
                full_card = "Jack ";
            }

            else if (value == 12)
            {
                full_card = "Queen ";
            }

            else if (value == 13)
            {
                full_card = "King ";
            }

            else
            {
                full_card = to_string(value) + " ";
            }
            
            full_card += "of ";

            if (suit == 1)
            {
                full_card += "Diamonds";
            }

            else if (suit == 2)
            {
                full_card += "Hearts";
            }

            else if (suit == 3)
            {
                full_card += "Clubs";
            }

            else if (suit == 4)
            {
                full_card += "Spades";
            }

            return full_card;
        }
