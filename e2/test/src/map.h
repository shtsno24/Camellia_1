/*
 * map.h
 *
 *  Created on: 2018/04/14
 *      Author: shts
 */

#ifndef MAP_H_
#define MAP_H_

typedef struct map {
	unsigned char pos_x;
	unsigned char pos_y;
	unsigned char goal_x;
	unsigned char goal_y;
	unsigned char direction;
	unsigned char wall;
	unsigned char tmp_path;
	unsigned char map_size;
	unsigned char path[16 * 16];
	unsigned char dist_map[16][16];
	unsigned char a_dist_map[16][16];
	unsigned int wall_map_x[16 - 1];
	unsigned int wall_map_y[16 - 1];
	unsigned int searched_map_x[16 - 1];
	unsigned int searched_map_y[16 - 1];
	unsigned int mixed_map_x[16 - 1];
	unsigned int mixed_map_y[16 - 1];
} MAP;

#endif /* MAP_H_ */
