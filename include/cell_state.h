#ifndef CELL_STATE_H
#define CELL_STATE_H

typedef enum {
    STATE_DEAD,
    STATE_ALIVE
}cell_state_t;

#define CELL_ALIVE 'X'
#define CELL_DEAD '+'

char state_to_char(cell_state_t state);

cell_state_t char_to_state(char chr);

cell_state_t flip_state(cell_state_t state);

#endif