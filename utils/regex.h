#ifndef UTILS_REGEX_H_
#define UTILS_REGEX_H_

#include <string>
#include <vector>
using std::string;
using std::vector;
namespace utils {
// extract all hashtags from subject and put them into vector
vector<string> HashParser(const string& subject);
}  // namespace utils
#endif  // UTILS_REGEX_H_
