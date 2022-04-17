#include "ISANParser.h"

ISANToken::ISANToken(ISANTokenType type, std::string value, int startPos, int endPos)
{
  this->type = type;
  this->value = value;
  this->startPos = startPos;
  this->endPos = endPos;
}

ISANObject ParseFromTokens(std::vector<ISANToken> tokens)
{
  ISANObject obj;
  if(tokens.size() == 1)
  {
    switch(tokens.front().type)
    {
      case ISANTokenType::COMMA:
      case ISANTokenType::OPEN_BRACKET:
      case ISANTokenType::CLOSE_BRACKET:
      {
        std::cerr << "ISAN SYNTAX ERROR: " << tokens.front().startPos << std::endl;
        exit(-1);
				break;
      }
      case ISANTokenType::INT:
      {
        obj.type = ISANObjectType::INT;
        obj.value = tokens[0].value;
        return obj;
      }
      case ISANTokenType::STRING:
      {
        obj.type = ISANObjectType::STRING;
        obj.value = tokens[0].value;
        return obj;
      }
    }
  }
  if(tokens.size() == 2)
  {
    if(tokens.front().type == ISANTokenType::OPEN_BRACKET && tokens.back().type == ISANTokenType::CLOSE_BRACKET)
    {
      obj.type = ISANObjectType::ARRAY;
      return obj;
    }
    else
    {
      std::cerr << "ISAN SYNTAX ERROR: " << tokens[0].startPos << std::endl;
      exit(-1);
    }
  }
  if(tokens.size() > 2)
  {
    obj.type = ISANObjectType::ARRAY;
    if((tokens.front().type != ISANTokenType::OPEN_BRACKET) && (tokens.back().type != ISANTokenType::CLOSE_BRACKET))
    {
      std::cerr << "ISAN SYNTAX ERROR: " << tokens.front().startPos;
      exit(-3); 
    }
    // checking for repetitive commas
    for(auto i = 1; i < tokens.size()-2; i++)
    {
      if(tokens[i].type == ISANTokenType::COMMA && tokens[i+1].type == ISANTokenType::COMMA)
      {
        std::cerr << "ISAN SYNTAX ERROR: " << tokens[i].startPos;
        exit(-3);
      }
    }
    // remove commas
    tokens.erase(std::remove_if(tokens.begin(), tokens.end(),[](ISANToken &tok) 
		{
      return tok.type == ISANTokenType::COMMA;
    }), tokens.end());
    // checking for correct bracket sequence
    int counter = 0;
    for(auto i = 1; i < tokens.size()-1; i++)
    {
			switch (tokens[i].type)
			{
				case ISANTokenType::OPEN_BRACKET:
				{
					counter++;
					break;
				}
				case ISANTokenType::CLOSE_BRACKET:
				{
					counter--;
					break;
				}
			}
    }
    if(counter!=0)
    {
      std::cerr << "ISAN SYNTAX ERROR: 0" << std::endl;//todo 
      exit(-3);
    }
    // splitting and recursice parsing
    counter = 0;
    auto recStart = 1;
    for(auto i = 1; i < tokens.size()-1; i++)
    {
      switch (tokens[i].type)
			{
				case ISANTokenType::OPEN_BRACKET:
				{
					counter++;
					break;
				}
				case ISANTokenType::CLOSE_BRACKET:
				{
					counter--;
					break;
				}
			}
      if(counter == 0)
      {
        std::vector<ISANToken> tmpVec(tokens.cbegin() + recStart, tokens.cbegin() + i + 1);
        auto tmpObject = ParseFromTokens(tmpVec);
        obj.items.push_back(std::make_shared<ISANObject>(tmpObject));
        recStart = i+1;
      }
    }
  }
  return obj;
}
