#include <iostream>

template <typename T> class Vector2 {
public:
  T val;
  Vector2<T>() { std::cout << "Hello World!" << std::endl; }
};
