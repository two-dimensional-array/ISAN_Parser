#ifndef ISAN_PARSER_H__
#define ISAN_PARSER_H__

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "ISANObject.h"

enum class ISANTokenType
{
  OPEN_BRACKET,
  CLOSE_BRACKET,
  INT,
  STRING,
  COMMA,
  OTHER
};

class ISANToken
{   
  public:
		ISANTokenType type{ISANTokenType::OTHER};
    std::string value;
    int startPos;
    int endPos;
    ISANToken(ISANTokenType typ, std::string val, int start, int end);
};

ISANObject ParseFromTokens(std::vector<ISANToken> tokens);
void TokensFromString(const std::string& source, std::vector<ISANToken>& tokens);

#endif // ISAN_PARSER_H__
