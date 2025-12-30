#pragma once
#include <string>

struct Message {
    int id;
    int sender_id;
    int receiver_id;
    std::string content;
    std::string timestamp;
    std::string sender_name;
    std::string receiver_name;
    
    Message(int _id, int _sender_id, int _receiver_id, 
           const std::string& _content, const std::string& _timestamp,
           const std::string& _sender_name = "", const std::string& _receiver_name = "")
        : id(_id), sender_id(_sender_id), receiver_id(_receiver_id),
          content(_content), timestamp(_timestamp),
          sender_name(_sender_name), receiver_name(_receiver_name) {}
};


