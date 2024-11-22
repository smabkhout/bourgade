#include "color.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_color_start() {
    assert(strcmp(color_start(BLACK), "\033[38;5;0m") == 0);
    assert(strcmp(color_start(WHITE), "\033[38;5;15m") == 0);
    assert(strcmp(color_start(ORANGE), "\033[38;5;202m") == 0);
    assert(strcmp(color_start(PINK), "\033[38;5;213m") == 0);
    assert(strcmp(color_start(VIOLET), "\033[38;5;129m") == 0);
    assert(strcmp(color_start(YELLOW), "\033[38;5;11m") == 0);
    assert(strcmp(color_start(BLUE), "\033[0;34m") == 0);
    assert(strcmp(color_start(BROWN), "\033[38;5;94m") == 0);
    assert(strcmp(color_start(GREEN), "\033[0;32m") == 0);
    assert(strcmp(color_start(RED), "\033[0;31m") == 0);
    assert(strcmp(color_start(SHINY), "\033[1;11m\033[5m") == 0);
    assert(strcmp(color_start(MAX_COLORS), "\033[0m") == 0);
}

void test_color_stop() {
    assert(strcmp(color_stop(), "\033[0m") == 0);
}

void test_color_to_string() {
    assert(strcmp(color_to_string(BLACK), "Black") == 0);
    assert(strcmp(color_to_string(WHITE), "White") == 0);
    assert(strcmp(color_to_string(ORANGE), "Orange") == 0);
    assert(strcmp(color_to_string(PINK), "Pink") == 0);
    assert(strcmp(color_to_string(VIOLET), "Violet") == 0);
    assert(strcmp(color_to_string(YELLOW), "Yellow") == 0);
    assert(strcmp(color_to_string(BLUE), "Blue") == 0);
    assert(strcmp(color_to_string(BROWN), "Brown") == 0);
    assert(strcmp(color_to_string(GREEN), "Green") == 0);
    assert(strcmp(color_to_string(RED), "Red") == 0);
    assert(strcmp(color_to_string(SHINY), "Shiny") == 0);
    assert(strcmp(color_to_string(MAX_COLORS), "UNKNOWN") == 0);
}

int main() {
    test_color_start();
    test_color_stop();
    test_color_to_string();

    printf("All color tests passed!\n");
    return 0;
}
