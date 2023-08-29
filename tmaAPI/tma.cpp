#ifndef TMA_API
#define TMA_API
#include <vector>
#include <set>
#include "money_data.h"
#include "db_communicator.cpp"
#include "tma_config.h"

class NoEnoughMoneyException{};

class TMA {
	TMAConfig *config_;
	DBCommunicator *storage_;
	std::vector<MoneyData*> *data_;
    std::set<std::string> namesCache_;
    std::set<std::string> categoriesCache_;
public:
	TMA() {
        //this->config_ = new TMAConfig();
        this->storage_ = new DBCommunicator("tr.db");
		this->data_ = new std::vector<MoneyData*>();
		this->getAll(this->data_);
        this->storage_->readCache(&namesCache_, "NAMES");
        this->storage_->readCache(&categoriesCache_, "CATEGORIES");
	}
	void reread() {
		delete data_;
		this->data_ = new std::vector<MoneyData*>();
		this->getAll(this->data_);
	}
	/* Append data */
    void append(uint32_t ID, unsigned long long date, bool code, std::string &name, uint32_t money_before, uint32_t sum, std::string &category) {
        if (data_->size() == this->getSize()) {
            namesCache_.insert(name);
            categoriesCache_.insert(category);
            storage_->writeCache("NAMES", name);
            storage_->writeCache("CATEGORIES", category);
            MoneyData *moneyData = new MoneyData();
			moneyData->ID = ID;
			moneyData->date = date;
			moneyData->code = code;
			moneyData->name = name;
			moneyData->money_before = money_before;
			moneyData->sum = sum;
			moneyData->category = category;
            if (sum > money_before && !code) {
                throw NoEnoughMoneyException();
            }
			data_->push_back(moneyData);
			this->storage_->write(moneyData);
		} else {
			this->reread();
		}
	}
    void append(unsigned long long date, bool code, std::string &name, uint32_t money_before, uint32_t sum, std::string &category) {
        if (data_->size() == this->getSize()) {
            namesCache_.insert(name);
            categoriesCache_.insert(category);
            storage_->writeCache("NAMES", name);
            storage_->writeCache("CATEGORIES", category);
			uint32_t ID = this->getSize();
            MoneyData *moneyData = new MoneyData();
			moneyData->ID = ID;
			moneyData->date = date;
			moneyData->code = code;
			moneyData->name = name;
			moneyData->money_before = money_before;
			moneyData->sum = sum;
			moneyData->category = category;
            if (sum > money_before && !code) {
                throw NoEnoughMoneyException();
            }
			data_->push_back(moneyData);
			this->storage_->write(moneyData);
		} else {
			this->reread();
		}
	}
    void append(unsigned long long date, bool code, std::string &name, uint32_t sum, std::string &category) {
        if (data_->size() == this->getSize()) {
            namesCache_.insert(name);
            categoriesCache_.insert(category);
            storage_->writeCache("NAMES", name);
            storage_->writeCache("CATEGORIES", category);
			uint32_t ID = this->getSize();
            uint32_t money_before = 0;
            if (data_->size()) {
            auto last = (*data_)[data_->size()-1];
                if (last->code) {
                 money_before = last->money_before + last->sum;
                } else {
                    money_before = last->money_before - last->sum;
                }
            }
            MoneyData *moneyData = new MoneyData();
			moneyData->ID = ID;
			moneyData->date = date;
			moneyData->code = code;
			moneyData->name = name;
			moneyData->money_before = money_before;
			moneyData->sum = sum;
			moneyData->category = category;
            if (sum > money_before && !code) {
                throw NoEnoughMoneyException();
            }
			data_->push_back(moneyData);
			this->storage_->write(moneyData);
		} else {
			this->reread();
		}
	}
    void append(uint32_t ID, unsigned long long date, bool code, std::string &name, uint32_t sum, std::string &category) {
        if (data_->size() && data_->size() == this->getSize()) {
            namesCache_.insert(name);
            categoriesCache_.insert(category);
            storage_->writeCache("NAMES", name);
            storage_->writeCache("CATEGORIES", category);
            uint32_t money_before = 0;
            if (data_->size()) {
                auto last = (*data_)[data_->size()-1];
                if (last->code) {
                    money_before = last->money_before + last->sum;
                } else {
                    money_before = last->money_before - last->sum;
                }
            }
            MoneyData *moneyData = new MoneyData();
			moneyData->ID = ID;
			moneyData->date = date;
			moneyData->code = code;
			moneyData->name = name;
			moneyData->money_before = money_before;
			moneyData->sum = sum;
			moneyData->category = category;
            if (sum > money_before && !code) {
                throw NoEnoughMoneyException();
            }
			data_->push_back(moneyData);
			this->storage_->write(moneyData);
		} else {
			this->reread();
		}
	}
    void get(std::vector<MoneyData*> *data, std::string &category, std::string &name, unsigned long long begDate, unsigned long long endDate) {
        this->storage_->select(data, category, name, begDate, endDate);
    }
    void get(std::vector<MoneyData*> *data, std::string &category, std::string &name) {
        this->storage_->select(data, category, name);
    }
    void get(std::vector<MoneyData*> *data, bool code, std::string &category, std::string &name, unsigned long long begDate, unsigned long long endDate) {
        this->storage_->select(data, code, category, name, begDate, endDate);
    }
    void get(std::vector<MoneyData*> *data, bool code, std::string &category, std::string &name) {
        this->storage_->select(data, code, category, name);
    }
	/* Reads ALL data */
	void getAll(std::vector<MoneyData*> *data) {
        this->storage_->read(data);
	}
	/* Reads data by category */
	void getByCat(std::vector<MoneyData*> *data, std::string &category) {
        this->storage_->readByCat(data, category);
	}
	/* Reads data by name */
	void getByName(std::vector<MoneyData*> *data, std::string &name) {
        this->storage_->readByName(data, name);
	}
	/* Reads data by date range */
	void getByDates(std::vector<MoneyData*> *data, uint32_t begDate, uint32_t endDate) {
        this->storage_->readByDates(data, begDate, endDate);
	}
	/* Reads data by one date */
	void getByDate(std::vector<MoneyData*> *data, uint32_t date) {
        this->storage_->readByDate(data, date);
	}
    const std::set<std::string> getNamesCached() {
        return namesCache_;
    }
    const std::set<std::string> getCategoriesCached() {
        return categoriesCache_;
    }
    uint32_t getBalance() {
        uint32_t balance = 0;
        if (data_->size()) {
            auto last = (*data_)[data_->size()-1];
            if (last->code) {
                balance = last->money_before + last->sum;
            } else {
                balance = last->money_before - last->sum;
            }
        }
        return balance;
    }
    /* return count of rows in database */
    int getSize() {;
		return this->storage_->size();
	}
	/* get config class for reconfiguring */
	TMAConfig *getConfig() {
		return config_;
	}
};

#endif
