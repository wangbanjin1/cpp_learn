#include "../common/shape.h"
#include <iostream>
class shape_wrapper {
public:
  explicit shape_wrapper(shape *ptr = nullptr) : ptr_(ptr) {}
  ~shape_wrapper(){delete ptr_} shape *get() const { return ptr_; }

private:
  shape *ptr_;
};
