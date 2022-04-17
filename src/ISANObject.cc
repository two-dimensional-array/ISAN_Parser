#include "ISANObject.h"

ISANObject ISANObject::get(std::vector<int>& path)
{
  auto element = *this;
  for(auto idx = 0; idx < path.size(); idx++)
  {
    try
    {
      element = *(element.items.at(path.at(idx)));
    }
    catch(std::exception e)
    {
      std::cerr << "ISAN PATH ERROR: " << idx << std::endl;
      exit(-3);
    }
  }
  return element;
}

void ISANObject::printValue()
{
  if (type == ISANObjectType::ARRAY)
  {
    std::cout << "array" << std::endl;
  }
  else
  {
    std::cout << value << std::endl;
  }
}

void ISANObject::printType()
{
  switch (type)
  {
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
    case ISANObjectType::ARRAY:
    {
      std::cout << "array" << std::endl;
      break;
    }
  }
}
