#include <iostream>
#include <string>
#include <vector>
#include "ISANParser.h"

enum class ISANRequest
{
  VALUE,
  TYPE,
  OTHER
};

ISANRequest getRequestType(char& c)
{
  switch (c)
  {
    case 't':
    {
      return ISANRequest::TYPE;
    }
    case 'v':
    {
      return ISANRequest::VALUE;
    }  
    default:
    {
      std::cerr << "ISAN COMMAND LINE ARGUMENTS ERROR(no type/value specifier at the end of elementPath provided)";
      exit(-1);
      return ISANRequest::OTHER;
    }
  }
}

const std::vector<int> getElementPath(const std::string& s)
{
  std::string buff{""};
  std::vector<int> path;
  for(auto n : s)
  {
    if(n != '.')
    {
      buff += n;
    }
    else
    {
		  if(n == '.' && !buff.empty())
      {
        try
        {
          path.push_back(std::stoi(buff));
          if(path.back() < 0)
          {
            std::cerr << "ISAN COMMAND LINE ARGUMENTS ERROR(no negative ints in elementPath allowed)" << std::endl;
            exit(-1);
          }
        }
        catch(std::exception& e)
        {
          std::cerr << "ISAN COMMAND LINE ARGUMENTS ERROR:" << buff << "is not an integer" << std::endl;
          exit(-1);
        }
        buff.clear();
      }
    }
  }
  if(!buff.empty())
  {
    try
    {
      path.push_back(std::stoi(buff));
      if(path.back() < 0)
      {
        std::cerr << "ISAN COMMAND LINE ARGUMENTS ERROR(no negative ints in elementPath allowed)" << std::endl;
        exit(-1);
      }
    }
    catch(std::exception& e)
    {
      std::cerr << "ISAN COMMAND LINE ARGUMENTS ERROR:" << buff << "is not an integer" << std::endl;
      exit(-1);
    }
    buff.clear();
  }
  return path;
}

int main(int argc, char ** argv)
{    
  std::string isanString;
  std::getline(std::cin, isanString);
  ISANRequest requestType = ISANRequest::OTHER;
  std::vector<int> elementPath;
  std::vector<ISANToken> tokens;
  ISANObject parsedObject;
  if(argc != 2)
  {
    std::cerr << "ISAN COMMAND LINE ARGUMENTS ERROR("<< argc << " arguments provided but 2 required)";
    exit(-1);
  }
  // check elementPath validity
  std::string elPathString(argv[1]);
  requestType = getRequestType(elPathString.back());
  elPathString.pop_back();
  elementPath = getElementPath(elPathString);
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
  //choise element of elements by path
  auto element = parsedObject.get(elementPath);
  switch (requestType)
  {
    case ISANRequest::TYPE:
    {
      element.printType();
    }
    case ISANRequest::VALUE:
    {
      element.printValue();
    }
  }
  return 0;
}
