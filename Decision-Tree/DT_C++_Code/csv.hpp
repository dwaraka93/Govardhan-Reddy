#ifndef CSV_HPP_INCLUDE
#define CSV_HPP_INCLUDE

#include <string>
#include <vector>

std::vector<std::vector<int>> read_csv(const std::string &filename);

class splitstring : public std::string {
    std::vector<std::string> flds;
public:
    splitstring(std::string s) : std::string(s) { };
    std::vector<std::string>& split(char delim, int rep=0);
};



#endif /* end of include guard: CSV_HPP_INCLUDE */

