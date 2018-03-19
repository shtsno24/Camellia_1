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

	float vel_min; //[mm/s]
	float vel_max; //[mm/s]
	float tar_vel_min; //[mm/s]
	float tar_vel_max; //[mm/s]

	short sen_max_R;
	short sen_max_CR;
	short sen_max_L;
	short sen_max_CL;

	short sen_min_R;
	short sen_min_CR;
	short sen_min_L;
	short sen_min_CL;

	short sen_ref_R; //中央時のセンサ値
	short sen_ref_L; //中央時のセンサ値
	short sen_ref_wall_R; //壁切れ
	short sen_ref_wall_L; //壁切れ
	short sen_diff_threshold;

	short sen_ref_non_CR;
	short sen_ref_non_R;
	short sen_ref_non_CL;
	short sen_ref_non_L;

	short pwm_base_clock; //set interrupt duty
	short pwm_half_clock;

} SPC;

typedef struct mot {
	int cnt;
	int duty;

	float vel;
	float acc;
	float max_vel;
	float min_vel;
	float max_acc;
	float min_acc;
	float tar_vel;

	char rot_dir_flag;
	char end_flag;
	char stop_flag;

} MOT;

typedef struct sen {
	short sen;
	short old[9];
	short ref_wall;
	short non_threshold;
	short diff_threshold;
	short diff;
} SEN;

void initIO(void);

void initCMT(void);

//void initMTU_S(void);

void initSPEC(void);

void initMTU(void);

void initMotor(void);

void initAD(void);

void initCPU(void);

void initALL(void);

#endif /* INIT_H_ */
