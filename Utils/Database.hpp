#pragma once
#include <vector>
#include <string>
#include <functional>
#include <type_traits>
#include "sqlite3.h"

class DB
{
private:
    sqlite3* db;

public:
    DB(const std::string& file)
    {
        sqlite3_open(file.c_str(), &db);
    }

    ~DB()
    {
        sqlite3_close(db);
    }

    bool exec(const std::string& sql)
    {
        return sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
    }

    template<typename T>
    std::vector<T> query(
        const std::string& sql,
        std::function<T(sqlite3_stmt*)> mapper)
    {
        std::vector<T> result;
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
            return result;

        while (sqlite3_step(stmt) == SQLITE_ROW)
            result.push_back(mapper(stmt));

        sqlite3_finalize(stmt);
        return result;
    }

    template<typename T>
    T scalar(const std::string& sql)
    {
        sqlite3_stmt* stmt;
        T value{};

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                if constexpr (std::is_same_v<T, int>)
                    value = sqlite3_column_int(stmt, 0);
                else if constexpr (std::is_same_v<T, double>)
                    value = sqlite3_column_double(stmt, 0);
                else if constexpr (std::is_same_v<T, std::string>)
                    value = (const char*)sqlite3_column_text(stmt, 0);
            }
        }
        sqlite3_finalize(stmt);
        return value;
    }
};
