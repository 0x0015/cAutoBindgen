#pragma once
#include <vector>
#include <string>

struct classDef{
	std::string name;
	unsigned int size;
	struct memberFunc{
		std::string name;
		std::string retType;
		std::vector<std::pair<std::string, std::string>> args;
	};
	std::vector<memberFunc> members;
	void print() const;

	void fixConstructDesconstructorTypes();
	std::string genCHeader() const;
	std::string genCppSource(const std::string& CHeaderName, const std::vector<std::string>& headersToInclude = {}) const;
};

