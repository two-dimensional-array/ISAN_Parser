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
  TokensFromString(isanString, tokens);
  // parse from tokens
  parsedObject = ParseFromTokens(tokens);
  if(parsedObject.type != ISANObjectType::ARRAY)
  {
    std::cerr << "ISAN SYNTAX ERROR: 0(high level object is not an array" << std::endl;
    exit(-2);
  }
  // choise element of elements by path
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
