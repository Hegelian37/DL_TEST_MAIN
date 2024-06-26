#include "header.h"

int N;
double C;

void clearInputBuffer()
{
    cin.clear();
    while (cin.peek() != '\n' && cin.peek() != EOF)
    {
        cin.ignore();
    }
    cin.ignore();
}

bool isValidNumber(const string& str)
{
    if (str.empty()) return false;
    int decimalCount = 0;
    for (char ch : str)
    {
        if (!isdigit(ch) && ch != '-' && ch != '.')
        {
            return false;
        }
        if (ch == '.')
        {
            decimalCount++;
            if (decimalCount > 1)
            {
                return false;
            }
        }
    }
    return true;
}

vector<string> preprocessAndSplitInput(const string& input)
{
    vector<string> tokens;
    string token;
    string cleanedInput;

    for (char ch : input)
    {
        if (isdigit(ch) || ch == '-' || ch == '.')
        {
            cleanedInput += ch;
        }
        else
        {
            cleanedInput += ' ';
        }
    }

    istringstream tokenStream(cleanedInput);
    while (tokenStream >> token)
    {
        tokens.push_back(token);
    }

    return tokens;
}

void getinput()
{
    bool f_Neg = 0;
    string select;
    string input;

    cout << "Please, input the Initial Capital: ";
    getline(cin, input);
    input = preprocessAndSplitInput(input)[0];

    if (!isValidNumber(input))
    {
        cout << "Invalid input! Please, enter a valid number.\n";
        getinput();
        return;
    }
    C = stod(input);

    if (C <= 0)
    {
        cout << "The number cannot be negative or equal to 0! Please, enter the correct values.\n";
        getinput();
        return;
    }

    cout << "The general number of sufficient laptops you can find on the Net is " << K << ".\nNow input the total number of laptops you are planning to buy: ";
    getline(cin, input);
    input = preprocessAndSplitInput(input)[0];

    if (!isValidNumber(input))
    {
        cout << "Invalid input! Please, enter a valid number.\n";
        getinput();
        return;
    }
    N = stoi(input);

    if (N > K)
    {
        cout << "\nThere are not this many laptops! Please, enter the correct values.\n";
        getinput_fix();
        return;
    }

    if (N <= 0)
    {
        cout << "\nPlease, enter the correct values.\n";
        getinput_fix();
        return;
    }

    gains.resize(N);
    price.resize(N);

    cout << "\nInput the profit you are planning to gain from each laptop: ";
    getline(cin, input);
    vector<string> gainInputs = preprocessAndSplitInput(input);

    if (gainInputs.size() != N)
    {
        cout << "Number of profits entered does not match the number of laptops.\n";
        getinput_fix();
        return;
    }

    for (int i = 0; i < N; i++)
    {
        if (!isValidNumber(gainInputs[i]))
        {
            cout << "Invalid input! Please, enter a valid number.\n";
            getinput_fix();
            return;
        }
        gains[i] = stod(gainInputs[i]);
        if (gains[i] <= 0) f_Neg = 1;
    }

    if (f_Neg == 1)
    {
        cout << "\nThe gain from one or more laptops is either negative or non-existent.\nAre you sure? Y/N: ";
        cin >> select;
        clearInputBuffer();
        if (select != "Y")
        {
            getinput_fix();
            return;
        }
    }

    cout << "Input the prices of the laptops you are buying: ";
    getline(cin, input);
    vector<string> priceInputs = preprocessAndSplitInput(input);

    if (priceInputs.size() != N)
    {
        cout << "Number of prices entered does not match the number of laptops.\n";
        getinput_fix();
        return;
    }

    for (int i = 0; i < N; i++)
    {
        if (!isValidNumber(priceInputs[i]))
        {
            cout << "Invalid input! Please, enter a valid number.\n";
            getinput_fix();
            return;
        }
        price[i] = stod(priceInputs[i]);
    }

    pricecheck();
}

void getoutput()
{
    cout << "\nCapital at the end of the summer: " << calcs() + C << ".\n";
}
