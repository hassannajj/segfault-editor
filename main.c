#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "safe_memory.h"


// *************************************************************************************************************************

/* Unsure of how to use this, but might use it later
struct textEditor {
  char ** lines;
};
*/


 /* Modifiable global variables */
size_t max_lines = 256;
size_t num_lines = 0;


char ** realloc_more_lines(char ** content) {
  char** new_content = (char **)safe_realloc(content, max_lines * sizeof(char *));
  return new_content;
}

void free_lines(char ** content) {
  for (size_t i = 0; i < num_lines; i++) {
    free(content[i]);
    content[i] = NULL;
  }
}

void populate_content_from_file(FILE *file, char *** content_ptr) {
  char **content = *content_ptr;
  char * line = NULL;
  size_t len = 0; /* allocated size for line */
  int read; /* number of chars in each line */
  
  num_lines = 0; /* Reset global var num_lines before reading */
    while ((read = getline(&line, &len, file)) != -1) {  
    // Check if i is greater than max_lines
    if (num_lines >= max_lines) {
      /* increase lines if i is greater than max_lines */
      max_lines = max_lines + 256; /* increaes max_lines by 256 */
      *content_ptr = realloc_more_lines(content); // Update caller's pointer
      content = *content_ptr; // Update local pointer
    }

    content[num_lines] = (char * ) safe_malloc((read + 1) * sizeof(char)); /* Allocates enough memory + 1 for null term */ 
    strcpy(content[num_lines], line); /* Copies line from file into content */
    num_lines ++;

  }
  free(line); // Free getline's buffer
    

}

void read_content(char ** content) {
  for (size_t i = 0; i < num_lines; i++) {
    printf("%s", content[i]);
  }
}
int main(int argc, char *argv[]) {

  if (argc != 2) { /* Incorrect command syntax */
    fprintf(stderr, "Usage %s <filename>\n", argv[0]);
    return 1;
  }

  FILE *file = fopen(argv[1], "r");
  
  if (file == NULL) { /* File was not opened successfully */
    perror("Error opening file");
    return 1;
  }

  /* Allocate space for storing lines */
  char ** content = safe_malloc(max_lines * sizeof(char *)); // allocates space for a bunch of strings 
  if (!content) {
      fprintf(stderr, "Memory allocation failed\n");
      fclose(file);
      return 1;
  }

  /* Read file content */
  populate_content_from_file(file, &content); // Pass reference to pointer
  fclose(file); // Close file




  /* Display Content */
  read_content(content);


  /* Cleanup */
  free_lines(content); 
  free(content);
  num_lines = 0;
  max_lines = 256;

  //while (1) { // Try to put some sort of input
  //}
  return 0;        

}

