
#include "player.h"
#include "game.h"
#include "utility.h"
#include "map.h"

#include <math.h>

static bool isCollision(Player * player, Map * map);

Player create_player(char* path, int row, int col) {
    Player player;
    memset(&player, 0, sizeof(player));

    player.coord = (Point){
        col * TILE_SIZE,
        row * TILE_SIZE
    };

    player.speed = 4;
    player.health = 50;

    player.image = al_load_bitmap(path);
    if (!player.image) {
        game_abort("Error Load Bitmap with path : %s", path);
    }

    return player;
}

void update_player(Player * player, Map * map) {

    Point original = player->coord;

    // Knockback effect
    if (player->knockback_CD > 0) {

        player->knockback_CD--;
        int next_x = player->coord.x + player->speed * cos(player->knockback_angle);
        int next_y = player->coord.y + player->speed * sin(player->knockback_angle);
        Point next;
        next = (Point){ next_x, player->coord.y };

        if (!isCollision(player, map)) {
            player->coord = next;
        }

        next = (Point){ player->coord.x, next_y };
        if (!isCollision(player, map)) {
            player->coord = next;
        }
    }

    /*
        [TODO HACKATHON 1-1] DONE

        Player Movement
        Adjust the movement by player->speed

        if (keyState[ALLEGRO_KEY_W]) {
            player->coord.y = ...
            player->direction = ...
        }
        if (keyState[ALLEGRO_KEY_S]) {
            player->coord.y = ...
            player->direction = ...
        }
    */

    if (controlType == CONTROL_WASD) {
        if (keyState[ALLEGRO_KEY_W]) { // WASD Up
            player->status = PLAYER_WALKING;
            player->coord.y -= player->speed;
            player->direction = UP;
        }
        if (keyState[ALLEGRO_KEY_S]) { // WASD Down
            player->status = PLAYER_WALKING;
            player->coord.y += player->speed;
            player->direction = DOWN;
        }
    }
    else if (controlType == CONTROL_IJKL) {
        if (keyState[ALLEGRO_KEY_I]) { // IJKL Up
            player->status = PLAYER_WALKING;
            player->coord.y -= player->speed;
            player->direction = UP;
        }
        if (keyState[ALLEGRO_KEY_K]) { // IJKL Down
            player->status = PLAYER_WALKING;
            player->coord.y += player->speed;
            player->direction = DOWN;
        }
    }

    // if Collide, snap to the grid to make it pixel perfect
    if (isCollision(player, map)) {
        player->coord.y = max(0, min(player->coord.y, map->row * TILE_SIZE - TILE_SIZE));
    }

    /*
        [TODO HACKATHON 1-2] DONE

        Player Movement
        Add/Subtract the movement by player->speed

        if (keyState[ALLEGRO_KEY_A]) {
            player->coord.y = ...
            player->direction = ...
        }
        if (keyState[ALLEGRO_KEY_D]) {
            player->coord.y = ...
            player->direction = ...

    }
    */

    if (controlType == CONTROL_WASD) {
        if (keyState[ALLEGRO_KEY_A]) { // WASD Left
            player->status = PLAYER_WALKING;
            player->coord.x -= player->speed;
            player->direction = LEFT;
        }
        if (keyState[ALLEGRO_KEY_D]) { // WASD Right
            player->status = PLAYER_WALKING;
            player->coord.x += player->speed;
            player->direction = RIGHT;
        }
    } else if (controlType == CONTROL_IJKL) {
        if (keyState[ALLEGRO_KEY_J]) { // IJKL Left
            player->status = PLAYER_WALKING;
            player->coord.x -= player->speed;
            player->direction = LEFT;
        }
        if (keyState[ALLEGRO_KEY_L]) { // IJKL Right
            player->status = PLAYER_WALKING;
            player->coord.x += player->speed;
            player->direction = RIGHT;
        }
    }

    if (isCollision(player, map)) {
        player->coord.x = max(0, min(player->coord.x, map->col * TILE_SIZE - TILE_SIZE));
    }
    if (
        !(keyState[ALLEGRO_KEY_W] || keyState[ALLEGRO_KEY_S] ||
            keyState[ALLEGRO_KEY_A] || keyState[ALLEGRO_KEY_D] ||
            keyState[ALLEGRO_KEY_I] || keyState[ALLEGRO_KEY_K] ||
            keyState[ALLEGRO_KEY_J] || keyState[ALLEGRO_KEY_L])
        ) {
        player->status = PLAYER_IDLE;
    }
    /*
        [TODO Homework]

        Calculate the animation tick to draw animation later
    */

    player->animation_tick = (player->animation_tick + 1) % 64; //reset back to 0 after it reaches 64, consists of fixed no. of cycles

}

void draw_player(Player * player, Point cam) {
    int dy = player->coord.y - cam.y; // destiny y axis
    int dx = player->coord.x - cam.x; // destiny x axis

    int flag = 0; // Change the flag to flip character

    /*
        [TODO Homework]

        Draw Animation of Dying, Walking, and Idle
    */

    // Set flip based on direction
    if (player->direction == LEFT) {
        flag = 0;// Tidak flip (arah kanan)
    }
    else {
        flag = ALLEGRO_FLIP_HORIZONTAL; 
    }

    if (player->status == PLAYER_IDLE) {
        int offset = 32 * ((int)(player->animation_tick / 30)%2);
        al_draw_tinted_scaled_bitmap(player->image, al_map_rgb(255, 255, 255),
            offset, 0, 32, 32, // source image x, y, width, height
            dx, dy, TILE_SIZE, TILE_SIZE, // destiny x, y, width, height
            flag // flip or not
        );
    }


    if (player->status == PLAYER_WALKING) {
        int offset = 32 * ((int)(player->animation_tick / 16)%4);
        al_draw_tinted_scaled_bitmap(player->image, al_map_rgb(255, 255, 255),
            offset, 32, 32, 32, // source image x, y, width, height
            dx, dy, TILE_SIZE, TILE_SIZE, // destiny x, y, width, height
            flag // flip or not
        );
    }

    if (player->status == PLAYER_DYING) {
        int offset = 32 * ((int)(player->animation_tick / 22)%4);
        al_draw_tinted_scaled_bitmap(player->image, al_map_rgb(255, 255, 255),
            offset, 64, 32, 32, // source image x, y, width, height
            dx, dy, TILE_SIZE, TILE_SIZE, // destiny x, y, width, height
            flag // flip or not
        );
    }




#ifdef DRAW_HITBOX
    al_draw_rectangle(
        dx, dy, dx + TILE_SIZE, dy + TILE_SIZE,
        al_map_rgb(255, 0, 0), 1
    );
#endif

}

void delete_player(Player * player) {
    al_destroy_bitmap(player->image);
}

static bool isCollision(Player * player, Map * map) {

    if (player->coord.x < 0 ||
        player->coord.y < 0 ||
        (player->coord.x + TILE_SIZE - 1) / TILE_SIZE >= map->col ||
        (player->coord.y + TILE_SIZE - 1) / TILE_SIZE >= map->row)
        return true;

    /*

        [TODO HACKATHON 2-1] DONE

        Check every corner of enemy(PLAYER)? if it's collide or not

        We check every corner if it's collide with the wall/hole or not
        You can activate DRAW_HITBOX in utility.h to see how the collision work

        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;

    */

    //hitbox 
    int tileX1 = player->coord.x / TILE_SIZE;
    int tileX2 = (player->coord.x + TILE_SIZE - 1) / TILE_SIZE;
    int tileY1 = player->coord.y / TILE_SIZE;
    int tileY2 = (player->coord.y + TILE_SIZE - 1) / TILE_SIZE;

    if (!isWalkable(map->map[tileY1][tileX1])) return true;
    if (!isWalkable(map->map[tileY1][tileX2])) return true;
    if (!isWalkable(map->map[tileY2][tileX1])) return true;
    if (!isWalkable(map->map[tileY2][tileX2])) return true;
    //done

    return false;
}

void hitPlayer(Player * player, Point enemy_coord, int damage) {
    if (player->knockback_CD == 0) {
        float dY = player->coord.y - enemy_coord.y;
        float dX = player->coord.x - enemy_coord.x;
        float angle = atan2(dY, dX);

        /*
            [TODO Homework] DONE

            Game Logic when the player get hit or die

            player->knockback_angle = angle;
            player->knockback_CD = 32;

            player->health = ...
            if(player->health <= 0){

                player->health = 0;
                player->status = ...

                ...
            }
        */
        //BOOKMARK TO CHECK AGAIN
        player->knockback_angle = angle;
        player->knockback_CD = 32;
        printf("enemy touch player");
        player->health -= damage;

        // Knockback player
        int knockback_distance = TILE_SIZE / 2;
        //float angle = atan2(player->coord.y - enemyCoord.y, player->coord.x - enemyCoord.x);
        player->coord.x += knockback_distance * cos(angle);
        player->coord.y += knockback_distance * sin(angle);

        if (player->health <= 0) {
            player->health = 0;
            player->status = PLAYER_DYING;

        }

    }
}
