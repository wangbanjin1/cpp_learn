#include <iostream>

using namespace std;
class Obj {
public:
  Obj() { puts("Obj()"); }
  ~Obj() { puts("~Obj()"); }
};

void foo(int n) {
  Obj obj;
  if (n == 42) {
    throw "life, the universe and everthing";
  }
}

int main() {
  try {
    foo(41);
    foo(42);
  } catch (const char *s) {
    puts(s);
  }
  return 0;
}
