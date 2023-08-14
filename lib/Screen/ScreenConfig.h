#define ST7735_DRIVER
#define ST7735_GREENTAB3
// #define ST7735_GREENTAB128

#define SPI_FREQUENCY 27000000
// #define SPI_READ_FREQUENCY 20000000
#define TFT_WIDTH 128
#define TFT_HEIGHT 128

#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS 5
#define TFT_DC 17
#define TFT_RST 16
#define SD_CS 15

#define TFT_RGB_ORDER TFT_RGB
#define LOAD_GLCD
/* #define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT */

#define USE_HSPI_PORT
#define SUPPORT_TRANSACTIONS
// #define TFT_SDA_READ