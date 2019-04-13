#ifndef UTILS_REGEX_H_
#define UTILS_REGEX_H_
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace utils {
// subject: a string contains several hashtags
// return all hashtags in subject(as vector)
vector<string> HashParser(const string& subject);
}  // namespace utils
#endif  // UTILS_REGEX_H_
