#pragma once

#include "instance.h"

#include <map>
#include <vector>

instance parse(const std::string& filename);
std::map<std::string, std::vector<std::string>> get_sections(const std::string& filename);