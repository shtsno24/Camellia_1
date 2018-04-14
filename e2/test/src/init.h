/*
 * init.h
 *
 *  Created on: 2017/10/01
 *      Author: shts
 */

#ifndef INIT_H_
#define INIT_H_

//typedef struct spc {
//	float tread; //[mm]
//	float tire_dim; //[mm]
//	float step_dist; //[mm]
//
//	float vel_min; //[mm/s]
//	float vel_max; //[mm/s]
//	float tar_vel_min; //[mm/s]
//	float tar_vel_max; //[mm/s]
//
//	short sen_max_R;
//	short sen_max_CR;
//	short sen_max_L;
//	short sen_max_CL;
//
//	short sen_min_R;
//	short sen_min_CR;
//	short sen_min_L;
//	short sen_min_CL;
//
//	short sen_ref_R; //中央時のセンサ値
//	short sen_ref_L; //中央時のセンサ値
//	short sen_ref_wall_R; //壁切れ
//	short sen_ref_wall_L; //壁切れ
//	short sen_diff_threshold;
//
//	short sen_ref_non_CR;
//	short sen_ref_non_R;
//	short sen_ref_non_CL;
//	short sen_ref_non_L;
//
//	short pwm_base_clock; //set interrupt duty
//	short pwm_half_clock;
//
//} SPC;
//
typedef struct mot {
	int cnt;//ステップ数
	int duty;//デューティー

	float vel;//現在の速度
	float acc;//現在の設定加速度
	float max_vel;//最大設定速度
	float min_vel;//最小設定速度
	float max_acc;//最大設定加速度
	float min_acc;//最小設定加速度
	float tar_vel;//現在の設定速度

	char rot_dir_flag;//回転方向
	char end_flag;
	char stop_flag;

} MOT;

//typedef struct sen {
//	short sen;//現在データ
//	short old[9];//過去データ
//	short ref_wall;//中央時のセンサ値
//	short non_threshold;//壁なし
//	short diff_threshold;//壁切れ
//	short diff;//センサ差分
//} SEN;

//void initIO(void);

//void initCMT(void);

//void initMTU_S(void);

//void initSPEC(void);

//void initMTU(void);

void initMotor(void);

//void initAD(void);

//void init_CPU(void);

void initALL(void);

#endif /* INIT_H_ */
