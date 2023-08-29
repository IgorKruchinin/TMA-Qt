#include <sqlite3.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
#include "db_communicator.h"
#include "money_data.h"

void DBCommunicator::exec(char const *sql_request, int (*callback)(void*,int,char**,char**), void *callback_arg) {
    rc = sqlite3_exec(db, sql_request, callback, callback_arg, &errMSG);
    if (rc != SQLITE_OK) {
        // Error (text in errMSG)
        throw DBException();
        sqlite3_free(errMSG);
    }
}

void DBCommunicator::exec(char const *sql_request) {
    exec(sql_request, 0, 0);
}

DBCommunicator::DBCommunicator(char const *filename) {
	rc = sqlite3_open(filename, &db);
	if (rc) {
		// Error in opening database
        throw DBException();
	}
	char create_table_request[] = "CREATE TABLE IF NOT EXISTS \'TRANSACTIONS\' ("\
											"ID INT PRIMARY KEY NOT NULL,"\
											"DATE DATE,"\
											"CODE BOOLEAN NOT NULL,"\
											"NAME TEXT NOT NULL,"\
											"MONEY_BEFORE INT NOT NULL,"\
											"SUM INT NOT NULL,"
                                            "CATEGORY TEXT );";
    char create_cache_table_request[] = "CREATE TABLE IF NOT EXISTS \'CACHE\' ("\
        "ID INT PRIMARY KEY NOT NULL,"\
        "FIELD TEXT,"\
        "VALUE TEXT );";
    this->exec(create_table_request);
    this->exec(create_cache_table_request);
}
DBCommunicator::~DBCommunicator() {
	sqlite3_close(db);
}
void DBCommunicator::write(MoneyData *data) {
    char *request = new char[1024];
    sprintf(request, "INSERT INTO \'TRANSACTIONS\' (ID, DATE, CODE, NAME, MONEY_BEFORE, SUM, CATEGORY)" \
            "VALUES(%d, %llu, \'%d\', \'%s\', %d, %d, \'%s\');", data->ID, data->date, data->code, data->name.c_str(), data->money_before, data->sum, data->category.c_str());
	this->exec(request);
    delete[] request;
}
void DBCommunicator::write(std::vector<MoneyData*> *data) {
    for (MoneyData *d: *data) {
        this->write(d);
	}
}
/* The read method reads ALL data from database */
void DBCommunicator::read(std::vector<MoneyData*> *data) {
    for (int i = 0; i < size(); ++i) {
        char *request = new char[1024];
        sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d;", i);
        this->exec(request, select_callback, data);
        delete[] request;
    }
}
void DBCommunicator::readCache(std::set<std::string> *cache, const std::string &fieldName) {
    for (int i = 0; i < cacheSize(fieldName); ++i) {
        char *cache_request = new char[1024];
        sprintf(cache_request, "SELECT * FROM \'CACHE\' WHERE ID = %d AND FIELD = \'%s\';", i, fieldName.c_str());
        this->exec(cache_request, select_cache_callback, cache);
        delete[] cache_request;
    }
}
void DBCommunicator::select(std::vector<MoneyData*> *data, std::string &category, std::string &name, unsigned long long begDate, unsigned long long endDate) {
    for (int i = 0; i < size(); ++i) {
        char *request = new char[1024];
        if (!category.empty() && !name.empty()) {
            sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND CATEGORY = \'%s\' AND NAME = \'%s\' AND DATE >= %llu AND DATE <= %llu;", i, category.c_str(), name.c_str(), begDate, endDate);
        } else if (!category.empty()) {
            sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND CATEGORY = \'%s\' AND DATE >= %llu AND DATE <= %llu;", i, category.c_str(), begDate, endDate);
        } else if (!name.empty()) {
            sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND NAME = \'%s\' AND DATE >= %llu AND DATE <= %llu;", i, name.c_str(), begDate, endDate);
        } else {
            sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND DATE >= %llu AND DATE <= %llu;", i, begDate, endDate);
        }
        this->exec(request, select_callback, data);
        delete[] request;
    }
}
void DBCommunicator::select(std::vector<MoneyData*> *data, std::string &category, std::string &name) {
    for (int i = 0; i < size(); ++i) {
        char *request = new char[1024];
        if (!category.empty() && !name.empty()) {
            sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND CATEGORY = \'%s\' AND NAME = \'%s\';", i, category.c_str(), name.c_str());
        } else if (!category.empty()) {
            sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND CATEGORY = \'%s\';", i, category.c_str());
        } else if (!name.empty()) {
            sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND NAME = \'%s\';", i, name.c_str());
        } else {
            sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d;", i);
        }
        this->exec(request, select_callback, data);
        delete[] request;
    }
}
void DBCommunicator::select(std::vector<MoneyData*> *data, bool code, std::string &category, std::string &name, unsigned long long begDate, unsigned long long endDate) {
    for (int i = 0; i < size(); ++i) {
        char *request = new char[1024];
        if (!category.empty() && !name.empty()) {
            sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND CATEGORY = \'%s\' AND NAME = \'%s\' AND CODE = %d AND DATE >= %llu AND DATE <= %llu;", i, category.c_str(), name.c_str(), code, begDate, endDate);
        } else if (!category.empty()) {
            sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND CATEGORY = \'%s\' AND CODE = %d AND DATE >= %llu AND DATE <= %llu;", i, category.c_str(), code, begDate, endDate);
        } else if (!name.empty()) {
            sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND NAME = \'%s\' AND CODE = %d AND DATE >= %llu AND DATE <= %llu;", i, name.c_str(), code, begDate, endDate);
        } else {
            sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND CODE = %d AND DATE >= %llu AND DATE <= %llu;", i, code, begDate, endDate);
        }
        this->exec(request, select_callback, data);
        delete[] request;
    }
}
void DBCommunicator::select(std::vector<MoneyData*> *data, bool code, std::string &category, std::string &name) {
    for (int i = 0; i < size(); ++i) {
        char *request = new char[1024];
        if (!category.empty() && !name.empty()) {
            sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND CATEGORY = \'%s\' AND NAME = \'%s\' AND CODE = %d;", i, category.c_str(), name.c_str(), code);
        } else if (!category.empty()) {
            sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND CATEGORY = \'%s\' AND CODE = %d;", i, category.c_str(), code);
        } else if (!name.empty()) {
            sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND NAME = \'%s\' AND CODE = %d;", i, name.c_str(), code);
        } else {
            sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND CODE = %d;", i,  code);
        }
        this->exec(request, select_callback, data);
        delete[] request;
    }
}
/* The readByCat method reads data from database by category */
void DBCommunicator::readByCat(std::vector<MoneyData*> *data, std::string &category) {
    for (int i = 0; i < size(); ++i) {
        char *request = new char[1024];
        sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND CATEGORY = \'%s\';", i, category.c_str());
        this->exec(request, select_callback, data);
        delete[] request;
	}
}
/* The readByName method reads data from database by name */
void DBCommunicator::readByName(std::vector<MoneyData*> *data, std::string &name) {
    for (int i = 0; i < size(); ++i) {
        char *request = new char[1024];
        sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND NAME = \'%s\';", i, name.c_str());
        this->exec(request, select_callback, data);
        delete[] request;
	}
}
/* The readByDates method reads data from database by date range */
void DBCommunicator::readByDates(std::vector<MoneyData*> *data, unsigned long long begDate, unsigned long long endDate) {
    for (int i = 0; i < size(); ++i) {
        char *request = new char[1024];
        sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND DATE >= %llu AND DATE <= %llu;", i, begDate, endDate);
        this->exec(request, select_callback, data);
        delete[] request;
	}
}
/* The readByDate method reads data from database by one date */
void DBCommunicator::readByDate(std::vector<MoneyData*> *data, unsigned long long date) {
   for (int i = 0; i < size(); ++i) {
        char *request = new char[1024];
        sprintf(request, "SELECT * FROM \'TRANSACTIONS\' WHERE ID = %d AND DATE = %llu;", i, date);
        this->exec(request, select_callback, data);
        delete[] request;
	}
}
void DBCommunicator::writeCache(const std::string &fieldName, const std::string &value) {
    char *caching_request = new char[1024];
    sprintf(caching_request, "INSERT INTO \'CACHE\' (ID, FIELD, VALUE) " \
            "VALUES(%d, \'%s\', \'%s\');", this->cacheSize(), fieldName.c_str(), value.c_str());
    printf("%s", caching_request);
    this->exec(caching_request);
    delete[] caching_request;
}
/* return count of rows in database */
int DBCommunicator::size() {
    int size = 0;
    char request[] = "SELECT COUNT(*) FROM \'TRANSACTIONS\'";
    this->exec(request, size_callback, &size);
	return size;
}
int DBCommunicator::cacheSize() {
    int cache_size = 0;
    char cache_request[] = "SELECT COUNT(*) FROM \'CACHE\'";
    this->exec(cache_request, size_callback, &cache_size);
    return cache_size;
}
int DBCommunicator::cacheSize(const std::string &fieldName) {
    int cache_size = 0;
    char *cache_request = new char[1024];
    sprintf(cache_request, "SELECT COUNT(*) FROM \'CACHE\' WHERE FIELD = \'%s\'", fieldName.c_str());
    this->exec(cache_request, size_callback, &cache_size);
    delete[] cache_request;
    return cache_size;
}
