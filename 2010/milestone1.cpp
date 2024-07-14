#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

class Words
{
    private:
        int minlen;
        int maxlen;
        int count = 0;
        string *choices;

        int count_candidates()
        {
            fstream file;
            file.open("enable1.txt", ios::in);
            if (file.is_open()) 
            {
                string word;
                while(getline(file, word))
                {
                    if (word.length() >= minlen && word.length() <= maxlen)
                    {
                        count++;
                    } 
                }
            }
            file.close();
            return count;
        }

        void load_words()
        {
            choices = new string[count];

            fstream file;
            file.open("enable1.txt", ios::in);
            if (file.is_open()) 
            {
                int i = 0;
                string word;
                while(getline(file, word))
                {
                    if (word.length() >= minlen && word.length() <= maxlen)
                    {
                        choices[i] = word;
                        i++;
                    }
                    
                }
            }
            file.close();
        }

    public:
        Words(int min, int max)
        {
            minlen = min;
            maxlen = max;
            count_candidates();
            load_words();
        }
        ~Words()
        {
            delete []choices;
        }

        string pick_word()
        {
            if (count == 0)
            {
                return "";
            }

            int index = rand() % count;

            string word = choices[index];

            return word;
        }
};

int main()
{
    srand(time(NULL));  // needs <ctime> included
    int min, max;

    cout << "Enter min: ";
    cin >> min;

    cout << "Enter max: ";
    cin >> max;

    Words words(min, max);

    cout << words.pick_word() << endl;

    return 0;
}