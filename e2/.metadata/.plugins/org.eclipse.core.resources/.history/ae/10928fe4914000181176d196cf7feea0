/*
 * map.h
 *
 *  Created on: 2018/04/14
 *      Author: shts
 */

#ifndef MAP_H_
#define MAP_H_

#define mp_size 16

typedef struct map {
	unsigned char pos_x;
	unsigned char pos_y;
	unsigned char goal_x;
	unsigned char goal_y;
	unsigned char direction;
	unsigned char wall;
	unsigned char tmp_path;
	unsigned char map_size;
	unsigned char path[mp_size * mp_size];
	unsigned char dist_map[mp_size][mp_size];
	unsigned char a_dist_map[mp_size][mp_size];
	unsigned int wall_map_x[mp_size - 1];
	unsigned int wall_map_y[mp_size - 1];
	unsigned int searched_map_x[mp_size - 1];
	unsigned int searched_map_y[mp_size - 1];
	unsigned int mixed_map_x[mp_size - 1];
	unsigned int mixed_map_y[mp_size - 1];
} MAP;

void init_Map();

void Detect_direction();

#endif /* MAP_H_ */
