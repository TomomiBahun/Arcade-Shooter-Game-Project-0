#include "DxLib.h"
#include "define.h"

#ifdef GLOBAL_INSTANCE
#define GLOBAL
#else
#define GLOBAL extern
#endif

#include "function.h"
#include "math.h"

GLOBAL ch_t ch;
GLOBAL enemy_t enemy;
GLOBAL cShot_t cShot;
GLOBAL item_t item[item_max];
GLOBAL message_t text;
//GLOBAL eShot_t eShot;


GLOBAL int count;
GLOBAL int enemy_count;
GLOBAL int shot_count;
GLOBAL int stage_count;

/*image arrays*/
GLOBAL int img_enemyMiko[12];
GLOBAL int img_ch[6];
GLOBAL int img_frame[4];
GLOBAL int img_myShot[8];
GLOBAL int img_myshot;
GLOBAL int img_tama[3];
GLOBAL int img_bullet01[8];
GLOBAL int img_background;
GLOBAL int img_enemyHealth;
GLOBAL int img_ch_effect;
GLOBAL int img_power;
GLOBAL int img_health;
GLOBAL int img_heart;
GLOBAL int img_item_health;
GLOBAL int img_item_power;
GLOBAL int img_number[10];
GLOBAL int img_messageBox;

