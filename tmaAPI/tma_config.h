#ifndef TMA_CONFIG
#define TMA_CONFIG
#include <string>
#include <cstdio>
#define CONFIG_FILE_PATH "TMA.conf"

#define DB_PATH_DEFAULT "data.db"

class TMAConfigFileOpenException {};


class TMAConfig {
    std::string db_path;
    FILE *config_file = NULL;
public:
    TMAConfig() {
        printf("iii");
        this->config_file = std::fopen(CONFIG_FILE_PATH, "rw");
        printf("%p", config_file);
        this->reread(1);
    }
    void reread(bool create_if_not_exists = 0) {
        if (!this->config_file) {
            if (create_if_not_exists) {
                //this->reset();
                printf("ffd");
            } else {
                throw TMAConfigFileOpenException();
            }
        }
        printf("fff");
        bool reading = 1;
        while(reading) {
            char *field_cstr = new char[512];
            char *value_cstr = new char[512];
            reading = std::fscanf(this->config_file, "%s: %s", field_cstr, value_cstr);
            std::string field = field_cstr;
            std::string value = value_cstr;
            if (field == "DB_PATH") {
                this->db_path = value;
            }
        }
    }
    void reset() {
        this->db_path = DB_PATH_DEFAULT;
        this->rewrite();
    }
    void rewrite() {
        if (!this->config_file) {
            throw TMAConfigFileOpenException();
        }
        std::fprintf(this->config_file, "DB_PATH: %s", db_path.c_str());
    }
    void setDBPath(std::string &DBPath) {
        this->db_path = DBPath;
        this->rewrite();
    }
    std::string &getDBPath() {
        return db_path;
    }
    ~TMAConfig() {
        std::fclose(config_file);
    }
};

#endif
