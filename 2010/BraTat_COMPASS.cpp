#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>

using namespace std;

int show_menu();
void get_range_num(string prompt, int min, int max, int & number);

class Words
{
    private:
        int minlen; 
        int count = 0;
        int maxlen;
        string *choices;

        int count_candidates()
        {
            ifstream fin;
            fin.open("enable1.txt");
            if (fin.is_open())
            {
               string word;
               while(!fin.eof())
               {
                   getline(fin, word);
                   if (word.length() >= minlen && word.length() <= maxlen)
                   {
                       count++;
                   }
               }
            }
            fin.close();
            return count;
        }

        void load_words()
        {
            choices = new string[count];

            ifstream fin;
            fin.open("enable1.txt");
            if (fin.is_open())
            {
                int i = 0;
                string word;
                while(!fin.eof())
                {
                    getline(fin, word);
                    if (word.length() >= minlen && word.length() <= maxlen)
                    {
                        choices[i] = word;
                        i++;
                    }
                }
                
            }  
            fin.close();
   
        }

    public:
        Words(int min, int max)
        {
            minlen = min;
            maxlen = max;

            count_candidates();
            load_words();
        }

        string pick_word()
        {
            if (count == 0)
            {
                return "";
            }
            
            int random = rand() % count;
            string word = choices[random];
            return word;
            
        }

        ~Words()
        {
            delete []choices;
        }

        void reset(int min, int max)
        {
            minlen = min;
            minlen = max;

            if (choices != NULL)
            {
                delete []choices;
            }

            count_candidates();
            load_words();
        }
};

class Hangman
{
    private:
        char word[40]; 
        char progress[40];
        int word_length;

        void clear_progress(int length)
        {
            for(int i = 0; i < length; i++)
            {
                progress[i] = '-';
            }
        }

    protected:
        int matches;
        char last_guess;
        string chars_guessed;
        int wrong_guesses;
        int remaining;
        int stage;

        bool check(char user_guess)
        {
            bool x;
            for(int i = 0; i < strlen(word); i++)
            {
                if (user_guess == word[i])
                {
                    progress[i] = user_guess;
                    x = true;
                }

                else
                {
                    x = false;
                }
                
                for (int i = 0; i < chars_guessed.length(); i++)
                {
                    if (user_guess != chars_guessed.at(i))
                    {
                        chars_guessed += user_guess;
                        wrong_guesses++;
                        remaining++;
                        stage++;
                        break;
                    }
                }
            }

         return x;
           
        }

    public:
        void initialize(string start)
        {
            chars_guessed = "";
            wrong_guesses = 0;
            remaining = 6;
            strcpy(word, start.c_str());
            word_length = strlen(word);
            clear_progress(strlen(word));
        }

        Hangman(string a)
        {
            initialize(a);
        }

        char * get_word()
        {
            return word;
        }

        char * get_progress()
        {
            return progress;
        }

        int get_remaining()
        {
            return remaining;
        }

        bool is_word_complete()
        {
            if (strcmp(progress, word) == 0)
            {
                return true;
            }
            
            else
            {
                return false;
            }
        }

};

class HangmanConsole: public Hangman
{
    public:
        HangmanConsole(string start)
        {
               
        }
        
        void show_status(int stage)
        {
            switch (stage)
            {
                case 0:
                    cout << " +-----\n";
                    cout << " |     \n";
                    cout << " |     \n";
                    cout << " |     \n";
                    cout << " |     \n";
                    cout << " ----- \n";
                    break;

                case 1:
                    cout << " +-----\n";
                    cout << " |  0   \n";
                    cout << " |     \n";
                    cout << " |     \n";
                    cout << " |     \n";
                    cout << " ----- \n";
                    break;
 
                case 2:
                    cout << " +-----\n";
                    cout << " |  0   \n";
                    cout << " |  |   \n";
                    cout << " |  |   \n";
                    cout << " |     \n";
                    cout << " ----- \n";
                    break;

                case 3:
                    cout << " +-----\n";
                    cout << " |  0   \n";
                    cout << " |  |   \n";
                    cout << " |  |    \n";
                    cout << " | /     \n";
                    cout << " ----- \n";
                    break;
 
                case 4:
                    cout << " +-----\n";
                    cout << " |  0  \n";
                    cout << " |  |   \n";
                    cout << " |  |   \n";
                    cout << " | / \\ \n";
                    cout << " ----- \n";
                    break;
 
                case 5:
                    cout << " +-----\n";
                    cout << " |  0   \n";
                    cout << " | /|    \n";
                    cout << " |  |    \n";
                    cout << " | / \\  \n";
                    cout << " ----- \n";
                    break;

                 case 6:
                    cout << " +-----\n";
                    cout << " |  0   \n";
                    cout << " | /|\\  \n";
                    cout << " |  |    \n";
                    cout << " | / \\  \n";
                    cout << " ----- \n";
                    break;
            }
        }

        void show_info()
        {
            get_progress();
            show_status(stage);
          /*  for (int i = 0; i < strlen(progress); i++)
            {
                if (last_guess == progress[i])
                {
                    matches++;
                }
            }*/
            cout << "Matches From Last Guess: " << matches << endl;
            cout << "Wrong Guesses: " << wrong_guesses << endl;
            cout << "Attempts Remaining: " << remaining << endl;
            cout << "Letters Guessed So Far: " << chars_guessed << endl;

        }

        friend istream & operator >> (istream & in, HangmanConsole & rhs);
};

int main()
{
    int menu_choice;
    srand(time(NULL));
    int max, min;
   
    do
    {
        int test = show_menu();

        if (test == 1)
        {
            Words words(rand() % (40 - 3 + 1) + 3, rand() % (40 - 3 + 1) + 3);
        }

        else if (test == 2)
        {
            Words words(7, 12);
        }

        else if (test == 3)
        {
            get_range_num("Enter min: ", 2, 40, min);
            get_range_num("Enter max: ", min, 40, max);

            Words words(min, max);
        }

        else if (test == 4)
        {
            if (max != 0 && min != 0)
            {
                Words words(min, max);
            }

            else
            {
                cout << "No previous values detected, enter new bounds" << endl;
                get_range_num("Enter min: ", 2, 40, min);
                get_range_num("Enter max: ", min, 40, max);

                Words words(min, max);

            }

        }

        else if (test == 5)
        {
            string player_word;
            cout << "Player 1 enter word: ";
            cin >> player_word;
            Hangman word(player_word);
        }

        HangmanConsole game(words.pick_word());
    
        cout << "HANGMAN" << endl << "-------" << endl << endl;
        cout << "Your words is: " << game.get_progress() << endl;

        while (!game.is_word_complete() && game.get_remaining() > 0)
        {
            cout << endl;
            cout << "Enter your guess: ";
            cin >> game;

            system("clear");
            game.show_info();
        }
    }
    while(menu_choice != 6);
    return 0;
}

istream & operator >> (istream & in, HangmanConsole & rhs)
{
    cout << "Enter a single character: ";
    in >> rhs.last_guess;
    check(rhs.last_guess);
    return in;
}

int show_menu()
{
    cout << "1. Play the computer (auto select random range)" << endl;
    cout << "2. Play the computer (min: 7, max: 12)" << endl;
    cout << "3. Play the computer (user selects/changes range)" << endl;
    cout << "4. Play the computer (use same range stored in 3)" << endl;
    cout << "5. Two player mode (user1 enters the word, user2 guesses)" << endl;
    cout << "6. Quit" << endl << endl;
    cout << "Enter choice: ";
    cin >> menu_choice;
    return menu_choice;
}

void get_range_num(string prompt, int min, int max, int & number)
{  
   int reply;

    while (!(reply >= min) && !(reply <= max))
    {
        cout << prompt;
        cin >> reply;
    }

   number = reply; 
}
