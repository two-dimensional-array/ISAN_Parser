#include <iostream>
#include <string>
#include <vector>
#include "ISANParser.h"

const std::vector<std::string> split(const std::string& s, const char& c)
{
	std::string buff{""};
	std::vector<std::string> v;
	for(auto n:s)
	{
		if(n != c) buff+=n;
    else
    {
		  if(n == c && buff != "")
      {
        v.push_back(buff); buff = "";
      }
    }
	}
	if(buff != "")
  {
    v.push_back(buff);
  }
	return v;
}

int main(int argc, char ** argv)
{    
  std::string isanString;
  std::getline(std::cin, isanString);
  bool isTypeRequested = false;
  bool isValueRequested = false;
  std::vector<int> elementPath;
  std::vector<ISANToken> tokens;
  ISANObject parsedObject;
  if(argc != 2)
  {
      std::cerr << "ISAN COMMAND LINE ARGUMENTS ERROR("<< argc << " arguments provided but 2 required)";
      exit(-1);
  }
  //check elementPath validity
  std::string elPathString(argv[1]);
  std::vector<std::string> tempPath;
  switch(elPathString.back())
  {
    case 't':
    {
      isTypeRequested = true;
      break;
    }
    case 'v':
    {
      isValueRequested = true;
      break;
    }
    default:
    {
      std::cerr << "ISAN COMMAND LINE ARGUMENTS ERROR(no type/value specifier at the end of elementPath provided)";
      exit(-1);
    }
  }
  elPathString = elPathString.substr(0, elPathString.length()-1);
  tempPath = split(elPathString, '.');
  for(auto& chunk : tempPath)
  {
    try
    {
      auto num = std::stoi(chunk);
      if(num >= 0)
      {
        elementPath.push_back(num);
      }
      else
      {
        std::cerr << "ISAN COMMAND LINE ARGUMENTS ERROR(no negative ints in elementPath allowed)" << std::endl;
        exit(-1);
      }
    }
    catch(std::exception& e)
    {
      std::cerr << "ISAN COMMAND LINE ARGUMENTS ERROR:" << chunk << "is not an integer" << std::endl;
      exit(-1);
    }
  }
  // let's split source string into tokens
  size_t idx = 0;
  while(idx < isanString.length())
  {
    if(isanString[idx] == '[')
    {
      tokens.push_back({ISANTokenType::OPEN_BRACKET, "[", idx, idx});
      idx++;
      continue;
    }
    if(isanString[idx] == ']')
    {
      tokens.push_back({ISANTokenType::CLOSE_BRACKET, "]", idx, idx});
      idx++;
      continue;
    }
    if(isanString[idx] == ',')
    {
      tokens.push_back({ISANTokenType::COMMA, ",", idx, idx});
      idx++;
      continue;
    }
    if(isanString[idx] == '"')
    {
      auto j = idx + 1;
      for(; j < isanString.length() && isanString[j] != '"'; j++);
      if(j == isanString.length())
      {
        std::cerr << "ISAN SYNTAX ERROR: " << idx + 1 << "(Unbalanced or unexpected quote)" << std::endl;
        exit(-2);
      }
      else
      {
        tokens.push_back({ISANTokenType::STRING, isanString.substr(idx + 1, j - idx - 1), idx + 1, j-idx - 1});
        idx = j + 1;
        continue;
      }
    }
    // seeking for integer
    if(isdigit(isanString[idx]) || isanString[idx] == '-')
    {
      auto j = idx;
      if(isanString[j] == '-')
      {
        j++;
      }
      for(; j < isanString.length() && isdigit(isanString[j]); j++);
      tokens.push_back({ISANTokenType::INT, isanString.substr(idx, j - idx), idx, j - idx});
      idx = j;
      continue;
    }
    if(isspace(isanString[idx]))
    {
      idx++;
      continue;
    }
    // for symbols not catched above
    std::cerr << "ISAN SYNTAX ERROR: " << idx << "(unrecognized symbol: " << isanString[idx] << ")" << std::endl;
    exit(-2);
  }
  // parse from tokens
  parsedObject = ParseFromTokens(tokens);
  if(parsedObject.type != ISANObjectType::ARRAY)
  {
    std::cerr << "ISAN SYNTAX ERROR: 0(high level object is not an array" << std::endl;
    exit(-2);
  }
  //elementPath
  auto element = parsedObject;
  for(auto idx = 0; idx < elementPath.size(); idx++)
  {
    element = element.get(elementPath[idx], idx);
  }
  if(true == isTypeRequested)
  {
    switch(element.type)
    {
      case ISANObjectType::ARRAY:
      {
        std::cout << "array" << std::endl;
        break;
      }
      case ISANObjectType::INT:
      {
        std::cout << "int" << std::endl;
        break;
      }
      case ISANObjectType::STRING:
      {
        std::cout << "string" << std::endl;
        break;
      }
    }
  }
  if(true == isValueRequested)
  {
    element.printValue();
  }
  return 0;
}
