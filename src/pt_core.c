#include "pt_core.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "safe_memory.h"



static void ensure_add_capacity(PieceTable *pt, int required) {
  /* Contanates to add buffer, ensures there is enough space allocated (reallocs if not) */
  while (pt->add_len + required >= pt->add_cap) {
    /* Realloc add buffer */ 
    pt->add_cap = pt->add_cap * 2;
    pt->add = safe_realloc(pt->add, pt->add_cap);
  }
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



/////////////////////////////////////////////////////////////////////////////////////////////////////////



/* PUBLIC piece table functions */



PieceTable * pt_init(char *text, int add_cap) {
  size_t text_len = strlen(text);

  PieceTable *pt = safe_malloc(sizeof(PieceTable));
  pt->original = safe_malloc(text_len + 1);  

  strncpy(pt->original, text, text_len);
  
  pt->add_cap = add_cap;
  pt->add_len = 0;
  pt->add = safe_malloc(sizeof(char) * add_cap);
  pt->add[0] = '\0'; // null-terminate initally empty add buffer

  pt->piece_head = safe_malloc(sizeof(Piece));
  pt->piece_count = 1;

  pt->piece_head->type = Original;
  pt->piece_head->offset = 0;
  pt->piece_head->len = text_len;
  pt->piece_head->next = NULL;

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
*/
void pt_insert_text(PieceTable *pt, char *text, int insert_point) {
  /* Ensures insertion point is legal (between 0 and total_len) */
  int total_len = pt_content_len(pt);
  if (insert_point < 0 || insert_point > total_len) {
    fprintf(stderr, "Error: Insertion point %d is out of bounds [0, %d]\n", insert_point, total_len);
    return;
  }
  /* we need to figure out what the offset is in the add buffer before we append to the add buffer */
  int add_offset = pt->add_len;

  /* Appends new text to the add buffer */
  append_to_add_buffer(pt, text);

  /* Finds the piece in the piece table where the insertion point falls */
  int running_len = 0;
  Piece *curr = pt->piece_head;
  Piece *prev = NULL;

  int count = 0;
  while (curr != NULL) {

    if (curr->next == NULL && insert_point >= running_len + curr->len) {
      /* Append at the end of the file */
      int local_insert = curr->len;
      //printf("End of file, local_insert = %d\n", local_insert);
      insert_piece(pt, curr, prev, local_insert, text, add_offset);

      return;
    }
    else if (insert_point < running_len + curr->len) {
      /* Append in middle of file */
      //printf("\ninserting text into piece %d\n", count);
      int local_insert = insert_point - running_len;
      //printf("local_insert=%d\n", local_insert);
      insert_piece(pt, curr, prev, local_insert, text, add_offset);
      return;
    }


    running_len += curr->len;
    prev = curr;
    curr = curr->next;
    count ++;
  } 
}

char *pt_get_content(PieceTable *pt) {
  Piece *curr = pt->piece_head;
  // Calculate total length
  int total_len = pt_content_len(pt); 
  char *result = safe_malloc(total_len + 1);
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
  result[total_len] = '\0';
  return result;
}

void pt_cleanup(PieceTable *pt, char *content) {
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
  free(pt);
  free(content);
}

void pt_print(PieceTable *pt) {
  printf("\n--- PIECES ---\n");
  Piece *curr = pt->piece_head;
  int index = 0;
  while (curr != NULL) {
    const char *type_str = curr->type == Original ? "Original" : "Added";
    printf("Piece %d: [%s | offset=%d | len=%d] -> \"", index, type_str, curr->offset, curr->len);
    
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
}

int pt_content_len(PieceTable *pt) {
  int result = 0;
  Piece *curr = pt->piece_head;
  while (curr) {
    result += curr->len;
    curr = curr->next;
  }
  return result;
}

