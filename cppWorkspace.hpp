#pragma once
#include <iostream>
#include <vector>
#include <optional>

struct cppWorkspace{
	std::string name;
	struct cppFunction{
		std::string name;
		std::string rtype;
		std::vector<std::pair<std::string, std::optional<std::string>>> args;
		std::vector<std::string> memberOfList; //list of parent class, parent class's parent's parent (including namespaces, etc...) up to base namespace
	};
	struct cppClass{
		std::string name;
		unsigned int size;
		std::vector<std::string> memberOfList;
	};
	struct cppEnum{
		std::string name;
		unsigned int size;
		std::vector<std::pair<std::string, std::optional<unsigned int>>> values;
		std::vector<std::string> memberOfList;
	};

	std::vector<cppFunction> functions;
	std::vector<cppClass> classes;
	std::vector<cppEnum> enums;

	void print() const;
	void fixConstructDesconstructorTypes();
	std::string genCHeader() const;
	std::string genCppSource(const std::string& CHeaderName, const std::vector<std::string>& headersToInclude = {}) const;
	std::string genJsonDetail() const;
};
