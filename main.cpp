#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <Windows.h>
#include <conio.h>
#include <iomanip>


enum ConsoleColor { // згрупований набір цілісних констант кольорів
    Black = 0, Blue = 1, Green = 2, Cyan = 3, Red = 4, Magenta = 5, Brown = 6,
    LightGray = 7, DarkGray = 8, LightBlue = 9, LightGreen = 10, LightCyan = 11,
    LightRed = 12, LightMagenta = 13, Yellow = 14, White = 15
};

using namespace std;

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); // функція зміни кольорів тексту

#define CHARSETSIZE 256 // кількість символів таблиці АСКІ

// -------KnuthMorrisPratt--------
vector<int> KnuthMorrisPratt(const string &, const string &, vector<int> &); // функція алгоритму Кнута-Моріса-Прата

vector<int> PrefixFunction(const string &); // допоміжна префікс функція для алгоритму Кнута-Моріса-Прата

void KnuthMorrisPrattResult(); // Результат роботи алгоритму Кнута-Моріса-Прата

// ---------BoyerMoore-----------
vector<int> BoyerMooreSearch(const string &, const string &); // функція алгоритму Бойера-Мура

vector<int> BadCharacterPreComputation(const string &); // допоміжна функція евристіки стоп-символу для алгоритму Бойера-Мура

vector<int> GoodSuffixHeuristicPreComputation(const string &); // допоміжна функція еврістики гарного суфіксу для алгоритму Бойера-Мура

void BoyerMooreSearchResult(); // результат роботи алгоритму Бойера-Мура

// ----------RabinKarp------------
vector<int> RabinKarp(const string &, const string &); // функція алгоритму Рабіна Карпа

int HashFunction(const string &, const string &, int &, int &, vector<int> &); // функція розрахуку хешу для алгоритму Рабіна  Карпа

void RabinKarpResult(); // Результат роботи алгоритму Рабіна Карпа

// --------Others Function---------
vector<int> TrivialAlgorithm(const string &Text, const string &Pattern); // реалізація тривіального алгоритму пошуку

string ReadingFromFile(); // допоміжна функція для читання файлу

void OutputTextFromFile(); // допоміжна функція виводу тексту з файлу

void ChangeTextInFile(); // змінення тексту який знаходиться в файлі

string EnterPattern(); // функція введення шаблону

void OutPutResult(vector<int> &); // функція виводу результату роботи алгоритмів

bool SpeedTest(); // функція для порівняння результатів роботи алгоритмів та швидкості їх виконання

void Print(const string &, const string &, vector<int> &); // допоміжна функція виводу тексту з зміненням підстроки іншим кольором

void SetTextColorFirst(ConsoleColor text, ConsoleColor background = Black) { // допоміжна функція для змінення кольору тексту
    HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsoleHandle, (WORD) ((background << 4) | text));
}


int main() {

    srand(time(nullptr));

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    string Menu[7] = {"Enter new Text", "View text from a file", "Pattern Search with Knuth-Morris-Pratt algorithm",
                      "Pattern Search with Boyer-Moore algorithm", "Pattern Search with Rabin-Karp algorithm",
                      "Speed Test of Algorithms", "Exit\n"};
    int pointer = 0;

    char ch;
    while (true) {
        system("cls");
        SetTextColorFirst(DarkGray);
        cout << "\t\t\t\tWelcome to the program for finding a substring in a string\n";
        cout << "\t\t\t\tUse the up and down arrows to move through the program\n";
        SetTextColorFirst(White);
        cout << "\n\n\n\n\n";
        for (int k = 0; k < 7; ++k) {
            if (k == pointer) {
                SetConsoleTextAttribute(hStdOut, 11);
                cout << "\t\t\t\t\t" << Menu[k] << endl;
            } else {
                SetConsoleTextAttribute(hStdOut, 15);
                cout << "\t\t\t\t\t" << Menu[k] << endl;
            }
        }
        while (true) {
            if (_kbhit()) {
                ch = _getch();
                if (ch == 72) {
                    pointer -= 1;
                    if (pointer == -1) {
                        pointer = 8;
                    }
                    break;
                }
                if (ch == 80) {
                    pointer += 1;
                    if (pointer == 9) {
                        pointer = 0;
                    }
                    break;
                }
                if (ch == 13) {
                    switch (pointer) {
                        case 0:
                            ChangeTextInFile();
                            break;
                        case 1:
                            OutputTextFromFile();
                            break;
                        case 2:
                            KnuthMorrisPrattResult();
                            break;
                        case 3:
                            BoyerMooreSearchResult();
                            break;
                        case 4:
                            RabinKarpResult();
                            break;
                        case 5:
                            SpeedTest();
                            break;
                        case 6:
                            return 0;
                    }
                    break;
                }
            }

        }
        Sleep(150);
    }
}

void Print(const string &Pattern, const string &Text, vector<int> &Result) {
    if (Result.empty()) return;
    cout << "Your Text: ";
    int k = 0, i = 0;
    while (i != Text.size()) {
        SetTextColorFirst(White);
        if (i == Result[k]) {
            SetTextColorFirst(LightBlue);
            for (char j: Pattern) {
                cout << j;
            }
            if (k != Result.size() - 1)
                k++;
            i += Pattern.size();

        } else {
            cout << Text[i];
            i++;
        }


    }
    SetTextColorFirst(White);
}

void OutPutResult(vector<int> &Result) {
    if (Result.empty())
        cout << "Pattern is not found in given text!!\n";
    else {
        cout << "Pattern is found at indices: ";
        for (int i: Result)
            cout << i << " ";
    }
    cout << '\n';
}

string ReadingFromFile() {
    ifstream DATA;
    DATA.open(R"(C:\Users\sabir\CLionProjects\term_paper\DATA)");
    stringstream Temp;
    Temp << DATA.rdbuf();
    DATA.close();
    return Temp.str();

}

void OutputTextFromFile() {
    string Text = ReadingFromFile();
    if (Text.length() == 0)
        cout << "File is empty!\n";
    else
        cout << "Text: " << Text << '\n';

    system("pause");
}

void ChangeTextInFile() {
    ofstream DATA;
    string Text;
    DATA.open(R"(C:\Users\sabir\CLionProjects\term_paper\DATA)");
    cout << "Enter Text: ";
    getline(cin, Text);
    DATA << Text;
    cout << "Your text has been successfully entered into the file" << '\n';
    DATA.close();
    cout << '\n';

    system("pause");
}

string EnterPattern() {
    string Pattern;
    cout << "Enter Pattern:";
    getline(cin, Pattern);
    return Pattern;
}

vector<int> TrivialAlgorithm(const string &Text, const string &Pattern) {
    vector<int> Result;
    bool Ex;
    for (int i = 0; i < Text.size(); ++i) {
        for (int j = 0; j < Pattern.size(); ++j) {
            if (Pattern[j] == Text[j + i]) {
                Ex = true;
                continue;
            } else {
                Ex = false;
                break;
            }
        }
        if (Ex) Result.push_back(i);
    }

    return Result;
}

vector<int> PrefixFunction(const string &Pattern) {
    vector<int> pi(Pattern.size(), 0);
    for (int i = 1; i < Pattern.size(); i++) {
        int j = pi[i - 1];

        while (j > 0 && Pattern[i] != Pattern[j]) {
            j = pi[j - 1];
        }

        if (Pattern[i] == Pattern[j]) {
            pi[i] = j + 1;
        } else {
            pi[i] = j;
        }
    }

    return pi;
}

vector<int> KnuthMorrisPratt(const string &Text, const string &Pattern, vector<int> &pi) {
    vector<int> KnutMorrisPrattResult;
    int l = 0, k = 0;

    while (k != Text.size()) {
        if (Text[k] == Pattern[l]) {
            k++;
            l++;
        } else if (l == 0) k++;
        else l = pi[l - 1];

        if (l == Pattern.size()) {
            KnutMorrisPrattResult.push_back((int) (k - Pattern.size()));
            k = k - l + 1;
            l = 0;

        }

    }
    return KnutMorrisPrattResult;
}

void KnuthMorrisPrattResult() {

    string Text = ReadingFromFile(), Pattern = EnterPattern();
    vector<int> pi = PrefixFunction(Pattern);
    cout << '\n';
    clock_t KMPStart = clock();
    vector<int> Result = KnuthMorrisPratt(Text, Pattern, pi);
    clock_t KMPEnd = clock();

    float KMPTime = ((float) (KMPEnd - KMPStart)) / CLOCKS_PER_SEC;

    cout << "Time of algorithm Knuth-Morris-Pratt = " << KMPTime << '\n';
    OutPutResult(Result);
    cout << '\n';
    Print(Pattern, Text, Result);
    cout << '\n';

    system("pause");
}


int HashFunction(const string &Pattern, const string &Text, int &i, int &LastHash, vector<int> &PowX) {
    int p = 3571, x = 5;
    int hash = 0;

    if (i == 0) {
        for (int j = 0; j < Pattern.size(); ++j)
            hash += (int) (Text[j] * PowX[j]);
    } else {
        int Temp = (int) (LastHash - (Text[i - 1] * PowX[0])) % p;
        while (Temp <= 0) Temp += p;
        hash = Temp * x + (int) Text[i + Pattern.size() - 1];
    }
    return hash % p;
}

vector<int> RabinKarp(const string &Text, const string &Pattern) {
    int x = 5;
    vector<int> PowX(Pattern.size());
    for (int i = 0; i < Pattern.size(); ++i) {
        PowX[i] = pow(x, Pattern.size() - i - 1);
    }
    bool Ex;
    int Temp = 0;
    int HashOfText;
    vector<int> RabinKarpResult;
    int HashOfPattern = HashFunction(Pattern, Pattern, Temp, HashOfText, PowX);
    for (int i = 0; i < Text.size() - Pattern.size() + 1; ++i) {
        HashOfText = HashFunction(Pattern, Text, i, HashOfText, PowX);
        if (HashOfText == HashOfPattern) {
            Ex = true;
            for (int j = 0; j < Pattern.size(); ++j) {
                if (Pattern[j] != Text[j + i]) {
                    Ex = false;
                    break;
                }
            }
            if (Ex) RabinKarpResult.push_back(i);
        }

    }
    return RabinKarpResult;
}

void RabinKarpResult() {
    string Text = ReadingFromFile(), Pattern = EnterPattern();

    clock_t RKStart = clock();

    vector<int> Result = RabinKarp(Text, Pattern);

    clock_t RKEnd = clock();
    float RKTime = ((float) (RKEnd - RKStart)) / CLOCKS_PER_SEC;
    cout << "Time of algorithm Rabin-Karp = " << RKTime << '\n';

    OutPutResult(Result);
    cout << '\n';
    Print(Pattern, Text, Result);
    cout << '\n';

    system("pause");
}


vector<int> BadCharacterPreComputation(const string &Pattern) {
    vector<int> badCharacterArray(CHARSETSIZE, -1);
    for (int i = 0; i < Pattern.size(); i++) {
        // Мы сохраняем последнее вхождение каждой буквы
        badCharacterArray[Pattern[i]] = i;
    }
    return badCharacterArray;
}


///Предварительное вычисление сдвигов для эвристики хорошего суффикса
vector<int> GoodSuffixHeuristicPreComputation(const string &Pattern) {
    int m = (int) Pattern.size(), i = m, j = m + 1;
    // Инициализировать все сдвиги нулем
    vector<int> borderPos(m + 1), shifts(m + 1, 0);

    borderPos[i] = j;
    // Приведенный ниже цикл используется для поиска границы для каждого индекса
    while (i > 0) {
        /*
                 если символ в позиции i-1 не равен символу в j-1,
                 то продолжить поиск границы справа от шаблона.*/
        while (j <= m && Pattern[i - 1] != Pattern[j - 1]) {
            // Предыдущий символ совпадающей строки отличается от несоответствующего символа.
            if (shifts[j] == 0)
                shifts[j] = j - i;
            // обновляем позицию следующей границы
            j = borderPos[j];
        }
        /*Поскольку шаблон[i-1] равен шаблону[j-1], положение границы
         для префикса, начинающегося с i-1, это j-1*/
        i--;
        j--;
        borderPos[i] = j;
    }
    /* до сих пор мы рассчитывали сдвиг в соответствии со случаем 1, т.е. при полном совпадении
     подстрока с другим предшествующим символом существует в шаблоне*/

    /* теперь мы заполним те сдвиги, которые сгенерированы из-за случая 2, т.е. какой-то суффикс
    совпавшей строки находится как префикс шаблона*/
    j = borderPos[0];
    for (i = 0; i <= m; i++) {
        if (shifts[i] == 0)
            shifts[i] = j;
        if (i == j)
            j = borderPos[j];
    }
    return shifts;
}


vector<int> BoyerMooreSearch(const string &Text, const string &Pattern) {
    int i = 0, j, m = (int) Pattern.size();
    int badShift, goodShift;
    //preprocessing
    vector<int> badCharacterArray = BadCharacterPreComputation(Pattern);
    vector<int> shifts = GoodSuffixHeuristicPreComputation(Pattern);
    vector<int> BoyerMooreResult;
    while (i <= Text.size() - m) {
        j = m - 1;
        while (j >= 0 && Pattern[j] == Text[i + j])
            j--;

        if (j < 0) {
            BoyerMooreResult.push_back(i);
            badShift = (i + m < Text.size()) ? m - badCharacterArray[Text[i + m]] : 1;
            goodShift = shifts[0];
            i += max(badShift, goodShift); //Мы берем максимальное смещение из обоих эвристик
        } else {
            badShift = max(1, j - badCharacterArray[Text[i + j]]);
            goodShift = shifts[j + 1];
            i += max(badShift, goodShift); //Мы берем максимальное смещение из обоих эвристик
        }

    }
    return BoyerMooreResult;
}

void BoyerMooreSearchResult() {
    string Text = ReadingFromFile(), Pattern = EnterPattern();

    clock_t BMStart = clock();
    vector<int> Result = BoyerMooreSearch(Text, Pattern);
    clock_t BMEnd = clock();
    float BMTime = ((float) (BMEnd - BMStart)) / CLOCKS_PER_SEC;
    cout << "Time of algorithm Boyer-Moore = " << BMTime << '\n';

    OutPutResult(Result);
    cout << '\n';
    Print(Pattern, Text, Result);
    cout << '\n';

    system("pause");
}


bool SpeedTest() {
    int iters;
    cout << '\n';
    cout << "In this test, we will check and compare the speed of the algorithms: Rabin-Karp,\n"
            "Knuth-Maurice-Prat, and Boyer-Moore, relative to each other.\n"
            "To do this, enter the number of patterns (better not less than 100)\n"
            "that will be randomly generated and subsequently searched for in\n"
            "a randomly generated text of 2000 characters:";
    cin >> iters;
    while (iters <= 0) {
        cout << "\nIters must be more than 0, enter again: ";
        cin >> iters;
    }
    cout << '\n';
    vector<string> Text(1);
    vector<string> Patterns(iters);
    bool FinalExam = false;
    for (int i = 0; i < iters; ++i) {
        int PatternSize = 3 + rand() % 4;

        for (int j = 0; j < PatternSize; ++j)
            Patterns[i].push_back((char) (65 + rand() % 7));


    }
    for (int i = 0; i < 2000; ++i)
        Text[0].push_back((char) (65 + rand() % 7));

    vector<vector<int>> KnuthMorrisPrattResults(iters);
    vector<vector<int>> BoyerMooreSearchResults(iters);
    vector<vector<int>> RabinKarpResults(iters);
    vector<vector<int>> TrivialResults(iters);


    clock_t TrivialStart = clock();
    for (int i = 0; i < iters; ++i) {
        TrivialResults[i] = TrivialAlgorithm(Text[0], Patterns[i]);
    }
    clock_t TrivialEnd = clock();
    float TrivialTime = ((float) (TrivialEnd - TrivialStart)) / CLOCKS_PER_SEC;

    clock_t KMPStart = clock();
    for (int i = 0; i < iters; ++i) {
        vector<int> pi = PrefixFunction(Patterns[i]);
        KnuthMorrisPrattResults[i] = KnuthMorrisPratt(Text[0], Patterns[i], pi);

    }
    clock_t KMPEnd = clock();
    float KMPTime = ((float) (KMPEnd - KMPStart)) / CLOCKS_PER_SEC;

    clock_t BMStart = clock();
    for (int i = 0; i < iters; ++i) {
        BoyerMooreSearchResults[i] = BoyerMooreSearch(Text[0], Patterns[i]);
    }
    clock_t BMEnd = clock();
    float BMTime = ((float) (BMEnd - BMStart)) / CLOCKS_PER_SEC;

    clock_t RKStart = clock();
    for (int i = 0; i < iters; ++i) {
        RabinKarpResults[i] = RabinKarp(Text[0], Patterns[i]);

    }
    clock_t RKEnd = clock();
    float RKTime = ((float) (RKEnd - RKStart)) / CLOCKS_PER_SEC;
    int count = 0;
    for (int i = 0; i < iters; ++i) {
        if (!RabinKarpResults[i].empty() && !BoyerMooreSearchResults[i].empty() && !RabinKarpResults[i].empty())
            count++;
        if (RabinKarpResults[i] == BoyerMooreSearchResults[i] && RabinKarpResults[i] == KnuthMorrisPrattResults[i] &&
            KnuthMorrisPrattResults[i] == TrivialResults[i])
            FinalExam = true;
        else {
            FinalExam = false;
            break;
        }
    }
    cout << "From " << iters << " patterns in the text was found: " << count << '\n';
    cout << "In others arrays entries were not found, their number: " << iters - count << '\n' << '\n';

    cout << "Time of algorithm Trivial = " << TrivialTime << '\n';
    cout << "Time of algorithm Knuth-Morris-Pratt = " << KMPTime << '\n';
    cout << "Time of algorithm Boyer-Moore = " << BMTime << '\n';
    cout << "Time of algorithm Rabin-Karp = " << RKTime << '\n' << '\n';

    cout << "Result: ";
    if (FinalExam) {
        cout << "all index arrays returned matched, and all algorithms passed the test!!! :)" << '\n';
        system("pause");
        return FinalExam;
    } else cout << "some of the index arrays did not match :(((" << '\n';
    system("pause");
    return FinalExam;


}
