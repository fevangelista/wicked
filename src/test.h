#ifndef _wicked_test_h_
#define _wicked_test_h_

#include <vector>
#include <tuple>
#include <string>

enum TestResult { TestPass, TestFail, TestException };

bool wicked_test(std::vector<std::tuple<TestResult, bool (*)(), const char *>>
                     test_functions);

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#endif // #ifndef _wicked_test_h_
