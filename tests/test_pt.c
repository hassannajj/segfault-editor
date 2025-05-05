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
  TEST_ASSERT_EQUAL_INT(5, pt->content_len);
  pt_cleanup(pt);
  free(result);
}

void test_len_insert(void) {
  PieceTable *pt = pt_init("Hello World", INITIAL_ADD_CAP);
  pt_insert_text(pt, " cool ", 5);
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_INT(17, pt->content_len);
  pt_cleanup(pt);
  free(result);
}

void test_pt_get_char_at_single_insert(void) {
  PieceTable *pt = pt_init("", INITIAL_ADD_CAP);

  pt_insert_char(pt, 'H', 0);  // Insert 'H' at beginning

  TEST_ASSERT_EQUAL_CHAR('H', pt_get_char_at_i(pt, 0));
  TEST_ASSERT_EQUAL_CHAR('\0', pt_get_char_at_i(pt, 1)); // Out of bounds
  
  pt_cleanup(pt);
}

void test_pt_get_char_at_multiple_insert(void) {
  PieceTable *pt = pt_init("", INITIAL_ADD_CAP);

  pt_insert_char(pt, 'H', 0);
  pt_insert_char(pt, 'E', 1);
  pt_insert_char(pt, 'Y', 2);

  TEST_ASSERT_EQUAL_CHAR('H', pt_get_char_at_i(pt, 0));
  TEST_ASSERT_EQUAL_CHAR('E', pt_get_char_at_i(pt, 1));
  TEST_ASSERT_EQUAL_CHAR('Y', pt_get_char_at_i(pt, 2));
  TEST_ASSERT_EQUAL_CHAR('\0', pt_get_char_at_i(pt, 3)); // Beyond content
  
  pt_cleanup(pt);
}

void test_pt_get_char_at_with_initial_text_and_insert(void) {
  PieceTable *pt = pt_init("Goodbye", INITIAL_ADD_CAP);

  // Initial assertions from original buffer
  TEST_ASSERT_EQUAL_CHAR('G', pt_get_char_at_i(pt, 0));
  TEST_ASSERT_EQUAL_CHAR('o', pt_get_char_at_i(pt, 1));
  TEST_ASSERT_EQUAL_CHAR('o', pt_get_char_at_i(pt, 2));
  TEST_ASSERT_EQUAL_CHAR('d', pt_get_char_at_i(pt, 3));
  TEST_ASSERT_EQUAL_CHAR('b', pt_get_char_at_i(pt, 4));  // this will shift after insert

  // Insert 'X' between 'd' and 'b'
  pt_insert_char(pt, 'X', 4);

  // Validate post-insert buffer: "GoodXbye"
  TEST_ASSERT_EQUAL_CHAR('G', pt_get_char_at_i(pt, 0));
  TEST_ASSERT_EQUAL_CHAR('o', pt_get_char_at_i(pt, 1));
  TEST_ASSERT_EQUAL_CHAR('o', pt_get_char_at_i(pt, 2));
  TEST_ASSERT_EQUAL_CHAR('d', pt_get_char_at_i(pt, 3));
  TEST_ASSERT_EQUAL_CHAR('X', pt_get_char_at_i(pt, 4));
  TEST_ASSERT_EQUAL_CHAR('b', pt_get_char_at_i(pt, 5));
  TEST_ASSERT_EQUAL_CHAR('y', pt_get_char_at_i(pt, 6));
  TEST_ASSERT_EQUAL_CHAR('e', pt_get_char_at_i(pt, 7));
  TEST_ASSERT_EQUAL_CHAR('\0', pt_get_char_at_i(pt, 8));  // Out of bounds

  pt_cleanup(pt);

}


void test_complex_edit_sequence(void) {
  PieceTable *pt = pt_init("Programming", INITIAL_ADD_CAP);

  // Insert at beginning
  pt_insert_text(pt, "I love ", 0);  // "I love Programming"

  // Insert in the middle
  pt_insert_text(pt, " in C", 18);   // "I love Programming in C"

  // Insert at end
  pt_insert_text(pt, "!", pt->content_len); // "I love Programming in C!"

  // Insert in the middle again
  pt_insert_text(pt, " and Python", 23);  // "I love Programming in C and Python!"

  // Insert more at the beginning
  pt_insert_text(pt, "Honestly, ", 0);  // "Honestly, I love Programming in C and Python!"
  printf("content_len: %d", pt->content_len);
  
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Honestly, I love Programming in C and Python!", result);

  // Expecting:
  // - 1 piece for original
  // - 5 pieces for each insertion (unless adjacent inserts are coalesced)
  TEST_ASSERT_TRUE(pt->piece_count >= 6);  // Minimum 6 pieces unless optimized

  TEST_ASSERT_EQUAL_INT(45, pt->content_len);


  pt_cleanup(pt);
  free(result);
}

void test_lineStarts_init(void) {
  const char *text = "line1\nline2\nline3";
  PieceTable *pt = pt_init((char *)text, INITIAL_ADD_CAP);

  TEST_ASSERT_EQUAL_INT(3, pt->num_lines);  // 3 lines total
  TEST_ASSERT_EQUAL_INT(0, pt->lineStarts[0]);   // line1
  TEST_ASSERT_EQUAL_INT(6, pt->lineStarts[1]);   // line2 starts after '\n'
  TEST_ASSERT_EQUAL_INT(12, pt->lineStarts[2]);  // line3

  pt_cleanup(pt);
}

void test_lineStarts_after_insert_newline(void) {
  PieceTable *pt = pt_init("Hello\nWorld", INITIAL_ADD_CAP);
  pt_insert_text(pt, "\nNewLine", 5);  // insert newline in middle of line 0

  TEST_ASSERT_TRUE(pt->num_lines == 3);  // should now have at least 3 lines

  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello\nNewLine\nWorld", result);

  // Line starts:
  TEST_ASSERT_EQUAL_INT(0, pt->lineStarts[0]);        // "Hello"
  TEST_ASSERT_EQUAL_INT(6, pt->lineStarts[1]);        // "\nNewLine"
  TEST_ASSERT_EQUAL_INT(14, pt->lineStarts[2]);       // "\nWorld"

  pt_cleanup(pt);
  free(result);
}

void test_lineStarts_with_no_newlines(void) {
  PieceTable *pt = pt_init("SingleLine", INITIAL_ADD_CAP);
  TEST_ASSERT_EQUAL_INT(1, pt->num_lines);
  TEST_ASSERT_EQUAL_INT(0, pt->lineStarts[0]);

  pt_cleanup(pt);
}

void test_lineStarts_expand_cap(void) {
  // Force lineStarts to grow
  char big_text[2048];
  int count = 0;
  for (long unsigned int i = 0; i < sizeof(big_text) - 2; i++) {
    big_text[i] = (i % 10 == 9) ? '\n' : 'a';  // every 10th char is newline
    if (big_text[i] == '\n') count++;
  }
  big_text[sizeof(big_text) - 2] = 'Z';
  big_text[sizeof(big_text) - 1] = '\0';

  PieceTable *pt = pt_init("", INITIAL_ADD_CAP);
  pt_insert_text(pt, big_text, 0);
  pt_insert_text(pt, "hello", 1000);

  TEST_ASSERT_TRUE(pt->num_lines >= count + 1);
  TEST_ASSERT_TRUE(pt->lineStarts[100] == 1000);
  TEST_ASSERT_TRUE(pt->lineStarts[101] == 1015);
  TEST_ASSERT_TRUE(pt->num_lines_cap >= pt->num_lines);
  pt_cleanup(pt);
}

void test_get_char_at_YX_basic_access(void) {
  PieceTable *pt = pt_init("abc\ndef\nghi", INITIAL_ADD_CAP);
  TEST_ASSERT_EQUAL_CHAR('a', pt_get_char_at_YX(pt, 0, 0));
  TEST_ASSERT_EQUAL_CHAR('f', pt_get_char_at_YX(pt, 1, 2));
  TEST_ASSERT_EQUAL_CHAR('d', pt_get_char_at_YX(pt, 1, 0));
  TEST_ASSERT_EQUAL_CHAR('i', pt_get_char_at_YX(pt, 2, 2));
  pt_cleanup(pt);
}

void test_get_char_at_YX_out_of_bounds(void) {
  PieceTable *pt = pt_init("abc\ndef\nghi", INITIAL_ADD_CAP);
  // Valid access for comparison
  TEST_ASSERT_EQUAL_CHAR('a', pt_get_char_at_YX(pt, 0, 0));

  // x < 0
  TEST_ASSERT_EQUAL_CHAR('\0', pt_get_char_at_YX(pt, 0, -1));

  // x == line length (line 0 is "abc\n", len = 3, so it should return \n)
  TEST_ASSERT_EQUAL_CHAR('\n', pt_get_char_at_YX(pt, 0, 3));

  // x > line length (line 0 is "abc\n", len = 4)
  TEST_ASSERT_EQUAL_CHAR('\0', pt_get_char_at_YX(pt, 0, 4));

  // y < 0
  TEST_ASSERT_EQUAL_CHAR('\0', pt_get_char_at_YX(pt, -1, 0));

  // y >= num_lines
  TEST_ASSERT_EQUAL_CHAR('\0', pt_get_char_at_YX(pt, pt->num_lines, 0));

  // Empty line test (middle line is empty)
  PieceTable *pt2 = pt_init("line1\n\nline3", INITIAL_ADD_CAP);
  TEST_ASSERT_EQUAL_CHAR('\0', pt_get_char_at_YX(pt2, 1, 1));  // line 1 is empty
  pt_cleanup(pt);
  pt_cleanup(pt2);
}

void test_get_char_at_YX_after_insert(void) {
  PieceTable *pt = pt_init("foo\nbar\nbaz", INITIAL_ADD_CAP);
  // Initial check
  TEST_ASSERT_EQUAL_CHAR('b', pt_get_char_at_YX(pt, 1, 0)); // line 1 = "bar"
  // Insert a newline in the middle of "bar"
  pt_insert_text(pt, "\n", 5); // "foo\nb\nar\nbaz"
  // Now expect: 
  // line 0: "foo"
  // line 1: "b"
  // line 2: "ar"
  // line 3: "baz"

  TEST_ASSERT_EQUAL_CHAR('b', pt_get_char_at_YX(pt, 1, 0));
  TEST_ASSERT_EQUAL_CHAR('a', pt_get_char_at_YX(pt, 2, 0));
  TEST_ASSERT_EQUAL_CHAR('z', pt_get_char_at_YX(pt, 3, 2));
  pt_cleanup(pt);
}

void test_get_char_at_YX_edge_cases(void) {
// Starts with newline
  PieceTable *pt1 = pt_init("\nHello", INITIAL_ADD_CAP);
  TEST_ASSERT_EQUAL_CHAR('\n', pt_get_char_at_YX(pt1, 0, 0));
  TEST_ASSERT_EQUAL_CHAR('H', pt_get_char_at_YX(pt1, 1, 0));
  pt_cleanup(pt1);

  // File ends without newline
  PieceTable *pt2 = pt_init("LastLineNoNewline", INITIAL_ADD_CAP);

  TEST_ASSERT_EQUAL_CHAR('L', pt_get_char_at_YX(pt2, 0, 0));
  TEST_ASSERT_EQUAL_CHAR('e', pt_get_char_at_YX(pt2, 0, 16));
  TEST_ASSERT_EQUAL_CHAR('\0', pt_get_char_at_YX(pt2, 0, 17));
  TEST_ASSERT_EQUAL_CHAR('\0', pt_get_char_at_YX(pt2, 0, 100));  // Way too far
  pt_cleanup(pt2);

  // Only newlines
  PieceTable *pt3 = pt_init("\n\n\n", INITIAL_ADD_CAP);
  TEST_ASSERT_EQUAL_CHAR('\n', pt_get_char_at_YX(pt3, 0, 0));
  TEST_ASSERT_EQUAL_CHAR('\n', pt_get_char_at_YX(pt3, 1, 0));
  TEST_ASSERT_EQUAL_CHAR('\n', pt_get_char_at_YX(pt3, 2, 0));
  TEST_ASSERT_EQUAL_CHAR('\0', pt_get_char_at_YX(pt3, 2, 1));  // Beyond line end
  pt_cleanup(pt3);

  // Single line no newline
  PieceTable *pt4 = pt_init("SoloLine", INITIAL_ADD_CAP);
  TEST_ASSERT_EQUAL_CHAR('S', pt_get_char_at_YX(pt4, 0, 0));
  TEST_ASSERT_EQUAL_CHAR('\0', pt_get_char_at_YX(pt4, 0, 20));
  pt_cleanup(pt4);
}

void test_pt_line_width_various_layouts(void) {
  // "abc\n" (4), "defg\n" (5), "hi" (2)
  PieceTable *pt1 = pt_init("abc\ndefg\nhi", INITIAL_ADD_CAP);
  TEST_ASSERT_EQUAL_INT(4, pt_line_width(pt1, 0));
  TEST_ASSERT_EQUAL_INT(5, pt_line_width(pt1, 1));
  TEST_ASSERT_EQUAL_INT(2, pt_line_width(pt1, 2));
  pt_cleanup(pt1);

  // Single line
  PieceTable *pt2 = pt_init("SoloLine", INITIAL_ADD_CAP);
  TEST_ASSERT_EQUAL_INT(8, pt_line_width(pt2, 0));
  pt_cleanup(pt2);

  // Only newlines
  PieceTable *pt3 = pt_init("\n\n\n", INITIAL_ADD_CAP);
  TEST_ASSERT_EQUAL_INT(1, pt_line_width(pt3, 0));
  TEST_ASSERT_EQUAL_INT(1, pt_line_width(pt3, 1));
  TEST_ASSERT_EQUAL_INT(1, pt_line_width(pt3, 2)); 
  TEST_ASSERT_EQUAL_INT(0, pt_line_width(pt3, 3));  // last line: empty
  
  pt_cleanup(pt3);
}

void test_pt_line_width_after_insert(void) {
  PieceTable *pt = pt_init("Hello\nWorld", INITIAL_ADD_CAP);
  // "Hello\n" (6), "World" (5)
  TEST_ASSERT_EQUAL_INT(6, pt_line_width(pt, 0));
  TEST_ASSERT_EQUAL_INT(5, pt_line_width(pt, 1));

  pt_insert_text(pt, "\n!", 5);  // Insert newline + "!" inside first line

  // Now expect:
  // "Hello\n" (5) → line 0
  // "!\n" (2)   → line 1
  // "World"     → line 2

  TEST_ASSERT_EQUAL_INT(6, pt_line_width(pt, 0));
  TEST_ASSERT_EQUAL_INT(2, pt_line_width(pt, 1));
  TEST_ASSERT_EQUAL_INT(5, pt_line_width(pt, 2));

  pt_cleanup(pt);
}

void test_insert_text_at_YX_basic_cases(void) {
  PieceTable *pt = pt_init("Hello\nWorld", INITIAL_ADD_CAP);

  // Insert "!!" in middle of "World"
  pt_insert_text_at_YX(pt, "!!", 1, 3);  // "Wor!!ld"
  char *result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello\nWor!!ld", result);
  free(result);

  // Insert "Start " at beginning of "Wor!!ld"
  pt_insert_text_at_YX(pt, "Start ", 1, 0);  // "Start Wor!!ld"
  result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello\nStart Wor!!ld", result);
  free(result);

  // Insert "!" at end of "Hello"
  pt_insert_char_at_YX(pt, '!', 0, 5);
  result = pt_get_content(pt);
  TEST_ASSERT_EQUAL_STRING("Hello!\nStart Wor!!ld", result);
  free(result);
  pt_cleanup(pt);
}

void test_insert_text_at_YX_edge_cases(void) {
  PieceTable *pt1 = pt_init("One\nTwo", INITIAL_ADD_CAP);

  // Line out of bounds
  pt_insert_text_at_YX(pt1, "Fail", 5, 0);
  char *result = pt_get_content(pt1);
  TEST_ASSERT_EQUAL_STRING("One\nTwo", result);
  free(result);

  pt_cleanup(pt1);

  PieceTable *pt2 = pt_init("Short", INITIAL_ADD_CAP);

  // Col too wide
  pt_insert_char_at_YX(pt2, 'X', 0, 5);
  result = pt_get_content(pt2);
  TEST_ASSERT_EQUAL_STRING("Short", result);
  free(result);

  pt_cleanup(pt2);
}

void test_bounds_valid_yx() {
  PieceTable *pt = pt_init("line1\nline2\nline3", 100);
  
  // Valid YX
  TEST_ASSERT_TRUE(isBoundsValid_YX(pt, 0, 2));  // "l i n e 1"
  TEST_ASSERT_TRUE(isBoundsValid_YX(pt, 1, 0));  // "l"
  
  // Edge case: last char of last line
  int last_line = pt->num_lines - 1;
  TEST_ASSERT_TRUE(isBoundsValid_YX(pt, last_line, pt_line_width(pt, last_line) - 1));
  
  // Out-of-bounds Y
  TEST_ASSERT_FALSE(isBoundsValid_YX(pt, -1, 0));
  TEST_ASSERT_FALSE(isBoundsValid_YX(pt, pt->num_lines, 0));

  // Out-of-bounds X
  TEST_ASSERT_FALSE(isBoundsValid_YX(pt, 0, -1));
  TEST_ASSERT_FALSE(isBoundsValid_YX(pt, 0, pt_line_width(pt, 0)));

  pt_cleanup(pt);
}

void test_bounds_valid_i() {
  PieceTable *pt = pt_init("abc\ndef", 100);

  // Valid indices
  for (int i = 0; i < pt->content_len; i++) {
    TEST_ASSERT_TRUE(isBoundsValid_i(pt, i));
  }

  // Out-of-bounds
  TEST_ASSERT_FALSE(isBoundsValid_i(pt, -1));
  TEST_ASSERT_FALSE(isBoundsValid_i(pt, pt->content_len));

  pt_cleanup(pt);
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

  // Line starts arr
  RUN_TEST(test_lineStarts_init);
  RUN_TEST(test_lineStarts_after_insert_newline);
  RUN_TEST(test_lineStarts_with_no_newlines);
  RUN_TEST(test_lineStarts_expand_cap);

  // Get char at YX
  RUN_TEST(test_get_char_at_YX_basic_access);
  RUN_TEST(test_get_char_at_YX_out_of_bounds);
  RUN_TEST(test_get_char_at_YX_after_insert);
  RUN_TEST(test_get_char_at_YX_edge_cases);
  
  /* Line length */
  RUN_TEST(test_pt_line_width_various_layouts);
  RUN_TEST(test_pt_line_width_after_insert);
  
  /* Insert Text/char at YX */
  RUN_TEST(test_insert_text_at_YX_basic_cases);
  RUN_TEST(test_insert_text_at_YX_edge_cases);

  /* Bounds checking */
  RUN_TEST(test_bounds_valid_yx);
  RUN_TEST(test_bounds_valid_i);

  return UNITY_END();
}

