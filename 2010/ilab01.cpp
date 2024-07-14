#include <iostream>

using namespace std;

int main()
{
    char input;
    int health = 100; 
    int lives = 3;
    string code;

    do
    {
        cout << "Let's Play Super Manibo!" << endl;
        cout << "  b - Break Pots (+5)" << endl;
        cout << "  s - Slay Bugs (+10)" << endl;
        cout << "  p - Poisoned by Borat (-7)" << endl;
        cout << "Command: ";
        cin >> input;

        switch (input)
        {
            case 'b': health += 5;
                      code += "b";
                      break;

            case 's': health += 10;
                      code += "s";
                      break;

            case 'p': health -= 7;
                      code += "p";
                      break;

            case 'l': lives += 1;
                      break;
        }

        if (health > 100)
        {
            health = 100;
        }

        if (health < 0)
        {
            health = 100;
            lives -= 1;
        }

        if (code == "bspbbs")
        {
            lives += 100;
        }

        cout << endl;
        cout << "Health: " << health << endl << endl;
        cout << "Lives : " << lives << endl << endl;

    } while(lives > 0);
   
    cout << "Game Over" << endl;
    
    return 0;
}
