#include "../include/player.h"

Player* player_create(
        double pos_x,
        double pos_y,
        double dir_x,
        double dir_y,
        double plane_x,
        double plane_y)
{
    sfVector2d pos = {pos_x, pos_y};
    sfVector2d dir = {dir_x, dir_y};
    sfVector2d plane = {plane_x, plane_y};

    Player* player = malloc(sizeof(Player));

    player->pos = pos;
    player->dir = dir;
    player->plane = plane;
    player->clock = sfClock_create();

    return player;
}

void player_handle_input(Player* player, const int* map, sfVector2u map_size)
{
    player->fps = sfClock_getElapsedTime(player->clock);
    sfClock_restart(player->clock);

    double moveSpeed = sfTime_asSeconds(player->fps) * 5.0; /* Constant value is in squares/second */
    double rotSpeed = sfTime_asSeconds(player->fps) * 3.0;  /* Constant value is in radians/second */

    /* Handle left movement */
    if (sfKeyboard_isKeyPressed(sfKeyLeft) || sfKeyboard_isKeyPressed(sfKeyA))
    {
        double oldDirX = player->dir.x;
        player->dir.x = player->dir.x * cos(rotSpeed) - player->dir.y * sin(rotSpeed);
        player->dir.y = oldDirX * sin(rotSpeed) + player->dir.y * cos(rotSpeed);

        double oldPlaneX = player->plane.x;
        player->plane.x = player->plane.x * cos(rotSpeed) - player->plane.y * sin(rotSpeed);
        player->plane.y = oldPlaneX * sin(rotSpeed) + player->plane.y * cos(rotSpeed);
    }

    /* Handle right movement */
    if (sfKeyboard_isKeyPressed(sfKeyRight) || sfKeyboard_isKeyPressed(sfKeyD))
    {
        double oldDirX = player->dir.x;
        player->dir.x = player->dir.x * cos(-rotSpeed) - player->dir.y * sin(-rotSpeed);
        player->dir.y = oldDirX * sin(-rotSpeed) + player->dir.y * cos(-rotSpeed);

        double oldPlaneX = player->plane.x;
        player->plane.x = player->plane.x * cos(-rotSpeed) - player->plane.y * sin(-rotSpeed);
        player->plane.y = oldPlaneX * sin(-rotSpeed) + player->plane.y * cos(-rotSpeed);
    }

    /* Handle forward movement */
    if (sfKeyboard_isKeyPressed(sfKeyUp) || sfKeyboard_isKeyPressed(sfKeyW))
    {
        sfVector2i currentPosX = {
                (int)(player->pos.x + player->dir.x * moveSpeed),
                (int)player->pos.y};
        sfVector2i currentPosY = {
                (int)player->pos.x,
                (int)(player->pos.y + player->dir.y * moveSpeed)};
        if (!map[player_get_map_pos(currentPosX, map_size.x)])
            player->pos.x += player->dir.x * moveSpeed;
        if (!map[player_get_map_pos(currentPosY, map_size.x)])
            player->pos.y += player->dir.y * moveSpeed;
    }

    /* Handle backward movement */
    if (sfKeyboard_isKeyPressed(sfKeyDown) || sfKeyboard_isKeyPressed(sfKeyS))
    {
        sfVector2i currentPosX = {
                (int)(player->pos.x - player->dir.x * moveSpeed),
                (int)player->pos.y};
        sfVector2i currentPosY = {
                (int)player->pos.x,
                (int)(player->pos.y - player->dir.y * moveSpeed)};
        if (!map[player_get_map_pos(currentPosX, map_size.x)])
            player->pos.x -= player->dir.x * moveSpeed;
        if (!map[player_get_map_pos(currentPosY, map_size.x)])
            player->pos.y -= player->dir.y * moveSpeed;
    }
}

int player_get_map_pos(sfVector2i pos, unsigned int map_size_x)
{
    return (int)(pos.x + pos.y * map_size_x);
}

void player_destroy(Player* player)
{
    free(player);
}