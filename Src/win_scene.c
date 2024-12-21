#include <allegro5/allegro.h>
#include "win_scene.h"
#include "Src/utility.h"
#include "Src/UI.h"
#include "Src/game.h"

// Gambar yang akan ditampilkan
//static ALLEGRO_BITMAP* winImage = NULL;
static additionalScene winImage;
 
static void init(void) {
	winImage = scene_create(SCREEN_W / 2 - 200, 500, 400, 100, "Assets/panda_win.png");

	game_log("Win scene initialized.");
}

static void draw(void) {
	// Bersihkan layar
	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_scene(winImage);
}

static void update(void) {
	destroy_scene(&winImage);
}

static void destroy(void) {

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