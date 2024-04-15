#include "array.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "CUnit/Basic.h"
#include "CUnit/CUnit.h"

#define CU_TEST_INFO(test_func) \
  { #test_func, test_func }

int noop(void){return 0;}


static void Array_Create_Success() {
  Array *array = NULL;
  ResultCode resultCode = Array_Create(PIntComparator, sizeof(int), &array);
  if (resultCode == kSuccess) {
    printf("PASS %c", resultCode);
  } else {
    printf("FAIL %c", resultCode);
  }
}

static void Array_Create_inits_values() {
  Array *array = NULL;
  ResultCode result_code = Array_Create(PIntComparator, sizeof(int), &array);
  CU_ASSERT_EQUAL(result_code, kSuccess);
}

int RegisterArrayTests(){
  CU_TestInfo Array_tests[] = {CU_TEST_INFO(Array_Create_Success),
                               CU_TEST_INFO(Array_Create_inits_values),
                               CU_TEST_INFO_NULL
};

CU_SuiteInfo suites[] = {{.pName = "Array",
                            .pInitFunc = noop,
                            .pCleanupFunc = noop,
                            .pTests = Array_tests},
                           CU_SUITE_INFO_NULL};

return CU_register_suites(suites);
}