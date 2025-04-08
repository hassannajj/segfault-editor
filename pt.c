#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "safe_memory.h"

#define INITIAL_ADD_CAP 1024
#define INITIAL_MAX_PIECES 10


typedef enum {
  Original,
  Added
} PieceType;

typedef struct {
  PieceType type;
  int offset;
  int len;
} Piece;

typedef struct {
  Piece *pieces;
  int pieces_max;
  int pieces_count;

  char *original; // original buffer
  char *add; // add buffer
  int add_cap;
  int add_len;
} PieceTable;



int main() {
  char text[] = "Code editing.\nRedefined.\nFree. Runs everywhere.";

  PieceTable *pt = safe_malloc(sizeof(PieceTable));
  pt->original = safe_malloc(strlen(text) + 1);  

  strcpy(pt->original, text);
  
  pt->add_cap = INITIAL_ADD_CAP;
  pt->add_len = 0;
  pt->add = safe_malloc(sizeof(char) * INITIAL_ADD_CAP);

  pt->pieces = safe_malloc(sizeof(Piece) * INITIAL_MAX_PIECES);
  pt->pieces_max = INITIAL_MAX_PIECES;
  pt->pieces_count = 1;

  pt->pieces[0].type = Original;
  pt->pieces[0].offset = 0;
  pt->pieces[0].len = strlen(text);


  printf("%s\n", pt->original);
  printf("%d\n", pt->pieces[0].len);


  /* Clean up */
  free(pt->original);
  free(pt->add);
  free(pt->pieces);
  free(pt);
  return 0;
}
