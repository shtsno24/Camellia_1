/*
 * util.h
 *
 *  Created on: 2018/04/13
 *      Author: shts
 */

#ifndef UTIL_H_
#define UTIL_H_

enum ux {
	error = 0, click = 1, alart = 2
};

enum on_off {
	off = 0, on = 1
};


typedef struct spc {
	float tread; //[mm]
	float tire_dim; //[mm]
	float step_dist; //[mm]

	short pwm_base_clock; //set interrupt duty
	short pwm_half_clock;

	float vel_min;
	float tar_vel_min;
	float batt;
	float kp_l;
	float kp_r;
	float diff;
	unsigned char cnt_ctl;
	unsigned char run_interruption;

} SPC;

typedef struct sw {
	unsigned char rot_sw; //ロータリーエンコーダ
	unsigned char rot_sw_now; //ロータリーエンコーダ
	unsigned char rot_sw_old; //ロータリーエンコーダ

	unsigned char tact_sw; //タクトスイッチ

} SW;

void init_Spec(void);

void init_IO(void);

void init_CPU(void);

void UX_effect(char);

void init_Buzzer(void);

void drv_Buzzer(char);

void detect_Rotate(void);

void select_Mode(void);
#endif /* UTIL_H_ */