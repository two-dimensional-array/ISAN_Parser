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
