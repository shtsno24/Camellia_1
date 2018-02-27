/***************************************************************/
/*                                                             */
/*      PROJECT NAME :  test                                   */
/*      FILE         :  test.c                                 */
/*      DESCRIPTION  :  Main Program                           */
/*      CPU SERIES   :  SH-2                                   */
/*      CPU TYPE     :  SH7125                                 */
/*                                                             */
/*      This file is generated by e2 studio.                   */
/*                                                             */
/***************************************************************/

#include "typedefine.h"
#include "iodefine.h"
#include "serial.h"
#include "init.h"
#include "math.h"

#define round(A)((int)(A + 0.5))
#define map_size 17

char status, dist_flag_l = 0, dist_flag_r = 0, end_l = 0, end_r = 0, clt = 0,
		stop_l = 0, stop_r = 0, cnt_ctl = 0, rot_sw = 0, sta_LED_flag = 0,
		run_interruption = 0, direction = 0, pos_x, pos_y, wall;

int count_cmt_0 = 0, count_cmt_1, i, j, k, rot = 0, old = 0, cnt_l = 0, cnt_r =
		0, r_distance, l_distance, duty_R = 0, duty_R_h = 0, duty_L = 0,
		duty_L_h = 0, half_block, full_block, map_x[map_size], map_y[map_size];

double vel_l = 250, vel_r = 250, tar_vel_l = 300, tar_vel_r = 300, acc_l = 200,
		acc_r = 200;
float batt, diff, kp_r = 0.2, kp_l = 0.2;
short sen_diff_l = 0, sen_diff_r = 0, sen_old_L, sen_old_R;
short duty[17];
short sen[4];

extern SPC spec;
extern SEN r_sen, cr_sen, l_sen, cl_sen;
extern MOT r_motor, l_motor;

enum AD_C {
	ad_0 = 0, ad_1 = 1
};
enum sensor {
	CR_sen = 0, R_sen, CL_sen, L_sen //センサの順番通り
};

enum status_LED {
	Red = 0, Yerrow = 1, Green = 2, Rst_status_LED = 3, ALL_status_LED
};

enum sensor_LED {
	CR_LED = 0, R_LED = 1, CL_LED = 2, L_LED = 3
};

enum on_off {
	off = 0, on = 1
};

enum cst {
	cst0 = 0, cst1, cst2, cst3, cst_all
};

enum turn {
	back = 0, left = 1, right = 2, straight = 3,
};

enum motor {
	R_motor = 0, L_motor = 1
};

enum ux {
	error = 0, click = 1, alart = 2
};

enum mode {
	sen_cal = 0, search = 1, run = 2, test = 3
};

void pass(void) {
	//pass
}

float mysqrt(float x) {
	return 0.0;
}

int get_sensor(int ch, int ad_c) {

	switch (ad_c) {
	case ad_0:
		AD0.ADCR.BIT.ADST = 0;		//AD停止
		AD0.ADCSR.BIT.CH = ch;		//チャンネル選択
		AD0.ADCR.BIT.ADST = 1;		//AD再開
		while (AD0.ADCSR.BIT.ADF == 0)
			;
		AD0.ADCSR.BIT.ADF = 0;
		if (ch == CR_LED) {
			return AD0.ADDR0 >> 6;
		} else if (ch == R_LED) {
			return AD0.ADDR1 >> 6;
		} else if (ch == CL_LED) {
			return AD0.ADDR2 >> 6;
		} else if (ch == L_LED) {
			return AD0.ADDR3 >> 6;
		} else {
			return -1;
		}
		break;

	case ad_1:
		AD1.ADCR.BIT.ADST = 0;		//AD停止
		AD1.ADCSR.BIT.CH = ch;		//チャンネル選択
		AD1.ADCR.BIT.ADST = 1;		//AD再開
		while (AD1.ADCSR.BIT.ADF == 0)
			;
		AD1.ADCSR.BIT.ADF = 0;
		if (ch == 0) {
			return AD1.ADDR4 >> 6;
		} else if (ch == 1) {
			return AD1.ADDR5 >> 6;
		} else if (ch == 2) {
			return AD1.ADDR6 >> 6;
		} else if (ch == 3) {
			return AD1.ADDR7 >> 6;
		} else {
			return -1;
		}
		break;
	}

}

void sen_LED_drv(char led, char status) {
	/*
	 PE9 : sensor_right
	 PE11 : sensor_center_right
	 PE13 : sensor_center_left
	 PE12 : sensor_left
	 */
	switch (led) {
	case R_LED:
		PE.DRL.BIT.B9 = status;
		break;
	case CR_LED:
		PE.DRL.BIT.B11 = status;
		break;
	case CL_LED:
		PE.DRL.BIT.B13 = status;
		break;
	case L_LED:
		PE.DRL.BIT.B12 = status;
		break;
	}
}

void sta_LED_drv(char led, char status) {
	switch (led) {
	case Red:
		PE.DRL.BIT.B8 = status;
		break;
	case Yerrow:
		PE.DRL.BIT.B6 = status;
		break;
	case Green:
		PE.DRL.BIT.B10 = status;
		break;
	case Rst_status_LED:
		PE.DRL.BIT.B8 = 0;
		PE.DRL.BIT.B6 = 0;
		PE.DRL.BIT.B10 = 0;
		break;
	}
}

void interrupt_cmt0() {
// write this function to interrupt_handlers.c
	CMT0.CMCSR.BIT.CMF = 0;
	++count_cmt_0;
}
void interrupt_cmt1() {
// write this function to interrupt_handlers.c
	CMT1.CMCSR.BIT.CMF = 0;
	++count_cmt_1;
}

void wait_h_ms(int t) {
	count_cmt_1 = 0;
	CMT1.CMCNT = 0;
	while (count_cmt_1 < t)
		;
}

void wait_ms(int t) {
	count_cmt_0 = 0;
	CMT0.CMCNT = 0;
	while (count_cmt_0 < t)
		;
}

void diff_calc(void) {
	short ref_boost_L, ref_boost_R;

	if (abs(r_sen.diff) > r_sen.diff_threshold) {
		ref_boost_R = 450;  //変化量が一定以上なら、閾値を引き上げる
	} else {
		ref_boost_R = 0; //変化量が一定以下なら、設定通りの閾値
	}

	if (abs(l_sen.diff) > l_sen.diff_threshold) {
		ref_boost_L = 450;  //変化量が一定以上なら、閾値を引き上げる
	} else {
		ref_boost_L = 0; //変化量が一定以下なら、設定通りの閾値
	}

	if (cnt_ctl == 1 || cr_sen.ref_wall < cr_sen.sen
			|| cl_sen.ref_wall < cl_sen.sen) {
		diff = (float) (l_motor.cnt - r_motor.cnt) * 50;
		if (sta_LED_flag == 1) {
			sta_LED_drv(Green, on);
			sta_LED_drv(Yerrow, on);
			sta_LED_drv(Red, on);
		}
		return;
	} else {
		if ((r_sen.sen >= r_sen.non_threshold + ref_boost_R)
				&& (l_sen.sen >= l_sen.non_threshold + ref_boost_L)) {
			diff = (float) ((l_sen.sen - l_sen.ref_wall)
					- (r_sen.sen - r_sen.ref_wall));

			if (sta_LED_flag == 1) {
				sta_LED_drv(Green, on);
				sta_LED_drv(Yerrow, off);
				sta_LED_drv(Red, off);
			}

		} else if ((r_sen.sen >= r_sen.non_threshold + ref_boost_R)
				&& (l_sen.sen < l_sen.non_threshold + ref_boost_L)) {
			diff = (float) (-2 * (r_sen.sen - r_sen.ref_wall));

			if (sta_LED_flag == 1) {
				sta_LED_drv(Green, off);
				sta_LED_drv(Yerrow, on);
				sta_LED_drv(Red, off);
			}

		} else if ((r_sen.sen < r_sen.non_threshold + ref_boost_R)
				&& (l_sen.sen >= l_sen.non_threshold + ref_boost_L)) {
			diff = (float) (2 * (l_sen.sen - l_sen.ref_wall));

			if (sta_LED_flag == 1) {
				sta_LED_drv(Green, off);
				sta_LED_drv(Yerrow, off);
				sta_LED_drv(Red, on);
			}

		} else {
			diff = (float) (l_motor.cnt - r_motor.cnt) * 50;

			if (sta_LED_flag == 1) {
				sta_LED_drv(Green, off);
				sta_LED_drv(Yerrow, off);
				sta_LED_drv(Red, off);
			}

		}
	}

}

void vel_calc() {

	if (l_motor.tar_vel + kp_l * diff > l_motor.vel) {
		l_motor.vel = l_motor.vel + (l_motor.acc * 0.001);

	} else if (l_motor.tar_vel + kp_l * diff <= l_motor.vel) {
		l_motor.vel = l_motor.vel - (l_motor.acc * 0.001);
	}

	if (r_motor.tar_vel - kp_r * diff > r_motor.vel) {
		r_motor.vel = r_motor.vel + (r_motor.acc * 0.001);

	} else if (r_motor.tar_vel - kp_r * diff <= r_motor.vel) {
		r_motor.vel = r_motor.vel - (r_motor.acc * 0.001);

	}

}

void sen_cmt1() {
// write this function to interrupt_handlers.c
	CMT1.CMCSR.BIT.CMF = 0;
	CMT1.CMCNT = 0;

	sen_LED_drv(R_LED, on);
	for (i = 0; i < 100; i++)
		;
	r_sen.sen = get_sensor(R_sen, ad_0); //R sensor
	sen_LED_drv(R_LED, off);

	sen_LED_drv(CR_LED, on);
	for (i = 0; i < 100; i++)
		;
	cr_sen.sen = get_sensor(CR_sen, ad_0);		//CR sensor
	sen_LED_drv(CR_LED, off);

	sen_LED_drv(CL_LED, on);
	for (i = 0; i < 100; i++)
		;
	cl_sen.sen = get_sensor(CL_sen, ad_0);		//CL sensor
	sen_LED_drv(CL_LED, off);

	sen_LED_drv(L_LED, on);
	for (i = 0; i < 100; i++)
		;
	l_sen.sen = get_sensor(L_sen, ad_0);		//L sensor
	sen_LED_drv(L_LED, off);

	cr_sen.sen -= get_sensor(CR_sen, ad_0);		//CR sensor
	cl_sen.sen -= get_sensor(CL_sen, ad_0);		//CL sensor
	l_sen.sen -= get_sensor(L_sen, ad_0);		//L sensor
	r_sen.sen -= get_sensor(R_sen, ad_0);		//R sensor

	cr_sen.diff = cr_sen.sen - cr_sen.old[8];
	r_sen.diff = r_sen.sen - r_sen.old[8];
	cl_sen.diff = cl_sen.sen - cl_sen.old[8];
	l_sen.diff = l_sen.sen - l_sen.old[8];

	for (i = 1; i < 9; i++) {
		r_sen.old[i] = r_sen.old[i - 1];
		cr_sen.old[i] = cr_sen.old[i - 1];
		l_sen.old[i] = l_sen.old[i - 1];
		cl_sen.old[i] = cl_sen.old[i - 1];
	}

	cr_sen.old[0] = cr_sen.sen;		//CR sensor
	r_sen.old[0] = r_sen.sen;		//R sensor
	cl_sen.old[0] = cl_sen.sen;		//CL sensor
	l_sen.old[0] = l_sen.sen;		//L sensor

	diff_calc();

	vel_calc();

}

void stopMTU(int cst) {
//stop MTU2's timer
	switch (cst) {
	case cst0:
		MTU2.TSTR.BIT.CST0 = 0;
		break;
	case cst1:
		MTU2.TSTR.BIT.CST1 = 0;
		break;
	case cst2:
		MTU2.TSTR.BIT.CST2 = 0;
		break;
	case cst3:
		MTU2.TSTR.BIT.CST3 = 0;
		MTU2.TSTR.BIT.CST4 = 0;
		break;
	case cst_all:
		MTU2.TSTR.BIT.CST0 = 0;
		MTU2.TSTR.BIT.CST1 = 0;
		MTU2.TSTR.BIT.CST2 = 0;
		MTU2.TSTR.BIT.CST3 = 0;
		MTU2.TSTR.BIT.CST4 = 0;
		break;
	}
}

void startMTU(int cst) {
//start MTU2's timer
	switch (cst) {
	case cst0:
		MTU2.TSTR.BIT.CST0 = 1;
		break;
	case cst1:
		MTU2.TSTR.BIT.CST1 = 1;
		break;
	case cst2:
		MTU2.TSTR.BIT.CST2 = 1;
		break;
	case cst3:
		MTU2.TSTR.BIT.CST3 = 1;
		MTU2.TSTR.BIT.CST4 = 1;
		break;
	case cst_all:
		MTU2.TSTR.BIT.CST0 = 1;
		MTU2.TSTR.BIT.CST1 = 1;
		MTU2.TSTR.BIT.CST2 = 1;
		MTU2.TSTR.BIT.CST3 = 1;
		MTU2.TSTR.BIT.CST4 = 1;
		break;
	}
}

void MTU20_INT_OVF(void) {
	/*
	 * this function operates R_motor
	 * */

	stopMTU(cst0); //stop count
	r_motor.duty = (1 / r_motor.vel) * (2.55e+6); //(1/v)*(step_distance / MTU_clock_duty)
	r_motor.cnt--;

//change duty
	if (MTU20.TSR.BIT.TGFA == 1) {

		MTU20.TCNT = 0; //reset counter

		if (r_motor.duty < 0) {
			MTU20.TGRA = 0;
			MTU20.TGRB = 0;
		} else {
			MTU20.TGRA = r_motor.duty; //(1/v)*(step_distance / MTU_clock_duty)
			MTU20.TGRB = round(r_motor.duty / 2); //(1/v)*(step_distance / MTU_clock_duty)
		}

	}

	MTU20.TSR.BIT.TGFA = 0; //reset flag
	PE.DRL.BIT.B1 = r_motor.rot_dir_flag;
	if (r_motor.cnt < 0 && r_motor.end_flag == 1) {
		r_motor.stop_flag = 1;
	}

	if (r_motor.stop_flag == 1) {
		stopMTU(cst0);
	} else {
		startMTU(cst0);
	}

}

void MTU21_INT_OVF(void) {
	/*
	 * this function operates L_motor
	 */

	stopMTU(cst1); //stop count
	l_motor.duty = (1 / l_motor.vel) * (2.55e+6); //(1/v)*(step_distance / MTU_clock_duty)
	l_motor.cnt--;

	if (MTU21.TSR.BIT.TGFA == 1) {
		//change duty
		MTU21.TCNT = 0; //reset counter

		if (l_motor.duty < 0) {
			MTU21.TGRA = 0;
			MTU21.TGRB = 0;
		} else {
			MTU21.TGRA = l_motor.duty;
			MTU21.TGRB = round(l_motor.duty / 2);
		}
	}

	MTU21.TSR.BIT.TGFA = 0; //reset flag
	PE.DRL.BIT.B5 = l_motor.rot_dir_flag;
	if (l_motor.cnt < 0 && l_motor.end_flag == 1) {
		l_motor.stop_flag = 1;

	}
	if (l_motor.stop_flag == 1) {
		stopMTU(cst1);
	} else {
		startMTU(cst1);
	}
}

void buzzer_drv(char status) {
	PE.DRL.BIT.B7 = status;
}

void UX_effect(char pattern) {
	switch (pattern) {
	case error:
		sta_LED_drv(Red, on);
		buzzer_drv(on);
		wait_ms(25);
		sta_LED_drv(Red, off);
		buzzer_drv(off);
		wait_ms(25);
		break;

	case click:
		buzzer_drv(on);
		wait_ms(25);
		buzzer_drv(off);
		break;

	case alart:
		buzzer_drv(on);
		sta_LED_drv(Red, on);
		wait_ms(750);
		sta_LED_drv(Red, off);
		buzzer_drv(off);
		break;
	}
}

float batt_vol(void) {
	float vol_ad, voltage;
	vol_ad = (float) get_sensor(3, ad_1) / 1024.0 * 5.0;
	voltage = vol_ad * ((100.0 + 51.0) / 51.0);
	return voltage;
}

void mot_drv(int dist, int t_vel, int t_acc, char rot_dir_flag, int end_flag,
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
		startMTU(cst0);
		break;
	case L_motor:
		l_motor.stop_flag = 0;
		l_motor.cnt = dist / spec.step_dist;
		l_motor.tar_vel = t_vel;
		l_motor.acc = t_acc;
		l_motor.rot_dir_flag = rot_dir_flag; //L_CW/CCW(1 : forward, 0 : backward)
		l_motor.end_flag = end_flag;
		startMTU(cst1);
		break;
	}

	PE.DRL.BIT.B2 = 0; //reset(0 : off, 1 : on)
}

void mot_onoff(char sw) {
	if (sw == on) {
		PE.DRL.BIT.B3 = 1;
	} else {
		PE.DRL.BIT.B3 = 0;
	}
}

void mot_app(int dist, int t_vel, int t_acc, char move_flag, char end_flag) {

	if (move_flag == straight) {
		cnt_ctl = 0;
	} else {
		cnt_ctl = 1;
	}

	if (t_vel * t_vel - r_motor.vel * r_motor.vel > t_acc * dist / 2) {
		t_vel = sqrt(r_motor.vel * r_motor.vel + t_acc * dist);
	}

	mot_drv(dist / 4 * 3, t_vel, t_acc, move_flag & 1, end_flag, R_motor);
	mot_drv(dist / 4 * 3, t_vel, t_acc, (move_flag & 2) >> 1, end_flag,
			L_motor);

	while (1) {
		if (l_motor.stop_flag == 1 || r_motor.stop_flag == 1) {
			break;
		}
	}

	mot_drv(dist / 4, r_motor.min_vel, t_acc, move_flag & 1, end_flag, R_motor);
	mot_drv(dist / 4, l_motor.min_vel, t_acc, (move_flag & 2) >> 1, end_flag,
			L_motor);

	while (1) {
		if (l_motor.stop_flag == 1 || r_motor.stop_flag == 1) {
			break;
		}
	}
	cnt_ctl = 0;
}

void mot_app2(int dist, int t_vel, int t_acc, char move_flag, char end_flag) {

	if (move_flag == straight) {
		cnt_ctl = 0;
	} else {
		cnt_ctl = 1;
	}

	if (t_vel * t_vel - r_motor.vel * r_motor.vel > t_acc * dist / 2) {
		t_vel = sqrt(r_motor.vel * r_motor.vel + t_acc * dist);
	}

	mot_drv(dist, t_vel, t_acc, move_flag & 1, end_flag, R_motor);
	mot_drv(dist, t_vel, t_acc, (move_flag & 2) >> 1, end_flag, L_motor);

	while (1) {
		if (l_motor.stop_flag == 1 || r_motor.stop_flag == 1) {
			break;
		}
	}

	cnt_ctl = 0;
}

void sen_calibration() {

	UX_effect(alart);

	cr_sen.non_threshold = cr_sen.sen;
	cl_sen.non_threshold = cl_sen.sen;
	r_sen.ref_wall = r_sen.sen;
	l_sen.ref_wall = l_sen.sen;
	while (PB.DR.BIT.B5 != 0) {
		//remove walls
		sta_LED_drv(Green, on);
	}

	UX_effect(alart);
	myprintf("ref_wall : %d | %d\n", l_sen.ref_wall, r_sen.ref_wall);

	cr_sen.non_threshold += cr_sen.sen;
	cl_sen.non_threshold += cl_sen.sen;
	r_sen.non_threshold = r_sen.sen;
	l_sen.non_threshold = l_sen.sen;

	cr_sen.non_threshold /= 2;
	cl_sen.non_threshold /= 2;

	while (PB.DR.BIT.B5 != 0) {
		sta_LED_drv(Yerrow, on);
	}

	myprintf("non_wall : %d | %d | %d | %d\n", l_sen.non_threshold,
			cl_sen.non_threshold, cr_sen.non_threshold, r_sen.non_threshold);
	UX_effect(alart);
	sta_LED_drv(Rst_status_LED, off);
}

void Rotate_detect(void) {
	int now = PE.DRL.BIT.B14;
	now = now << 1;
	now += PE.DRL.BIT.B15;

	if (now == 3) {
		now = 2;
	} else if (now == 2) {
		now = 3;
	}

	if (old == 0 && now == 3) {
		rot -= 1;
		UX_effect(click);
	} else if (old == 3 && now == 0) {
		rot += 1;
		UX_effect(click);
	} else if (now > old) {
		rot += 1;
		UX_effect(click);
	} else if (now < old) {
		rot -= 1;
		UX_effect(click);
	} else {
		rot += 0;
	}
	old = now;

}

void mode_selector() {
	rot_sw = (char) (rot % 5);
	Rotate_detect();
	sta_LED_drv(Red, (rot_sw + 1) & 1);
	sta_LED_drv(Yerrow, ((rot_sw + 1) & 2) >> 1);
	sta_LED_drv(Green, ((rot_sw + 1) & 4) >> 2);
}

void direction_detect() {
	if (direction == 0) {
		pos_y += 1;
		/*
		 sta_LED_drv(Red, off);
		 sta_LED_drv(Yerrow, off);
		 sta_LED_drv(Green, on);
		 */
	} else if (direction == 1) {
		pos_x += 1;
		/*
		 sta_LED_drv(Red, on);
		 sta_LED_drv(Yerrow, off);
		 sta_LED_drv(Green, off);
		 */
	} else if (direction == 3) {
		pos_x -= 1;
		/*
		 sta_LED_drv(Red, off);
		 sta_LED_drv(Yerrow, on);
		 sta_LED_drv(Green, off);
		 */
	} else if (direction == 2) {
		pos_y -= 1;
		/*
		 sta_LED_drv(Red, off);
		 sta_LED_drv(Yerrow, off);
		 sta_LED_drv(Green, off);
		 */
	}
	sta_LED_drv(Red, pos_y & 1);
	sta_LED_drv(Yerrow, (pos_x & 1) >> 0);
	sta_LED_drv(Green, (pos_x & 2) >> 1);

}

void move_left() {
	mot_app(half_block, 305, 2000, straight, on);
	wait_ms(100);
	mot_onoff(off);
	wait_ms(450);
	mot_onoff(on);
	wait_ms(100);
	mot_app(l_distance, 310, 2000, left, on);
	wait_ms(100);
	mot_onoff(off);
	wait_ms(450);
	mot_onoff(on);
	wait_ms(100);
	mot_app2(half_block, 330, 2000, straight, on);
	//direction_detect();
}

void move_right() {
	mot_app(half_block, 305, 2000, straight, on);
	wait_ms(100);
	mot_onoff(off);

	wait_ms(450);
	mot_onoff(on);
	wait_ms(100);
	mot_app(r_distance, 310, 2000, right, on);
	wait_ms(100);
	mot_onoff(off);
	wait_ms(450);
	mot_onoff(on);
	wait_ms(100);
	mot_app2(half_block, 330, 2000, straight, on);
	//direction_detect();

}

void move_forward() {
	mot_app2(full_block, 410, 2000, straight, on);
	//direction_detect();
}

void move_back() {
	mot_app(half_block, 305, 2000, straight, on);
	wait_ms(1000);
	mot_app(r_distance, 310, 2000, right, on);
	wait_ms(1000);
	mot_app(half_block, 270, 2000, back, on);
	mot_onoff(off);
	wait_ms(1000);
	mot_onoff(on);
	mot_app(18, 280, 2000, straight, on);
	wait_ms(1000);
	mot_app(r_distance, 310, 2000, right, on);
	wait_ms(1000);
	mot_app(half_block, 270, 2000, back, on);
	mot_onoff(off);
	wait_ms(1000);
	mot_onoff(on);
	mot_app2(15 + half_block, 330, 2000, straight, on);
	//direction_detect();
}

void iter_map() {
	/*
	 * wall : W-S-E-N
	 * 		 MSB   LSB
	 *
	 * map_x : vertical walls to x-axis
	 * map_y : vertical walls to y-axis
	 * */

	wall = 0;
	if (r_sen.sen > r_sen.non_threshold) {
		wall |= 1 << ((1 + direction) % 4);
	}
	if (l_sen.sen > l_sen.non_threshold) {
		wall |= 1 << ((3 + direction) % 4);
	}
	if (cl_sen.sen > cl_sen.non_threshold) {
		wall |= 1 << ((0 + direction) % 4);
	}

	if (pos_x < map_size) {
		map_x[pos_x] |= ((wall & 2) >> 1) << pos_y;
	}
	if (pos_x - 1 >= 0) {
		map_x[pos_x - 1] |= ((wall & 8) >> 3) << pos_y;
	}

	if (pos_y < map_size) {
		map_y[pos_y] |= ((wall & 1) >> 0) << pos_x;
	}
	if (pos_y - 1 >= 0) {
		map_y[pos_y - 1] |= ((wall & 4) >> 2) << pos_x;
	}
}

void print_xmap(int row) {
	int i, mask = 1;
	mask <<= row;
	myprintf("|");
	for (i = 0; i < map_size; i++) {
		myprintf("  ");
		if (map_x[i] & mask) {
			myprintf("|");
		} else {
			myprintf(" ");
		}
	}
	myprintf("\n", mask);
	//myprintf(" |\n");
}

void print_ymap(int row) {
	int i, mask;
	for (i = 0; i < map_size; i++) {
		myprintf("+");
		mask = 1 << i;
		if (map_y[row] & mask) {
			myprintf("--");
		} else {
			myprintf("  ");
		}
	}
	myprintf("+\n");
}

void print_map() {
	int i, buff_x, buff_y;
	for (i = 0; i < map_size; i++) {
		print_ymap(map_size - 1 - i);
		print_xmap(map_size - 1 - i);
	}
	for (i = 0; i < map_size; i++) {
		myprintf("+--");
	}
	myprintf("+\n");
	/*
	for (i = 0; i < map_size; i++) {
		myprintf("%d\n", map_x[map_size - 1 - i]);
		myprintf("%d\n", map_y[map_size - 1 - i]);
	}
    */
}

int main(void) {
	float v_r = 0;
	float v_l = 0;
	sta_LED_flag = 0;
	init_sci1();
	initALL();
	mot_onoff(off);
	PE.DRL.BIT.B7 = 0;

	duty[0] = 6250 - 1;		//(1ms)
	duty[1] = round(duty[0] / 2);		//(about 0.5ms)
	duty[4] = 6250 - 1;		//(1ms)
	duty[5] = round(duty[4] / 2);		//(about 0.5ms)

	half_block = 92;
	full_block = 178.5;
	r_distance = (int) ((90.0 / 180 * 3.141592) * (spec.tire_dim / 2)) + 1;
	l_distance = (int) ((90.0 / 180 * 3.141592) * (spec.tire_dim / 2)) - 0.5;

	for (i = 0; i < map_size; i++) {
		map_x[i] = 0;
		map_y[i] = 0;
	}

	while (1) {
		while (batt_vol() < 11.3) {
			myprintf("Low_battery\n");
			UX_effect(error);
		}

		sta_LED_drv(Red, off);

		while (PB.DR.BIT.B5 != 0) {
			batt = batt_vol();
			mode_selector();
			myprintf("batt : %f\n", batt);
			myprintf("sen : %d | %d | %d | %d\n", l_sen.sen, cl_sen.sen,
					cr_sen.sen, r_sen.sen);
			//myprintf("sen : %d | %d | %d | %d\n", l_sen.diff, cl_sen.diff,
			//cr_sen.diff, r_sen.diff);
			myprintf("mode : %d\n", rot_sw);
			wait_ms(100);
		}
		wait_ms(100);
		sta_LED_drv(Rst_status_LED, off);
		switch (rot_sw) {

		case sen_cal:

			//sen_calibration();

			sta_LED_flag = 1;

			UX_effect(alart);
			mot_onoff(on);
			wait_ms(100);
			mot_app2(half_block, 330, 2000, straight, on);
			mot_app(half_block, 295, 2000, straight, on);
			wait_ms(100);
			mot_onoff(off);

			while (PB.DR.BIT.B5 != 0)
				;
			UX_effect(alart);
			mot_onoff(on);
			wait_ms(100);
			mot_app(r_distance, 310, 2000, right, on);
			wait_ms(100);
			mot_onoff(off);

			while (PB.DR.BIT.B5 != 0)
				;
			UX_effect(alart);
			mot_onoff(on);
			wait_ms(100);
			mot_app(l_distance, 310, 2000, left, on);
			wait_ms(100);
			mot_onoff(off);

			while (PB.DR.BIT.B5 != 0)
				;
			UX_effect(alart);
			mot_onoff(on);
			wait_ms(100);
			mot_app2(half_block, 400, 2000, straight, on);
			mot_app2(full_block * 14, 400, 2000, straight, on);
			mot_app(half_block, 400, 2000, straight, on);
			wait_ms(100);
			mot_onoff(off);

			sta_LED_flag = 0;
			break;

		case search:
			k = 0;
			sta_LED_flag = 0;
			pos_x = 0;
			pos_y = 1;
			UX_effect(alart);
			mot_onoff(on);
			mot_app2(half_block, 310, 1500, straight, on);

			while (run_interruption != 1) {
				iter_map();
				/*
				 while (PB.DR.BIT.B5 != 0) {
				 }
				 UX_effect(alart);

				 myprintf("%d | %d \n", wall, direction);
				 while (PB.DR.BIT.B5 != 0) {
				 }
				 UX_effect(alart);
				 */
				if (r_sen.sen <= r_sen.non_threshold) {
					direction += 1;
					move_right();

				} else if (l_sen.sen <= l_sen.non_threshold) {
					direction += 3;
					move_left();
				} else if (cl_sen.sen <= cl_sen.non_threshold) {
					direction += 0;
					move_forward();
				} else {
					direction += 2;
					move_back();
				}
				direction %= 4;

				direction_detect();
				if (pos_x == 0 & pos_y == 0) {
					run_interruption = 1;
				}
			}
			mot_app(half_block, 310, 1500, straight, on);
			wait_ms(300);
			sta_LED_flag = 0;
			break;

		case run:
			sta_LED_flag = 0;
			UX_effect(alart);
			print_map();
			break;

		case test:
			sta_LED_flag = 1;
			UX_effect(alart);
			mot_onoff(on);
			wait_ms(100);
			mot_app(full_block * 5, 400, 500, straight, on);
			wait_ms(100);
			sta_LED_flag = 0;
			mot_onoff(off);
			break;
		}

		mot_onoff(off);

	}

}

