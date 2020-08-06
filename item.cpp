#include "GV.h"

int search_item() {
	for (int i = 0; i < item_max; i++) {
		if (item[i].flag == 0)
			return i; // if available item array found, return the location
	}
	return -1; // if no item is available, return -1
}

// item kinds: (1)small power, (2)large power, (3)health
/*define item properties
 @param x initial location of the item
 @param y initial location of the item
 @param num the number of item defined here
 @param eOrC if 0, items are coming from the enemy. if 1, items are coming from my charactor
*/
void set_item(double x, double y, int kind, int num) {
	int k;
	int index = num; // index to set up items for "num" times
	double r[3] = { 0.7, 1.0, 1.0 }; // size of each items (1)small power, (2)large power, (3)health
	do {
    		k = search_item();
		if (k != -1) {
			item[k].flag = 1;
			item[k].v = -1.0;
			item[k].count = 0;
			item[k].state = 0;
			item[k].x = x;
			item[k].y = y;
			// if there are more than 1 item, set initial item location randomly
			if (num > 0) {
				int random = GetRand(7);
				if (random > 3) {
					item[k].x += rang(40);
					item[k].y += rang(40);
				}
				else {
					item[k].x -= rang(40);
					item[k].y -= rang(40);
				}
			}
			// if item location is set outside of frame, relocate the item
			if (item[k].x + frame_x_from > frame_x_to) item[k].x -= 70.0;
			if (item[k].x + frame_x_from < frame_x_from) item[k].x += 70.0;
			if (item[k].y + frame_y_from > frame_y_to) item[k].y -= 70.0;
			if (item[k].y + frame_y_from < frame_y_from) item[k].y += 70.0;

			item[k].kind = kind;
			item[k].r = r[kind];
		}
		index--;
	} while (index != 0);
}

// items that appear when my charactor gets hit by bullets
void ch_item() {
	int numItem;
	if (cShot.power > 6.0) { // if power is more than 7, my charactor can get 5 power back
		numItem = 5;
	}
	else if (cShot.power > 3.0 && cShot.power < 7.0) {
		numItem = 3;
	}
	else if (cShot.power < 4.0) {
		numItem = 2;
	}
	set_item(ch.x, ch.y, 1, numItem);
}

void item_attract(int n) {
	double v = item[n].state ? 8 : 3; // if state ==1, v = 8 / if not, v = 3
	double angle = atan2(ch.y - item[n].y, ch.x - item[n].x); // get angle between my charactor and the item
	item[n].x += cos(angle) * v;
	item[n].y += sin(angle) * v;
}

void calc_item() {
	for (int i = 0; i < item_max; i++) {
		if (item[i].flag > 0) {
			// if my charactor is above the item_get_line, change all of the item status
			// items become ready to be catched by my charactor
			if (item[i].state == 0) {
				if (ch.y < item_get_line && ch.flag == 1)
					item[i].state = 1;
			}
			
			if (item[i].state == 0) {
				double x = ch.x - item[i].x;
				double y = ch.y - item[i].y;
				if (x * x + y * y < item_attract_range * item_attract_range && ch.flag == 1) {
					item[i].state = 1;
				}
				else {
					if (item[i].v < 2.5)
						item[i].v += 0.06;
					item[i].y += item[i].v;
				}
			}

			if(item[i].state > 0 && ch.flag == 1) { // if item is ready to be catched by my charactor
				item_attract(i);
			}
			item[i].count++;
			if (item[i].y > frame_y_to + 50)
				item[i].flag = 0;
		}
	}
}

// item kinds: (1)small power, (2)large power, (3)health
void graph_item() {
	for (int i = 0; i < item_max; i++) {
		if (item[i].flag == 1) {
			if (item[i].kind == 0 || item[i].kind == 1)
				DrawRotaGraphF(item[i].x + frame_x_from, item[i].y + frame_y_from, item[i].r, 0.0, img_item_power, TRUE);
			if (item[i].kind == 2)
				DrawRotaGraphF(item[i].x + frame_x_from, item[i].y + frame_y_from, item[i].r, 0.0, img_item_health, TRUE);
		}
	}
}

/*if item is within the catch range, item will be caught by my charactor.
  increase power or health depending on the item*/
void item_catch_check() {
	for (int i = 0; i < item_max; i++) {
		if (item[i].flag > 0) {
			double x = item[i].x - ch.x;
			double y = item[i].y - ch.y;
			// check if my charactor is in the catch range
			if (x * x + y * y < item_range * item_range) {
				switch (item[i].kind) {
				case 0: cShot.power += 0.5; break; // if catch small power, gain 0.5 power
				case 1: cShot.power += 1.0; break; // if catch large power, gain 1.0 power
				case 2: ch.health += 1.0; break; // if catch health, gain 1 health
				default: break; // do nothing if no items catched.
				}
				// if power or health goes beyond max, stay at max
				if (cShot.power > cShot.power_max) cShot.power = cShot.power_max;
				if (ch.health > ch.health_max) ch.health = ch.health_max;
				// when my charactor catches the item, the item will dissapear
				item[i].flag = 0;
			}
		}
	}
}