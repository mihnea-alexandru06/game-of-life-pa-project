#include "cell_state.h"
#include <ctype.h>

char state_to_char(cell_state_t state) {
    return (state == STATE_ALIVE ? CELL_ALIVE : CELL_DEAD);
}

cell_state_t char_to_state(char chr) {
    return (chr == 'X' ? STATE_ALIVE : STATE_DEAD);
}

cell_state_t flip_state(cell_state_t state) {
    return (state == STATE_ALIVE) ? STATE_DEAD : STATE_ALIVE;
}