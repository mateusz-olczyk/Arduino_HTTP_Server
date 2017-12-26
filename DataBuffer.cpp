#include "DataBuffer.h"

void DataBuffer::add(DataObject obj) {
  if (current_size != MAX_SIZE) {
    data[current_size++] = obj;
  } else {
    data[data_begin++] = obj;
    if (data_begin == MAX_SIZE)
      data_begin = 0;
  }
}

void DataBuffer::print(EthernetClient & client) {
  for (int i = 0; i < current_size; i++) {
    int index = (data_begin + i) % MAX_SIZE;
    client.print("{ x: ");
    client.print(data[index].x);
    client.print(", y: ");
    client.print(data[index].y);
    client.print("}, ");
  }
}

