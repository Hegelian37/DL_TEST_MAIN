#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct Order
{
    int64_t user_id;
    int64_t amount;
    int64_t price;
    bool side; //sell 0 buy 1

    Order(int64_t uid, int64_t amt, int64_t prc, bool sd) : user_id(uid), amount(amt), price(prc), side(sd) {}
};

struct BalanceChange
{
    int64_t user_id;
    int64_t value;
    string currency;
};

class OrderBook {
private:
    vector<Order> sellOrders;
    vector<Order> buyOrders;
    unordered_map<int64_t, int64_t> userBalancesUAH;
    unordered_map<int64_t, int64_t> userBalancesUSD;

    static bool compareSellOrders(const Order& a, const Order& b)
    {
        return a.price < b.price;
    }

    static bool compareBuyOrders(const Order& a, const Order& b)
    {
        return a.price > b.price;
    }

public:
    void addOrder(const Order& order)
    {
        if (order.side)
        {
            buyOrders.push_back(order);
            sort(buyOrders.begin(), buyOrders.end(), compareBuyOrders);
            saveOrderToFile(order, "Orderbook\orders_buy.csv");
        }
        else
        {
            sellOrders.push_back(order);
            sort(sellOrders.begin(), sellOrders.end(), compareSellOrders);
            saveOrderToFile(order, "Orderbook\orders_sell.csv");
        }
        matchOrders();
    }

    void matchOrders() 
    {
        while (!buyOrders.empty() && !sellOrders.empty())
        {
            Order& buyOrder = buyOrders.front();
            Order& sellOrder = sellOrders.front();

            if (buyOrder.price >= sellOrder.price)
            {
                int64_t tradeAmount = min(buyOrder.amount, sellOrder.amount);
                int64_t tradeValue = tradeAmount * sellOrder.price;

                buyOrder.amount -= tradeAmount;
                sellOrder.amount -= tradeAmount;

                updateBalances(buyOrder.user_id, sellOrder.user_id, tradeAmount, tradeValue);

                if (buyOrder.amount == 0)
                {
                    buyOrders.erase(buyOrders.begin());
                }
                if (sellOrder.amount == 0)
                {
                    sellOrders.erase(sellOrders.begin());
                }
            }
            else
            {
                break;
            }
        }
    }

    void updateBalances(int64_t buyerId, int64_t sellerId, int64_t amount, int64_t value)
    {
        userBalancesUSD[buyerId] -= value;
        userBalancesUAH[buyerId] += amount;
        userBalancesUSD[sellerId] += value;
        userBalancesUAH[sellerId] -= amount;

        printBalanceChange(buyerId, sellerId, amount, value);
    }

    void printBalanceChange(int64_t buyerId, int64_t sellerId, int64_t amount, int64_t value)
    {
        cout << "BalanceChange{" << buyerId << ", -" << value << ", USD}" << "\n";
        cout << "BalanceChange{" << buyerId << ", +" << amount << ", UAH}" << "\n";
        cout << "BalanceChange{" << sellerId << ", +" << value << ", USD}" << "\n";
        cout << "BalanceChange{" << sellerId << ", -" << amount << ", UAH}" << "\n";
    }

    void saveOrderToFile(const Order& order, const string& filename)
    {
        ofstream file(filename, ios_base::app);
        file << order.user_id << ";" << order.amount << ";" << order.price << ";" << order.side << "\n";
        file.close();
    }

    void processOrdersFromCSV(const string& buyFile, const string& sellFile)
    {
        processFile(buyFile, true);
        processFile(sellFile, false);
    }

    void processFile(const string& filename, bool isBuyFile)
    {
        ifstream file(filename);
        string line;

        while (getline(file, line))
        {
            istringstream ss(line);
            int64_t user_id, amount, price;
            bool side;
            char delimiter;

            ss >> user_id >> delimiter >> amount >> delimiter >> price >> delimiter >> side;

            if (side != isBuyFile)
            {
                cerr << "Error: Mismatched order type in file " << filename << "\n";
                continue;
            }

            Order order(user_id, amount, price, side);
            addOrder(order);
        }
    }
};

int main()
{
    OrderBook orderBook;

    orderBook.processOrdersFromCSV("Orderbook\orders_buy.csv", "Orderbook\orders_sell.csv");

    cout << "Enter orders in format: user_id,amount,price,side (0 for sell, 1 for buy)." << "\n";
    cout << "Enter 'q' to stop entering orders." << "\n";

    string input;
    while (true)
    {
        cout << "Enter order: ";
        getline(cin, input);

        if (input == "q")
        {
            break;
        }

        istringstream ss(input);
        int64_t user_id, amount, price;
        bool side;
        char separator;

        ss >> user_id >> separator >> amount >> separator >> price >> separator >> side;

        if (ss.fail() || separator != ';' || (side != 0 && side != 1))
        {
            cerr << "Invalid input format. Please try again." << "\n";
            continue;
        }

        Order order(user_id, amount, price, side);
        orderBook.addOrder(order);
    }
    system("pause");
    return 0;
}
