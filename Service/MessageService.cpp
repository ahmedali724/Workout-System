#include "MessageService.hpp"
#include <string>

int MessageService::sendMessage(int senderId, int receiverId, const std::string &content) {
    std::string sql = "INSERT INTO Message (sender_id, receiver_id, content) VALUES (?, ?, ?);";
    return db.executePrepared(sql, senderId, receiverId, content);
}

std::vector<Message> MessageService::getInbox(int userId) {
    return db.queryPrepared<Message>(
        "SELECT m.id, m.sender_id, m.receiver_id, m.content, m.timestamp, "
        "s.name as sender_name, r.name as receiver_name "
        "FROM Message m "
        "INNER JOIN User s ON m.sender_id = s.id "
        "INNER JOIN User r ON m.receiver_id = r.id "
        "WHERE m.receiver_id=? "
        "ORDER BY m.timestamp DESC;",
        [](sqlite3_stmt *s) {
            return Message{
                sqlite3_column_int(s, 0),
                sqlite3_column_int(s, 1),
                sqlite3_column_int(s, 2),
                (const char *)sqlite3_column_text(s, 3),
                (const char *)sqlite3_column_text(s, 4),
                (const char *)sqlite3_column_text(s, 5),
                (const char *)sqlite3_column_text(s, 6)
            };
        },
        userId);
}

