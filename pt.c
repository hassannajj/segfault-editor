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


PieceTable * init_pt(char *text) {
  size_t text_len = strlen(text);

  PieceTable *pt = safe_malloc(sizeof(PieceTable));
  pt->original = safe_malloc(text_len + 1);  

  strncpy(pt->original, text, text_len);
  
  pt->add_cap = INITIAL_ADD_CAP;
  pt->add_len = 0;
  pt->add = safe_malloc(sizeof(char) * INITIAL_ADD_CAP);
  pt->add[0] = '\0'; // null-terminate initally empty add buffer

  pt->pieces = safe_malloc(sizeof(Piece) * INITIAL_MAX_PIECES);
  pt->pieces_max = INITIAL_MAX_PIECES;
  pt->pieces_count = 1;

  pt->pieces[0].type = Original;
  pt->pieces[0].offset = 0;
  pt->pieces[0].len = text_len;

  return pt;
}


void insert_text(PieceTable *pt, char *text, int pos) {
  size_t text_len = strlen(text);
  
  /* Contanates to add buffer, ensures there is enough space allocated (reallocs if not) */
  while (pt->add_len + text_len >= pt->add_cap) {
    /* Realloc add buffer */ 
    printf("add_cap went from %d", pt->add_cap);

    pt->add_cap = pt->add_cap * 2;
    pt->add = safe_realloc(pt->add, pt->add_cap);

    printf("to %d\n", pt->add_cap);
  }
  pt->add_len = pt->add_len + text_len;
  strncat(pt->add, text, text_len);

  /* Creates new piece */
  pt->pieces_count++;

  pt->pieces[pt->pieces_count-1].type = Added;
  pt->pieces[pt->pieces_count-1].offset = 0; // Unsure
  pt->pieces[pt->pieces_count-1].len = text_len;

}



int main() {
  char *text = "Code editing.\nRedefined.\nFree.Runs everywhere.";
  PieceTable *pt = init_pt(text);

  printf("\n\nOriginal Text:\n");
  printf("%s\n", pt->original);
  printf("len: %d\n", pt->pieces[0].len);
  printf("\n\nAdded Text:\n");
  printf("%s\n", pt->add);
  printf("len: %d\n", pt->pieces[1].len);
  
  insert_text(pt, "Open Source.", 25);

  /* Clean up */
  free(pt->original);
  free(pt->add);
  free(pt->pieces);
  free(pt);
  return 0;
}
