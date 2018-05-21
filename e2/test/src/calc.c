/*
 * calc.c
 *
 *  Created on: 2018/04/14
 *      Author: shts
 */

#include "iodefine.h"
#include "sensor.h"
#include "motor.h"
#include "calc.h"
#include "math.h"
#include "util.h"
#include "LED.h"

extern SEN r_sen, l_sen, cr_sen, cl_sen;
extern SPC spec;
extern MOT r_motor, l_motor;

void diff_calc(void) {
	short ref_boost_L, ref_boost_R;

	if (abs(r_sen.diff) > r_sen.diff_threshold) {
		ref_boost_R = 80;  //変化量が一定以上なら、閾値を引き上げる
	} else {
		ref_boost_R = 0; //変化量が一定以下なら、設定通りの閾値
	}

	if (abs(l_sen.diff) > l_sen.diff_threshold) {
		ref_boost_L = 80;  //変化量が一定以上なら、閾値を引き上げる
	} else {
		ref_boost_L = 0; //変化量が一定以下なら、設定通りの閾値
	}

	if (spec.cnt_ctl == 1 || cr_sen.ref_wall < cr_sen.sen
			|| cl_sen.ref_wall < cl_sen.sen) {
		spec.diff = (float) (l_motor.cnt - r_motor.cnt) * 10;
		if (spec.sta_LED_flag == 1) {
			drv_Status_LED(Green, on);
			drv_Status_LED(Yerrow, on);
			drv_Status_LED(Red, on);
		}
		return;
	} else {
		if ((r_sen.sen >= r_sen.non_threshold + ref_boost_R)
				&& (l_sen.sen >= l_sen.non_threshold + ref_boost_L)) {
			spec.diff = (float) ((l_sen.sen - l_sen.ref_wall)
					- (r_sen.sen - r_sen.ref_wall));

			if (spec.sta_LED_flag == 1) {
				drv_Status_LED(Green, on);
				drv_Status_LED(Yerrow, off);
				drv_Status_LED(Red, off);
			}

		} else if ((r_sen.sen >= r_sen.non_threshold + ref_boost_R)
				&& (l_sen.sen < l_sen.non_threshold + ref_boost_L)) {
			spec.diff = (float) (-2 * (r_sen.sen - r_sen.ref_wall));

			if (spec.sta_LED_flag == 1) {
				drv_Status_LED(Green, off);
				drv_Status_LED(Yerrow, on);
				drv_Status_LED(Red, off);
			}

		} else if ((r_sen.sen < r_sen.non_threshold + ref_boost_R)
				&& (l_sen.sen >= l_sen.non_threshold + ref_boost_L)) {
			spec.diff = (float) (2 * (l_sen.sen - l_sen.ref_wall));

			if (spec.sta_LED_flag == 1) {
				drv_Status_LED(Green, off);
				drv_Status_LED(Yerrow, off);
				drv_Status_LED(Red, on);
			}

		} else {
			spec.diff = (float) (l_motor.cnt - r_motor.cnt) * 10;

			if (spec.sta_LED_flag == 1) {
				drv_Status_LED(Green, off);
				drv_Status_LED(Yerrow, off);
				drv_Status_LED(Red, off);
			}

		}
	}

}

void vel_calc() {

	if (l_motor.tar_vel + spec.kp_l * spec.diff > l_motor.vel) {
		l_motor.vel += (l_motor.acc * 0.001);

	} else if (l_motor.tar_vel + spec.kp_l * spec.diff <= l_motor.vel) {
		l_motor.vel -= (l_motor.acc * 0.001);
	}

	if (r_motor.tar_vel - spec.kp_r * spec.diff > r_motor.vel) {
		r_motor.vel +=  (r_motor.acc * 0.001);

	} else if (r_motor.tar_vel - spec.kp_r * spec.diff <= r_motor.vel) {
		r_motor.vel -=  (r_motor.acc * 0.001);
	}
}
