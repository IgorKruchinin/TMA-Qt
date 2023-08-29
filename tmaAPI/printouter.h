#ifndef PRINTOUTER
#define PRINTOUTER
#include <string>
#include <vector>
#include <fstream>
#include "money_data.h"

class CannotOpenCSVFileException{};

class Printout {
public:
    static void toCSV(std::vector<MoneyData*> *data, const std::vector<std::string> &dateAsString, const std::string &filename) {
        std::ofstream csvFile;
        csvFile.open(filename);
        if (csvFile.is_open()) {
            std::string str = "ID;DATE;NAME;GET/WASTE;CATEGORY;MONEY BEFORE;MONEY AFTER;SUM\n";
            int i = 0;
            for (MoneyData *item: *data) {
                str += std::to_string(item->ID) + ";" + dateAsString[i] + ";" + item->name + ";" + (item->code ? "G" : "W") + ";" + item->category + ";" + std::to_string(item->money_before) + ";" + std::to_string(item->code ? item->money_before + item->sum : item->money_before - item->sum) + ";" + std::to_string(item->sum) + "\n";
                ++i;
            }
            csvFile << str;
            csvFile.close();
        } else {
            throw CannotOpenCSVFileException();
        }
    }
};

#endif
