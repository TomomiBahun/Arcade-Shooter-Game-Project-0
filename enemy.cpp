#include "GV.h"

/*returns the angle between my charactor and the enemy*/
float enemyatan2() {
	return atan2(ch.y - enemy.y, ch.x - enemy.x);
}

int e_shot_search() {
	for (int i = 0; i < enemy_shot_max; i++) {
		if (enemy.shot[i].flag == 0) {
			return i;
		}
	}
	return -1; // if there are no available shots
}

/*set enemy's property in order to move enemy to a certain position (xf, yf)*/
void e_move_set(int t, double xf, double yf) {
	double max_x;
	double max_y;
	if (t == 0) t = 1;
	enemy.motion_flag = 1; // the whole move (regular + slow)
	enemy.move_flag = 1; // only regular move flag is true at the beginning of move
	enemy.move_count = 0;
	enemy.slow_time = t / 5; // time range to move with a slow speed (this comes after moving regular speed)
	enemy.move_time = t - enemy.slow_time; // time range to move with a regular speed

	/* after moving with "regular move" for a certain distance
	the enemy will start moving with "slow move" for the rest of the distance*/
	/* move fast first, and slow down to stop */

	// regular move x
	max_x = xf - enemy.x;
	enemy.move_v0_x = max_x / t;
	enemy.move_prex = enemy.x;
	enemy.move_dis_x = enemy.move_time * enemy.move_v0_x;
	// regular move y
	max_y = yf - enemy.y;
	enemy.move_v0_y = max_y / t;
	enemy.move_prey = enemy.y;
	enemy.move_dis_y = enemy.move_time * enemy.move_v0_y;

	// slow move x
	enemy.slow_dis_x = max_x - enemy.move_dis_x;
	enemy.slow_v0_x = enemy.move_v0_x;
	enemy.slow_a_x = 2 * enemy.slow_v0_x / enemy.slow_time;
	enemy.slow_prex = enemy.x + enemy.move_dis_x;

	// slow move y
	enemy.slow_dis_y = max_y - enemy.move_dis_y;
	enemy.slow_v0_y = enemy.move_v0_y;
	enemy.slow_a_y = 2 * enemy.slow_v0_y / enemy.slow_time;
	enemy.slow_prey = enemy.y + enemy.move_dis_y;

	// select enemy image depending on the motion
	if (xf > enemy.x) {
		enemy.direction = 8; // when moving towards right
	}
	else {
		enemy.direction = 2; // when moving towards left
	}
}

/* calculate the position of the enemy*/
void e_calc_position() {
	double t = enemy.move_count;
	if (enemy.motion_flag == 1) {
		if (enemy.slow_flag == 1) {
			if (enemy.move_count <= enemy.slow_time) {
				//double dummyX = (enemy.slow_v0_x * t) - 0.5 * enemy.slow_a_x * t * t; debugging line
				//double dummyY = (enemy.slow_v0_y * t) - 0.5 * enemy.slow_a_y * t * t; debugging line
				enemy.x = enemy.slow_prex + ((enemy.slow_v0_x * t) - 0.5 * enemy.slow_a_x * t * t);
				enemy.y = enemy.slow_prey + ((enemy.slow_v0_y * t) - 0.5 * enemy.slow_a_y * t * t);
				// I shouldn't have to devide slow_time by two. Find a way to improve it.
				if (enemy.move_count == enemy.slow_time/2) {
					enemy.slow_flag = 0;
				}
			}
		}
		if (enemy.move_flag == 1) {
			if (enemy.move_count <= enemy.move_time) {
				enemy.x = enemy.move_prex + (enemy.move_v0_x * t);
				enemy.y = enemy.move_prey + (enemy.move_v0_y * t);
			}
			if (enemy.move_count == enemy.move_time) {
				enemy.slow_flag = 1;
				enemy.move_flag = 0;
				enemy.move_count = 0;
				t = 0;
			}
		}
		enemy.move_count++;
		if (enemy.move_flag == 0 && enemy.slow_flag == 0)
			enemy.motion_flag = 0;
	}
}

/* calculate the bullet location and graph it*/
void e_calc_shot() {
	for (int i = 0; i < enemy_shot_max; i++) {
		if (enemy.shot[i].flag > 0) { // if the bullet's flag is on
			enemy.shot[i].x += cos(enemy.shot[i].angle) * enemy.shot[i].speed;
			enemy.shot[i].y += sin(enemy.shot[i].angle) * enemy.shot[i].speed;
		}

		if (enemy.shot[i].y > 480.0 || enemy.shot[i].y < 0 || enemy.shot[i].x >416 || enemy.shot[i].x < 32) {
			enemy.shot[i].flag = 0;
			enemy.shot[i].x = enemy.x; // initialize the next bullet's coordinate
			enemy.shot[i].y = enemy.y; // initialize the next bullet's coordinate
		}

		// if the bullet is not active, do not draw the bullet
		if (enemy.shot[i].flag > 0)
			DrawRotaGraphF(enemy.shot[i].x, enemy.shot[i].y, 1.0f, 0.0, img_bullet01[enemy.shot[i].color], TRUE);
	}
}

void e_shot00() {
	/*simple bullet like a water fall*/
	int j[8];
	for (int i = 0; i < 8; i++) {
		j[i] = e_shot_search();
		if (enemy_count % 30 == 0) {
			/* properties to graph and regulate the bullet location*/
			enemy.shot[j[i]].flag = 2;
			enemy.shot[j[i]].angle = PI / 2; // perpendicular to the top and the bottom of the game board
			enemy.shot[j[i]].x = 56.0 * (i+1); //
			enemy.shot[j[i]].y = 100.0;
			enemy.shot[j[i]].speed = 5.0;
			enemy.shot[j[i]].color = GetRand(7);

			/* properties to check if the bullet hits my charactor*/
			GetGraphSize(img_bullet01[enemy.shot[j[i]].color], &enemy.shot[j[i]].width, &enemy.shot[j[i]].height);
			enemy.shot[j[i]].width = enemy.shot[j[i]].width / 2;
			enemy.shot[j[i]].height = enemy.shot[j[i]].height / 2;
			enemy.shot[j[i]].range = sqrt(enemy.shot[j[i]].width * enemy.shot[j[i]].width + enemy.shot[j[i]].height * enemy.shot[j[i]].height);
		}
	}

	// calculate the bullets' location and draw each of them
	e_calc_shot();
}

void e_shot01() {
	/* 1 shot every 10 counts, the bullet targets me*/
	int k = e_shot_search();
	if (enemy_count % 10 == 0) {
		/* properties to graph and regulate the bullet location*/
		enemy.shot[k].flag = 1;
		enemy.shot[k].angle = enemyatan2();
		enemy.shot[k].color = 0;
		enemy.shot[k].speed = 8;
		GetGraphSize(img_bullet01[enemy.shot[k].color], &enemy.shot[k].width, &enemy.shot[k].height);
		enemy.shot[k].x = enemy.x; // initialize the next bullet's coordinate
		enemy.shot[k].y = enemy.y; // initialize the next bullet's coordinate

		/* properties to check if the bullet hits my charactor*/
		enemy.shot[k].width = enemy.shot[k].width / 2;
		enemy.shot[k].height = enemy.shot[k].height / 2;
		enemy.shot[k].range = sqrt(enemy.shot[k].width * enemy.shot[k].width + enemy.shot[k].height * enemy.shot[k].height);
	}

	/*simple bullet like a water fall*/
	int j[8];
	for (int i = 0; i < 8; i++) {
		j[i] = e_shot_search();
		if (enemy_count % 30 == 0) {
			/* properties to graph and regulate the bullet location*/
			enemy.shot[j[i]].flag = 2;
			enemy.shot[j[i]].angle = PI / 2; // perpendicular to the top and the bottom of the game board
			enemy.shot[j[i]].x = 56.0 * (i+1); // 
			enemy.shot[j[i]].y = 100.0;
			enemy.shot[j[i]].speed = 5.0;
			enemy.shot[j[i]].color = GetRand(7);

			/* properties to check if the bullet hits my charactor*/
			GetGraphSize(img_bullet01[enemy.shot[j[i]].color], &enemy.shot[j[i]].width, &enemy.shot[j[i]].height);
			enemy.shot[j[i]].width = enemy.shot[j[i]].width / 2;
			enemy.shot[j[i]].height = enemy.shot[j[i]].height / 2;
			enemy.shot[j[i]].range = sqrt(enemy.shot[j[i]].width * enemy.shot[j[i]].width + enemy.shot[j[i]].height * enemy.shot[j[i]].height);
		}
	}

	// calculate the bullets' location and draw each of them
	e_calc_shot();
	
	// enemy move
	if (enemy.motion_flag == 0 && enemy_count == 0) {
		e_move_set(100, 358.0, 100.0);
		enemy.move_loc = 1;
	}
	if (enemy.motion_flag == 0 && enemy.move_loc == 1) {
		e_move_set(200, 68.0, 100.0);
		enemy.move_loc = 2;
	}
	if (enemy.motion_flag == 0 && enemy.move_loc == 2) {
		e_move_set(200, 358.0, 100.0);
		enemy.move_loc = 1;
	}
	if (enemy.motion_flag == 1) {
		e_calc_position();
	}
}

void e_shot_02() {
	double angle;
	int k;
	if (enemy_count == 0) {
		enemy.base_angle = enemyatan2();
	}

	if (enemy_count %60 == 0) {
		enemy.base_angle += PI2 / 20 / 2;
	}
	if (enemy_count % 6 == 0) {
		angle = enemyatan2();
		for (int i = 0; i < 20; i++) {
			k = e_shot_search();
			if (k != -1) {
				enemy.shot[k].flag = 1;
				enemy.shot[k].angle = enemy.base_angle + PI2 / 20 * i;
				enemy.shot[k].color = 2;
				enemy.shot[k].speed = 2.7;
				GetGraphSize(img_bullet01[enemy.shot[k].color], &enemy.shot[k].width, &enemy.shot[k].height);
				enemy.shot[k].x = enemy.x;
				enemy.shot[k].y = enemy.y;

				/* properties to check if the bullet hits my charactor*/
				enemy.shot[k].width = enemy.shot[k].width / 2;
				enemy.shot[k].height = enemy.shot[k].height / 2;
				enemy.shot[k].range = sqrt(enemy.shot[k].width * enemy.shot[k].width + enemy.shot[k].height * enemy.shot[k].height);
			}
		}
	}

	// random water fall bullets
	if (enemy_count % 7 == 0) {
		k = e_shot_search();
		if (k != -1) {
			enemy.shot[k].flag = 1;
			enemy.shot[k].angle = PI / 2;
			enemy.shot[k].color = 4;
			enemy.shot[k].speed = 1+GetRand(1);
			GetGraphSize(img_bullet01[enemy.shot[k].color], &enemy.shot[k].width, &enemy.shot[k].height);
			enemy.shot[k].x = GetRand(frame_x_to);
			enemy.shot[k].y = GetRand(200);

			/* properties to check if the bullet hits my charactor*/
			enemy.shot[k].width = enemy.shot[k].width / 2;
			enemy.shot[k].height = enemy.shot[k].height / 2;
			enemy.shot[k].range = sqrt(enemy.shot[k].width * enemy.shot[k].width + enemy.shot[k].height * enemy.shot[k].height);
		}
	}

	// calculate the bullets' location and draw each of them
	e_calc_shot();

	/* enemy moves around ramdomly*/
	if (enemy.motion_flag == 0 && enemy_count % 80) {
		double xf = 0.0; // new location
		double yf = 0.0; // new location
		int dummy = GetRand(6);
		// move the enemy position eveyr 80 counts randomly
		if (dummy > 3) {
			xf = enemy.x + GetRand(20);
			yf = enemy.y + GetRand(20);
		}
		else {
			xf = enemy.x - GetRand(20);
			yf = enemy.y - GetRand(20);
		}

		/*recalculate the new enemy location if the enemy goes beyond the game board*/
		if (xf > frame_x_to - 70.0) xf = xf = enemy.x - GetRand(20);
		if (xf < frame_x_from + 70.0) xf = xf = enemy.x + GetRand(20);
		if (yf > 200) yf = enemy.y - GetRand(20);
		if (yf < frame_x_from + 30.0) yf = enemy.y + GetRand(20);

		e_move_set(50, xf, yf);
	}

	if (enemy.motion_flag == 1)
		e_calc_position();
}

// check if the shot hits my charactor
int e_shot_check(int n) {
	if (enemy.shot[n].flag != 0) { // if the bullet is active on the board
		float hit_x = enemy.shot[n].x - ch.x;
		float hit_y = enemy.shot[n].y - ch.y;

		float hit_range = enemy.shot[n].range + ch.range;

		if (enemy.shot[n].speed > hit_range) {
			float pre_x = enemy.shot[n].x + cos(enemy.shot[n].angle + PI) * enemy.shot[n].speed;
			float pre_y = enemy.shot[n].y + sin(enemy.shot[n].angle + PI) * enemy.shot[n].speed;
			float px, py;
			for (int i = 0; i < enemy.shot[n].speed / hit_range; i++) {
				px = pre_x - ch.x;
				py = pre_y - ch.y;
				if (px * px + py + py < hit_range * hit_range)
					return 1; // hit
				pre_x += cos(enemy.shot[n].angle) * hit_range;
				pre_y += sin(enemy.shot[n].angle) * hit_range;
			}
		}
		if (hit_x * hit_x + hit_y * hit_y < hit_range * hit_range)
			return 1; // hit
	}
	return 0;
}

/**/
void ch_hit_result() {
	int index = 0;
	for (int i = 0; i < enemy_shot_max; i++) {
		if (enemy.shot[i].flag != 0) {
			if (e_shot_check(i) && ch.flag != 0) {
				DrawRotaGraph(430, 150, 1.0f, 0.0f, img_tama[0], TRUE); // debug line
				enemy.shot[i].flag = 0; // erase the hitting shot from the game board
				ch.flag = 0; // put the charactor in noDamage mode
				if (ch.health > 0) {
					// my charactor's health shouldn't be less than 0
					ch.health--;
				}
				cShot.power = 1;
				ch_item();
				return;
			}
		}
	}
}

/* draw enemy health bar*/
void e_health() {
	if (enemy.flag == 0) return;
	if (enemy.health_max == 0) {printfDx("enemy health error");return;}
	for (int i = 0; i < frame_x_to * 0.90 * enemy.health / enemy.health_max; i++) {
		DrawGraph(3 + frame_x_from + i, 2 + frame_y_from, img_enemyHealth, FALSE);
	}

}

void e_shot_main() {
	// conversation test without bullets
	/*if (stage_count == 0 && enemy.flag == 1) {
		read_file();
	}*/
	// first round of shots
	if (stage_count == 0 && enemy.flag == 1) {
		e_shot00();
		if (enemy_count == 500 || enemy.health < 1200) {
			stage_count++;
			enemy_count = 0;
			set_item(enemy.x, enemy.y, 2, 1); // health
			set_item(enemy.x, enemy.y, 1, 2); // large power
			//set_item(enemy.x, enemy.y, 0, 2); // small power
			memset(enemy.shot, 0, sizeof(enemy.shot)); // initialize all bullets
		}
	}
	// second round of shots. Enemy moves around in this round.
	if (stage_count == 1 && enemy.flag == 1) {
		e_shot01();
		if (enemy_count == 500 || enemy.health < 1000) {
			stage_count++;
			enemy.motion_flag = 0;
			enemy_count = 0;
			memset(enemy.shot, 0, sizeof(enemy.shot)); // initialize all bullets
		}
	}
	// move the enemy back to the basePosition
	if (stage_count == 2 && enemy.flag == 1) {
		if (enemy.motion_flag == 0)
			e_move_set(30, enemy.basePosition_x, enemy.basePosition_y);
		if (enemy.motion_flag == 1){
			e_calc_position();
			// when e_calc_position() is completed, motion_flag becomes 0.
			// this is when the enemy comes back to the basePosition. Get ready for next bullets
			if (enemy.motion_flag == 0) {
				stage_count++;
				enemy_count = 0;
				memset(enemy.shot, 0, sizeof(enemy.shot)); // initialize all bullets
			}
		}
	}
	if (stage_count == 3 && enemy.flag == 1) {
		e_shot_02();
	}
	ch_hit_result();
}