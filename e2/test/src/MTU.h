/*
 * MTU.h
 *
 *  Created on: 2017/09/17
 *      Author: shts
 */

#ifndef MTU_H_
#define MTU_H_

enum cst {
	cst0 = 0, cst1, cst2, cst3, cst_all
};

void init_MTU(void);

void stop_MTU(char);

void start_MTU(char);

#endif /* MTU_H_ */
