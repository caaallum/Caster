#include "caster.h"

Caster* caster_create(
        unsigned int width,
        unsigned int height,
        unsigned int map_width,
        unsigned int map_height,
        int* map
)
{
    sfVector2u window_size = { width, height };
    sfVector2u map_size = { map_width, map_height };

    Caster* caster = malloc(sizeof(Caster));

    caster->window_size = window_size;
    caster->map_size = map_size;
    caster->map = map;
    caster->player = player_create(22, 12, -1, 0, 0, 0.66);

    return caster;
}

void caster_destroy(Caster* caster)
{
    free(caster);
}

void caster_draw(sfRenderWindow* window, Caster* caster)
{
    for (unsigned int x = 0; x < caster->window_size.x; x++)
    {
        double cameraX = 2 * x / (double)caster->window_size.x - 2; /* x-coordinate in camera space */
        sfVector2d rayPos = caster->player->pos;
        sfVector2d rayDir = {
            caster->player->dir.x + caster->player->plane.x * cameraX,
            caster->player->dir.y + caster->player->plane.y * cameraX
        };

        /* Which box of the map */
        sfVector2i map = { rayPos.x, rayPos.y };

        /* Length of ray from current position to next x or y-side */
        sfVector2d sideDist;

        /* Length of ray from one x or y-side to next x or y-side */
        sfVector2d deltaDist = {
            sqrt(1 + (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x)),
            sqrt(1 + (rayDir.x * rayDir.x) / (rayDir.y * rayDir.y))
        };
        double perpWallDist;

        /* What direction to step in x or y-direction (either +1 or -1) */
        sfVector2i step;

        int hit = 0; /* Was there a wall hit? */
        int side; /* Was it NS or EW wall hit */

        /* Calculate step and initial sideDist */
        if (rayDir.x < 0)
        {
            step.x = -1;
            sideDist.x = (rayPos.x - map.x) * deltaDist.x;
        }
        else
        {
            step.x = 1;
            sideDist.x = (map.x + 1.0 - rayPos.x) * deltaDist.x;
        }

        if (rayDir.y < 0)
        {
            step.y = -1;
            sideDist.y = (rayPos.y - map.y) * deltaDist.y;
        }
        else
        {
            step.y = 1;
            sideDist.y = (map.y + 1.0 - rayPos.y) * deltaDist.y;
        }

        /* Perform DDA */
        while (!hit)
        {
            /* Jump to next map square, OR in x-direction, OR in y-direction */
            if (sideDist.x < sideDist.y) {
                sideDist.x += deltaDist.x;
                map.x += step.x;
                side = 0;
            } else {
                sideDist.y += deltaDist.y;
                map.y += step.y;
                side = 1;
            }
            /* Check if ray has hit a wall */
            if (caster->map[map.x + map.y * caster->map_size.x] > 0) hit = 1;
        }

        /* Calculate distance projected on camera direction (oblique distance will give fisheye effect!) */
        if (side == 0)
            perpWallDist = fabs((map.x - rayPos.x + (1 - step.x) / 2) / rayDir.x);
        else
            perpWallDist = fabs((map.y - rayPos.y + (1 - step.y) / 2) / rayDir.y);

        /* Calculate height of line to draw on screen */
        int lineHeight = abs((int)(caster->window_size.y / perpWallDist));

        /* Calculate lowest and height pixel to fille in current stripe */
        int drawStart = -lineHeight / 2 + caster->window_size.y / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + caster->window_size.y / 2;
        if (drawEnd >= (int)caster->window_size.y)
            drawEnd = caster->window_size.y - 1;

        /* Choose wall color */
        sfColor color;
        switch (caster->map[map.x + map.y * caster->map_size.x])
        {
            case 1:     color = sfRed;      break;
            case 2:     color = sfGreen;    break;
            case 3:     color = sfBlue;     break;
            case 4:     color = sfWhite;    break;
            default:    color = sfYellow;   break;
        }

        /* Give x and y sides different brightness */
        if (side == 1)
        {
            color.r = color.r / 2;
            color.g = color.g / 2;
            color.b = color.b / 2;
        }

        /* Draw vertical line */
        sfVertex start;
        sfVector2f startPos = { x, drawStart };
        start.position = startPos;
        start.color = color;

        sfVertex end;
        sfVector2f endPos = { x, drawEnd };
        end.position = endPos;
        end.color = color;

        sfVertexArray* line = sfVertexArray_create();
        sfVertexArray_setPrimitiveType(line, sfLines);
        sfVertexArray_append(line, start);
        sfVertexArray_append(line, end);

        sfRenderWindow_drawVertexArray(window, line, 0);

        sfVertexArray_destroy(line);
    }

    player_handle_input(caster->player, caster->map, caster->map_size);
}