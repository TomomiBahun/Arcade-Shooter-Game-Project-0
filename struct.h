#pragma once

/*my character property*/
typedef struct {
	const static int width = 50;
	const static int height = 50;
	float range = 6.0;
	int direction = 0; // 2 <- 0 -> 4
	int flag = 1;
	int slow = 0;
	int noDamage_time = 40;
	float x = board_x_middle; // initial position at middle
	float y = 400.0; // initial position at y = 400.0
	int health = 3; // default initial health
	int health_max = 8;
}ch_t;

/*my charactor shot property*/
typedef struct {
	int max = 200;
	float x[200];
	float y[200];
	int flag[200]; // 0 or 2
	int hit_flag[200];
	float range = 10.0;
	double power = 1.0;
	double power_max = 9.0;
}cShot_t;

/*enemy shot property*/
typedef struct {
	float x;
	float y;
	float angle = 0.0;
	int speed = 1;
	int height = 0;
	int width = 0;
	float range = 0.0;
	int color = 0;
	int flag = 0; // 0: ready to use, 1-9: in use
}eShot_t;

/*enemy property*/
typedef struct {
	float x = board_x_middle; // itinial position at middle
	float y = 100.0; // initial position at y = 100.0
	float basePosition_x = board_x_middle;
	float basePosition_y = 100.0;
	double health_max = 3000;
	double health = 3000;
	float range = 45.0;
	int flag = 0; // 0: enemy isn't there, 1: enemy is here
	const static int width = 129;
	const static int height = 128;
	eShot_t shot[enemy_shot_max];
	double base_angle = 0.0;
	int motion_flag = 0;
	int move_count = 0;
	// regular move properties
	int move_time = 0;
	int move_flag = 0;
	double move_v0_x = 0.0;
	double move_v0_y = 0.0;
	// double move_vf_x = 0.0; might not need this
	// double move_vf_y = 0.0; might not need this
	double move_prex = 0.0;
	double move_prey = 0.0;
	double move_dis_x = 0.0;
	double move_dis_y = 0.0;
	// slow move properties
	int slow_flag = 0;
	int slow_time = 0;
	double slow_v0_x = 0.0;
	double slow_v0_y = 0.0;
	// double slow_vf_x = 0.0; might not need this
	// double slow_vf_y = 0.0; might not need this
	double slow_a_x = 0.0;
	double slow_a_y = 0.0;
	double slow_prex = 0.0;
	double slow_prey = 0.0;
	double slow_dis_x = 0.0;
	double slow_dis_y = 0.0;

	int move_loc = 0;
	int direction= 0;
}enemy_t;

typedef struct {
	int flag = 0;
	int count = 0;
	int kind = 0;
	int state = 0;
	double v = 0.0;
	double x = 0.0;
	double y = 0.0;
	double r = 0.0;
}item_t;

typedef struct {
	int flag = 0;
	int index = 0;
	int y;
	// READ_LINE_MAX = 30, READ_LENGTH_MAX = 100
	char message[READ_LINE_MAX][READ_LENGTH_MAX];
}message_t;
