/*
 * map.c
 *
 *  Created on: 2018/04/14
 *      Author: shts
 */
#include "iodefine.h"
#include "map.h"
#include "LED.h"
#include "sensor.h"
#include "serial.h"

MAP map;

extern SEN r_sen,l_sen,cr_sen,cl_sen;

void init_Map(void) {
	int i, j;

	map.pos_x = 0;
	map.pos_y = 0;
	map.direction = 0;
	map.goal_x = 1;
	map.goal_y = 0;
	map.map_size = mp_size;
	map.tmp_path = 0;
	map.wall = 0;

	for (i = 0; i < map.map_size - 1; i++) {
		map.wall_map_x[i] = 0;
		map.wall_map_y[i] = 0;
		map.searched_map_x[i] = 0xffffffff;
		map.searched_map_y[i] = 0xffffffff;
	}

	for (i = 0; i < map.map_size; i++) {
		for (j = 0; j < map.map_size; j++) {
			map.a_dist_map[i][j] = 255;
			map.dist_map[i][j] = 255;
		}
	}
	map.a_dist_map[map.goal_x][map.goal_y] = 0;
	map.dist_map[map.goal_x][map.goal_y] = 0;
}

void Detect_direction() {
	if (map.direction == 0) {
		map.pos_y += 1;
		/*
		 sta_LED_drv(Red, off);
		 sta_LED_drv(Yerrow, off);
		 sta_LED_drv(Green, on);
		 */
	} else if (map.direction == 1) {
		map.pos_x += 1;
		/*
		 sta_LED_drv(Red, on);
		 sta_LED_drv(Yerrow, off);
		 sta_LED_drv(Green, off);
		 */
	} else if (map.direction == 3) {
		map.pos_x -= 1;
		/*
		 sta_LED_drv(Red, off);
		 sta_LED_drv(Yerrow, on);
		 sta_LED_drv(Green, off);
		 */
	} else if (map.direction == 2) {
		map.pos_y -= 1;
		/*
		 sta_LED_drv(Red, off);
		 sta_LED_drv(Yerrow, off);
		 sta_LED_drv(Green, off);
		 */
	}
	drv_Status_LED(Red, map.pos_y & 1);
	drv_Status_LED(Yerrow, (map.pos_x & 1) >> 0);
	drv_Status_LED(Green, (map.pos_x & 2) >> 1);

}

void update_Wall_map() {
	/*
	 * wall : W-S-E-N
	 * 		 MSB   LSB
	 *
	 * map_x : vertical walls to x-axis
	 * map_y : vertical walls to y-axis
	 * */

	map.wall = 0;
	if (r_sen.sen > r_sen.non_threshold) {
		map.wall |= 1 << ((1 + map.direction) % 4);
	}
	if (l_sen.sen > l_sen.non_threshold) {
		map.wall |= 1 << ((3 + map.direction) % 4);
	}
	if (cl_sen.sen > cl_sen.non_threshold) {
		map.wall |= 1 << ((0 + map.direction) % 4);
	}

	if (map.pos_x < map.map_size - 1) {
		map.wall_map_x[map.pos_x] |= ((map.wall & 2) >> 1) << map.pos_y;
		map.searched_map_x[map.pos_x] &= ~(1 << map.pos_y);
	}
	if (map.pos_x - 1 >= 0) {
		map.wall_map_x[map.pos_x - 1] |= ((map.wall & 8) >> 3) << map.pos_y;
		map.searched_map_x[map.pos_x - 1] &= ~(1 << map.pos_y);
	}

	if (map.pos_y < map.map_size - 1) {
		map.wall_map_y[map.pos_y] |= ((map.wall & 1) >> 0) << map.pos_x;
		map.searched_map_y[map.pos_y] &= ~(1 << map.pos_x);
	}
	if (map.pos_y - 1 >= 0) {
		map.wall_map_y[map.pos_y - 1] |= ((map.wall & 4) >> 2) << map.pos_x;
		map.searched_map_y[map.pos_y - 1] &= ~(1 << map.pos_x);
	}
}

void print_Wall_map_x(int row) {
	int i, mask = 1;
	mask <<= row;

	for (i = 0; i < map.map_size - 1; i++) {
		myprintf("  ");
		if (map.wall_map_x[i] & mask) {
			myprintf("|");
		} else {
			myprintf(" ");
		}
	}

}

void print_Wall_map_y(int row) {
	int i, mask;
	for (i = 0; i < map.map_size - 1; i++) {
		mask = 1 << i;
		if (map.wall_map_y[row] & mask) {
			myprintf("--");
		} else {
			myprintf("  ");
		}
		myprintf("+");
	}
	mask <<= 1;
	if (map.wall_map_y[row] & mask) {
		myprintf("--");
	} else {
		myprintf("  ");
	}

}

void print_Wall_map() {
	int i;
	myprintf("\n");
	for (i = 0; i < map.map_size; i++) {
		myprintf("+--");
	}
	myprintf("+\n|");

	print_Wall_map_x(map.map_size - 1);
	myprintf("  |\n+");
	for (i = 1; i < map.map_size; i++) {
		print_Wall_map_y(map.map_size - 1 - i);
		myprintf("+\n|");
		print_Wall_map_x(map.map_size - 1 - i);
		myprintf("  |\n+");
	}

	for (i = 0; i < map.map_size; i++) {
		myprintf("--+");
	}
	myprintf("\n\n");
}


char read_Wall_map(char x, char y) {

	/*
	 *  wall : W-S-E-N
	 * 		 MSB   LSB
	 */

	char wall = 0;

	//myprintf("\n\n");

	if (x < map.map_size - 1) {
		map.wall |= ((map.wall_map_x[x] & (1 << y)) >> y) << 1;
		//myprintf("%d\n", ((mixed_map_x[x] & (1 << y)) >> y) << 1);
	}
	if (x - 1 >= 0) {
		map.wall |= ((map.wall_map_x[x - 1] & (1 << y)) >> y) << 3;
		//myprintf("%d\n", ((mixed_map_x[x - 1] & (1 << y)) >> y) << 3);
	}

	if (y < map.map_size - 1) {
		map.wall |= (map.wall_map_y[y] & (1 << x)) >> x;
		//myprintf("%d\n", (mixed_map_y[y] & (1 << x)) >> x);
	}
	if (y - 1 >= 0) {
		map.wall |= ((map.wall_map_y[y - 1] & (1 << x)) >> x) << 2;
		//myprintf("%d\n", ((mixed_map_y[y - 1] & (1 << x)) >> x) << 2);
	}
	return wall;
}

void print_Searched_map_x(int row) {
	int i, mask = 1;
	mask <<= row;

	for (i = 0; i < map.map_size - 1; i++) {
		myprintf("  ");
		if (map.searched_map_x[i] & mask) {
			myprintf("|");
		} else {
			myprintf(" ");
		}
	}

}

void print_Searched_map_y(int row) {
	int i, mask;
	for (i = 0; i < map.map_size - 1; i++) {
		mask = 1 << i;
		if (map.searched_map_y[row] & mask) {
			myprintf("--");
		} else {
			myprintf("  ");
		}
		myprintf("+");
	}
	mask <<= 1;
	if (map.searched_map_y[row] & mask) {
		myprintf("--");
	} else {
		myprintf("  ");
	}

}

void print_Searched_map() {
	int i;
	myprintf("\n");
	for (i = 0; i < map.map_size; i++) {
		myprintf("+--");
	}
	myprintf("+\n|");

	print_Searched_map_x(map.map_size - 1);
	myprintf("  |\n+");
	for (i = 1; i < map.map_size; i++) {
		print_Searched_map_y(map.map_size - 1 - i);
		myprintf("+\n|");
		print_Searched_map_x(map.map_size - 1 - i);
		myprintf("  |\n+");
	}

	for (i = 0; i < map.map_size; i++) {
		myprintf("--+");
	}
	myprintf("\n\n");
}

void mix_map() {
	int i;
	for (i = 0; i < map.map_size - 1; i++) {
		map.mixed_map_x[i] = map.wall_map_x[i] | map.searched_map_x[i];
		map.mixed_map_y[i] = map.wall_map_y[i] | map.searched_map_y[i];
	}
}

void print_Mixed_map_x(int row) {
	int i, mask = 1;
	mask <<= row;

	for (i = 0; i < map.map_size - 1; i++) {
		myprintf("  ");
		if (map.mixed_map_x[i] & mask) {
			myprintf("|");
		} else {
			myprintf(" ");
		}
	}

}

void print_Mixed_map_y(int row) {
	int i, mask;
	for (i = 0; i < map.map_size - 1; i++) {
		mask = 1 << i;
		if (map.mixed_map_y[row] & mask) {
			myprintf("--");
		} else {
			myprintf("  ");
		}
		myprintf("+");
	}
	mask <<= 1;
	if (map.mixed_map_y[row] & mask) {
		myprintf("--");
	} else {
		myprintf("  ");
	}

}

void print_Mixed_map() {
	int i;
	myprintf("\n");
	for (i = 0; i < map.map_size; i++) {
		myprintf("+--");
	}
	myprintf("+\n|");

	print_Mixed_map_x(map.map_size - 1);
	myprintf("  |\n+");
	for (i = 1; i < map.map_size; i++) {
		print_Mixed_map_y(map.map_size - 1 - i);
		myprintf("+\n|");
		print_Mixed_map_x(map.map_size - 1 - i);
		myprintf("  |\n+");
	}

	for (i = 0; i < map.map_size; i++) {
		myprintf("--+");
	}
	myprintf("\n\n");
}
