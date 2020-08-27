#define GLOBAL_INSTANCE 
#include "DxLib.h"
#include "GV.h"
#include <math.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//ChangeWindowMode(TRUE); // commented out while making game...
	if (DxLib_Init() == -1)
		return -1;

	/*count regulates the whole game*/
	count = 0;
	enemy_count = 0;
	stage_count = 0;
	int enemy_img_index[3] = { 0, 3, 6 };
	int index = 0;

	/*load images here*/
	load();
	
	/*initialize arrays*/
	ch_shot_ini();

	SetDrawScreen(DX_SCREEN_BACK);

	while (1) {
		ClearDrawScreen();
		
		/* draw background*/
		//DrawGraph(32, 16, img_background, FALSE);

		/*draw the game board frame*/
		DrawGraph(0, 0, img_frame[2], FALSE); // top of the frame
		DrawGraph(0, 16, img_frame[1], FALSE); // left side of frame
		DrawGraph(0, 464, img_frame[3], FALSE); // bottom of the frame
		DrawGraph(416, 0, img_frame[0], TRUE); // title and numbers: right side of frame
		// DrawGraph(32, 360, img_messageBox, TRUE); // Dialogue Box
		DrawGraph(frame_x_to + 10, 180, img_power, TRUE);
		DrawGraph(frame_x_to + 10, 100, img_health, TRUE);
		//read_file();
		ch_health();
		ch_power();

		// let the enemy come. Later, maybe add some conversation phase...
		enemy.flag = 1;
		if (enemy_count == 0) {
			enemy.x = enemy.basePosition_x;
			enemy.y = enemy.basePosition_y;
		}

		/*Draw Enemy*/
		if (enemy.flag == 1) {
			DrawRotaGraphF(enemy.x, enemy.y, 1.0f, 0.0f, img_enemyMiko[enemy.direction], TRUE);
			//DrawRotaGraphF(enemy.x, enemy.y, 1.0f, 0.0f, img_tama[1], TRUE); // debug line show the middle
			
			//enemy_count ++;
			e_shot_main();
			e_health();
			enemy_count++;
		}

		/*my charactor move*/
		ch_shot();
		ch_move();

		/*item move*/
		calc_item();
		graph_item();
		item_catch_check();

		/*increment one count per cycle*/
		count++;

		ScreenFlip();
		WaitTimer(20);
		if (ProcessMessage() == -1) break;
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;

	}

	DxLib_End();

	return 0;
}