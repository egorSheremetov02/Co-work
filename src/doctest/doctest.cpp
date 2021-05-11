#include "doctest.h"
int num_all_test, num_success_test;
bool failed;

//#include <iostream>

int main() {
  std::cerr << "===== Tests passed: " << num_success_test << "/" << num_all_test
            << " =====\n";
  if (num_success_test != num_all_test) {
    return 1;
  }
}
