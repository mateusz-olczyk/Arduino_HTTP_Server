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

/*
 * Formula:
 * a = SUM( (x_i - avg_x)(y_i - avg_y) )
 * b = SUM( (x_i - avg_x)^2 )
 * result = a/b
 */
double DataBuffer::linearRegression() {
  // Special case
  if (current_size < 2)
    return 0;

  // Compute averages
  double avg_x = 0, avg_y = 0;
  for (int i = 0; i < current_size; ++i) {
    avg_x += data[i].x;
    avg_y += data[i].y;
  }
  avg_x /= current_size;
  avg_y /= current_size;

  // Compute sums
  double a = 0, b = 0;
  for (int i = 0; i < current_size; ++i) {
    a += (data[i].x - avg_x)*(data[i].y - avg_y);
    b += (data[i].x - avg_x)*(data[i].x - avg_x);
  }

  // Result
  return a/b;
}

void DataBuffer::print(EthernetClient & client) {
  double currentTime = (double) millis() / 1000;
  for (int i = 0; i < current_size; i++) {
    int index = (data_begin + i) % MAX_SIZE;
    client.print("{ x: ");
    client.print(currentTime - data[index].x);
    client.print(", y: ");
    client.print(data[index].y);
    client.print("}, ");
  }
}

