#include <Adafruit_NeoPixel.h>

#define PIN_NEO_PIXEL 16
#define NUM_PIXELS 30
#define MAX_COLORS 6

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

static uint32_t RED = NeoPixel.Color(255, 0, 0);
static uint32_t GREEN = NeoPixel.Color(0, 255, 0);
static uint32_t BLUE = NeoPixel.Color(0, 0, 255);
static uint32_t YELLOW = NeoPixel.Color(255, 255, 0);
static uint32_t ORANGE = NeoPixel.Color(255, 165, 0);
static uint32_t PURPLE = NeoPixel.Color(128, 0, 128);

static uint32_t COLORS[MAX_COLORS] = {RED, GREEN, BLUE, YELLOW, ORANGE, PURPLE};

struct Bubble
{
    uint8_t pixel;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

float position = NUM_PIXELS - 1; // sub-pixel position, moves top → bottom
float speed = 7.0f;              // LEDs per second
uint8_t currentColorIndex = 0;
unsigned long lastUpdate = 0;

void setup()
{
    NeoPixel.begin();
    delay(200);
    NeoPixel.clear();
    NeoPixel.setBrightness(150);
    NeoPixel.show();
    lastUpdate = millis();
}

void loop()
{
    unsigned long now = millis();
    float dt = (now - lastUpdate) / 1000.0f;
    lastUpdate = now;

    position -= speed * dt;

    if (position < 0.0f)
    {
        position = NUM_PIXELS - 1;
        currentColorIndex = (currentColorIndex + 1) % MAX_COLORS;
    }

    uint32_t color = COLORS[currentColorIndex];
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    // nearest integer LED — snaps at 0.5, so both sides of the crossover are at 50%
    int nearest = constrain((int)round(position), 0, NUM_PIXELS - 1);
    float dist = fabsf(position - nearest); // 0.0 (right on LED) → 0.5 (halfway to next)
    float brightness = 1.0f - dist;         // 100% at center, 50% at crossover

    NeoPixel.clear();
    NeoPixel.setPixelColor(nearest,
                           (uint8_t)(r * brightness),
                           (uint8_t)(g * brightness),
                           (uint8_t)(b * brightness));
    NeoPixel.show();
}
