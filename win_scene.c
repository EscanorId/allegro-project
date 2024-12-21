#include <allegro5/allegro.h>
#include "Src/menu_scene.h"
#include "Src/win_scene.h"
#include "Src/game_scene.h"
#include "Src/setting_scene.h"
#include "Src/utility.h"
#include "Src/UI.h"
#include "Src/game.h"

static ALLEGRO_BITMAP* player_bitmap;

static void init(void) {
  

    player_bitmap = al_load_bitmap("Assets/panda_win.png");
    if (!player_bitmap) {
        game_abort("Failed to load player bitmap");
    }

    change_bgm("None");
}

static void update(void) {

    
}

static void draw(void) {

    al_clear_to_color(al_map_rgb(0, 0, 0)); // Layar hitam

    al_draw_scaled_bitmap(player_bitmap,
        0, 0, 32, 32,
        SCREEN_W / 4, SCREEN_H / 2, 64, 64, 0);
}

static void destroy(void) {
    if (player_bitmap) { // Pastikan player_bitmap valid
        al_destroy_bitmap(player_bitmap);
        player_bitmap = NULL; // Set ke NULL agar aman
    }
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
