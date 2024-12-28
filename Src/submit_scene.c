#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "submit_scene.h"
#include "utility.h"
#include "game.h"
#include "UI.h"
#include "menu_scene.h"
#include "game_scene.h"

static char playerName[50] = "";
static int playerNameIndex = 0;
static Button submitButton;

// Timer untuk kontrol kecepatan input
static float inputDelayTimer = 0.0f;
static const float inputDelay = 0.2f; // Delay antara setiap karakter dalam detik

// Fungsi untuk inisialisasi scene
static void init_submit_scene(void) {
    playerName[0] = '\0'; // Reset nama pemain
    playerNameIndex = 0;
    submitButton = button_create(SCREEN_W / 2 - 100, SCREEN_H / 2 + 50, 200, 70, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
}

// Fungsi untuk update scene
static void update_submit_scene(void) {
    // Update timer untuk kontrol kecepatan input
    inputDelayTimer += 1.0f / 60.0f; // Asumsikan 60 FPS

    if (keyState[ALLEGRO_KEY_BACKSPACE] && playerNameIndex > 0 && inputDelayTimer >= inputDelay) {
        playerName[--playerNameIndex] = '\0'; // Hapus karakter terakhir
        inputDelayTimer = 0.0f; // Reset timer setelah input
    }

    for (int key = ALLEGRO_KEY_A; key <= ALLEGRO_KEY_Z; key++) {
        if (keyState[key] && playerNameIndex < 49 && inputDelayTimer >= inputDelay) {
            playerName[playerNameIndex++] = 'A' + (key - ALLEGRO_KEY_A);
            playerName[playerNameIndex] = '\0'; // Null-terminate string
            inputDelayTimer = 0.0f; // Reset timer setelah input
            break;
        }
    }

    update_button(&submitButton);
    if (mouseState.buttons && submitButton.hovered) {
        FILE* file = fopen("Assets/leaderboard.txt", "a");
        if (file) {
            fprintf(file, "%s %d\n", playerName, coins_obtained);
            fclose(file);
        }
        coins_obtained = 0;
        change_scene(create_menu_scene()); // Kembali ke menu utama
    }
}

// Fungsi untuk menggambar scene
static void draw_submit_scene(void) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(TITLE_FONT, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 - 100, ALLEGRO_ALIGN_CENTER, "Enter Your Name:");

    al_draw_text(P2_FONT, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2, ALLEGRO_ALIGN_CENTER, playerName);

    draw_button(submitButton);

    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        SCREEN_H / 2 + 65 + submitButton.hovered * 5,
        ALLEGRO_ALIGN_CENTER,
        "SUBMIT"
    );
}

// Fungsi untuk membersihkan resource
static void destroy_submit_scene(void) {
    destroy_button(&submitButton);
}

// Fungsi untuk membuat scene
Scene create_submit_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "submit";
    scene.init = &init_submit_scene;
    scene.update = &update_submit_scene;
    scene.draw = &draw_submit_scene;
    scene.destroy = &destroy_submit_scene;

    return scene;
}
