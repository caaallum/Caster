#ifndef CASTER_CASTER_H
#define CASTER_CASTER_H

#include "stdafx.h"
#include "player.h"

typedef struct
{
    int* map;
    sfVector2u map_size;
    sfVector2u window_size;
    Player* player;
} Caster;

/**
 * Create caster object
 *
 * \param width window width
 * \param height window height
 * \param map_width map width
 * \param map_height map height
 * \param map pointer to map array
 * \return caster object
 */
Caster* caster_create(unsigned int, unsigned int, unsigned int, unsigned int, int*);

/**
 * Destroy caster object
 *
 * \param caster
 */
void caster_destroy(Caster*);

/**
 * Draw caster to window
 *
 * \param window
 * \param caster
 */
void caster_draw(sfRenderWindow*, Caster*);

#endif // CASTER_CASTER_H
