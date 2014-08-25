#include "main.hpp"

#ifndef UTILS_H
#define UTILS_H

std::vector<byte> read_all_bytes(std::string filename)
{
    std::ifstream ifs;
    ifs.open(filename,std::ios::binary);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char> result(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&result[0], pos);

    return std::vector<byte>(result.begin(),result.end());
}
#endif
