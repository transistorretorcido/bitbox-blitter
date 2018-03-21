#include <stdint.h>
// tile ids
// layers
#define ninja_bg_fondo 0
// max indices :  49
#define ninja_bg_saw_nb 2
extern const int16_t ninja_bg_saw[ninja_bg_saw_nb][4]; // x,y,tid,0
#define ninja_bg_header TMAP_HEADER(40,30,TSET_16,TMAP_U8)
extern const uint16_t ninja_bg_tset[]; // from sheet.png
extern const uint8_t ninja_bg_tmap[][40*30];
