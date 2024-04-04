#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>

class assembler
{
private:
	std::vector<std::string> split(const std::string& s, char delimiter);

public:
	std::pair<std::vector<unsigned char>, std::vector<std::string>> assemble(const std::string& assembly);
};