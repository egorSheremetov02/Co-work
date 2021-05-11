#ifndef MYTEST_H_
#define MYTEST_H_
#include <iostream>

extern int num_all_test, num_success_test;
extern bool failed;
namespace mytest {
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define CHECK(x) CHECK2(x, __LINE__, __FILE__)
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define CHECK_MESSAGE(x, message) CHECK_MESSAGE2(x, message, __LINE__, __FILE__)
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define CHECK2(x, line, file)                                           \
  [&]() {                                                               \
    if (!(x)) {                                                         \
      failed = true;                                                    \
      std::cerr << "CHECK(" << #x << ") at " << (file) << ":" << (line) \
                << " failed!\n";                                        \
    }                                                                   \
  }()
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define CHECK_MESSAGE2(x, message, line, file)                          \
  [&]() {                                                               \
    if (!(x)) {                                                         \
      failed = true;                                                    \
      std::cerr << "CHECK(" << #x << ") at " << (file) << ":" << (line) \
                << " failed!\n"                                         \
                << "    message: " << (message) << "\n";                \
    }                                                                   \
  }()
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define MYTEST_INTERNAL_TEST_CASE(class_name, name, file, line) \
  static void class_name();                                     \
  namespace {                                                   \
  struct MagicInit_##class_name {                               \
    MagicInit_##class_name() {                                  \
      std::cerr << "Running " << #name << "...\n";              \
      num_all_test++;                                           \
      num_success_test++;                                       \
      failed = false;                                           \
      class_name();                                             \
      if (failed)                                               \
        num_success_test--;                                     \
    }                                                           \
  } magic_init_##class_name;                                    \
  }                                                             \
  static void class_name()

// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define MYTEST_INTERNAL_TEST_CASE2(file, line, name) \
  MYTEST_INTERNAL_TEST_CASE(mytest_test_class_##line, name, file, line)

// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define MYTEST_INTERNAL_TEST_CASE1(file, line, name) \
  MYTEST_INTERNAL_TEST_CASE2(file, line, name)
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define TEST_CASE(name) MYTEST_INTERNAL_TEST_CASE1(__FILE__, __LINE__, name)

};  // namespace mytest
// namespace
#endif  // MYTEST_H_
