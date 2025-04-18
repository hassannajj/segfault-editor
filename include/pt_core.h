

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
  char *original; // original buffer
  char *add; // add buffer
  int add_cap;
  int add_len;
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

/*
 * Returns the content that is stitched together from the piece table
 */
char *pt_get_content(PieceTable *pt);

/*
 * Frees any dynamically allocated memory
 */
void pt_cleanup(PieceTable *pt, char *content);

/*
 * Prints the pieces with their respected buffers
 */
void pt_print(PieceTable *pt);


/*
 * Returns the length of the content
 */
int pt_content_len(PieceTable *pt);


#endif
