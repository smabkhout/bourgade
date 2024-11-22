#include "resource.h"
#include "color.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_resource_to_string() {
    assert(strcmp(resource_to_string(NOTHING), "Nothing") == 0);
    assert(strcmp(resource_to_string(CORN), "Corn") == 0);
    assert(strcmp(resource_to_string(FISH), "Fish") == 0);
    assert(strcmp(resource_to_string(WOOD), "Wood") == 0);
    assert(strcmp(resource_to_string(STONE), "Stone") == 0);
    assert(strcmp(resource_to_string(GOLD), "Gold") == 0);
    assert(strcmp(resource_to_string(RUBY), "Ruby") == 0);
    assert(strcmp(resource_to_string(SILVER), "Silver") == 0);
    assert(strcmp(resource_to_string(SAPPHIRE), "Sapphire") == 0);
    assert(strcmp(resource_to_string(EMERALD), "Emerald") == 0);
    assert(strcmp(resource_to_string(MAX_RESOURCES), "UNKNOWN") == 0);
}

void test_resource_to_short_string() {
    assert(strcmp(resource_to_short_string(NOTHING), "N") == 0);
    assert(strcmp(resource_to_short_string(CORN), "C") == 0);
    assert(strcmp(resource_to_short_string(FISH), "F") == 0);
    assert(strcmp(resource_to_short_string(WOOD), "W") == 0);
    assert(strcmp(resource_to_short_string(STONE), "S") == 0);
    assert(strcmp(resource_to_short_string(GOLD), "G") == 0);
    assert(strcmp(resource_to_short_string(RUBY), "R") == 0);
    assert(strcmp(resource_to_short_string(SILVER), "V") == 0);
    assert(strcmp(resource_to_short_string(SAPPHIRE), "P") == 0);
    assert(strcmp(resource_to_short_string(EMERALD), "E") == 0);
    assert(strcmp(resource_to_short_string(MAX_RESOURCES), "U") == 0);
}

void test_resource_color_start() {
    assert(strcmp(resource_color_start(NOTHING), color_start(BLACK)) == 0);
    assert(strcmp(resource_color_start(CORN), color_start(YELLOW)) == 0);
    assert(strcmp(resource_color_start(FISH), color_start(BLUE)) == 0);
    assert(strcmp(resource_color_start(WOOD), color_start(BROWN)) == 0);
    assert(strcmp(resource_color_start(STONE), color_start(WHITE)) == 0);
    assert(strcmp(resource_color_start(GOLD), color_start(YELLOW)) == 0);
    assert(strcmp(resource_color_start(RUBY), color_start(RED)) == 0);
    assert(strcmp(resource_color_start(SILVER), color_start(BLACK)) == 0);
    assert(strcmp(resource_color_start(SAPPHIRE), color_start(BLUE)) == 0);
    assert(strcmp(resource_color_start(EMERALD), color_start(GREEN)) == 0);
    assert(strcmp(resource_color_start(MAX_RESOURCES), color_start(11)) == 0);
}

void test_resource_color_stop() {
    assert(strcmp(resource_color_stop(), color_stop()) == 0);
}

void test_resource_le_than() {
    unsigned int r1[NUM_RESOURCES] = {1, 2, 3, 4, 6, 6};
    unsigned int r2[NUM_RESOURCES] = {1, 2, 3, 4, 5, 6};
    unsigned int r3[NUM_RESOURCES] = {1, 2, 3, 4, 7, 6};
    
    assert(resource_le_than(r1, r2) == 1);
    assert(resource_le_than(r1, r3) == 0); 
}

void test_resource_add() {
    unsigned int r1[NUM_RESOURCES] = {1, 2, 3, 4, 5, 6};
    unsigned int r2[NUM_RESOURCES] = {1, 1, 1, 1, 1, 1};
    unsigned int res[NUM_RESOURCES];
    resource_add(r1, r2, res);
    for (int i = 0; i < NUM_RESOURCES; i++) {
        assert(res[i] == r1[i] + r2[i]);
    }
}

void test_resource_sub() {
    unsigned int r1[NUM_RESOURCES] = {10, 9, 8, 7, 6, 5};
    unsigned int r2[NUM_RESOURCES] = {1, 2, 3, 4, 5, 6};
    unsigned int res[NUM_RESOURCES];
    
    resource_sub(r1, r2, res);
    for (int i = 0; i < NUM_RESOURCES; i++) {
        assert(res[i] == r1[i] - r2[i]);
    }
}

int main() {
    test_resource_to_string();
    test_resource_to_short_string();
    test_resource_color_start();
    test_resource_color_stop();
    test_resource_le_than();
    test_resource_add();
    test_resource_sub();

    printf("All resource tests passed!\n");
    return 0;
}
