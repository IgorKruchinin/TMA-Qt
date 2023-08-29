#ifndef DB_COMMUNICATOR
#define DB_COMMUNICATOR
#include <sqlite3.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
#include "money_data.h"
class DBException{};

class DBCommunicator {
	sqlite3 *db;
	int rc; // code of condition
	char *errMSG = 0;

    static void static_exec(char const *filename_tmp, char const *sql_request, int (*callback)(void*,int,char**,char**), void *callback_arg) {
        sqlite3 *db_tmp;
        char *errMSG_tmp = 0;
        int rc_tmp;
        rc_tmp = sqlite3_open(filename_tmp, &db_tmp);
        if (!rc_tmp) {
            rc_tmp = sqlite3_exec(db_tmp, sql_request, callback, callback_arg, &errMSG_tmp);
            if (rc_tmp != SQLITE_OK) {
                // Error (text in errMSG)
                throw DBException();
                sqlite3_free(errMSG_tmp);
            }
        sqlite3_close(db_tmp);
        }
    }

    void exec(char const *sql_request, int (*callback)(void*,int,char**,char**), void *callback_arg);

    void exec(char const *sql_request);
    // callbacks
    static int size_callback(void *p_data, int num_fields, char **p_fields, char **p_col_names) {
        int *size = (int*)p_data;
        *size = atoi(p_fields[0]);
        return 0;
    }

    static int select_callback(void *p_data, int num_fields, char **p_fields, char **p_col_names) {
        std::vector<MoneyData*> *data = static_cast<std::vector<MoneyData*>*>(p_data);
        int ID;
        long long unsigned date;
        bool code;
        std::string name;
        uint32_t money_before;
        uint32_t sum;
        std::string category;
        ID = atoi(p_fields[0]);
        date = atoll(p_fields[1]);
        code = atoi(p_fields[2]);
        name = p_fields[3];
        money_before = atoi(p_fields[4]);
        sum = atoi(p_fields[5]);
        category = p_fields[6];
        MoneyData *moneyData = new MoneyData();
        moneyData->ID = ID;
        moneyData->date = date;
        moneyData->code = code;
        moneyData->name = name;
        moneyData->money_before = money_before;
        moneyData->sum = sum;
        moneyData->category = category;
        data->push_back(moneyData);
        return 0;
    }
    static int select_cache_callback(void *p_data, int num_fields, char **p_fields, char **p_col_names) {
        std::set<std::string> *cache_data = static_cast<std::set<std::string>*>(p_data);
        int ID;
        std::string field;
        std::string value;
        ID = atoi(p_fields[0]);
        field = p_fields[1];
        value = p_fields[2];
        cache_data->insert(value);
        return 0;
    }

public:
	DBCommunicator(char const *filename);
    ~DBCommunicator();
	void write(MoneyData *data);
	void write(std::vector<MoneyData*> *data);
	/* The read method reads ALL data from database */
	void read(std::vector<MoneyData*> *data);
    void readCache(std::set<std::string> *cache, const std::string &fieldName);
    void select(std::vector<MoneyData*> *data, std::string &category, std::string &name, unsigned long long begDate, unsigned long long endDate);
    void select(std::vector<MoneyData*> *data, std::string &category, std::string &name);
    void select(std::vector<MoneyData*> *data, bool code, std::string &category, std::string &name, unsigned long long begDate, unsigned long long endDate);
    void select(std::vector<MoneyData*> *data, bool code, std::string &category, std::string &name);
	/* The readByCat method reads data from database by category */
	void readByCat(std::vector<MoneyData*> *data, std::string &category);
	/* The readByName method reads data from database by name */
	void readByName(std::vector<MoneyData*> *data, std::string &name);
	/* The readByDates method reads data from database by date range */
    void readByDates(std::vector<MoneyData*> *data, unsigned long long begDate, unsigned long long endDate);
	/* The readByDate method reads data from database by one date */
    void readByDate(std::vector<MoneyData*> *data, unsigned long long date);
    void writeCache(const std::string &fieldName, const std::string &value);
	/* return count of rows in database */
	int size();
    int cacheSize();
    int cacheSize(const std::string &fieldName);
};
#endif

