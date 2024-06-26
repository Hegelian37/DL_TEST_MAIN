#include "header.h"

vector<Transaction> parseCSV(const string& filename)
{
    vector<Transaction> transactions;
    ifstream file(filename);
    string line;

    if (!file.is_open())
    {
        throw runtime_error("Could not open file: " + filename);
    }

    getline(file, line);

    while (getline(file, line))
    {
        stringstream ss(line);
        string tx_id;
        int size, fee;

        if (getline(ss, tx_id, ';') && ss >> size && ss.ignore() && ss >> fee)
        {
            transactions.push_back(Transaction(tx_id, size, fee));
        }
        else
        {
            ss.clear();
            ss.str(line);

            if (getline(ss, tx_id, ',') && ss >> size && ss.ignore() && ss >> fee)
            {
                transactions.push_back(Transaction(tx_id, size, fee));
            }
            else
            {
                throw runtime_error("Error parsing line: " + line);
            }
        }
    }

    return transactions;
}

vector<Transaction> findOptimalTransactions(const vector<Transaction>& transactions, int maxBlockSize)
{
    vector<Transaction> selectedTransactions;
    int currentBlockSize = 0;
    auto sortedTransactions = transactions;
    sort(sortedTransactions.begin(), sortedTransactions.end(), [](const Transaction& a, const Transaction& b)
    {
        return a.fee_per_byte > b.fee_per_byte;
    });

    for (const auto& tx : sortedTransactions)
    {
        if (currentBlockSize + tx.size <= maxBlockSize)
        {
            selectedTransactions.push_back(tx);
            currentBlockSize += tx.size;
        }
    }
    return selectedTransactions;
}
