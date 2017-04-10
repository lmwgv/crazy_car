#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;

const PROGMEM byte car_bitmap[]
{
    8,8,
    B00000000,
    B00001110,
    B00010001,
    B01111111,
    B10000001,
    B11110111,
    B11011101,
    B01110111
};

const PROGMEM byte levels[]
{
    1,
    5,
    20, LCDHEIGHT - 8, 16,
    15, LCDHEIGHT - 32, 20,
    5, LCDHEIGHT - 14, 10,
    50, LCDHEIGHT - 25, 7,
    40, LCDHEIGHT - 16, 14
};
const byte MAX_LEVEL_SIZE = 16;

byte current_level = 0;
byte level_size = 0;
byte level[MAX_LEVEL_SIZE][3];

const byte car_vx = 2;
const byte gravity = 1;
byte car_x = LCDWIDTH / 2;
byte car_y = LCDHEIGHT - 8;
bool car_facing_left = true;
char car_vy = 0;

bool is_on_ground(byte x, byte y, byte w)
{
    if(car_y + 8 == LCDHEIGHT)
        return true;
    if(car_x > x + w || car_x + 8 < x)
        return false;
    if(car_y + 8 <= y && car_y + 8 + car_vy >= y)
    {
        car_y = y - 8;
        return true;
    }
}

bool is_on_any_ground()
{
    i = level_size;
    while(--i)
        if(is_on_ground(level[i][0], level[i][1], level[i][2]))
            return true;
    return false;
}

void load_level(byte level_number)
{
    if(level_number > pgm_read_byte(levels))
        return;
    offset = 1;
    for(int l=0; l<level_number; l++)
        offset += 3 * pgm_read_byte(levels + offset) + 1;
    level_size = pgm_read_byte(levels + offset);
    current_level = level_number;
    memcpy_P(level, levels + offset + 1, 3 * level_size);
}

void setup()
{
    gb.begin(); 
    gb.titleScreen(F("Crazy Car!!"), car_bitmap);
    gb.battery.show = false;
    load_level(0);
}

void loop()
{
    if(gb.update())
    {
        if(gb.buttons.pressed(BTN_A))
        {
            if(is_on_any_ground())
                car_vy = -4;
        }
        else if(gb.buttons.repeat(BTN_DOWN, 1) && car_vy == 0 && is_on_any_ground())
        {
            car_y += 1;
        }
        if(gb.buttons.repeat(BTN_RIGHT, 1))
        {
            car_x = min(LCDWIDTH - 8, car_x + car_vx);
            car_facing_left = false;
        }
        if(gb.buttons.repeat(BTN_LEFT, 1))
        {
            car_x = max(0, car_x - car_vx);
            car_facing_left = true;
        }
        car_y = min(LCDHEIGHT - 8, car_y + car_vy);
        car_vy = is_on_any_ground()? 0: car_vy + gravity;
        i = level_size;
        while(--i)
            gb.display.drawFastHLine(level[i][0], level[i][1], level[i][2]);
        gb.display.drawBitmap(car_x, car_y, car_bitmap, NOROT, car_facing_left? NOFLIP:FLIPH);
    }
}
