#include <string.h>
#include "game_scene.h"
#include "menu_scene.h"
#include "win_scene.h"
#include "loading_scene.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include "enemy.h"
#include "weapon.h"
#include "UI.h"

#include <math.h>

Player player; // Player
Map map; // Map
enemyNode * enemyList; // Enemy List
BulletNode * bulletList; // Bullet List

// Weapon
Weapon weapon; 
int coins_obtained;

static void init(void){
    
    initEnemy();
    
    map = create_map("Assets/map0.txt", 0);

    player = create_player("Assets/panda2.png", map.Spawn.x, map.Spawn.y);

    enemyList = createEnemyList();
    bulletList = createBulletList();

    weapon = create_weapon("Assets/guns.png", "Assets/yellow_bullet.png", 16, 8, 10);
    
    for(int i=0; i<map.EnemySpawnSize; i++){
        Enemy enemy = createEnemy(map.EnemySpawn[i].x, map.EnemySpawn[i].y, map.EnemyCode[i]);
        insertEnemyList(enemyList, enemy);
    }

    game_log("coord x:%d \n coords y:%d \n", map.Spawn.x, map.Spawn.y);
    change_bgm("Assets/audio/game_bgm.mp3");
}

static void update(void){
    /*
        [TODO Homework]
        
        Change the scene if winning/losing to win/lose scene
    */
    // cek lagi kondisional kalah
    if (player.health <= 0) {
        //change_scene("lose_scene");
        printf("You loseee"); 
        return;
    }

    // cek lagi kondisional menang
    if (coins_obtained >= map.coin_assets || enemyList->next == NULL) {
        //change_scene("win scene");
        //change_scene(create_win_scene());
        printf("You winnnn!!");
        return;
    }

    update_player(&player, &map);

    Point Camera;
    /*
        [TODO HACKATHON 1-3]
        
        Calcualte the formula for the Camera
        Camera.x = ...
        Camera.y = ...

        Hint: Adjust it based on player position variable, then subtract it with half of the gameplay screen
    */
    //Camera = (Point){ 0, 0 };
    Camera.x = player.coord.x - (SCREEN_W / 2);
    Camera.y = player.coord.y - (SCREEN_H / 2);
    Camera = (Point){ Camera.x, Camera.y };

    /*if (Camera.x < 0) {
        Camera.x = 0;
    }
    if (Camera.y < 0) {
        Camera.y = 0;
    }
    if (Camera.x > map.col * TILE_SIZE - GAME_WIDTH) {
        Camera.x = map.col * TILE_SIZE - GAME_WIDTH;
    }
    if (Camera.y > map.row * TILE_SIZE - GAME_HEIGHT) {
        Camera.y = map.row * TILE_SIZE - GAME_HEIGHT;
    }*/


    updateEnemyList(enemyList, &map, &player);
    update_weapon(&weapon, bulletList, player.coord, Camera);
    updateBulletList(bulletList, enemyList, &map);
    update_map(&map, player.coord, &coins_obtained);
    
}

static void draw(void){
    Point Camera;
    /*
        [TODO HACKATHON 1-4]
        
        Calcualte the formula for the Camera
        Camera.x = ...
        Camera.y = ...

        Hint: Just copy from the [TODO HACKATHON 1-3]
    */
    /*Camera = (Point){ 0, 0 };*/
    Camera.x = player.coord.x - (SCREEN_W / 2);
    Camera.y = player.coord.y - (SCREEN_H / 2);
    Camera = (Point){ Camera.x, Camera.y };
   /* if (Camera.x < 0) {
        Camera.x = 0;
    }
    if (Camera.y < 0) {
        Camera.y = 0;
    }
    if (Camera.x > map.col * TILE_SIZE - GAME_WIDTH) {
        Camera.x = map.col * TILE_SIZE - GAME_WIDTH;
    }
    if (Camera.y > map.row * TILE_SIZE - GAME_HEIGHT) {
        Camera.y = map.row * TILE_SIZE - GAME_HEIGHT;
    }*/
    
    // Draw
    draw_map(&map, Camera);
    drawEnemyList(enemyList, Camera);
    drawBulletList(bulletList, Camera);
    draw_player(&player, Camera);
    draw_weapon(&weapon, player.coord, Camera);

    /*
        [TODO Homework]
        
        Draw the UI of Health and Total Coins
    */
    

}

static void destroy(void){
    delete_player(&player);
    delete_weapon(&weapon);
    destroy_map(&map);
    destroyBulletList(bulletList);
    destroyEnemyList(enemyList);
    terminateEnemy();
}


Scene create_game_scene(void){
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    
    scene.name = "game";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;
    
    return scene;
}
