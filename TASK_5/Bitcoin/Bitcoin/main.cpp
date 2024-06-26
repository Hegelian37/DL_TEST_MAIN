#include "header.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <transactions_file.csv>\n";
        return 1;
    }

    string filename = argv[1];

    try
    {
        auto start_time = chrono::high_resolution_clock::now();
        vector<Transaction> transactions = parseCSV(filename);
        int maxBlockSize = 1000000;

        vector<Transaction> block = findOptimalTransactions(transactions, maxBlockSize);
        auto end_time = chrono::high_resolution_clock::now();

        int totalSize = 0;
        int totalFee = 0;
        for (const auto& tx : block)
        {
            totalSize += tx.size;
            totalFee += tx.fee;
        }

        cout << "\nConstructed block:\n";
        for (const auto& tx : block)
        {
            cout << tx.tx_id << " (Size: " << tx.size << ", Fee: " << tx.fee << ")\n";
        }
        cout << "\nAmount of transactions in the block: " << block.size() << "\n";
        cout << "Block size: " << totalSize << " bytes\n";
        cout << "Total extracted value: " << totalFee << " satoshis\n";
        chrono::duration<double> construction_time = end_time - start_time;
        cout << "Construction time: " << construction_time.count() << " seconds\n";
        cout << "Max memory dedicated to storing intermediate pre-calculations: " << (sizeof(Transaction) * transactions.size() + sizeof(Transaction) * block.size()) << " bytes\n";
    }
    catch (const exception& e)
    {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    system("pause");
    return 0;
}
