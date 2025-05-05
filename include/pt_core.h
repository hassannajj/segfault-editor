#include <stdbool.h>

#ifndef PT_CORE_H
#define PT_CORE_H


typedef enum {
  Original,
  Added
} PieceType;

typedef struct Piece {
  PieceType type;
  int offset;
  int len;
  struct Piece *next;
} Piece;

/*
We have capacity for the add buffer since we are changing it, but 
we dont have a cap for the original buffer since it is supposed 
to be IMMUTABLE
*/
typedef struct {
  Piece *piece_head;
  int piece_count;
  int content_len; // Number of chars in the content of pieces
                   
  char *original; // original buffer
  char *add; // add buffer
  int add_len;
  int add_cap; // Capacity of add buffer size, doubles when full

  int *line_starts; // array of where each line starts
  int num_lines;
  int num_lines_cap; // Capacity of number of lines, doubles when full
} PieceTable;

/*
 * Inializes and allocates the piece table struct 
 @param Text to inialize piece table with
 */
PieceTable * pt_init(char *text, int add_cap);

 /*
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
void pt_insert_text(PieceTable *pt, char *text, int insert_point);
void pt_insert_char(PieceTable *pt, char c, int index);

void pt_insert_text_at_YX(PieceTable *pt, char *text, int y, int x);
void pt_insert_char_at_YX(PieceTable *pt, char c, int y, int x);


/* Bounds checking and error printing
 * YX for line, cursor
 * i for raw point indexing 
 */
bool isBoundsValid_YX(PieceTable *pt, int y, int x);
bool isBoundsValid_i(PieceTable *pt, int i);


/*
 * Dynamically allocates content that is stitched together from the piece table
 * and returns it
 */
char *pt_get_content(PieceTable *pt);

/*
 * Frees any dynamically allocated memory in piece table
 */
void pt_cleanup(PieceTable *pt);

/*
 * Prints the pieces with their respected buffers
 */
void pt_print(PieceTable *pt);


/* Gets character at a specific RAW index */
char pt_get_char_at_i(PieceTable *pt, int i);

/* Gets character using line and cursor index */
char pt_get_char_at_YX(PieceTable *pt, int y, int x);

/* Gets length of specific line */
int pt_line_len(PieceTable *pt, int y);

#endif
