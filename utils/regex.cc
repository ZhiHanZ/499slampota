#include "regex.h"
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using std::regex;
using std::smatch;
using std::sregex_iterator;
using std::vector;

namespace utils {
vector<string> HashParser(const string& subject) {
  // find all candidates # with at least one char
  regex re("#[^\\s]+");
  std::sregex_iterator next(subject.begin(), subject.end(), re);
  std::sregex_iterator end;
  vector<string> tags;
  while (next != end) {
    std::smatch match = *next;
    tags.push_back(match.str());
    next++;
  }
  return tags;
}

}  // namespace utils
