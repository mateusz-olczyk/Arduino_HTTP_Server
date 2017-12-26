#pragma once

template <typename T, int SIZE>
class Buffer {
  
private:
  T buffer[SIZE];
  T * get_ptr = buffer;
  T * volatile put_ptr = buffer;

public:

  void put(T value) {
    *put_ptr++ = value;
    if (put_ptr - buffer >= SIZE)
      put_ptr = buffer;
  }

  T get() {
    T value = *get_ptr++;
    if (get_ptr - buffer >= SIZE)
      get_ptr = buffer;
    return value;
  }

  bool empty() {
    return get_ptr == put_ptr;
  }

};
