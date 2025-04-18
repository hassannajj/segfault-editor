#define UNITY_OUTPUT_COLOR
#define UNITY_INCLUDE_PRINT_COLOR
#include "unity.h"
#include "safe_memory.h" // if needed
#include "pt_core.h"

void setUp(void) {}
void tearDown(void) {}

void test_init_empty_text(void) {
  PieceTable *pt = pt_init("");
  TEST_ASSERT_NOT_NULL(pt);
  TEST_ASSERT_EQUAL_INT(1, pt->pieces_count);

  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("", result);
  pt_cleanup(pt, result);
}

void test_init_with_text(void) {
  PieceTable *pt = pt_init("Hello");
  TEST_ASSERT_NOT_NULL(pt);
  TEST_ASSERT_EQUAL_INT(1, pt->pieces_count);

  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  pt_cleanup(pt, result);
}


void test_insert_beginning(void) {
  PieceTable *pt = pt_init("ello");
  pt_insert_text(pt, "H", 0);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  pt_cleanup(pt, result);
}

void test_insert_middle(void) {
  PieceTable *pt = pt_init("Helo");
  pt_insert_text(pt, "l", 3);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  pt_cleanup(pt, result);

}

void test_insert_end(void) {
  PieceTable *pt = pt_init("Hell");
  pt_insert_text(pt, "o", 4);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  pt_cleanup(pt, result);
}

void test_insert_invalid_low_index(void) {
  PieceTable *pt = pt_init("Hello");
  pt_insert_text(pt, "X", -1);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  pt_cleanup(pt, result);
} 

void test_insert_invalid_high_index(void) {
  PieceTable *pt = pt_init("Hello");
  pt_insert_text(pt, "X", 6);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  pt_cleanup(pt, result);
} 

void test_multiple_insertions(void) {
  PieceTable *pt = pt_init("Heo");
  pt_insert_text(pt, "l", 2);
  pt_insert_text(pt, "l", 2);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  pt_cleanup(pt, result);
}

void test_insert_multichar_beginning(void) {
    PieceTable *pt = pt_init("World");
    pt_insert_text(pt, "Hello ", 0);
    char *result = pt_get_content(pt);
    TEST_ASSERT_EQUAL_STRING("Hello World", result);
    pt_cleanup(pt, result);
}

void test_insert_multichar_middle(void) {
    PieceTable *pt = pt_init("Hellorld");
    pt_insert_text(pt, "o W", 4);
    char *result = pt_get_content(pt);
    TEST_ASSERT_EQUAL_STRING("Hello World", result);
    pt_cleanup(pt, result);
}

void test_insert_multichar_end(void) {
    PieceTable *pt = pt_init("Hello");
    pt_insert_text(pt, " World", 5);
    char *result = pt_get_content(pt);
    TEST_ASSERT_EQUAL_STRING("Hello World", result);
    pt_cleanup(pt, result);
}



int main(void) {
  UNITY_BEGIN();

  // Initializations
  RUN_TEST(test_init_empty_text);
  RUN_TEST(test_init_with_text);

  // Test Single-char insertions
  RUN_TEST(test_insert_beginning);
  RUN_TEST(test_insert_middle);
  RUN_TEST(test_insert_end);
  RUN_TEST(test_multiple_insertions);

  // Multi-character insertions
  RUN_TEST(test_insert_multichar_beginning);
  RUN_TEST(test_insert_multichar_middle);
  RUN_TEST(test_insert_multichar_end);

  // Test Errors
  RUN_TEST(test_insert_invalid_low_index);
  RUN_TEST(test_insert_invalid_high_index);



  /*TODO:: test add_capacity (try to be able to control the add_cap const value) */

  return UNITY_END();
}

