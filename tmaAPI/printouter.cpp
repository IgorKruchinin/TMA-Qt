#ifndef PRINTOUTER
#define PRINTOUTER
#include <string>
#include <vector>
#include "money_data.h"

class Printout {
    static int digitCount(int val) {
        int v = val;
        int digits = 0;
        while (v) {
            digits += 1;
            v /= 10;
        }
        return digits;
    }
    static std::vector<int> maxLen(std::vector<MoneyData*> *data) {
        std::vector<int> len(2);
        for (MoneyData *item: *data) {
            if (item->name.size() > len[0]) {
                len[0] = item->name.size();
            }
            if (item->category.size() > len[1]) {
                len[1] = item->category.size();
            }
        }
        return len;
    }
    static std::string& formRow(std::vector<int> maxLens, uint32_t ID, long long unsigned date, std::string &category, bool code, uint32_t moneyBefore, uint32_t sum) {
        std::string res = "ID";
        for (int i = 0; i < 4; ++i)
            res += " " ;
        res += "DATE";
        for (int i = 0; i < 7; ++i)
            res += " " ;
        res += "NAME";
        for (int i = 0; i < maxLens[0] + 1 - 4; ++i)
            res += " " ;
        res += "CATEGORY";
        for (int i = 0; i < maxLens[1] + 1 - 8; ++i)
            res += " " ;
        res += "Get/Waste";
        res += " ";
        res += "Money before";
        res += " ";
        res += "Money After";
        res += " ";
        res += "Sum";
        for (int i = 0; i < 8; ++i)
            res += " " ;
    }
    static std::string& preparePrintout(std::vector<MoneyData*> *data) {
        auto mxln = maxLen(data);
        std::string res = "ID";
        for (int i = 0; i < 4; ++i)
            res += " " ;
        res += "DATE";
        for (int i = 0; i < 7; ++i)
            res += " " ;
        res += "NAME";
        for (int i = 0; i < mxln[0] + 1 - 4; ++i)
            res += " " ;
        res += "CATEGORY";
        for (int i = 0; i < mxln[1] + 1 - 8; ++i)
            res += " " ;
        res += "Get/Waste";
        res += " ";
        res += "Money before";
        res += " ";
        res += "Money After";
        res += " ";
        res += "Sum";
        for (int i = 0; i < 8; ++i)
            res += " " ;
    }
};

#endif
