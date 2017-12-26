#pragma once

#include <UIPEthernet.h>

struct DataObject {
  double x;
  double y;
  DataObject() {}
  DataObject(double x, double y) {
    this->x = x;
    this->y = y;
  }
};

class DataBuffer {

private:
  static const int MAX_SIZE = 50;
  int current_size = 0;
  int data_begin = 0;
  DataObject data[MAX_SIZE];

public:
  void add(DataObject);
  void print(EthernetClient &);

};

