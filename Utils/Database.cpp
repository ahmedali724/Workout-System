#include "Database.hpp"
#include <iostream>

DB::DB(const std::string& file) : db(nullptr)
{
    if (sqlite3_open(file.c_str(), &db) != SQLITE_OK)
        db = nullptr;
}

DB::~DB()
{
    if (db)
        sqlite3_close(db);
}

bool DB::exec(const std::string& sql)
{
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

template<typename T>
std::vector<T> DB::query(
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
T DB::scalar(const std::string& sql)
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
                value = reinterpret_cast<const char*>(
                    sqlite3_column_text(stmt, 0));
        }
    }

    sqlite3_finalize(stmt);
    return value;
}
