#include <SFML/Graphics.h>

int main(int argc, char** argv)
{
    sfVideoMode mode = { 800, 600, 31 };
    sfRenderWindow* window;

    sfEvent event;

    window = sfRenderWindow_create(mode, "SFML window", sfResize, NULL);
    if (!window)
        return 1;

    while (sfRenderWindow_isOpen(window))
    {
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed || sfKeyboard_isKeyPressed(sfKeyEscape))
                sfRenderWindow_close(window);
        }

        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_display(window);
    }

    sfRenderWindow_destroy(window);

    return 0;
}