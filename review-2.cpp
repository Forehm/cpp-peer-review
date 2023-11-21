#include <algorithm>
#include <execution>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
public:
    Domain(const string& str)
        : domain_(str) {
    }

    bool operator==(const Domain& rhs) const {
        return domain_ == rhs.domain_;
    }

    bool IsSubDomain(const Domain& rhs) const {
        if (rhs.domain_.size() <= domain_.size()) {
            return equal(rhs.domain_.begin(), rhs.domain_.end(), domain_.begin());
        }
        return false;
    }

    string GetValue() const {
        return domain_;
    }

private:
    string domain_;
};

class DomainChecker {
public:
    template <typename It>
    DomainChecker(It begin, It end)
        : forbidden_(begin, end) {
        SortUnique();
    }

    bool IsForbidden(const Domain& domain) {
        if (forbidden_.empty()) {
            return false;
        }
        auto it = upper_bound(forbidden_.begin(), forbidden_.end(), domain, [](const Domain& value, const Domain& rhs) {
            return value.GetValue() < rhs.GetValue();
            });
        if (it != forbidden_.begin()) {
            return domain.IsSubDomain(forbidden_[it - forbidden_.begin() - 1]);
        }
        else {
            return domain.IsSubDomain(forbidden_[it - forbidden_.begin()]);
        }
        return false;
    }
private:
    void SortUnique() {
        sort(forbidden_.begin(), forbidden_.end(), [](const Domain& lhs, const Domain& rhs) {
            return lhs.GetValue() < rhs.GetValue();
            });
        auto last = unique(forbidden_.begin(), forbidden_.end(), [](const Domain& lhs, const Domain& rhs) {
            return rhs.IsSubDomain(lhs);
            });
        forbidden_.erase(last, forbidden_.end());
    }

    vector<Domain> forbidden_;
};

template <typename Number>
vector<Domain> ReadDomains(istream& input, Number number) {
    vector<Domain> result;
    for (Number i = 0; i < number; ++i) {
        string line;
        getline(input, line);
        line.insert(0, 1, '.');
        reverse(line.begin(), line.end());
        result.emplace_back(move(line));
    }
    return result;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
