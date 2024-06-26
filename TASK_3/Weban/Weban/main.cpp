#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>

using namespace std;

struct Webvisit
{
    string user_id;
    string page_category;
    Webvisit(const string& u, const string& p) : user_id(u), page_category(p) {}
};

vector<Webvisit> parseCSV(const string& filename)
{
    vector<Webvisit> webvisits;
    ifstream file(filename);
    string line;

    if (!file.is_open())
    {
        throw runtime_error("Could not open file: <" + filename + ">");
    }

    getline(file, line);
    while (getline(file, line))
    {
        stringstream ss(line);
        string user_id, page_category;

        if (getline(ss, user_id, ';') && getline(ss, page_category, ';'))
        {
            webvisits.push_back(Webvisit(user_id, page_category));
        }
        else
        {
            ss.clear();
            ss.str(line);

            if (getline(ss, user_id, ',') && getline(ss, page_category, ','))
            {
                webvisits.push_back(Webvisit(user_id, page_category));
            }
            else
            {
                throw runtime_error("Error parsing line: " + line);
            }
        }
    }
    return webvisits;
}

int main()
{
    try
    {
        vector<Webvisit> day1 = parseCSV("dayone.csv");
        vector<Webvisit> day2 = parseCSV("daytwo.csv");

        unordered_map<string, unordered_set<string>> day1_visits;
        unordered_map<string, unordered_set<string>> day2_visits;

        for (const auto& webvisits : day1)
        {
            day1_visits[webvisits.user_id].insert(webvisits.page_category);
        }
        for (const auto& webvisits : day2)
        {
            day2_visits[webvisits.user_id].insert(webvisits.page_category);
        }

        unordered_set<string> result_users;
        for (const auto& entry : day2_visits)
        {
            const string& user_id = entry.first;
            const auto& day2_categories = entry.second;

            if (day1_visits.find(user_id) != day1_visits.end())
            {
                const auto& day1_categories = day1_visits[user_id];
                for (const auto& page_category : day2_categories)
                {
                    if (day1_categories.find(page_category) == day1_categories.end())
                    {
                        result_users.insert(user_id);
                        break;
                    }
                }
            }
        }

        cout << "User IDs who fit into the requirements:";
        for (const auto& user_id : result_users)
        {
            cout << " " << user_id << ";";
        }
        cout << "\n";
    }
    catch (const exception& ex)
    {
        cerr << "Error: " << ex.what() << "\n";
    }
    system("pause");
    return 0;
}
