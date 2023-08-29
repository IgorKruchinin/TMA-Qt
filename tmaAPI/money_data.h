#ifndef MONEY_DATA
#define MONEY_DATA
#include <string>

struct MoneyData {
    uint32_t ID;
    unsigned long long date; // date in unixtime
    bool code;
    std::string name;
    uint32_t money_before;
    uint32_t sum;
    std::string category;
};

#endif
