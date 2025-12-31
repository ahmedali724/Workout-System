#pragma once
#include <vector>
#include <string>
#include <functional>
#include "sqlite3.h"

class DB
{
private:
    sqlite3 *db;

public:
    DB(const std::string &file)
    {
        sqlite3_open(file.c_str(), &db);
    }

    ~DB()
    {
        sqlite3_close(db);
    }

    bool exec(const std::string &sql)
    {
        return sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
    }

    template <typename T, typename... Args>
    std::vector<T> queryPrepared(
        const std::string &sql,
        std::function<T(sqlite3_stmt *)> mapper,
        Args... args)
    {
        std::vector<T> result;
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
            return result;

        int idx = 1;
        auto bindArg = [&](auto value)
        {
            using V = decltype(value);
            if constexpr (std::is_same_v<V, int>)
                sqlite3_bind_int(stmt, idx++, value);
            else if constexpr (std::is_same_v<V, std::string>)
                sqlite3_bind_text(stmt, idx++, value.c_str(), -1, SQLITE_TRANSIENT);
        };

        (bindArg(args), ...);

        while (sqlite3_step(stmt) == SQLITE_ROW)
            result.push_back(mapper(stmt));

        sqlite3_finalize(stmt);
        return result;
    }

    template <typename T, typename... Args>
    T scalarPrepared(const std::string &sql, Args... args)
    {
        sqlite3_stmt *stmt;
        T value{};

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
            return value;

        int idx = 1;
        auto bindArg = [&](auto v)
        {
            using V = decltype(v);
            if constexpr (std::is_same_v<V, int>)
                sqlite3_bind_int(stmt, idx++, v);
            else if constexpr (std::is_same_v<V, std::string>)
                sqlite3_bind_text(stmt, idx++, v.c_str(), -1, SQLITE_TRANSIENT);
        };

        (bindArg(args), ...);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            if constexpr (std::is_same_v<T, int>)
                value = sqlite3_column_int(stmt, 0);
            else if constexpr (std::is_same_v<T, double>)
                value = sqlite3_column_double(stmt, 0);
            else if constexpr (std::is_same_v<T, std::string>)
                value = (const char *)sqlite3_column_text(stmt, 0);
        }

        sqlite3_finalize(stmt);
        return value;
    }

    template <typename... Args>
    int executePrepared(const std::string &sql, Args... args)
    {
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
            return -1;

        int idx = 1;
        auto bindArg = [&](auto value)
        {
            using T = decltype(value);
            if constexpr (std::is_same_v<T, int>)
                sqlite3_bind_int(stmt, idx++, value);
            else if constexpr (std::is_same_v<T, std::string>)
                sqlite3_bind_text(stmt, idx++, value.c_str(), -1, SQLITE_TRANSIENT);
        };

        (bindArg(args), ...);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            sqlite3_finalize(stmt);
            return -1;
        }

        int changes = sqlite3_changes(db);
        sqlite3_finalize(stmt);
        return changes;
    }
};
