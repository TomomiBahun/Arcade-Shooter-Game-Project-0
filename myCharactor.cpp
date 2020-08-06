#include "GV.h"

double rang(double ang) {
	return (-ang + ang * 2 * GetRand(10000) / 10000.0);
}

void ch_move() {
	/* response to direction input */
	/*if (count % 2 == 0) {
		ch.direction = 0;
	}
	else {
		ch.direction = 1;
	}*/

	/*respose to the user input*/
	if (CheckHitKey(KEY_INPUT_LSHIFT) != 1) {
		if (CheckHitKey(KEY_INPUT_UP) == 1) ch.y -= 4.0;
		if (CheckHitKey(KEY_INPUT_DOWN) == 1) ch.y += 4.0;
		if (CheckHitKey(KEY_INPUT_LEFT) == 1) {
			ch.x -= 4.0;
			ch.direction = 2;
		}
		if (CheckHitKey(KEY_INPUT_RIGHT) == 1) {
			ch.x += 4.0;
			ch.direction = 4;
		}
	}
	else {
		ch.slow = 1;
		if (CheckHitKey(KEY_INPUT_UP) == 1) ch.y -= 2.0;
		if (CheckHitKey(KEY_INPUT_DOWN) == 1) ch.y += 2.0;
		if (CheckHitKey(KEY_INPUT_LEFT) == 1) {
			ch.x -= 2.0;
			ch.direction = 2;
		}
		if (CheckHitKey(KEY_INPUT_RIGHT) == 1) {
			ch.x += 2.0;
			ch.direction = 4;
		}
	}

	/* regulate the movable area */
	if (ch.x > frame_x_to - 20.0) {
		ch.x = frame_x_to - 20.0;
	}
	else if (ch.x < frame_x_from + 20.0) {
		ch.x = frame_x_from + 20.0;
	}
	if (ch.y > frame_y_to - 25.0) {
		ch.y = frame_y_to - 25.0;
	}
	else if (ch.y < frame_y_from + 25.0) {
		ch.y = frame_y_from + 25.0;
	}

	/*drawing my charactor*/
	// if the charactor is in normal status (no hit)
	if (ch.flag == 1) {
		DrawRotaGraphF(ch.x, ch.y, 1.0f, 0.0f, img_ch[ch.direction], TRUE);
		if (ch.slow == 1) {
			// when moving slow, show the hit area of my charactor
			DrawRotaGraphF(ch.x, ch.y, 1.0f, 0.0f, img_tama[1], TRUE);
		}
	}
	else { // if enemy's bullet hits my charactor
		if (ch.noDamage_time != 0) {
			// my charactor will blink for 40 counts
			if (count % 5 == 0) {
				DrawRotaGraphF(ch.x, ch.y, 1.0f, 0.0f, img_ch[ch.direction], TRUE);
				if (ch.slow == 1) {
					// when moving slow, show the "hit" area of my charactor
					DrawRotaGraphF(ch.x, ch.y, 1.0f, 0.0f, img_tama[1], TRUE);
				}
			}
			ch.noDamage_time--;
			if (ch.noDamage_time == 0) {
				// when noDamage time ends, change the charactor status back to normal
				ch.flag = 1;
				ch.noDamage_time = 40;
			}
		}
	}
	// reset some of the charactor properties for the next cycle
	ch.direction = 0; // fix the direction of me
	ch.slow = 0;
}

void ch_shot() {
	/*enable my shots*/
   	if (CheckHitKey(KEY_INPUT_SPACE)) {
		if (count % 3 == 0) { // my shot per 3 counts
			for (int i = 0; i < cShot.max; i++) {
				if (cShot.flag[i] != 1) {
					int shotW, shotH;
					GetGraphSize(img_myshot, &shotW, &shotH);

					cShot.x[i] = ch.x;
					cShot.y[i] = (ch.height - shotH) / 2.0 + ch.y;

					cShot.flag[i] = 1;
					break;
				}
			}
		}
	}

	/*draw my shots*/
	for (int i = 0; i < cShot.max; i++) {
		if (cShot.flag[i] == 1 && cShot.y[i] > (frame_y_from+ch.height/2)) {
			cShot.y[i] -= 20.0; // shots speed

			//if the shot goes beyond (the board frame + half of the charactor's height)
			if (cShot.y[i] < (frame_y_from+ch.height/2)) {
				cShot.flag[i] = 0;
			}
			DrawRotaGraphF(cShot.x[i], cShot.y[i], 1.0f, 0.0f, img_myShot[7], FALSE);
		}
	}

	/*check if my shot hits the enemy*/
	for (int i = 0; i < cShot.max; i++) {
		if (cShot.flag[i] == 1) {
			float hit_x = cShot.x[i] - enemy.x;
			float hit_y = cShot.y[i] - enemy.y;

			float hit_range = cShot.range - enemy.range;
			if (hit_x * hit_x + hit_y * hit_y < hit_range * hit_range) {
				cShot.flag[i] = 0;
				enemy.health -= cShot.power;
				DrawRotaGraphF(cShot.x[i], cShot.y[i], 1.0f, rang(PI), img_ch_effect, TRUE);
				if (enemy.health < 0)
					enemy.flag = 0;
			}
		}
	}
}

void ch_health() {
	if (ch.health == 0) return;
	if (ch.health_max == 0) { printfDx("my charactor health error");return; }
	// draw the health symbols
	for (int i = 0; i < ch.health; i++) {
		DrawGraph((frame_x_to + 10 + i*health_width), 130, img_heart, TRUE);
	}
}

void ch_power() {
	int power = cShot.power;
	DrawGraph((frame_x_to + 10), 210, img_number[power], TRUE);
	DrawGraph((frame_x_to + 10 + power_width), 210, img_number[0], TRUE);
}


void ch_shot_ini() {
	for (int i = 0; i < cShot.max; i++) {
		cShot.x[i] = 0.0;
		cShot.y[i] = 0.0;
		cShot.flag[i] = 0;
		cShot.hit_flag[i] = 0;
	}
}