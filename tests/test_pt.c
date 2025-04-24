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
  pt_cleanup(pt);
  free(result);
}

void test_init_with_text(void) {
  PieceTable *pt = pt_init("Hello", INITIAL_ADD_CAP);
  TEST_ASSERT_NOT_NULL(pt);

  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  TEST_ASSERT_EQUAL_INT(1, pt->piece_count);
  pt_cleanup(pt);
  free(result);
}

void test_insert_beginning(void) {
  PieceTable *pt = pt_init("ello", INITIAL_ADD_CAP);
  pt_insert_text(pt, "H", 0);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  TEST_ASSERT_EQUAL_INT(2, pt->piece_count);
  pt_cleanup(pt);
  free(result);
}

void test_insert_middle(void) {
  PieceTable *pt = pt_init("Helo", INITIAL_ADD_CAP);
  pt_insert_text(pt, "l", 3);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  TEST_ASSERT_EQUAL_INT(3, pt->piece_count);
  pt_cleanup(pt);
  free(result);
}

void test_insert_end(void) {
  PieceTable *pt = pt_init("Hell", INITIAL_ADD_CAP);
  pt_insert_char(pt, 'o', 4);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  TEST_ASSERT_EQUAL_INT(2, pt->piece_count);
  pt_cleanup(pt);
  free(result);
}

void test_insert_invalid_low_index(void) {
  PieceTable *pt = pt_init("Hello", INITIAL_ADD_CAP);
  pt_insert_char(pt, 'X', -1);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  TEST_ASSERT_EQUAL_INT(1, pt->piece_count);
  pt_cleanup(pt);
  free(result);
} 

void test_insert_invalid_high_index(void) {
  PieceTable *pt = pt_init("Hello", INITIAL_ADD_CAP);
  pt_insert_char(pt, 'X', 6);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  TEST_ASSERT_EQUAL_INT(1, pt->piece_count);
  pt_cleanup(pt);
  free(result);
} 

void test_multiple_insertions(void) {
  PieceTable *pt = pt_init("Heo", INITIAL_ADD_CAP);
  pt_insert_char(pt, 'l', 2);
  pt_insert_char(pt, 'l', 2);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  TEST_ASSERT_EQUAL_INT(4, pt->piece_count);
  pt_cleanup(pt);
  free(result);
}

void test_insert_multichar_beginning(void) {
  PieceTable *pt = pt_init("World", INITIAL_ADD_CAP);
  pt_insert_text(pt, "Hello ", 0);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello World", result);
  TEST_ASSERT_EQUAL_INT(2, pt->piece_count);
  pt_cleanup(pt);
  free(result);

}

void test_insert_multichar_middle(void) {
  PieceTable *pt = pt_init("Hellorld", INITIAL_ADD_CAP);
  pt_insert_text(pt, "o W", 4);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello World", result);
  TEST_ASSERT_EQUAL_INT(3, pt->piece_count);
  pt_cleanup(pt);
  free(result);
}

void test_insert_multichar_end(void) {
  PieceTable *pt = pt_init("Hello", INITIAL_ADD_CAP);
  pt_insert_text(pt, " World", 5);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello World", result);
  TEST_ASSERT_EQUAL_INT(2, pt->piece_count);
  pt_cleanup(pt);
  free(result);
}

void test_add_cap_init(void) {
  // Initialization should add to the original buffer, not add buffer so add cap shouldn't change
  PieceTable *pt = pt_init("Hello", 1); 
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello", result);
  TEST_ASSERT_EQUAL_INT(1, pt->add_cap);
  pt_cleanup(pt);
  free(result);
}

void test_add_cap_insert(void) {
  PieceTable *pt = pt_init("Hello", 1);
  pt_insert_text(pt, " World", 5);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello World", result);
  TEST_ASSERT_EQUAL_INT(8, pt->add_cap);
  pt_cleanup(pt);
  free(result);
}

void test_len_init(void) {
  PieceTable *pt = pt_init("Hello", INITIAL_ADD_CAP);
  char *result = pt_get_content(pt);
  int len = pt_content_len(pt);
  TEST_ASSERT_EQUAL_INT(5, len);
  pt_cleanup(pt);
  free(result);
}

void test_len_insert(void) {
  PieceTable *pt = pt_init("Hello World", INITIAL_ADD_CAP);
  pt_insert_text(pt, " cool ", 5);
  char *result = pt_get_content(pt);
  int len = pt_content_len(pt);
  TEST_ASSERT_EQUAL_INT(17, len);
  pt_cleanup(pt);
  free(result);
}

void test_pt_get_char_at_single_insert(void) {
  PieceTable *pt = pt_init("", INITIAL_ADD_CAP);

  pt_insert_char(pt, 'H', 0);  // Insert 'H' at beginning

  TEST_ASSERT_EQUAL_CHAR('H', pt_get_char_at(pt, 0));
  TEST_ASSERT_EQUAL_CHAR('\0', pt_get_char_at(pt, 1)); // Out of bounds
  
  pt_cleanup(pt);
}

void test_pt_get_char_at_multiple_insert(void) {
  PieceTable *pt = pt_init("", INITIAL_ADD_CAP);

  pt_insert_char(pt, 'H', 0);
  pt_insert_char(pt, 'E', 1);
  pt_insert_char(pt, 'Y', 2);

  TEST_ASSERT_EQUAL_CHAR('H', pt_get_char_at(pt, 0));
  TEST_ASSERT_EQUAL_CHAR('E', pt_get_char_at(pt, 1));
  TEST_ASSERT_EQUAL_CHAR('Y', pt_get_char_at(pt, 2));
  TEST_ASSERT_EQUAL_CHAR('\0', pt_get_char_at(pt, 3)); // Beyond content
  
  pt_cleanup(pt);
}

void test_pt_get_char_at_with_initial_text_and_insert(void) {
  PieceTable *pt = pt_init("Goodbye", INITIAL_ADD_CAP);

  // Initial assertions from original buffer
  TEST_ASSERT_EQUAL_CHAR('G', pt_get_char_at(pt, 0));
  TEST_ASSERT_EQUAL_CHAR('o', pt_get_char_at(pt, 1));
  TEST_ASSERT_EQUAL_CHAR('o', pt_get_char_at(pt, 2));
  TEST_ASSERT_EQUAL_CHAR('d', pt_get_char_at(pt, 3));
  TEST_ASSERT_EQUAL_CHAR('b', pt_get_char_at(pt, 4));  // this will shift after insert

  // Insert 'X' between 'd' and 'b'
  pt_insert_char(pt, 'X', 4);

  // Validate post-insert buffer: "GoodXbye"
  TEST_ASSERT_EQUAL_CHAR('G', pt_get_char_at(pt, 0));
  TEST_ASSERT_EQUAL_CHAR('o', pt_get_char_at(pt, 1));
  TEST_ASSERT_EQUAL_CHAR('o', pt_get_char_at(pt, 2));
  TEST_ASSERT_EQUAL_CHAR('d', pt_get_char_at(pt, 3));
  TEST_ASSERT_EQUAL_CHAR('X', pt_get_char_at(pt, 4));
  TEST_ASSERT_EQUAL_CHAR('b', pt_get_char_at(pt, 5));
  TEST_ASSERT_EQUAL_CHAR('y', pt_get_char_at(pt, 6));
  TEST_ASSERT_EQUAL_CHAR('e', pt_get_char_at(pt, 7));
  TEST_ASSERT_EQUAL_CHAR('\0', pt_get_char_at(pt, 8));  // Out of bounds

  pt_cleanup(pt);

}


void test_complex_edit_sequence(void) {
  PieceTable *pt = pt_init("Programming", INITIAL_ADD_CAP);

  // Insert at beginning
  pt_insert_text(pt, "I love ", 0);  // "I love Programming"

  // Insert in the middle
  pt_insert_text(pt, " in C", 18);   // "I love Programming in C"

  // Insert at end
  pt_insert_text(pt, "!", pt_content_len(pt));  // "I love Programming in C!"

  // Insert in the middle again
  pt_insert_text(pt, " and Python", 23);  // "I love Programming in C and Python!"

  // Insert more at the beginning
  pt_insert_text(pt, "Honestly, ", 0);  // "Honestly, I love Programming in C and Python!"
  
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Honestly, I love Programming in C and Python!", result);

  // Expecting:
  // - 1 piece for original
  // - 5 pieces for each insertion (unless adjacent inserts are coalesced)
  TEST_ASSERT_TRUE(pt->piece_count >= 6);  // Minimum 6 pieces unless optimized

  TEST_ASSERT_EQUAL_INT(45, pt_content_len(pt));


  pt_cleanup(pt);
  free(result);
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

  // Test getting character at a specific index
  RUN_TEST(test_pt_get_char_at_single_insert);
  RUN_TEST(test_pt_get_char_at_multiple_insert); 
  RUN_TEST(test_pt_get_char_at_with_initial_text_and_insert);

  // Complex
  RUN_TEST(test_complex_edit_sequence); 

  return UNITY_END();
}

