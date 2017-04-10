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

#define LEVEL1_SIZE 5
const byte level1[LEVEL1_SIZE][3]
{
  {20, LCDHEIGHT - 8, 16},
  {15, LCDHEIGHT - 32, 20},
  {5, LCDHEIGHT - 14, 10},
  {50, LCDHEIGHT - 25, 7},
  {40, LCDHEIGHT - 16, 14}
};

int car_x = LCDWIDTH / 2;
int car_y = LCDHEIGHT - 8;
bool car_left = true;
int car_vy = 0;
int car_vx = 2;
int gravity = 1;

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
  for(int i=0; i<LEVEL1_SIZE; i++)
    if(is_on_ground(level1[i][0], level1[i][1], level1[i][2]))
      return true;
  return false;
}

void setup()
{
  gb.begin(); 
  gb.titleScreen(F("Crazy Car!!"), car_bitmap);
  gb.battery.show = false;
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
    else if(gb.buttons.repeat(BTN_DOWN, 1))
    {
      car_y += 1;
    }
    if(gb.buttons.repeat(BTN_RIGHT, 1))
    {
      car_x = min(LCDWIDTH - 8, car_x + car_vx);
      car_left = false;
    }
    if(gb.buttons.repeat(BTN_LEFT, 1))
    {
      car_x = max(0, car_x - car_vx);
      car_left = true;
    }
    car_y = min(LCDHEIGHT - 8, car_y + car_vy);
    car_vy = is_on_any_ground()? 0: car_vy + gravity;
    for(int i=0; i<LEVEL1_SIZE; i++)
      gb.display.drawFastHLine(level1[i][0], level1[i][1], level1[i][2]);
    gb.display.drawBitmap(car_x, car_y, car_bitmap, NOROT, car_left? NOFLIP:FLIPH);
  }
}
