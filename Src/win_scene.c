#include <allegro5/allegro.h>
#include "menu_scene.h"
#include "win_scene.h"
#include "game_scene.h"
#include "setting_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"
#include "bullet.h"



static ALLEGRO_BITMAP* player_bitmap;
static Button menuButton;

static void init(void) {

    printf("function didalam void ini pada win_scene");
    player_bitmap = al_load_bitmap("Assets/panda_win.png");
    if (!player_bitmap) {
        game_abort("Failed to load player bitmap");
    }

    menuButton = button_create(SCREEN_W / 2 - 200, 650, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    //change_bgm("None");
    
}

static void update(void) {
    update_button(&menuButton);
    if (mouseState.buttons && menuButton.hovered == true) {
        change_scene(create_menu_scene());
    }
}

static void draw(void) {
    // Bersihkan layar dengan warna hitam
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Ambil ukuran asli dari bitmap
    int bitmap_width = al_get_bitmap_width(player_bitmap);
    int bitmap_height = al_get_bitmap_height(player_bitmap);

    // Gambar bitmap dengan ukuran 2x lebih besar
    al_draw_scaled_bitmap(
        player_bitmap,
        0, 0, bitmap_width, bitmap_height,  // Gambar penuh
        SCREEN_W / 2.5, SCREEN_H / 2 - bitmap_height,  // Atur posisi gambar
        bitmap_width * 2, bitmap_height * 2,         // Skala 2x ukuran asli
        0                                           // Tidak ada flag
    );

    // Gambar teks di bawah gambar
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(146, 161, 185),
        SCREEN_W / 2,              // Pusat layar secara horizontal
        SCREEN_H / 2 + bitmap_height,  // Di bawah gambar
        ALLEGRO_ALIGN_CENTER,
        "YOU WIN"
    );

    // button back
    draw_button(menuButton);
    //button back text
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        650 + 28 + menuButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "MAIN MENU"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        650 + 31 + menuButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "MAIN MENU"
    );
}

static void destroy(void) {
    if (player_bitmap) {
        al_destroy_bitmap(player_bitmap);
    }

    destroy_button(&menuButton);
}


Scene create_win_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "win";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
