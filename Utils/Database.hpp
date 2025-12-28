#pragma once
#include <vector>
#include <string>
#include <functional>
#include "sqlite3.h"

class DB
{
private:
    sqlite3* db;

public:
    DB(const std::string& file);
    ~DB();

    bool exec(const std::string& sql);

    template<typename T>
    std::vector<T> query(
        const std::string& sql,
        std::function<T(sqlite3_stmt*)> mapper
    );

    template<typename T>
    T scalar(const std::string& sql);
};
