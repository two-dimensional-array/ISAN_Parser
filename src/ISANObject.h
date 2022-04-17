#ifndef ISAN_OBJECT_H__
#define ISAN_OBJECT_H__

#include <iostream>
#include <vector>
#include <string>
#include <memory>

enum class ISANObjectType
{
  INT,
  STRING,
  ARRAY
};

class ISANObject
{
  public:
    ISANObjectType type;
    std::string value;
    std::vector<std::shared_ptr<ISANObject>> items;
    ISANObject get(std::vector<int>& path);
    void printValue();
    void printType();
};

#endif //ISAN_OBJECT_H__
