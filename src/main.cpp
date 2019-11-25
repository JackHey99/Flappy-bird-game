#include <Arduino.h>
#include "Adafruit_SSD1306.h"
#include <Wire.h>
#include <Adafruit_GFX.h>


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

enum states {start_screen, game_playing, paused_screen, game_over};

enum states mode;

uint32_t brick1_x_position = 128;       //initilizing bricks 1 & 2
int16_t random1_brick_size = 20;
uint32_t brick1 = 20;
uint32_t brick2 = 20;

uint32_t brick3_x_position = 192;        //initilizing bricks 3 & 4
int16_t random3_brick_size = 20;
uint32_t brick3 = 20;
uint32_t brick4 = 20;

uint32_t bird_position = 0;             //Setting up other variables
uint32_t points = 0;

volatile bool button_pressed = 0;
bool brick_hit = false;
uint32_t debouncedelay_ms = 80; 

#define pot_pin 14
#define start_button_pin 10


#define LOGO_HEIGHT   13            //logo size needed for drawing bit map  
#define LOGO_WIDTH    17

	
const unsigned char myBitmap [] PROGMEM = {
	// 'Bird, 76x64px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x18, 0x86, 0x00, 0x3c, 0x8e, 0x00, 0x46, 
0x8e, 0x00, 0x67, 0x7f, 0x00, 0x77, 0x7f, 0x80, 0x3e, 0xff, 0x80, 0x1c, 0xff, 0x00, 0x1c, 0x7f, 
0x00, 0x1f, 0xfe, 0x00, 0x00, 0x00, 0x00
};


void create_obstacles();
void obstacles_position();
void get_bird_position();
void display_current_screen();
void game();
void home();
void ISR_button_pressed();
void score();
void final_score();
void paused();


void setup()
{
  randomSeed(analogRead(23));
  Wire.begin();
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // setting address of the display I2C
  display.clearDisplay();
  pinMode(start_button_pin, INPUT_PULLUP); // setting internal pull up resistors
  attachInterrupt(start_button_pin, ISR_button_pressed, FALLING);  // interrupt for button
}

void loop()
{
  switch (mode)
  {
  case start_screen:
    home();
    points = 0;
    if(button_pressed == true)
    {
      mode = game_playing;
      button_pressed = false;
    }
    break;

  case game_playing:
    game();
    if(button_pressed == true)
    {
      mode = paused_screen;
      button_pressed = false;
    }
    break;
    
  case game_over:
    final_score();
    brick1_x_position = 0;
    brick3_x_position = 0;
    break;  

  case paused_screen:
    paused();
    if(button_pressed == true)
    {
      mode = game_playing;
      button_pressed = false;
    }
    break;

  default:
    break;
  }
}


void home()
{
  brick_hit = false;
  display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(30, 0);     // Start at top-left corner
  display.println(F("Flappy"));
  display.setCursor(40, 22);     // Start at top-left corner
  display.println(F("Bird"));
  display.setTextSize(1);
  display.setCursor(29, 45);     // Start at top-left corner
  display.println(F("Press button"));
  display.setCursor(26, 53);     // Start at top-left corner
  display.println(F("to start game"));

  display.display();
  delay(2000);
}


void final_score()
{
  display.clearDisplay();
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(40, 0);     // Start at top-left corner
  display.println(F("Game"));
  display.setCursor(40, 22);     // Start at top-left corner
  display.println(F("Over"));
  display.setTextSize(1);
  display.setCursor(29, 45);     // Start at top-left corner
  display.println(F("Your Score"));
  display.setCursor(100, 45);     // Start at top-left corner
  display.println(points);

  display.display();

  delay(5000);

  mode = start_screen;
}


void game()
{
  obstacles_position();
  create_obstacles();
  get_bird_position();
  display_current_screen();
  score();
}

void paused()
{
  display.clearDisplay();
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(30, 15);     // Start at top-left corner
  display.println(F("Paused"));

  display.drawBitmap(55,45,myBitmap, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
}


void get_bird_position()
{
static uint32_t avg_pot_value = 0; // static is better than a global
uint32_t new_pot_value = analogRead(pot_pin);
avg_pot_value = (0.25 * new_pot_value) + (0.75 * avg_pot_value);
bird_position = map(avg_pot_value, 0, 1023, 0, 64);
  //Serial.println(bird_position);
}

void obstacles_position()
{
  if (brick1_x_position == 0)
  {
    brick1_x_position = 128;
  }
  brick1_x_position = brick1_x_position - 2;

  if (brick1_x_position == 64)
  {
    brick3_x_position = 128;
  }
  brick3_x_position = brick3_x_position - 2;
}

void create_obstacles()
{
  if (brick1_x_position == 0)
  {
    random1_brick_size = random(1, 20);
  }

  if (brick3_x_position == 128)
  {
    random3_brick_size = random(1, 20);
    while (random3_brick_size - random1_brick_size > !4)
    {
      random3_brick_size = random(1, 20);
    }
  }

  brick1 = random1_brick_size;
  brick2 = 40 - random1_brick_size;

  brick3 = random3_brick_size;
  brick4 = 40 - random3_brick_size;
}

void display_current_screen()
{
  display.clearDisplay();
  //display.clearDisplay();
  // The INVERSE color is used so rectangles alternate white/black
  display.fillRect(brick1_x_position, 0, 10, brick1, INVERSE);
  display.fillRect(brick1_x_position, display.height() - brick2, 10, brick2, INVERSE);

  display.fillRect(brick3_x_position, 0, 10, brick3, INVERSE);
  display.fillRect(brick3_x_position, display.height() - brick4, 10, brick4, INVERSE);

  //display.fillRect(30, bird_position, 5, 5, INVERSE);
  
  display.drawBitmap(30,bird_position,myBitmap, LOGO_WIDTH, LOGO_HEIGHT, 1);

  display.display();
}

void score ()
{

  if ((bird_position <= brick1) && (brick1_x_position <= 47 && brick1_x_position >= 37 ))
  {
    brick_hit = true;
    mode = game_over;
  }

  if (bird_position + 13 >= (display.height() - brick2) && (brick1_x_position <= 47 && brick1_x_position >= 37 ))
  {
    brick_hit = true;
    mode = game_over;
  }
  
  if ((bird_position <= brick3) && (brick3_x_position <= 47 && brick3_x_position >= 37 ))
  {
    brick_hit = true;
    mode = game_over;
  }

  if (bird_position + 13 >= (display.height() - brick4) && (brick3_x_position <= 47 && brick3_x_position >= 37 ))
  {
    brick_hit = true;
    mode = game_over;

  }
  if (brick1_x_position == 24 || brick3_x_position == 24)
  {
    points++;
    //Serial.println(points);
  }
}


void ISR_button_pressed()
{
  noInterrupts();
  uint32_t currenttime_ms = millis();
  static uint32_t previoustime_ms = 0;

  if ((currenttime_ms - previoustime_ms) > debouncedelay_ms){
    button_pressed = true;
    previoustime_ms = currenttime_ms;
  }
  interrupts();

}