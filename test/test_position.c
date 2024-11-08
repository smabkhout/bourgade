#include <stdio.h>
#include "../src/position.h"

void test_make_position() {
    struct position_t* pos = make_position(2, 1);
    if (pos != NULL && position_x(pos) == 2 && position_y(pos) == 1) {
        printf("test_make_position passed\n");
    } else {
        printf("test_make_position failed\n");
    }
}

void test_invalid_position() {
    struct position_t* pos = make_invalid_position();
    if (pos != NULL && !is_valid_position(pos)) {
        printf("test_invalid_position passed\n");
    } else {
        printf("test_invalid_position failed\n");
    }
}

void test_position_to_string() {
    struct position_t* pos = make_position(3, 2);
    char buffer[MAX_POS_STR];
    position_to_string(pos, buffer);
    printf("Position string: %s\n", buffer);
}

int main() {
    test_make_position();
    test_invalid_position();
    test_position_to_string();
    return 0;
}
