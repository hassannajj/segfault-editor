#include "unity.h"
#include "safe_memory.h" // if needed
#include "pt_core.h"

#define INITIAL_ADD_CAP 1024

void setUp(void) {}
void tearDown(void) {}

void test_init_empty_text(void) {
  PieceTable *pt = pt_init("", INITIAL_ADD_CAP);
  TEST_ASSERT_NOT_NULL(pt);
  
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("", result);
  TEST_ASSERT_EQUAL_INT(1, pt->piece_count);
  pt_cleanup(pt, result);
}

void test_init_with_text(void) {
  PieceTable *pt = pt_init("Hello", INITIAL_ADD_CAP);
  TEST_ASSERT_NOT_NULL(pt);

  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  TEST_ASSERT_EQUAL_INT(1, pt->piece_count);
  pt_cleanup(pt, result);
}

void test_insert_beginning(void) {
  PieceTable *pt = pt_init("ello", INITIAL_ADD_CAP);
  pt_insert_text(pt, "H", 0);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  TEST_ASSERT_EQUAL_INT(2, pt->piece_count);
  pt_cleanup(pt, result);
}

void test_insert_middle(void) {
  PieceTable *pt = pt_init("Helo", INITIAL_ADD_CAP);
  pt_insert_text(pt, "l", 3);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  TEST_ASSERT_EQUAL_INT(3, pt->piece_count);
  pt_cleanup(pt, result);
}

void test_insert_end(void) {
  PieceTable *pt = pt_init("Hell", INITIAL_ADD_CAP);
  pt_insert_text(pt, "o", 4);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  TEST_ASSERT_EQUAL_INT(2, pt->piece_count);
  pt_cleanup(pt, result);
}

void test_insert_invalid_low_index(void) {
  PieceTable *pt = pt_init("Hello", INITIAL_ADD_CAP);
  pt_insert_text(pt, "X", -1);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  TEST_ASSERT_EQUAL_INT(1, pt->piece_count);
  pt_cleanup(pt, result);
} 

void test_insert_invalid_high_index(void) {
  PieceTable *pt = pt_init("Hello", INITIAL_ADD_CAP);
  pt_insert_text(pt, "X", 6);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  TEST_ASSERT_EQUAL_INT(1, pt->piece_count);
  pt_cleanup(pt, result);
} 

void test_multiple_insertions(void) {
  PieceTable *pt = pt_init("Heo", INITIAL_ADD_CAP);
  pt_insert_text(pt, "l", 2);
  pt_insert_text(pt, "l", 2);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  TEST_ASSERT_EQUAL_INT(4, pt->piece_count);
  pt_cleanup(pt, result);
}

void test_insert_multichar_beginning(void) {
  PieceTable *pt = pt_init("World", INITIAL_ADD_CAP);
  pt_insert_text(pt, "Hello ", 0);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello World", result);
  TEST_ASSERT_EQUAL_INT(2, pt->piece_count);
  pt_cleanup(pt, result);
}

void test_insert_multichar_middle(void) {
  PieceTable *pt = pt_init("Hellorld", INITIAL_ADD_CAP);
  pt_insert_text(pt, "o W", 4);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello World", result);
  TEST_ASSERT_EQUAL_INT(3, pt->piece_count);
  pt_cleanup(pt, result);
}

void test_insert_multichar_end(void) {
  PieceTable *pt = pt_init("Hello", INITIAL_ADD_CAP);
  pt_insert_text(pt, " World", 5);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello World", result);
  TEST_ASSERT_EQUAL_INT(2, pt->piece_count);
  pt_cleanup(pt, result);
}

void test_add_cap_init(void) {
  // Initialization should add to the original buffer, not add buffer so add cap shouldn't change
  PieceTable *pt = pt_init("Hello", 1); 
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  TEST_ASSERT_EQUAL_INT(1, pt->add_cap);
  pt_cleanup(pt, result);
}

void test_add_cap_insert(void) {
  PieceTable *pt = pt_init("Hello", 1);
  pt_insert_text(pt, " World", 5);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello World", result);
  TEST_ASSERT_EQUAL_INT(8, pt->add_cap);
  pt_cleanup(pt, result);
}

void test_len_init(void) {
  PieceTable *pt = pt_init("Hello", INITIAL_ADD_CAP);
  char *result = pt_get_content(pt);
  int len = pt_len(pt);
  TEST_ASSERT_EQUAL_INT(5, len);
  pt_cleanup(pt, result);
}

void test_len_insert(void) {
  PieceTable *pt = pt_init("Hello World", INITIAL_ADD_CAP);
  pt_insert_text(pt, " cool ", 5);
  char *result = pt_get_content(pt);
  int len = pt_len(pt);
  TEST_ASSERT_EQUAL_INT(17, len);
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

  // Test add capacity resizing
  RUN_TEST(test_add_cap_init);
  RUN_TEST(test_add_cap_insert);

  // Test len
  RUN_TEST(test_len_init); 
  RUN_TEST(test_len_insert); 
  //RUN_TEST(test_len_complex); 

  return UNITY_END();
}

