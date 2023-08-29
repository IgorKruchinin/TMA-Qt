#ifndef TMA_API
#define TMA_API
#include <vector>
#include <set>
#include "money_data.h"
#include "db_communicator.h"
#include "tma_config.h"

class NoEnoughMoneyException{};

class TMA {
	TMAConfig *config_;
	DBCommunicator *storage_;
	std::vector<MoneyData*> *data_;
    std::set<std::string> namesCache_;
    std::set<std::string> categoriesCache_;
public:
	TMA();
	void reread();
	/* Append data */
    void append(uint32_t ID, unsigned long long date, bool code, std::string &name, uint32_t money_before, uint32_t sum, std::string &category);
    void append(unsigned long long date, bool code, std::string &name, uint32_t money_before, uint32_t sum, std::string &category);
    void append(unsigned long long date, bool code, std::string &name, uint32_t sum, std::string &category);
    void append(uint32_t ID, unsigned long long date, bool code, std::string &name, uint32_t sum, std::string &category);
    void get(std::vector<MoneyData*> *data, std::string &category, std::string &name, unsigned long long begDate, unsigned long long endDate);
    void get(std::vector<MoneyData*> *data, std::string &category, std::string &name);
    void get(std::vector<MoneyData*> *data, bool code, std::string &category, std::string &name, unsigned long long begDate, unsigned long long endDate);
    void get(std::vector<MoneyData*> *data, bool code, std::string &category, std::string &name);
	/* Reads ALL data */
	void getAll(std::vector<MoneyData*> *data);
	/* Reads data by category */
	void getByCat(std::vector<MoneyData*> *data, std::string &category);
	/* Reads data by name */
	void getByName(std::vector<MoneyData*> *data, std::string &name);
	/* Reads data by date range */
	void getByDates(std::vector<MoneyData*> *data, uint32_t begDate, uint32_t endDate);
	/* Reads data by one date */
	void getByDate(std::vector<MoneyData*> *data, uint32_t date);
    const std::set<std::string> getNamesCached();
    const std::set<std::string> getCategoriesCached();
    uint32_t getBalance();
    /* return count of rows in database */
    int getSize();
	/* get config class for reconfiguring */
	TMAConfig *getConfig();
};

#endif

