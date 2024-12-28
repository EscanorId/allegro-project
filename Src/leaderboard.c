#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include "leaderboard.h"
#include "UI.h"
#include "menu_scene.h"
#include "game.h"

#define MAX_ENTRIES 10

// Arrays to store the leaderboard data
char names[MAX_ENTRIES][100]; // Array to store names
int points[MAX_ENTRIES]; // Array to store points
static ALLEGRO_BITMAP* board;
static Button backButton;
static bool buttonPressedLastFrame = false;

// Function to sort leaderboard by points in descending order
void sort_leaderboard(void) {
    for (int i = 0; i < MAX_ENTRIES - 1; i++) {
        for (int j = 0; j < MAX_ENTRIES - i - 1; j++) {
            if (points[j] < points[j + 1]) { // Swap if the current point is less than the next
                // Swap points
                int temp = points[j];
                points[j] = points[j + 1];
                points[j + 1] = temp;

                // Swap names accordingly using strcpy_s
                char temp_name[100];
                strcpy_s(temp_name, sizeof(temp_name), names[j]);
                strcpy_s(names[j], sizeof(names[j]), names[j + 1]);
                strcpy_s(names[j + 1], sizeof(names[j + 1]), temp_name);
            }
        }
    }
}

// Initialize leaderboard by reading from file
void init_ldb(void) {
    board = al_load_bitmap("Assets/UI_Button_hovered.png");
    backButton = button_create(SCREEN_W / 2 - 200, 650, 400, 100, "Assets/UI_Button_Purple.png", "Assets/UI_Button_hovered_Purple.png");

    FILE* f = fopen("assets/leaderboard.txt", "r"); // Open leaderboard file for reading
    if (!f) {
        printf("Error opening file for reading.\n");
        return;
    }

    int i = 0;
    // Read names and points until the end of the file or max entries
    while (fscanf_s(f, "%s %d", names[i], (unsigned)_countof(names[i]), &points[i]) == 2 && i < MAX_ENTRIES) {
        i++;
    }
    fclose(f); // Close the file

    // Sort leaderboard after reading the data
    sort_leaderboard();
}

// Update leaderboard (if necessary, add new entries or adjust ranking)
void update_ldb(void) {
    // You could add code to add new scores and maintain the leaderboard.
    // For now, no logic is needed in this example.
    update_button(&backButton);

    if (!buttonPressedLastFrame && mouseState.buttons && backButton.hovered == true) {
        change_scene(create_menu_scene());
    }

    // Perbarui status tombol dari frame sebelumnya
    buttonPressedLastFrame = mouseState.buttons;
}

// Display leaderboard
void draw_ldb(void) {
    // Gambar background leaderboard menggunakan satu gambar
    int bitmap_width = al_get_bitmap_width(board);
    int bitmap_height = al_get_bitmap_height(board);

    // Posisi background di tengah layar
    int background_x = (SCREEN_W - bitmap_width * 3) / 2;
    int background_y = (SCREEN_H - bitmap_height * 3) / 2;

     //Gambar background dengan skala 3x lebih besar
    al_draw_scaled_bitmap(
        board,
        0, 0, bitmap_width, bitmap_height,   // Gambar penuh
        background_x, background_y,         // Posisi gambar
        bitmap_width * 3, bitmap_height * 3.5, // Skala 3x ukuran asli
        0                                    // Tidak ada flag
    );

    al_draw_text(
        TITLE_FONT,
        al_map_rgb(55, 85, 204),
        SCREEN_W / 2,              // Pusat layar secara horizontal
        SCREEN_H / 8 + bitmap_height,  // Di bawah gambar
        ALLEGRO_ALIGN_CENTER,
        "LEADERBOARD"
    );
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(204, 194, 49),
        SCREEN_W / 2,              // Pusat layar secara horizontal
        SCREEN_H / 7.5 + bitmap_height,  // Di bawah gambar
        ALLEGRO_ALIGN_CENTER,
        "LEADERBOARD"
    );
    // Gambar teks leaderboard di atas background
    for (int i = 0; i < MAX_ENTRIES; i++) {
        // Format score as a string
        char score[10];
        sprintf_s(score, sizeof(score), "%d", points[i]);

        // Gabungkan nama dan skor menjadi satu string
        char entry[150];
        sprintf_s(entry, sizeof(entry), "%d. %s - %s", i + 1, names[i], score);

        // Tentukan kolom untuk entri ini
        int column = (i < 5) ? 0 : 1; // 0 untuk kolom kiri, 1 untuk kolom kanan

        // Hitung posisi teks berdasarkan kolom
        int text_x = background_x + 100 + column * (bitmap_width * 4 / 3); // Jarak antar kolom
        int text_y = background_y + 120 + (i % 5) * 40; // Jarak vertikal antar entri

        // Gambar teks leaderboard di kolom yang sesuai
        al_draw_text(P3_FONT, al_map_rgb(255, 255, 255), text_x, text_y, ALLEGRO_ALIGN_LEFT, entry);
    }

    draw_button(backButton);
    //button back text
    al_draw_text(
        P2_FONT,
        al_map_rgb(255,255,255),
        SCREEN_W / 2,
        650 + 28 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        650 + 31 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );

    // Tampilkan semua perubahan
    al_flip_display();
}





// Cleanup any resources used (if necessary)
void destroy_ldb(void) {
    // No cleanup needed if no dynamic memory is allocated
    al_destroy_bitmap(board);
    destroy_button(&backButton);
}

// Create leaderboard scene
Scene create_leaderboard_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "leaderboard";
    scene.init = &init_ldb;
    scene.draw = &draw_ldb;
    scene.update = &update_ldb;
    scene.destroy = &destroy_ldb;

    return scene;
}
