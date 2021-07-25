#ifndef _wicked_test_h_
#define _wicked_test_h_

#include <vector>
#include <tuple>
#include <string>

#ifdef PRINT_DEBUG
#define TEST_DEBUG_PRINT(code)                                                 \
  {                                                                            \
    std::cout << "\n  ---DEBUG---" << std::endl;                               \
    code std::cout << "  ---DEBUG---" << std::endl;                            \
  }
#else
#define TEST_DEBUG_PRINT(code)                                                 \
  {}
#endif

enum TestResult { TestPass, TestFail, TestException };

using test_return_t = std::tuple<TestResult, bool, std::vector<std::string>>;

test_return_t make_return_t(TestResult result, bool pass,
                            const std::vector<const char *> &cstr_vec);

bool wicked_test(std::vector<std::tuple<TestResult, bool (*)(), const char *>>
                     test_functions);
bool wicked_test(
    std::vector<std::tuple<TestResult, bool, std::vector<std::string>> (*)()>
        test_functions);

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#endif // #ifndef _wicked_test_h_
