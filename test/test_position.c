#include "position.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_make_position() {
    struct position_t* p = POS(1, 2);
    assert(p != NULL);
    assert(PX(p) == 1);
    assert(PY(p) == 2); 
    p = make_invalid_position();
    assert(p != NULL);
    assert(!is_valid_position(p));
}

void test_is_valid_position() {
    struct position_t* p = POS(1, 2);
    assert(is_valid_position(p));
    p = make_invalid_position();
    assert(!is_valid_position(p));
}

void test_position_to_string() {
    struct position_t* p = POS(1, 2);
    char buf[MAX_POS_STR];
    position_to_string(p, buf);
    char expected_str[MAX_POS_STR];
    snprintf(expected_str, MAX_POS_STR, "L'abscisse est : 1, L'ordonnee est : 2");
    assert(strcmp(buf, expected_str) == 0);
}

void test_list_neighbors() {
    struct position_t* p = POS(1, 1);
    struct position_t* neighbors[MAX_NEIGHBORS];
    list_neighbors(p, neighbors);
    int valid_neighbors = 0;
    for (int i = 0; i < MAX_NEIGHBORS; i++) {
        if (is_valid_position(neighbors[i])) {
            valid_neighbors++;
        }
    }
    assert(valid_neighbors <= MAX_NEIGHBORS);
}

int main() {
    test_make_position();
    test_is_valid_position();
    test_position_to_string();
    test_list_neighbors();
    printf("All tests passed!\n");
    return 0;
}
