#include <time.h>
#include "system.h"

int main()
{
    srand(time(NULL));
    chip8_t *c8 = init_chip8();
    load_rom(c8);
    run(c8);
}