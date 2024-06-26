#include "header.h"

void getinput_fix()
{
    clearInputBuffer();

    bool f_Neg = 0;
    string select;
    string input;

    cout << "Input the total number of laptops you are planning to buy: ";
    getline(cin, input);
    input = preprocessAndSplitInput(input)[0];

    if (!isValidNumber(input))
    {
        cout << "Invalid input! Please, enter a valid number.\n";
        getinput_fix();
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
