#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>


std::string readFile(const std::string &filename)
{
    std::ifstream file(filename);
    std::string content, line;
    while (getline(file, line))
        content += line + "\n";
    return content;
}