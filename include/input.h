typedef enum {
  INPUT_INSERT_CHAR,
  INPUT_MOVE_LEFT,
  INPUT_MOVE_RIGHT,
  INPUT_MOVE_DOWN,
  INPUT_MOVE_UP,
  INPUT_DELETE_CHAR,
  INPUT_ENTER_CHAR,
  INPUT_SAVE,
  INPUT_QUIT,
  INPUT_UNKNOWN
} InputType;

typedef struct {
  InputType type;
  char data;
} InputResult;

/*
 * Reads the raw key and returns the interpreted result
 */
InputResult read_input(int c);


