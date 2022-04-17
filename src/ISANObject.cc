#include "ISANObject.h"

ISANObject ISANObject::get(int idx, int pathNum)
{
  try
  {
    return *((this->items).at(idx));
  }
  catch(std::exception e)
  {
    std::cerr << "ISAN PATH ERROR: " << pathNum << std::endl;
    exit(-3);
  }
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
