//optimized by: Bradley Tate
//date: 3/24/2022
//
//Lab framework
//This program spell checks the words in a text file.
//
//Original author: Gordon Griesel
//Date:            2014 - 2022
//
//Assignment:
//Optimize this program for running time.
//Your time will be compared to other students in the class.
//
//Test like this:
//   $ time ./lab9 test
//   $ time ./lab9 article
//
//
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <fstream>
using namespace std;
const char dictname[] = "/home/fac/gordon/public_html/3350/dictionary";
const double oobillion = 1.0 / 1e9;

class Profile {
public:
	timespec start, end;
	double diff, total;
	Profile() {
		diff = 0.0;
		total = 0.0;
	}
	void get_start_time() {
		clock_gettime(CLOCK_REALTIME, &start);
	}
	void get_end_time() {
		clock_gettime(CLOCK_REALTIME, &end);
		total += timeDiff();
	}
	double timeDiff() {
	    return (double)(end.tv_sec - start.tv_sec ) +
	            (double)(end.tv_nsec - start.tv_nsec) * oobillion;
	}
	void timeCopy(timespec *dest, timespec *source) {
	    memcpy(dest, source, sizeof(timespec));
	}
} p1, p2, p3;

enum {
	LOWER,
	UPPER
};

//function prototypes
int spell_check(char *text);
int search_for_word(char **words, int n, char *xword);

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("\nUsage: %s <file name>\n\n", argv[0]);
		return 0;
	}
	char fname[256];
	strcpy(fname, argv[1]);
	printf("\n");
	printf("Lab-9 program optimization.\n");
	printf("spell-check this file: **%s**\n", fname);
	ifstream fin(fname);
	if (fin.fail()) {
		printf("ERROR: opening **%s** for input\n", fname);
		return 0;
	}
	int nMisspelled = 0;
	char word[256];
	fin >> word;
	while (!fin.eof()) {
        p1.get_start_time(); // <<---profiling
		nMisspelled += spell_check(word);
        p1.get_end_time();   // << ---profiling
		fin >> word;
	}
	fin.close();
	printf("\nTotal misspelled words: %i\n", nMisspelled);
	printf("Program complete.\n\n");
    printf("profiler p1: %lf\n", p1.total);
    printf("profiler p2: %lf\n", p2.total);
    printf("profiler p3 case change: %lf\n", p3.total);
	return 0;
}

char **array(ifstream &fin, int &n)
{
    char word[100];
    int count = 0;
    while(1){
        fin >> word;
        if (fin.eof())
            break;
        ++count;
    }
    char **words = new char *[count];
    fin.clear();
    fin.seekg(fin.beg);
    n = 0;
    while(1){
        fin >> word;
        if (fin.eof())
            break;
        words[n] = new char [strlen(word)+1];
        strcpy(words[n++], word);
    }
    fin.close();

    for (int i = 0; i < n-1; i++){
        int nswaps = 0;
        for (int j = 0; j < n-1-i; j++){
            if (strcmp(words[j], words[j+1]) > 0){
                char *tmp = words[j];
                words[j] = words[j+1];
                words[j+1] = tmp;
                ++nswaps;
            }
        }
        if (!nswaps)
            break;
    }
    return words;
}

int spell_check(char *xword)
{
    static int first = 1;
    static int n = 0;
    static char **words;
	//Open the dictionary file
	ifstream din(dictname);
	if (din.fail()) {
		printf("Dictionary file failed to open!\n\n");
		exit(1);
	}
    if (first) {
        first = 0;
        words = array(din, n);
    }
	int found = search_for_word(words, n, xword);
	if (found != 1) {
		printf("  Misspelled: %s\n", xword);
	} else {
		//Nothing to do here yet.
	}
	din.close();
	if (found != 0)
		return 0;
	else
		return 1;
}

void convert_to_a_case(int case_flag, char *str)
{
	//convert a c-string to a case
	//case_flag: 0 = lower
	//           1 = upper
	int slen = strlen(str);
	for (int i=1; i<=slen; i++) {
		if (case_flag == 0)
			str[i-1] = (char)tolower(str[i-1]);
	}
	for (int i=1; i<=slen; i++) {
		if (case_flag == 1)
			str[i-1] = (char)toupper(str[i-1]);
	}
}

void cleanup_punctuation_characters(char *str)
{
	//Strip comma, period, colon, semi-colon from end of a string.
	char *p = str;
	//First, go all the way to the end of the string.
	while (*p)
		++p;
	--p;
	//Go backwards looking for a comma.
	while (p >= str && (*p == ',')) {
		*p = '\0';
		--p;
	}
	//Go backwards looking for a period.
	while (p >= str && (*p == '.')) {
		*p = '\0';
		--p;
	}
	//Go backwards looking for a colon.
	while (p >= str && (*p == ':')) {
		*p = '\0';
		--p;
	}
	//Go backwards looking for a semi-colon.
	while (p >= str && (*p == ';')) {
		*p = '\0';
		--p;
	}
}

int search_for_word(char **words, int n, char *word)
{
	//Search the dictionary for a word.
	//
	//returns: 0 = word not found.
	//         1 = word found.
	//
	//This function call will remove punctuation characters from the end of
	//a word from the article being spell-checked.
	cleanup_punctuation_characters(word);
	//
	//Go to the top of the dictionary file.
	//fin.clear();
	//fin.seekg(0, fin.beg);
	//Now search through the whole file looking for the word.
	//Search top to bottom.
	//char line[256];
	int found = 0;

    //moved here from inside while loop
    convert_to_a_case(LOWER, word);

    p2.get_start_time();  // <---profiling
	//fin >> line;
	//while (!fin.eof()) {
    int start = 0;
    int end = n - 1;
    while (1) {
        int mid = (start + end) / 2;
        int diff = strcmp(word, words[mid]);
        if (diff == 0){
            found = 1;
            break;
        }
        if (diff < 0)
            end = mid - 1;
        if (diff > 0)
            start = mid + 1;
        if (start > end)
            break;
    }
   /* for (int i = 0; i < n; i++) {
        strcpy(line, words[i]);
		//Make sure both words are the same case.
        p3.get_start_time();  // <---profiling 
		convert_to_a_case(LOWER, line);
        //-------------------------------
        //moved outside of the loop
		//convert_to_a_case(LOWER, word);
        //-------------------------------
        p3.get_end_time();   // <---profiling
		//Compare words for match.
		//If match is found, the word is spelled correctly.
		if (strlen(line) > 0)
			if (strcmp(line, word) == 0) {
                //--------------------------------------
                //old code, optimized below
				//found = found + 1;
                //--------------------------------------
                //new optimized code
                //Word was found, fall through to the ending logic
                found++;
                break;
                //--------------------------------------
            }
		fin >> line;
	}*/
    p2.get_end_time();  // <---profiling
	if (found > 0)
		return 1;
	else
		return 0;
}
















