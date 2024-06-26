#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <stdexcept>

using namespace std;

struct Transaction
{
    string tx_id;
    int size;
    int fee;
    double fee_per_byte;

    Transaction(const string& id, int s, int f) : tx_id(id), size(s), fee(f), fee_per_byte(static_cast<double>(f) / s) {}
};

vector<Transaction> parseCSV(const string& filename);
vector<Transaction> findOptimalTransactions(const vector<Transaction>& transactions, int maxBlockSize);

#endif
