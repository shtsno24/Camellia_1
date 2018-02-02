/*
 * init.h
 *
 *  Created on: 2017/10/01
 *      Author: shts
 */

#ifndef INIT_H_
#define INIT_H_

typedef struct spc {
	float tread; //[mm]
	float tire_dim; //[mm]
	float step_dist; //[mm]

	float vel_min;//[mm/s]
	float vel_max;//[mm/s]
	float tar_vel_min;//[mm/s]
	float tar_vel_max;//[mm/s]

	short sen_max_R;
	short sen_max_CR;
	short sen_max_L;
	short sen_max_CL;

	short sen_min_R;
	short sen_min_CR;
	short sen_min_L;
	short sen_min_CL;

	short sen_ref_R;//中央時のセンサ値
	short sen_ref_L;//中央時のセンサ値
	short sen_ref_wall_R;//壁切れ
	short sen_ref_wall_L;//壁切れ
	short sen_diff_threshold;

	short sen_ref_non_CR;
	short sen_ref_non_R;
	short sen_ref_non_CL;
	short sen_ref_non_L;

	short pwm_base_clock; //set interrupt duty
	short pwm_half_clock;

} SPEC;

void initIO(void);

void initCMT(void);

//void initMTU_S(void);

void initSPEC(void);

void initMTU(void);

void initMotor(void);

void initAD(void);

void initALL(void);

#endif /* INIT_H_ */
