#include <stdio.h>
#include <assert.h>
#include "../src/color.h"

// Fonction de test pour color_start
void test_color_start() {
    printf("Testing color_start...\n");

    // Boucle sur toutes les couleurs et vérifie les codes de couleur
    for (int i = 0; i < MAX_COLORS; i++) {
        enum color_t color = (enum color_t) i;
        const char* color_code = color_start(color);
        
        // Vérifie que color_start ne retourne pas NULL
        assert(color_code != NULL);

        // Affiche le nom de la couleur avec le code de couleur
        printf("%s%s%s\n", color_code, color_to_string(color), color_stop());
    }

    printf("color_start tests passed!\n");
}

// Fonction de test pour color_stop
void test_color_stop() {
    printf("Testing color_stop...\n");

    const char* reset_code = color_stop();
    // Vérifie que color_stop ne retourne pas NULL
    assert(reset_code != NULL);

    printf("color_stop test passed!\n");
}

// Fonction de test pour color_to_string
void test_color_to_string() {
    printf("Testing color_to_string...\n");

    // Boucle sur toutes les couleurs et vérifie que les chaînes retournées sont non-nulles
    for (int i = 0; i < MAX_COLORS; i++) {
        enum color_t color = (enum color_t) i;
        const char* color_name = color_to_string(color);

        // Vérifie que color_to_string ne retourne pas NULL
        assert(color_name != NULL);
        printf("Color %d is called %s\n", i, color_name);
    }

    printf("color_to_string tests passed!\n");
}

int main() {
    printf("Starting tests for color module...\n");

    test_color_start();
    test_color_stop();
    test_color_to_string();

    printf("All tests for color module passed successfully!\n");

    return 0;
}
