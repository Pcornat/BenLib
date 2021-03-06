/*
** BENSUPERPC PROJECT, 2020
** Filesystem
** File description:
** filesystem.hpp
*/

// https://stackoverflow.com/a/46931770/10152334

#ifndef PARSING_HPP_
#define PARSING_HPP_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace my
{
namespace string
{
std::vector<std::string> split(const std::string &s, const std::string &);
void split(std::vector<std::string> &, const std::string &, const std::string &);
std::vector<std::string> split(const std::string &s, const char);
void split(std::vector<std::string> &, const std::string &, const char);

void csv_parse(std::vector<std::vector<std::string>> &, const std::string &, const char);
} // namespace string
} // namespace my
#endif
