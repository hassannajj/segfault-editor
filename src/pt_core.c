#include "pt_core.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "safe_memory.h"



static void ensure_add_capacity(PieceTable *pt, int required) {
  /* Contanates to add buffer, ensures there is enough space allocated (reallocs if not) */
  while (pt->add_len + required >= pt->add_cap) {
    /* Realloc add buffer */ 
    pt->add_cap *= 2;
    pt->add = safe_realloc(pt->add, pt->add_cap);
  }
}

static void expand_num_lines_cap(PieceTable *pt) {
  /* Realloc lineStarts arr */ 
  pt->num_lines_cap *= 2; 
  pt->lineStarts = safe_realloc(pt->lineStarts, sizeof(unsigned int) * pt->num_lines_cap);
}


static void append_to_add_buffer(PieceTable *pt, char *text) {
  size_t text_len = strlen(text);
 
  ensure_add_capacity(pt, text_len);

  pt->add_len = pt->add_len + text_len;
  strncat(pt->add, text, text_len);
}

static void insert_piece(PieceTable *pt, Piece *curr, Piece *prev, int local_insert, const char *text, int add_offset) {

  Piece *left = NULL;
  if (local_insert > 0) {
    left = safe_malloc(sizeof(Piece));
    left->type = curr->type;
    left->offset = curr->offset;
    left->len = local_insert;
  }

  Piece *added = safe_malloc(sizeof(Piece));
  added->type = Added;
  added->offset = add_offset;
  added->len = strlen(text);
  
  Piece *right = NULL;
  if (local_insert < curr->len) {
    right = safe_malloc(sizeof(Piece));
    right->type = curr->type;
    right->offset = curr->offset + local_insert;
    right->len = curr->len - local_insert;
  }


  if (prev == NULL) {
    // Add the beginning
    pt->piece_head = left ? left : added;
  } else {
    prev->next = left ? left : added;
  }
  
  if (left) {
    left->next = added;
  }
  if (right) {
    added->next = right;
    right->next = curr->next;
  } else {
    added->next = curr->next;
  }
  
  free(curr); // free old piece
  pt->piece_count += (left ? 1 : 0) + (right ? 1 : 0);
}

// Gets the piece where the local delete index starts and manipulates piece table to delete the text

static void delete_between_pieces(PieceTable* pt, Piece *prev, Piece *left, int left_local_index, Piece *right, int right_local_index) {
  printf("same piece: %d\n", left==right);
  printf("prev exists: %d\n", prev!=NULL);
  Piece* next = right->next;
  printf("next exists: %d\n", next!=NULL);

  Piece* new_left = NULL;
  Piece* new_right = NULL;


  if (left_local_index > 0) {
    new_left = safe_malloc(sizeof(Piece));
    new_left->type = left->type;
    new_left->offset = left->offset;
    new_left->len = left_local_index;
  }
  if (right_local_index < right->len-1) {
    new_right = safe_malloc(sizeof(Piece));
    new_right->type = right->type;
    new_right->offset = right->offset + right_local_index + 1;
    new_right->len = right->len - right_local_index - 1;
  }
  Piece *new_chain = NULL;

  if (new_left && new_right) {
    new_left->next = new_right;
    new_right->next = next;
    new_chain = new_left;
  } else if (new_left) {
    new_left->next = next;
    new_chain = new_left;
  } else if (new_right) {
    new_right->next = next;
    new_chain = new_right;
  } else {
    new_chain = next;
  }

  if (prev) {
    prev->next = new_chain;
  } else {
    pt->piece_head = new_chain;
  }

  /* Free any deleted pieces between left & right */
  Piece* curr_deleted = left;
  Piece* curr_next = NULL;
  while (curr_deleted != right) {
    curr_next = curr_deleted->next;
    free(curr_deleted); 
    curr_deleted = curr_next;
  }
  free(right);
  pt->piece_count += (new_left ? 1 : 0) + (new_right ? 1 : 0) - 1;
}



/* TODO: add a outdated_start_line_index
 * Maybe, when doing one "edit" , instead of going through entire content again , make it so that you have an upgrade lines and if you insert 5 characters to line 4,
 * you just need to find where lineStarts arr starts at line 4 , and add 5 characters to each element after that line 4.
 */
void reset_lines(PieceTable *pt) {
    pt->lineStarts[0] = 0;
    pt->num_lines = 1;
    for (int i = 0; i < pt->content_len; i++) {
        if (pt_get_char_at_i(pt, i) == '\n') {
            if (pt->num_lines >= pt->num_lines_cap) {
                expand_num_lines_cap(pt);
            }
            pt->lineStarts[pt->num_lines] = i + 1;
            pt->num_lines++;
        }
    }
}

/*
 * TODO: When accessing an outdated line, update the lineStarts arr first
  */
static int lineStarts_index(PieceTable *pt, int i) {
  if (i < 0 || i >= pt->num_lines) {
    fprintf(stderr, "Error: LineStarts index %d is out of bounds [0, %d)\n", i, pt->num_lines);
    return -1;
  } 
  return pt->lineStarts[i];
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
///

/* PUBLIC piece table functions */

bool isBoundsValid_YX(PieceTable *pt, int y, int x) {
  if (y < 0 || y >= pt->num_lines) {
    // Line is out of bounds
    fprintf(stderr, "Error: Line %d is out of bounds [0, %d)\n", y, pt->num_lines); 
    return false;
  }
  int line_len = pt_line_len(pt, y);

  if (x < 0 || x >= line_len) {
    // Cursor is out of bounds
    fprintf(stderr, "Error: Cursor %d is out of bounds [0, %d)\n", x, line_len);
    return false;
  }
  return true;

}

bool isBoundsValid_i(PieceTable *pt, int i) {
  if (i < 0 || i > pt->content_len) {
    fprintf(stderr, "Error: Raw point %d is out of bounds [0, %d)\n", i, pt->content_len);
    return false;
  } 
  return true;
}


/* Initializes the values for the piece table */
PieceTable * pt_init(char *text, int add_cap) {
  size_t text_len = strlen(text);
  if (text_len == 0) {
    // No text
    text = "\n";
    text_len = 1;
  }
    

  PieceTable *pt = safe_malloc(sizeof(PieceTable));
  pt->original = safe_malloc(text_len + 1);  

  strncpy(pt->original, text, text_len);
  
  pt->add_cap = add_cap;
  pt->add_len = 0;
  pt->add = safe_malloc(sizeof(char) * add_cap);
  pt->add[0] = '\0'; // null-terminate initally empty add buffer

  pt->piece_head = safe_malloc(sizeof(Piece));
  pt->piece_count = 1;

  pt->content_len = text_len;

  pt->piece_head->type = Original;
  pt->piece_head->offset = 0;
  pt->piece_head->len = text_len;
  pt->piece_head->next = NULL;

  // lines
  int initial_num_lines = 1;
  for (size_t i = 0; i < text_len; i++) {
    if (text[i] == '\n') initial_num_lines++;
  }
  pt->lineStarts = safe_malloc(sizeof(unsigned int) * initial_num_lines);
  pt->num_lines = initial_num_lines;
  pt->num_lines_cap = initial_num_lines;

  /* since inserting text can shift the indices of where lines start, we reset the line starts */
  reset_lines(pt); // This is unoptimal for now, because it resets lines that haven't changed
  return pt;
}

 /*
  *
  *

 * The Insert Algorithm
- Checks insertion point is legal
- Append new text to the add buffer.
- Find the piece in the piece table where the insertion point falls.
- Split that piece into up to 3 (if at leftmost / rightmost of piece, it will only split into two pieces):
    Pieces:
    1. The content before the insertion
    2. A new piece for the inserted text
    3. The remaining part of the original piece
  then frees the old piece

  Also increments the content length
*/
void pt_insert_text(PieceTable *pt, char *text, int index) {
  /* Ensures insertion point is legal (between 0 and content_len) */
  if (!isBoundsValid_i(pt, index)) return; 
  /*
  if (index < 0 || index > pt->content_len) {
    fprintf(stderr, "Error: Insertion point %d is out of bounds [0, %d]\n", index, pt->content_len);
    return;
  }
  */
  /* we need to figure out what the offset is in the add buffer before we append to the add buffer */
  int add_offset = pt->add_len;

  /* Appends new text to the add buffer */
  append_to_add_buffer(pt, text);

  /* Finds the piece in the piece table where the insertion point falls */
  int global_index = 0;
  Piece *curr = pt->piece_head;
  Piece *prev = NULL;

  int local_insert = -1;
  while (curr != NULL) {

    if (curr->next == NULL && index >= global_index + curr->len) {
      /* Append at the end of the file */
      local_insert = curr->len;
      break;
    }
    else if (index < global_index + curr->len) {
      /* Append in middle of file */
      local_insert = index - global_index;
      break;
    }
    global_index += curr->len;
    prev = curr;
    curr = curr->next;
  } 
  if (local_insert == -1) return;

  /* Insert piece */
  insert_piece(pt, curr, prev, local_insert, text, add_offset);

  /* Increments the content length */
  pt->content_len += strlen(text);
  
  /* Sets line starts arr */
  reset_lines(pt);

  return;
}

void pt_insert_char(PieceTable *pt, char c, int index) {
  char s[2] = {c, '\0'};
  pt_insert_text(pt, s, index);
}

/* 
* Inserts the text at line y, cursor x
* Y corresponds to line number
* X corresponds to horizontal cursor
*/
void pt_insert_text_at_YX(PieceTable *pt, char *text, int y, int x) {
  int line_start = lineStarts_index(pt, y);
  pt_insert_text(pt, text, line_start + x);
}

/* 
* Inserts the the char at line y, cursor x
* Y corresponds to line number
* X corresponds to horizontal cursor
*/
void pt_insert_char_at_YX(PieceTable *pt, char c, int y, int x) {
  int line_start = lineStarts_index(pt, y);
  pt_insert_char(pt, c, line_start + x);
}


/* Delete Algorithm 
 * 1. figure out where left segment is
 * 2. Skip over middle segment (will get deleted)
 * 3. Figure out where right semment is
 * 4. Cut out the deleted part*/

void pt_delete_text(PieceTable *pt, int delete_start, int delete_size) {
  if (delete_size < 1) {
    // This nsures the size is greater or equal to 1
    fprintf(stderr, "Delete fail: delete_size less than 1\n");
    return;
  }
  if (!isBoundsValid_i(pt, delete_start)|| delete_start == pt->content_len) {
    // This ensures the beginning point of the delete is valid
    fprintf(stderr, "Delete fail: delete_start index out of bounds\n");
    return;
  } 
  int delete_end = delete_start + delete_size; // -1 ? 
  if (!isBoundsValid_i(pt, delete_end) || delete_end == pt->content_len)  {
    // This ensures the end point of the delete is valid
    fprintf(stderr, "Delete fail: calculated delete_end out of bounds\n");
    return; 
  }

  int global_index = 0; // running length
  Piece *curr = pt->piece_head;
  Piece *prev = NULL;

  bool found = false;
  Piece *left_piece = NULL;
  int left_local_index = 0;
  Piece *right_piece = NULL;
  int right_local_index = 0;
  

  // Finds the piece where left segment (delete_start) is 
  while (curr != NULL) {
   if (delete_start < global_index + curr->len) {
      found = true;
      left_local_index = delete_start - global_index; // index of left segment
      left_piece = curr; 

      // Iterate pieces until right segment (delete_end) is found 
      while (delete_end > global_index+curr->len) {
        global_index += curr->len;
        curr = curr->next;
      }
      right_local_index = delete_end - global_index - 1; //-1 because when size is 1, both left and right index point to the same char 
      right_piece = curr;
      break;
    } 
    
    // increment
    global_index += curr->len;
    prev = curr;
    curr = curr->next;
  }
  if (!found) {
    // Delete pieces were not found
    fprintf(stderr, "Deletion was unsuccessful\n");
    return;
  }
  printf("left_local_index: %d  left_piece->len:%d\nright_local_index: %d  right_piece->len:%d\n", left_local_index, left_piece->len, right_local_index, right_piece->len);

  delete_between_pieces(pt, prev, left_piece, left_local_index, right_piece, right_local_index);

  /* Decrement the content length */
  pt->content_len -= delete_size;
  
  /* Sets line starts arr */
  reset_lines(pt);
}

void pt_delete_char(PieceTable *pt, int index) {
  pt_delete_text(pt, index, 1);
}

void pt_delete_text_at_YX(PieceTable *pt, int y, int x, int length) {
  int line_start = lineStarts_index(pt, y);
  pt_delete_text(pt, line_start + x, length);
}

void pt_delete_char_at_YX(PieceTable *pt, int y, int x) {
  int line_start = lineStarts_index(pt, y);
  pt_delete_char(pt, line_start + x);

}


char *pt_get_content(PieceTable *pt) {
  Piece *curr = pt->piece_head;
  // Calculate total length
 
  /* Allocated on heap */
  char *result = safe_malloc(pt->content_len + 1);

  int result_pos = 0;
  /* Combine all substrings of pieces */
  curr = pt->piece_head;
  int count = 0;
  while (curr != NULL) {
    if (curr->type == 0) {
      /* Original */ 
      strncpy(result+result_pos, pt->original + curr->offset, curr->len);
    } else {
      /* Added */
      strncpy(result+result_pos, pt->add + curr->offset, curr->len);
    }
    result_pos += curr->len;
    curr = curr->next;
    count ++;
  }
  result[pt->content_len] = '\0';
  return result;
}

void pt_cleanup(PieceTable *pt) {
  /* Clean up */
  Piece *curr = pt->piece_head;
  Piece *next = NULL;
  int count = 0;
  while (curr != NULL) {
    next = curr->next; 
    free(curr);
    curr = next;
    count ++;
  }
  free(pt->original);
  free(pt->add);
  free(pt->lineStarts);
  free(pt);
}

void pt_print(PieceTable *pt) {
  printf("\n--- PIECES ---\n");
  Piece *curr = pt->piece_head;
  int index = 0;
  while (curr != NULL) {
    const char *type_str = curr->type == Original ? "Original" : "Added";
    printf("Piece %d: [%s | offset=%d | len=%d] -> \n\"", index, type_str, curr->offset, curr->len);
    // Print actual text content of the piece
    for (int i = 0; i < curr->len; i++) {
      char ch = (curr->type == Original) ? pt->original[curr->offset + i] : pt->add[curr->offset + i];
      putchar(ch);
    }
    printf("\"\n");

    curr = curr->next;
    index++;
  }
  printf("--- END ---\n\n");
  printf("--- ORIGINAL BUFFER --- \n");
  printf("%s\n", pt->original);
  printf("--- ADDED BUFFER --- \n");
  printf("%s\n", pt->add);
  printf("\n\n");

  printf("--- line starts --- \n");
  for (int i=0; i < pt->num_lines; i++) {
    printf("Line start %d: %d\n", i, pt->lineStarts[i]);
  }
  printf("\n\n");
}


char pt_get_char_at_i(PieceTable *pt, int i) {
  if (!isBoundsValid_i(pt, i)) {
    return '\0';
  }

  Piece *curr = pt->piece_head;
  int global_index = 0;
  while (curr) {
    if (i < global_index + curr->len) {
      int buffer_index = curr->offset + (i - global_index);
      if (curr->type == Original) {
        return pt->original[buffer_index];
      } else {
        return pt->add[buffer_index];
      }
    }
    global_index += curr->len;
    curr = curr->next;
  }
  /* Didn't return anything */  
  fprintf(stderr, "Error: Point %d is out of bounds [0, %d]\n", i, pt->content_len);
  return '\0';
}

/*
* Gets the length of the line, INCLUDING \n CHARACTERS
* Y corresponds to line number
*/
int pt_line_len(PieceTable *pt, int y) {
  if (y == pt->num_lines-1) {
    // Last line
    return pt->content_len - lineStarts_index(pt, y); 
  }
  return lineStarts_index(pt, y+1) - lineStarts_index(pt, y);
}


/*
* Gets the width of the line, NOT INCLUDING \n CHARACTERS
* For rendering reasons, since we do not render the \n character
* Y corresponds to line number
*/
int pt_line_width(PieceTable *pt, int y) {
  if (y == pt->num_lines-1) {
    // Last line
    return pt_line_len(pt, y);
  }
  return pt_line_len(pt, y) - 1;
}

int pt_num_lines(PieceTable *pt) {
  return pt->num_lines;
}


/* 
* This returns the char at line y, cursor x
* Y corresponds to line number
* X corresponds to horizontal cursor
*/
char pt_get_char_at_YX(PieceTable *pt, int y, int x) {
  if (!isBoundsValid_YX(pt, y, x)) {
    return '\0';
  }

  int line_start = lineStarts_index(pt, y);
  return pt_get_char_at_i(pt, line_start + x);
}


