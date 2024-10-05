#include <iostream>
#include <clang-c/Index.h>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <fstream>
#include <filesystem>
#include "classDef.hpp"

std::ostream& operator<<(std::ostream& stream, const CXString& str)
{
  stream << clang_getCString(str);
  clang_disposeString(str);
  return stream;
}

std::unordered_set<std::string> lookingForStructures;
std::unordered_map<std::string, classDef> structDefs;
std::unordered_map<std::string, CXCursor> notableCursors;
std::list<std::pair<std::string, classDef::memberFunc*>> memberFuncs;

int main(int argc, char** argv){
	if(argc < 3){
		std::cout<<"Please pass the header and structures of intrest you want to do something with as an argument"<<std::endl;
		return -1;
	}

	std::string header = argv[1];
	for(unsigned int i=2;i<argc;i++){
		lookingForStructures.insert(argv[i]);
	}
	CXIndex index = clang_createIndex(0, 0);
	CXTranslationUnit unit = clang_parseTranslationUnit(index, header.c_str(), nullptr, 0, nullptr, 0, CXTranslationUnit_None);
	if(unit == nullptr){
		std::cerr<<"Unable to parse translation unit.  Quitting"<<std::endl;
		return -1;
	}

	CXCursor cursor = clang_getTranslationUnitCursor(unit);
	clang_visitChildren(cursor, [](CXCursor c, CXCursor parent, CXClientData client_data){
		std::string cursorName = clang_getCString(clang_getCursorSpelling(c));
		if(lookingForStructures.contains(cursorName)){
			switch(clang_getCursorKind(c)){
				case CXCursor_StructDecl:
				case CXCursor_UnionDecl:
				case CXCursor_ClassDecl:
				case CXCursor_EnumDecl:
					notableCursors[cursorName] = c;
					structDefs[cursorName] = {cursorName};
					std::cout<<"Found def for "<<cursorName<<std::endl;
					break;
				default:
					std::cout<<"Skipping def of kind: "<<clang_getCursorKind(c)<<" in "<<cursorName<<std::endl;
					break;
			}
		}
		return CXChildVisit_Recurse;
	}, nullptr);

	for(auto& [name, cursor] : notableCursors){
		clang_visitChildren(cursor, [](CXCursor c, CXCursor parent, CXClientData client_data){
			std::string parentName = clang_getCString(clang_getCursorSpelling(parent));
			std::string cursorName = clang_getCString(clang_getCursorSpelling(c));
			switch(clang_getCursorKind(c)){
				case CXCursor_Constructor:
					structDefs[parentName].members.push_back({parentName});
					memberFuncs.push_back({parentName + "::" + cursorName, &structDefs[parentName].members.back()});
					break;
				case CXCursor_Destructor:
					structDefs[parentName].members.push_back({"~" + parentName});
					memberFuncs.push_back({parentName + "::~" + parentName, &structDefs[parentName].members.back()});
					break;
				case CXCursor_CXXMethod:{
					CXType type = clang_getCursorType(c);
					std::string fullFuncType = clang_getCString(clang_getTypeSpelling(type));
					std::string retType = fullFuncType.substr(0, fullFuncType.find(' '));
					structDefs[parentName].members.push_back({cursorName, retType});
					memberFuncs.push_back({parentName + "::" + cursorName, &structDefs[parentName].members.back()});
					break;
					}
				case CXCursor_ParmDecl:{
					CXType type = clang_getCursorType(c);
					memberFuncs.back().second->args.push_back({clang_getCString(clang_getTypeSpelling(type)), clang_getCString(clang_getCursorSpelling(c))});
					break;
					}
				default:
					std::cout<<"Cursor \""<<clang_getCursorSpelling(c)<<"\" of kind: \""<<clang_getCursorKind(c)<<"\""<<std::endl;
					return CXChildVisit_Continue;
			}
			return CXChildVisit_Recurse;
		}, nullptr);
	}

	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);

	for(auto& [name, def] : structDefs){
		std::cout<<"Found def for "<<name<<":"<<std::endl;
		def.print();
		def.fixConstructDesconstructorTypes();
		std::string outputFn = std::filesystem::path(header).replace_extension("").string() + "_c";
		std::ofstream headerWrite(outputFn + ".h");
		headerWrite << def.genCHeader();
		std::ofstream sourceWrite(outputFn + ".cpp");
		sourceWrite<<def.genCppSource(outputFn + ".h", {header, "stdbool.h"});
	}
	std::cout<<"done."<<std::endl;
}
