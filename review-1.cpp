#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>

using namespace std;

class EBook {
public:
    EBook() :
        user_page_counts_(MAX_USER_COUNT_ + 1, 0),
        users_number_(0) {}


    void Read(int user_id, int page_count) {
        int prev_page_count = user_page_counts_[user_id];
        if (!prev_page_count)
        {
            users_number_++;
        }
        else
        {
            auto it = pages_to_users_.find(prev_page_count);
            if (it->second == 1)
            {
                pages_to_users_.erase(it);
            }
            else
            {
                it->second--;
            }
        }
        user_page_counts_[user_id] = page_count;
        pages_to_users_[page_count]++;
    }

    double Cheer(int user_id) const {
        int page_count = user_page_counts_[user_id];
        if (!page_count)
        {
            return 0;
        }
        if (users_number_ == 1)
        {
            return 1;
        }
        int num_users_with_less_pages = 0;
        for (auto& p : pages_to_users_)
        {
            if (p.first >= page_count)
            {
                break;
            }
            num_users_with_less_pages += p.second;
        }

        return (num_users_with_less_pages) * 1.0 / (users_number_ - 1);
    }

private:

    static const int MAX_USER_COUNT_ = 100'000;

    vector<int> user_page_counts_;
    map<int, int> pages_to_users_;
    unsigned int users_number_;
};


int main() {



    EBook book;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id)
    {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ")
        {
            int page_count;
            cin >> page_count;
            book.Read(user_id, page_count);
        }
        else if (query_type == "CHEER")
        {
            cout << setprecision(6) << book.Cheer(user_id) << "\n";
        }
    }

    return 0;
}