#pragma once
#include "Database.hpp"
#include "Message.hpp"
#include <vector>

class MessageService {
    DB &db;
    
public:
    MessageService(DB &database) : db(database) {}
    
    int sendMessage(int senderId, int receiverId, const std::string &content);
    std::vector<Message> getInbox(int userId);

};


