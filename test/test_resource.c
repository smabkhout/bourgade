#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../src/resource.h"

void test_resource_to_string() {
    printf("Testing resource_to_string...\n");
    assert(strcmp(resource_to_string(CORN), "Corn") == 0);
    assert(strcmp(resource_to_string(FISH), "Fish") == 0);
    assert(strcmp(resource_to_string(WOOD), "Wood") == 0);
    assert(strcmp(resource_to_string(STONE), "Stone") == 0);
    assert(strcmp(resource_to_string(GOLD), "Gold") == 0);
    assert(strcmp(resource_to_string(RUBY), "Ruby") == 0);
    assert(strcmp(resource_to_string(SILVER), "Silver") == 0);
    assert(strcmp(resource_to_string(SAPPHIRE), "Sapphire") == 0);
    assert(strcmp(resource_to_string(EMERALD), "Emerald") == 0);
    assert(strcmp(resource_to_string(NOTHING), "Nothing") == 0);
    assert(strcmp(resource_to_string(MAX_RESOURCES), "UNKNOWN") == 0);
    printf("resource_to_string passed!\n");
}

void test_resource_to_short_string() {
    printf("Testing resource_to_short_string...\n");
    assert(strcmp(resource_to_short_string(CORN), "C") == 0);
    assert(strcmp(resource_to_short_string(FISH), "F") == 0);
    assert(strcmp(resource_to_short_string(WOOD), "W") == 0);
    assert(strcmp(resource_to_short_string(STONE), "S") == 0);
    assert(strcmp(resource_to_short_string(GOLD), "G") == 0);
    assert(strcmp(resource_to_short_string(RUBY), "R") == 0);
    assert(strcmp(resource_to_short_string(SILVER), "V") == 0);
    assert(strcmp(resource_to_short_string(SAPPHIRE), "P") == 0);
    assert(strcmp(resource_to_short_string(EMERALD), "E") == 0);
    assert(strcmp(resource_to_short_string(NOTHING), "N") == 0);
    assert(strcmp(resource_to_short_string(MAX_RESOURCES), "U") == 0);
    printf("resource_to_short_string passed!\n");
}

void test_resource_color_start() {
    printf("Testing resource_color_start...\n");
    assert(strcmp(resource_color_start(CORN), "\033[38;5;11m") == 0);
    assert(strcmp(resource_color_start(FISH), "\033[0;34m") == 0);
    assert(strcmp(resource_color_start(WOOD), "\033[38;5;94m") == 0);
    assert(strcmp(resource_color_start(STONE), "\033[38;5;15m") == 0);
    assert(strcmp(resource_color_start(GOLD), "\033[38;5;11m") == 0);
    assert(strcmp(resource_color_start(RUBY), "\033[0;31m") == 0);
    assert(strcmp(resource_color_start(SILVER), "\033[38;5;0m") == 0);
    assert(strcmp(resource_color_start(SAPPHIRE), "\033[0;34m") == 0);
    assert(strcmp(resource_color_start(EMERALD), "\033[0;32m") == 0);
    assert(strcmp(resource_color_start(NOTHING), "\033[38;5;0m") == 0);
    printf("resource_color_start passed!\n");
}

void test_resource_operations() {
    printf("Testing resource operations...\n");
    unsigned int r1[NUM_RESOURCES] = {10, 5, 2, 1, 3, 7};
    unsigned int r2[NUM_RESOURCES] = {3, 2, 0, 1, 2, 4};
    unsigned int result[NUM_RESOURCES];
    resource_add(r1, r2, result);
    assert(result[CORN] == 7);
    assert(result[FISH] == 2);
    assert(result[WOOD] ==2);
    assert(result[STONE] == 5);
    assert(result[GOLD] == 11);
    resource_sub(r1, r2, result);
    assert(result[CORN] == 3);
    assert(result[FISH] == 2);
    assert(result[WOOD] == 0);
    assert(result[STONE] == 1);
    assert(result[GOLD] == 3);
    int less_result = resource_le_than(r1, r2);
    assert(less_result == 1);
    unsigned int r3[NUM_RESOURCES] = {1, 1, 1, 1, 1, 1};
    unsigned int r4[NUM_RESOURCES] = {1, 1, 1, 1, 1, 1};
    less_result = resource_le_than(r3, r4);
    assert(less_result == 1);
    printf("resource_operations passed!\n");
}

int main() {
    test_resource_to_string();
    test_resource_to_short_string();
    test_resource_color_start();
    test_resource_operations();

    printf("All tests passed successfully!\n");
    return 0;
}
