/*
 * motor.c
 *
 *  Created on: 2018/04/14
 *      Author: shts
 */
#include "iodefine.h"
#include "motor.h"
#include "util.h"
#include "MTU.h"

MOT r_motor, l_motor;
extern SPC spec;

void init_Motor(void) {
	r_motor.min_acc = 0.0;
	l_motor.min_acc = 0.0;
	r_motor.max_acc = 1e+5;
	l_motor.max_acc = 1e+5;
	r_motor.acc = 0.0;
	l_motor.acc = 0.0;

	r_motor.min_vel = 240.0;
	l_motor.min_vel = 240.0;
	r_motor.max_vel = 2000.0;
	l_motor.max_vel = 2000.0;
	r_motor.vel = 300.0;
	l_motor.vel = 300.0;

	r_motor.cnt = 0.0;
	l_motor.cnt = 0.0;

	r_motor.end_flag = 0.0;
	l_motor.end_flag = 0.0;
	r_motor.stop_flag = 0.0;
	l_motor.stop_flag = 0.0;

	r_motor.rot_dir_flag = 0;
	l_motor.rot_dir_flag = 1;

	r_motor.dist = 0.0;

	PE.DRL.BIT.B2 = 0; //reset (0 : off, 1 : on)
	PE.DRL.BIT.B3 = 0; //excitation_enable (1 : on, 0 : off)

	PE.DRL.BIT.B1 = 0; //R_CW/CCW(0 : forward, 1 : backward)
	PE.DRL.BIT.B5 = 1; //L_CW/CCW(1 : forward, 0 : backward)

	PE.DRL.BIT.B0 = 1; //R_Clock
	PE.DRL.BIT.B4 = 1; //L_Clock

	PE.DRL.BIT.B2 = 0; //reset(0 : off, 1 : on)
	PE.DRL.BIT.B3 = 1; //excitation_enable (1 : on, 0 : off)
}

void drv_Motor(int dist, int t_vel, int t_acc, char rot_dir_flag, int end_flag,
		int ch) {
	/*
	 rot_dir
	 1 : forward
	 0 : backward
	 */

	if (t_acc > r_motor.max_acc) {
		t_acc = r_motor.max_acc;
	} else if (t_acc < r_motor.min_acc) {
		t_acc = r_motor.min_acc;
	}

	if (t_vel > r_motor.max_vel) {
		t_vel = r_motor.max_vel;
	} else if (t_vel < r_motor.min_vel) {
		t_vel = r_motor.min_vel;
	}
	PE.DRL.BIT.B2 = 1; //reset(0 : off, 1 : on)
	switch (ch) {
	case R_motor:
		r_motor.stop_flag = 0;
		r_motor.cnt = dist / spec.step_dist;
		r_motor.tar_vel = t_vel;
		r_motor.acc = t_acc;
		r_motor.rot_dir_flag = !rot_dir_flag; //R_CW/CCW(0 : forward, 1 : backward)
		r_motor.end_flag = end_flag;
		start_MTU(cst0);
		break;
	case L_motor:
		l_motor.stop_flag = 0;
		l_motor.cnt = dist / spec.step_dist;
		l_motor.tar_vel = t_vel;
		l_motor.acc = t_acc;
		l_motor.rot_dir_flag = rot_dir_flag; //L_CW/CCW(1 : forward, 0 : backward)
		l_motor.end_flag = end_flag;
		start_MTU(cst1);
		break;
	}

	PE.DRL.BIT.B2 = 0; //reset(0 : off, 1 : on)
}

void switch_Motor(char sw) {
	if (sw == on) {
		PE.DRL.BIT.B3 = 1;
	} else {
		PE.DRL.BIT.B3 = 0;
	}
}

void mot_app(int dist, int t_vel, int t_acc, char move_flag, char end_flag) {

	if (move_flag == straight) {
		spec.cnt_ctl = 0;
	} else {
		spec.cnt_ctl = 1;
	}

	if (t_vel * t_vel - r_motor.vel * r_motor.vel > t_acc * dist / 2) {
		t_vel = sqrt(r_motor.vel * r_motor.vel + t_acc * dist);
	}

	drv_Motor(dist / 4 * 3, t_vel, t_acc, move_flag & 1, end_flag, R_motor);
	drv_Motor(dist / 4 * 3, t_vel, t_acc, (move_flag & 2) >> 1, end_flag,
			L_motor);

	while (1) {
		if (l_motor.stop_flag == 1 || r_motor.stop_flag == 1) {
			break;
		}
	}

	drv_Motor(dist / 4, r_motor.min_vel, t_acc, move_flag & 1, end_flag, R_motor);
	drv_Motor(dist / 4, l_motor.min_vel, t_acc, (move_flag & 2) >> 1, end_flag,
			L_motor);

	while (1) {
		if (l_motor.stop_flag == 1 || r_motor.stop_flag == 1) {
			break;
		}
	}
	spec.cnt_ctl = 0;
}

void mot_app2(int dist, int t_vel, int t_acc, char move_flag, char end_flag) {

	if (move_flag == straight) {
		spec.cnt_ctl = 0;
	} else {
		spec.cnt_ctl = 1;
	}

	if (t_vel * t_vel - r_motor.vel * r_motor.vel > t_acc * dist / 2) {
		t_vel = sqrt(r_motor.vel * r_motor.vel + t_acc * dist);
	}

	drv_Motor(dist, t_vel, t_acc, move_flag & 1, end_flag, R_motor);
	drv_Motor(dist, t_vel, t_acc, (move_flag & 2) >> 1, end_flag, L_motor);

	while (1) {
		if (l_motor.stop_flag == 1 || r_motor.stop_flag == 1) {
			break;
		}
	}

	spec.cnt_ctl = 0;
}

void move_Left() {
	spec.kp_r -= 0.1;
	spec.kp_l -= 0.1;
	mot_app(spec.half_block, 330, 2000, straight, on);
	wait_ms(100);
	switch_Motor(off);
	wait_ms(450);
	switch_Motor(on);
	wait_ms(100);
	mot_app(spec.l_distance, 310, 2000, left, on);
	wait_ms(100);
	switch_Motor(off);
	wait_ms(450);
	switch_Motor(on);
	wait_ms(100);
	mot_app2(spec.half_block, 330, 2000, straight, on);
	spec.kp_r += 0.1;
	spec.kp_l += 0.1;
}

void move_Right() {
	spec.kp_r -= 0.1;
	spec.kp_l -= 0.1;
	mot_app(spec.half_block, 330, 2000, straight, on);
	wait_ms(100);
	switch_Motor(off);
	wait_ms(450);
	switch_Motor(on);
	wait_ms(100);
	mot_app(spec.r_distance, 310, 2000, right, on);
	wait_ms(100);
	switch_Motor(off);
	wait_ms(450);
	switch_Motor(on);
	wait_ms(100);
	mot_app2(spec.half_block, 330, 1800, straight, on);
	spec.kp_r += 0.1;
	spec.kp_l += 0.1;

}

void move_Forward() {
	spec.kp_r += 0.1;
	spec.kp_l += 0.1;
	mot_app2(spec.full_block, 480, 2000, straight, on);
	spec.kp_r -= 0.1;
	spec.kp_l -= 0.1;
}

void move_Backward() {
	mot_app(spec.half_block, 330, 2000, straight, on);
	wait_ms(1000);
	mot_app(spec.r_distance, 310, 2000, right, on);
	wait_ms(1000);
	mot_app(spec.half_block, 270, 2000, back, on);
	switch_Motor(off);
	wait_ms(1000);
	switch_Motor(on);
	mot_app(18, 280, 2000, straight, on);
	wait_ms(1000);
	mot_app(spec.r_distance, 310, 2000, right, on);
	wait_ms(1000);
	mot_app(spec.half_block, 270, 2000, back, on);
	switch_Motor(off);
	wait_ms(1000);
	switch_Motor(on);
	mot_app2(15 + spec.half_block, 330, 2000, straight, on);

}

void move_Backward_2() {
	mot_app(spec.half_block, 330, 2000, straight, on);
	wait_ms(100);
	switch_Motor(off);
	wait_ms(450);
	switch_Motor(on);
	wait_ms(100);
	mot_app(spec.r_distance, 310, 2000, right, on);
	mot_app(spec.r_distance, 310, 2000, right, on);
	wait_ms(100);
	switch_Motor(off);
	wait_ms(450);
	switch_Motor(on);
	wait_ms(100);
	mot_app2(spec.half_block, 330, 2000, straight, on);

}
