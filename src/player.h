#ifndef CASTER_PLAYER_H
#define CASTER_PLAYER_H

#include "stdafx.h"

typedef struct
{
    sfVector2d pos;
    sfVector2d dir;
    sfVector2d plane;
    sfClock* clock;
    sfTime fps;
} Player;

/**
 * Create player object
 *
 * \param pos_x
 * \param pos_y
 * \param dir_x
 * \param dir_y
 * \param plane_x
 * \param plane_y
 *
 * \return player
 */
Player* player_create(double, double, double, double, double, double);

/**
 * Handle player input
 *
 * \param player
 * \param map
 * \param map_size
 */
void player_handle_input(Player*, const int*, sfVector2u);

/**
 * Get position on map
 *
 * \param pos
 * \param map_size_x
 * \return pos
 */
int player_get_map_pos(sfVector2i, unsigned int);

/**
 * Destroy player object
 *
 * \param player
 */
void player_destroy(Player*);



#endif // CASTER_PLAYER_H