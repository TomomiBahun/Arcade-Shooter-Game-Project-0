#include "DxLib.h"
#include "GV.h"

void load() {
	LoadDivGraph("img/enemy.png", 12, 3, 4, 129, 128, img_enemyMiko);
	LoadDivGraph("img/me_reimu.png", 6, 2, 3, 48, 50, img_ch);
	LoadDivGraph("img/shot_fuda.png", 8, 8, 1, 18, 20, img_myShot);
	LoadDivGraph("img/tama.png", 3, 3, 1, 20, 20, img_tama);
	LoadDivGraph("img/s_bullet.png", 8, 8, 1, 12, 14, img_bullet01);
	LoadDivGraph("img/numbers.png", 10, 10, 1, 16, 18, img_number);
	img_myshot = LoadGraph("img/fuda_red.png");
	img_frame[0] = LoadGraph("img/title_frame.png");
	img_frame[1] = LoadGraph("img/thin_frame_vertical.png");
	img_frame[2] = LoadGraph("img/thin_frame_horizontal1.png");
	img_frame[3] = LoadGraph("img/thin_frame_horizontal2.png");
	//img_background = LoadGraph("img/background_green.png");
	img_enemyHealth = LoadGraph("img/enemy_health.png");
	img_ch_effect = LoadGraph("img/effect_small.png");
	img_power = LoadGraph("img/power_white.png");
	img_health = LoadGraph("img/health_white.png");
	img_heart = LoadGraph("img/heart.png");
	img_item_health = LoadGraph("img/item_health.png");
	img_item_power = LoadGraph("img/item_power.png");
	img_messageBox = LoadGraph("img/messageBox.png");
}