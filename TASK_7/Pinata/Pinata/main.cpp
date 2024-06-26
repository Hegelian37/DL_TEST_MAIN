#include "header.h"

int main()
{
    int n;
    int num;
    int max_candies = 0;
    vector<int> pinatas;
    string inputLine;

    cout << "Input array of numbers: ";
    getline(cin, inputLine);

    inputLine = preprocessInput(inputLine);

    istringstream stream(inputLine);
    string token;
    while (stream >> token)
    {
        int num = stoi(token);
        if (num < 0)
        {
            cerr << "Error: Negative input: " << num << ". Please enter only non-negative numbers.\n";
            return 1;
        }
        pinatas.push_back(num);
    }

    if (pinatas.empty())
    {
        cerr << "No numbers entered.\n";
        return 1;
    }
    n = pinatas.size();

    for (int i = 0; i < n; i++)
    {
        int prev = (i == 0) ? 1 : pinatas[i - 1];
        int curr = pinatas[i];
        int next = (i == n - 1) ? 1 : pinatas[i + 1];

        int pinata = prev * curr * next;
        max_candies += pinata;
    }

    cout << "Max amount of candies is " << max_candies << ".\n";
    system("pause");
    return 0;
}
