#include "Colors.h"

uint16_t HEX2RGB565(uint32_t hex)
{
    return ((hex & 0xF80000) >> 8) | ((hex & 0x00FC00) >> 5) | ((hex & 0x0000F8) >> 3);
}