/*
 * CMT.h
 *
 *  Created on: 2017/09/17
 *      Author: shts
 */

#ifndef CMT_H_
#define CMT_H_

typedef struct cmt_01 {
	int count_cmt_0;
	int count_cmt_1;
} CMT_01;

void init_CMT(void);

void interrupt_cmt0(void);

void interrupt_cmt1(void);

void wait_ms(int);

void sen_cmt1(void);
#endif /* CMT_H_ */
