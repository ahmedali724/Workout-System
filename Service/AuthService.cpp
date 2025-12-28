#include "AuthService.hpp"

int AuthService::registerUser(User& newUser)
{
    std::string sql = "INSERT INTO User (username, password, email, name, age, gender, role) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?);";

    return db.executePrepared(sql,
        newUser.username,
        newUser.password,
        newUser.email,
        newUser.name,
        newUser.age,
        newUser.gender,
        newUser.role
    );
}

std::optional<User> AuthService::login(const std::string& username, const std::string& password)
{
    auto users = db.query<User>(
        "SELECT id, username, password, email, name, age, gender, role FROM User "
        "WHERE username=? AND password=?;",
        [&](sqlite3_stmt* s)
        {
            return User{
                sqlite3_column_int(s, 0),
                (const char*)sqlite3_column_text(s, 1),
                (const char*)sqlite3_column_text(s, 2),
                (const char*)sqlite3_column_text(s, 3),
                (const char*)sqlite3_column_text(s, 4),
                sqlite3_column_int(s, 5),
                (const char*)sqlite3_column_text(s, 6),
                (const char*)sqlite3_column_text(s, 7)
            };
        }
    );

    if (!users.empty())
        return users[0];
    return std::nullopt;
}

std::optional<User> AuthService::getProfile(int userId)
{
    auto users = db.query<User>(
        "SELECT id, username, password, email, name, age, gender, role FROM User "
        "WHERE id=?;",
        [&](sqlite3_stmt* s)
        {
            return User{
                sqlite3_column_int(s, 0),
                (const char*)sqlite3_column_text(s, 1),
                (const char*)sqlite3_column_text(s, 2),
                (const char*)sqlite3_column_text(s, 3),
                (const char*)sqlite3_column_text(s, 4),
                sqlite3_column_int(s, 5),
                (const char*)sqlite3_column_text(s, 6),
                (const char*)sqlite3_column_text(s, 7)
            };
        }
    );

    if (!users.empty())
        return users[0];
    return std::nullopt;
}

int AuthService::updateProfile(User &user)
{
    std::string sql = "UPDATE User SET username=?, password=?, email=?, name=?, age=?, gender=?, role=? "
                      "WHERE id=?;";

    return db.executePrepared(sql,
        user.username,
        user.password,
        user.email,
        user.name,
        user.age,
        user.gender,
        user.role,
        user.id
    );
}
