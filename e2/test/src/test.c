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
#include "util.h"
#include "LED.h"
#include "CMT.h"
#include "sensor.h"
#include "MTU.h"
#include "motor.h"
#include "map.h"
#include "math.h"

#define round(A)((int)(A + 0.5))
//#define map_size 16

/*

char status, dist_flag_l = 0, dist_flag_r = 0, end_l = 0, end_r = 0, stop_l = 0,
		stop_r = 0, cnt_ctl = 0, rot_sw = 0, sta_LED_flag = 0,
		run_interruption = 0, direction = 0, pos_x, pos_y, wall, path[map_size
				* map_size], tmp_path, goal_x = 1, goal_y = 0;

unsigned char dist_map[map_size][map_size], a_star_dist_map[map_size][map_size];

int count_cmt_0 = 0, count_cmt_1, i, j, k, rot = 0, old = 0, cnt_l = 0, cnt_r =
		0, r_distance, l_distance, duty_R = 0, duty_R_h = 0, duty_L = 0,
		duty_L_h = 0, half_block, full_block, route_index;

unsigned int wall_map_x[map_size - 1], wall_map_y[map_size - 1],
		searched_map_x[map_size - 1], searched_map_y[map_size - 1],
		mixed_map_x[map_size - 1], mixed_map_y[map_size - 1];

double vel_l = 250, vel_r = 250, tar_vel_l = 300, tar_vel_r = 300, acc_l = 200,
		acc_r = 200;

float batt, diff, //kp_r = 0.175, kp_l = 0.175;
		kp_r = 0.19, kp_l = 0.19;
*/

int route_index;

extern SPC spec;
extern SEN r_sen, cr_sen, l_sen, cl_sen;
extern MOT r_motor, l_motor;
extern SW Switch;
extern CMT_01 tim;
extern MAP map;

//enum AD_C {
//	ad_0 = 0, ad_1 = 1
//};
//enum sensor {
//	CR_sen = 0, R_sen, CL_sen, L_sen //センサの順番通り
//};

//enum status_LED {
//	Red = 0, Yerrow = 1, Green = 2, Rst_status_LED = 3, ALL_status_LED
//};
//
//enum sensor_LED {
//	CR_LED = 0, R_LED = 1, CL_LED = 2, L_LED = 3
//};

//enum on_off {
//	off = 0, on = 1
//};

//enum cst {
//	cst0 = 0, cst1, cst2, cst3, cst_all
//};

//enum turn {
//	back = 0, left = 1, right = 2, straight = 3,
//};

//enum motor {
//	R_motor = 0, L_motor = 1
//};

//enum ux {
//	error = 0, click = 1, alart = 2
//};

enum mode {
	astar = 0, search = 1, show_map = 2, test = 3, run = 4
};

//int get_Sensor(int ch, int ad_c) {
//
//	switch (ad_c) {
//	case ad_0:
//		AD0.ADCR.BIT.ADST = 0;		//AD停止
//		AD0.ADCSR.BIT.CH = ch;		//チャンネル選択
//		AD0.ADCR.BIT.ADST = 1;		//AD再開
//		while (AD0.ADCSR.BIT.ADF == 0)
//			;
//		AD0.ADCSR.BIT.ADF = 0;
//		if (ch == CR_LED) {
//			return AD0.ADDR0 >> 6;
//		} else if (ch == R_LED) {
//			return AD0.ADDR1 >> 6;
//		} else if (ch == CL_LED) {
//			return AD0.ADDR2 >> 6;
//		} else if (ch == L_LED) {
//			return AD0.ADDR3 >> 6;
//		} else {
//			return -1;
//		}
//		break;
//
//	case ad_1:
//		AD1.ADCR.BIT.ADST = 0;		//AD停止
//		AD1.ADCSR.BIT.CH = ch;		//チャンネル選択
//		AD1.ADCR.BIT.ADST = 1;		//AD再開
//		while (AD1.ADCSR.BIT.ADF == 0)
//			;
//		AD1.ADCSR.BIT.ADF = 0;
//		if (ch == 0) {
//			return AD1.ADDR4 >> 6;
//		} else if (ch == 1) {
//			return AD1.ADDR5 >> 6;
//		} else if (ch == 2) {
//			return AD1.ADDR6 >> 6;
//		} else if (ch == 3) {
//			return AD1.ADDR7 >> 6;
//		} else {
//			return -1;
//		}
//		break;
//	}
//}

//void drv_Sensor_LED(char led, char status) {
//	/*
//	 PE9 : sensor_right
//	 PE11 : sensor_center_right
//	 PE13 : sensor_center_left
//	 PE12 : sensor_left
//	 */
//	switch (led) {
//	case R_LED:
//		PE.DRL.BIT.B9 = status;
//		break;
//	case CR_LED:
//		PE.DRL.BIT.B11 = status;
//		break;
//	case CL_LED:
//		PE.DRL.BIT.B13 = status;
//		break;
//	case L_LED:
//		PE.DRL.BIT.B12 = status;
//		break;
//	}
//}
//
//void sta_LED_drv(char led, char status) {
//	switch (led) {
//	case Red:
//		PE.DRL.BIT.B8 = status;
//		break;
//	case Yerrow:
//		PE.DRL.BIT.B6 = status;
//		break;
//	case Green:
//		PE.DRL.BIT.B10 = status;
//		break;
//	case Rst_status_LED:
//		PE.DRL.BIT.B8 = 0;
//		PE.DRL.BIT.B6 = 0;
//		PE.DRL.BIT.B10 = 0;
//		break;
//	}
//}

//void interrupt_cmt0() {
// write this function to interrupt_handlers.c
//	CMT0.CMCSR.BIT.CMF = 0;
//	++count_cmt_0;
//}
//void interrupt_cmt1() {
// write this function to interrupt_handlers.c
//	CMT1.CMCSR.BIT.CMF = 0;
//	++count_cmt_1;
//}
//
//void wait_ms(int t) {
//	count_cmt_0 = 0;
//	CMT0.CMCNT = 0;
//	while (count_cmt_0 < t)
//		;
//}

//void diff_calc(void) {
//	short ref_boost_L, ref_boost_R;
//
//	if (abs(r_sen.diff) > r_sen.diff_threshold) {
//		ref_boost_R = 450;  //変化量が一定以上なら、閾値を引き上げる
//	} else {
//		ref_boost_R = 0; //変化量が一定以下なら、設定通りの閾値
//	}
//
//	if (abs(l_sen.diff) > l_sen.diff_threshold) {
//		ref_boost_L = 450;  //変化量が一定以上なら、閾値を引き上げる
//	} else {
//		ref_boost_L = 0; //変化量が一定以下なら、設定通りの閾値
//	}
//
//	if (cnt_ctl == 1 || cr_sen.ref_wall < cr_sen.sen
//			|| cl_sen.ref_wall < cl_sen.sen) {
//		diff = (float) (l_motor.cnt - r_motor.cnt) * 50;
//		if (sta_LED_flag == 1) {
//			drv_Status_LED(Green, on);
//			drv_Status_LED(Yerrow, on);
//			drv_Status_LED(Red, on);
//		}
//		return;
//	} else {
//		if ((r_sen.sen >= r_sen.non_threshold + ref_boost_R)
//				&& (l_sen.sen >= l_sen.non_threshold + ref_boost_L)) {
//			diff = (float) ((l_sen.sen - l_sen.ref_wall)
//					- (r_sen.sen - r_sen.ref_wall));
//
//			if (sta_LED_flag == 1) {
//				drv_Status_LED(Green, on);
//				drv_Status_LED(Yerrow, off);
//				drv_Status_LED(Red, off);
//			}
//
//		} else if ((r_sen.sen >= r_sen.non_threshold + ref_boost_R)
//				&& (l_sen.sen < l_sen.non_threshold + ref_boost_L)) {
//			diff = (float) (-2 * (r_sen.sen - r_sen.ref_wall));
//
//			if (sta_LED_flag == 1) {
//				drv_Status_LED(Green, off);
//				drv_Status_LED(Yerrow, on);
//				drv_Status_LED(Red, off);
//			}
//
//		} else if ((r_sen.sen < r_sen.non_threshold + ref_boost_R)
//				&& (l_sen.sen >= l_sen.non_threshold + ref_boost_L)) {
//			diff = (float) (2 * (l_sen.sen - l_sen.ref_wall));
//
//			if (sta_LED_flag == 1) {
//				drv_Status_LED(Green, off);
//				drv_Status_LED(Yerrow, off);
//				drv_Status_LED(Red, on);
//			}
//
//		} else {
//			diff = (float) (l_motor.cnt - r_motor.cnt) * 50;
//
//			if (sta_LED_flag == 1) {
//				drv_Status_LED(Green, off);
//				drv_Status_LED(Yerrow, off);
//				drv_Status_LED(Red, off);
//			}
//
//		}
//	}
//
//}
//
//void vel_calc() {
//
//	if (l_motor.tar_vel + kp_l * diff > l_motor.vel) {
//		l_motor.vel = l_motor.vel + (l_motor.acc * 0.001);
//
//	} else if (l_motor.tar_vel + kp_l * diff <= l_motor.vel) {
//		l_motor.vel = l_motor.vel - (l_motor.acc * 0.001);
//	}
//
//	if (r_motor.tar_vel - kp_r * diff > r_motor.vel) {
//		r_motor.vel = r_motor.vel + (r_motor.acc * 0.001);
//
//	} else if (r_motor.tar_vel - kp_r * diff <= r_motor.vel) {
//		r_motor.vel = r_motor.vel - (r_motor.acc * 0.001);
//
//	}
//
//}

//void sen_cmt1() {
//	short int i;
//// write this function to interrupt_handlers.c
//	CMT1.CMCSR.BIT.CMF = 0;
//	CMT1.CMCNT = 0;
//
//	drv_Sensor_LED(R_LED, on);
//	for (i = 0; i < 309; i++)
//		;
//	r_sen.sen = get_Sensor(R_sen, ad_0); //R sensor
//	drv_Sensor_LED(R_LED, off);
//
//	drv_Sensor_LED(CR_LED, on);
//	for (i = 0; i < 309; i++)
//		;
//	cr_sen.sen = get_Sensor(CR_sen, ad_0);		//CR sensor
//	drv_Sensor_LED(CR_LED, off);
//
//	drv_Sensor_LED(CL_LED, on);
//	for (i = 0; i < 309; i++)
//		;
//	cl_sen.sen = get_Sensor(CL_sen, ad_0);		//CL sensor
//	drv_Sensor_LED(CL_LED, off);
//
//	drv_Sensor_LED(L_LED, on);
//	for (i = 0; i < 309; i++)
//		;
//	l_sen.sen = get_Sensor(L_sen, ad_0);		//L sensor
//	drv_Sensor_LED(L_LED, off);
//
//	cr_sen.sen -= get_Sensor(CR_sen, ad_0);		//CR sensor
//	cl_sen.sen -= get_Sensor(CL_sen, ad_0);		//CL sensor
//	l_sen.sen -= get_Sensor(L_sen, ad_0);		//L sensor
//	r_sen.sen -= get_Sensor(R_sen, ad_0);		//R sensor
//
//	cr_sen.diff = cr_sen.sen - cr_sen.old[8];
//	r_sen.diff = r_sen.sen - r_sen.old[8];
//	cl_sen.diff = cl_sen.sen - cl_sen.old[8];
//	l_sen.diff = l_sen.sen - l_sen.old[8];
//
//	for (i = 8; i > 0; i--) {
//		r_sen.old[i] = r_sen.old[i - 1];
//		cr_sen.old[i] = cr_sen.old[i - 1];
//		l_sen.old[i] = l_sen.old[i - 1];
//		cl_sen.old[i] = cl_sen.old[i - 1];
//	}
//
//	cr_sen.old[0] = cr_sen.sen;		//CR sensor
//	r_sen.old[0] = r_sen.sen;		//R sensor
//	cl_sen.old[0] = cl_sen.sen;		//CL sensor
//	l_sen.old[0] = l_sen.sen;		//L sensor
//
//	diff_calc();
//
//	vel_calc();
//
//}

//void stopMTU(int cst) {
////stop MTU2's timer
//	switch (cst) {
//	case cst0:
//		MTU2.TSTR.BIT.CST0 = 0;
//		break;
//	case cst1:
//		MTU2.TSTR.BIT.CST1 = 0;
//		break;
//	case cst2:
//		MTU2.TSTR.BIT.CST2 = 0;
//		break;
//	case cst3:
//		MTU2.TSTR.BIT.CST3 = 0;
//		MTU2.TSTR.BIT.CST4 = 0;
//		break;
//	case cst_all:
//		MTU2.TSTR.BIT.CST0 = 0;
//		MTU2.TSTR.BIT.CST1 = 0;
//		MTU2.TSTR.BIT.CST2 = 0;
//		MTU2.TSTR.BIT.CST3 = 0;
//		MTU2.TSTR.BIT.CST4 = 0;
//		break;
//	}
//}
//
//void start_MTU(int cst) {
////start MTU2's timer
//	switch (cst) {
//	case cst0:
//		MTU2.TSTR.BIT.CST0 = 1;
//		break;
//	case cst1:
//		MTU2.TSTR.BIT.CST1 = 1;
//		break;
//	case cst2:
//		MTU2.TSTR.BIT.CST2 = 1;
//		break;
//	case cst3:
//		MTU2.TSTR.BIT.CST3 = 1;
//		MTU2.TSTR.BIT.CST4 = 1;
//		break;
//	case cst_all:
//		MTU2.TSTR.BIT.CST0 = 1;
//		MTU2.TSTR.BIT.CST1 = 1;
//		MTU2.TSTR.BIT.CST2 = 1;
//		MTU2.TSTR.BIT.CST3 = 1;
//		MTU2.TSTR.BIT.CST4 = 1;
//		break;
//	}
//}

//void change_Duty_MTU20(void) {
//	/*
//	 * this function operates R_motor
//	 * */
//
//	//stopMTU(cst0); //stop count
//	r_motor.duty = (1 / r_motor.vel) * (2.55e+6); //(1/v)*(step_distance / MTU_clock_duty)
//	r_motor.cnt--;
//
////change duty
//	if (MTU20.TSR.BIT.TGFA == 1) {
//
//		MTU20.TCNT = 0; //reset counter
//
//		if (r_motor.duty < 0) {
//			MTU20.TGRA = 0;
//			MTU20.TGRB = 0;
//		} else {
//			MTU20.TGRA = r_motor.duty; //(1/v)*(step_distance / MTU_clock_duty)
//			MTU20.TGRB = round(r_motor.duty / 2); //(1/v)*(step_distance / MTU_clock_duty)
//		}
//
//	}
//
//	MTU20.TSR.BIT.TGFA = 0; //reset flag
//	PE.DRL.BIT.B1 = r_motor.rot_dir_flag;
//	if (r_motor.cnt < 0 && r_motor.end_flag == 1) {
//		r_motor.stop_flag = 1;
//		stopMTU(cst0);
//	} else {
//		start_MTU(cst0);
//	}
//
//}
//
//void change_Duty_MTU21(void) {
//	/*
//	 * this function operates L_motor
//	 */
//
//	//stopMTU(cst1); //stop count
//	l_motor.duty = (1 / l_motor.vel) * (2.55e+6); //(1/v)*(step_distance / MTU_clock_duty)
//	l_motor.cnt--;
//
//	if (MTU21.TSR.BIT.TGFA == 1) {
//		//change duty
//		MTU21.TCNT = 0; //reset counter
//
//		if (l_motor.duty < 0) {
//			MTU21.TGRA = 0;
//			MTU21.TGRB = 0;
//		} else {
//			MTU21.TGRA = l_motor.duty;
//			MTU21.TGRB = round(l_motor.duty / 2);
//		}
//	}
//
//	MTU21.TSR.BIT.TGFA = 0; //reset flag
//	PE.DRL.BIT.B5 = l_motor.rot_dir_flag;
//	if (l_motor.cnt < 0 && l_motor.end_flag == 1) {
//		l_motor.stop_flag = 1;
//		stopMTU(cst1);
//	} else {
//		start_MTU(cst1);
//	}
//
//}

//void drv_Buzzer(char status) {
//	PE.DRL.BIT.B7 = status;
//}

//void UX_effect(char pattern) {
//	switch (pattern) {
//	case error:
//		sta_LED_drv(Red, on);
//		drv_Buzzer(on);
//		wait_ms(25);
//		sta_LED_drv(Red, off);
//		drv_Buzzer(off);
//		wait_ms(25);
//		break;
//
//	case click:
//		drv_Buzzer(on);
//		wait_ms(25);
//		drv_Buzzer(off);
//		break;
//
//	case alart:
//		drv_Buzzer(on);
//		sta_LED_drv(Red, on);
//		wait_ms(750);
//		sta_LED_drv(Red, off);
//		drv_Buzzer(off);
//		break;
//	}
//}

//float get_Battery_Voltage(void) {
//	float vol_ad, voltage;
//	vol_ad = (float) get_Sensor(3, ad_1) / 1024.0 * 5.0;
//	voltage = vol_ad * ((100.0 + 51.0) / 51.0);
//	return voltage;
//}

//void drv_Motor(int dist, int t_vel, int t_acc, char rot_dir_flag, int end_flag,
//		int ch) {
//	/*
//	 rot_dir
//	 1 : forward
//	 0 : backward
//	 */
//
//	if (t_acc > r_motor.max_acc) {
//		t_acc = r_motor.max_acc;
//	} else if (t_acc < r_motor.min_acc) {
//		t_acc = r_motor.min_acc;
//	}
//
//	if (t_vel > r_motor.max_vel) {
//		t_vel = r_motor.max_vel;
//	} else if (t_vel < r_motor.min_vel) {
//		t_vel = r_motor.min_vel;
//	}
//	PE.DRL.BIT.B2 = 1; //reset(0 : off, 1 : on)
//	switch (ch) {
//	case R_motor:
//		r_motor.stop_flag = 0;
//		r_motor.cnt = dist / spec.step_dist;
//		r_motor.tar_vel = t_vel;
//		r_motor.acc = t_acc;
//		r_motor.rot_dir_flag = !rot_dir_flag; //R_CW/CCW(0 : forward, 1 : backward)
//		r_motor.end_flag = end_flag;
//		start_MTU(cst0);
//		break;
//	case L_motor:
//		l_motor.stop_flag = 0;
//		l_motor.cnt = dist / spec.step_dist;
//		l_motor.tar_vel = t_vel;
//		l_motor.acc = t_acc;
//		l_motor.rot_dir_flag = rot_dir_flag; //L_CW/CCW(1 : forward, 0 : backward)
//		l_motor.end_flag = end_flag;
//		start_MTU(cst1);
//		break;
//	}
//
//	PE.DRL.BIT.B2 = 0; //reset(0 : off, 1 : on)
//}

//void switch_Motor(char sw) {
//	if (sw == on) {
//		PE.DRL.BIT.B3 = 1;
//	} else {
//		PE.DRL.BIT.B3 = 0;
//	}
//}

//void mot_app(int dist, int t_vel, int t_acc, char move_flag, char end_flag) {
//
//	if (move_flag == straight) {
//		cnt_ctl = 0;
//	} else {
//		cnt_ctl = 1;
//	}
//
//	if (t_vel * t_vel - r_motor.vel * r_motor.vel > t_acc * dist / 2) {
//		t_vel = sqrt(r_motor.vel * r_motor.vel + t_acc * dist);
//	}
//
//	drv_Motor(dist / 4 * 3, t_vel, t_acc, move_flag & 1, end_flag, R_motor);
//	drv_Motor(dist / 4 * 3, t_vel, t_acc, (move_flag & 2) >> 1, end_flag,
//			L_motor);
//
//	while (1) {
//		if (l_motor.stop_flag == 1 || r_motor.stop_flag == 1) {
//			break;
//		}
//	}
//
//	drv_Motor(dist / 4, r_motor.min_vel, t_acc, move_flag & 1, end_flag, R_motor);
//	drv_Motor(dist / 4, l_motor.min_vel, t_acc, (move_flag & 2) >> 1, end_flag,
//			L_motor);
//
//	while (1) {
//		if (l_motor.stop_flag == 1 || r_motor.stop_flag == 1) {
//			break;
//		}
//	}
//	cnt_ctl = 0;
//}
//
//void mot_app2(int dist, int t_vel, int t_acc, char move_flag, char end_flag) {
//
//	if (move_flag == straight) {
//		cnt_ctl = 0;
//	} else {
//		cnt_ctl = 1;
//	}
//
//	if (t_vel * t_vel - r_motor.vel * r_motor.vel > t_acc * dist / 2) {
//		t_vel = sqrt(r_motor.vel * r_motor.vel + t_acc * dist);
//	}
//
//	drv_Motor(dist, t_vel, t_acc, move_flag & 1, end_flag, R_motor);
//	drv_Motor(dist, t_vel, t_acc, (move_flag & 2) >> 1, end_flag, L_motor);
//
//	while (1) {
//		if (l_motor.stop_flag == 1 || r_motor.stop_flag == 1) {
//			break;
//		}
//	}
//
//	cnt_ctl = 0;
//}

void sen_calibration() {

	UX_effect(alart);

	cr_sen.non_threshold = cr_sen.sen;
	cl_sen.non_threshold = cl_sen.sen;
	r_sen.ref_wall = r_sen.sen;
	l_sen.ref_wall = l_sen.sen;
	while (PB.DR.BIT.B5 != 0) {
		//remove walls
		drv_Status_LED(Green, on);
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
		drv_Status_LED(Yerrow, on);
	}

	myprintf("non_wall : %d | %d | %d | %d\n", l_sen.non_threshold,
			cl_sen.non_threshold, cr_sen.non_threshold, r_sen.non_threshold);
	UX_effect(alart);
	drv_Status_LED(Rst_status_LED, off);
}

//void Rotate_detect(void) {
//	int now = PE.DRL.BIT.B14;
//	now = now << 1;
//	now += PE.DRL.BIT.B15;
//
//	if (now == 3) {
//		now = 2;
//	} else if (now == 2) {
//		now = 3;
//	}
//
//	if (old == 0 && now == 3) {
//		rot -= 1;
//		UX_effect(click);
//	} else if (old == 3 && now == 0) {
//		rot += 1;
//		UX_effect(click);
//	} else if (now > old) {
//		rot += 1;
//		UX_effect(click);
//	} else if (now < old) {
//		rot -= 1;
//		UX_effect(click);
//	} else {
//		rot += 0;
//	}
//	old = now;
//
//}
//
//void select_Mode() {
//	rot_sw = (char) (rot % 5);
//	Rotate_detect();
//	sta_LED_drv(Red, (rot_sw + 1) & 1);
//	sta_LED_drv(Yerrow, ((rot_sw + 1) & 2) >> 1);
//	sta_LED_drv(Green, ((rot_sw + 1) & 4) >> 2);
//}

//void detect_Direction() {
//	if (direction == 0) {
//		pos_y += 1;
//		/*
//		 sta_LED_drv(Red, off);
//		 sta_LED_drv(Yerrow, off);
//		 sta_LED_drv(Green, on);
//		 */
//	} else if (direction == 1) {
//		pos_x += 1;
//		/*
//		 sta_LED_drv(Red, on);
//		 sta_LED_drv(Yerrow, off);
//		 sta_LED_drv(Green, off);
//		 */
//	} else if (direction == 3) {
//		pos_x -= 1;
//		/*
//		 sta_LED_drv(Red, off);
//		 sta_LED_drv(Yerrow, on);
//		 sta_LED_drv(Green, off);
//		 */
//	} else if (direction == 2) {
//		pos_y -= 1;
//		/*
//		 sta_LED_drv(Red, off);
//		 sta_LED_drv(Yerrow, off);
//		 sta_LED_drv(Green, off);
//		 */
//	}
//	drv_Status_LED(Red, pos_y & 1);
//	drv_Status_LED(Yerrow, (pos_x & 1) >> 0);
//	drv_Status_LED(Green, (pos_x & 2) >> 1);
//
//}

//void move_Left() {
//	kp_r -= 0.1;
//	kp_l -= 0.1;
//	mot_app(half_block, 330, 2000, straight, on);
//	wait_ms(100);
//	switch_Motor(off);
//	wait_ms(450);
//	switch_Motor(on);
//	wait_ms(100);
//	mot_app(l_distance, 310, 2000, left, on);
//	wait_ms(100);
//	switch_Motor(off);
//	wait_ms(450);
//	switch_Motor(on);
//	wait_ms(100);
//	mot_app2(half_block, 330, 2000, straight, on);
//	kp_r += 0.1;
//	kp_l += 0.1;
//}
//
//void move_Right() {
//	kp_r -= 0.1;
//	kp_l -= 0.1;
//	mot_app(half_block, 330, 2000, straight, on);
//	wait_ms(100);
//	switch_Motor(off);
//	wait_ms(450);
//	switch_Motor(on);
//	wait_ms(100);
//	mot_app(r_distance, 310, 2000, right, on);
//	wait_ms(100);
//	switch_Motor(off);
//	wait_ms(450);
//	switch_Motor(on);
//	wait_ms(100);
//	mot_app2(half_block, 330, 1800, straight, on);
//	kp_r += 0.1;
//	kp_l += 0.1;
//
//}
//
//void move_Forward() {
//	kp_r += 0.1;
//	kp_l += 0.1;
//	mot_app2(full_block, 480, 2000, straight, on);
//	kp_r -= 0.1;
//	kp_l -= 0.1;
//}
//
//void move_Backward() {
//	mot_app(half_block, 330, 2000, straight, on);
//	wait_ms(1000);
//	mot_app(r_distance, 310, 2000, right, on);
//	wait_ms(1000);
//	mot_app(half_block, 270, 2000, back, on);
//	switch_Motor(off);
//	wait_ms(1000);
//	switch_Motor(on);
//	mot_app(18, 280, 2000, straight, on);
//	wait_ms(1000);
//	mot_app(r_distance, 310, 2000, right, on);
//	wait_ms(1000);
//	mot_app(half_block, 270, 2000, back, on);
//	switch_Motor(off);
//	wait_ms(1000);
//	switch_Motor(on);
//	mot_app2(15 + half_block, 330, 2000, straight, on);
//
//}
//
//void move_Backward_2() {
//	mot_app(half_block, 330, 2000, straight, on);
//	wait_ms(100);
//	switch_Motor(off);
//	wait_ms(450);
//	switch_Motor(on);
//	wait_ms(100);
//	mot_app(r_distance, 310, 2000, right, on);
//	mot_app(r_distance, 310, 2000, right, on);
//	wait_ms(100);
//	switch_Motor(off);
//	wait_ms(450);
//	switch_Motor(on);
//	wait_ms(100);
//	mot_app2(half_block, 330, 2000, straight, on);
//
//}

//void update_Wall_map() {
//	/*
//	 * wall : W-S-E-N
//	 * 		 MSB   LSB
//	 *
//	 * map_x : vertical walls to x-axis
//	 * map_y : vertical walls to y-axis
//	 * */
//
//	wall = 0;
//	if (r_sen.sen > r_sen.non_threshold) {
//		wall |= 1 << ((1 + direction) % 4);
//	}
//	if (l_sen.sen > l_sen.non_threshold) {
//		wall |= 1 << ((3 + direction) % 4);
//	}
//	if (cl_sen.sen > cl_sen.non_threshold) {
//		wall |= 1 << ((0 + direction) % 4);
//	}
//
//	if (pos_x < map_size - 1) {
//		wall_map_x[pos_x] |= ((wall & 2) >> 1) << pos_y;
//		searched_map_x[pos_x] &= ~(1 << pos_y);
//	}
//	if (pos_x - 1 >= 0) {
//		wall_map_x[pos_x - 1] |= ((wall & 8) >> 3) << pos_y;
//		searched_map_x[pos_x - 1] &= ~(1 << pos_y);
//	}
//
//	if (pos_y < map_size - 1) {
//		wall_map_y[pos_y] |= ((wall & 1) >> 0) << pos_x;
//		searched_map_y[pos_y] &= ~(1 << pos_x);
//	}
//	if (pos_y - 1 >= 0) {
//		wall_map_y[pos_y - 1] |= ((wall & 4) >> 2) << pos_x;
//		searched_map_y[pos_y - 1] &= ~(1 << pos_x);
//	}
//}

//void print_Wall_map_x(int row) {
//	int i, mask = 1;
//	mask <<= row;
//
//	for (i = 0; i < map_size - 1; i++) {
//		myprintf("  ");
//		if (wall_map_x[i] & mask) {
//			myprintf("|");
//		} else {
//			myprintf(" ");
//		}
//	}
//
//}
//
//void print_Wall_map_y(int row) {
//	int i, mask;
//	for (i = 0; i < map_size - 1; i++) {
//		mask = 1 << i;
//		if (wall_map_y[row] & mask) {
//			myprintf("--");
//		} else {
//			myprintf("  ");
//		}
//		myprintf("+");
//	}
//	mask <<= 1;
//	if (wall_map_y[row] & mask) {
//		myprintf("--");
//	} else {
//		myprintf("  ");
//	}
//
//}

//void print_Wall_map() {
//	int i;
//	myprintf("\n");
//	for (i = 0; i < map_size; i++) {
//		myprintf("+--");
//	}
//	myprintf("+\n|");
//
//	print_Wall_map_x(map_size - 1);
//	myprintf("  |\n+");
//	for (i = 1; i < map_size; i++) {
//		print_Wall_map_y(map_size - 1 - i);
//		myprintf("+\n|");
//		print_Wall_map_x(map_size - 1 - i);
//		myprintf("  |\n+");
//	}
//
//	for (i = 0; i < map_size; i++) {
//		myprintf("--+");
//	}
//	myprintf("\n\n");
//}

//char read_Wall_map(char x, char y) {
//
//	/*
//	 *  wall : W-S-E-N
//	 * 		 MSB   LSB
//	 */
//
//	char wall = 0;
//
//	//myprintf("\n\n");
//
//	if (x < map_size - 1) {
//		wall |= ((wall_map_x[x] & (1 << y)) >> y) << 1;
//		//myprintf("%d\n", ((mixed_map_x[x] & (1 << y)) >> y) << 1);
//	}
//	if (x - 1 >= 0) {
//		wall |= ((wall_map_x[x - 1] & (1 << y)) >> y) << 3;
//		//myprintf("%d\n", ((mixed_map_x[x - 1] & (1 << y)) >> y) << 3);
//	}
//
//	if (y < map_size - 1) {
//		wall |= (wall_map_y[y] & (1 << x)) >> x;
//		//myprintf("%d\n", (mixed_map_y[y] & (1 << x)) >> x);
//	}
//	if (y - 1 >= 0) {
//		wall |= ((wall_map_y[y - 1] & (1 << x)) >> x) << 2;
//		//myprintf("%d\n", ((mixed_map_y[y - 1] & (1 << x)) >> x) << 2);
//	}
//	return wall;
//
//}

/*void print_Searched_map_x(int row) {
	int i, mask = 1;
	mask <<= row;

	for (i = 0; i < map_size - 1; i++) {
		myprintf("  ");
		if (searched_map_x[i] & mask) {
			myprintf("|");
		} else {
			myprintf(" ");
		}
	}

}

void print_Searched_map_y(int row) {
	int i, mask;
	for (i = 0; i < map_size - 1; i++) {
		mask = 1 << i;
		if (searched_map_y[row] & mask) {
			myprintf("--");
		} else {
			myprintf("  ");
		}
		myprintf("+");
	}
	mask <<= 1;
	if (searched_map_y[row] & mask) {
		myprintf("--");
	} else {
		myprintf("  ");
	}

}

void print_Searched_map() {
	int i;
	myprintf("\n");
	for (i = 0; i < map_size; i++) {
		myprintf("+--");
	}
	myprintf("+\n|");

	print_Searched_map_x(map_size - 1);
	myprintf("  |\n+");
	for (i = 1; i < map_size; i++) {
		print_Searched_map_y(map_size - 1 - i);
		myprintf("+\n|");
		print_Searched_map_x(map_size - 1 - i);
		myprintf("  |\n+");
	}

	for (i = 0; i < map_size; i++) {
		myprintf("--+");
	}
	myprintf("\n\n");
}*/

/*void initwall_map() {
	int i;
	for (i = 0; i < map_size - 1; i++) {
		wall_map_x[i] = 0;
		wall_map_y[i] = 0;
		searched_map_x[i] = 0xffffffff;
		searched_map_y[i] = 0xffffffff;

	}
}*/

/*void mix_map() {
	int i;
	for (i = 0; i < map_size - 1; i++) {
		mixed_map_x[i] = wall_map_x[i] | searched_map_x[i];
		mixed_map_y[i] = wall_map_y[i] | searched_map_y[i];
	}
}*/

/*void print_Mixed_map_x(int row) {
	int i, mask = 1;
	mask <<= row;

	for (i = 0; i < map_size - 1; i++) {
		myprintf("  ");
		if (mixed_map_x[i] & mask) {
			myprintf("|");
		} else {
			myprintf(" ");
		}
	}

}

void print_Mixed_map_y(int row) {
	int i, mask;
	for (i = 0; i < map_size - 1; i++) {
		mask = 1 << i;
		if (mixed_map_y[row] & mask) {
			myprintf("--");
		} else {
			myprintf("  ");
		}
		myprintf("+");
	}
	mask <<= 1;
	if (mixed_map_y[row] & mask) {
		myprintf("--");
	} else {
		myprintf("  ");
	}

}

void print_Mixed_map() {
	int i;
	myprintf("\n");
	for (i = 0; i < map_size; i++) {
		myprintf("+--");
	}
	myprintf("+\n|");

	print_Mixed_map_x(map_size - 1);
	myprintf("  |\n+");
	for (i = 1; i < map_size; i++) {
		print_Mixed_map_y(map_size - 1 - i);
		myprintf("+\n|");
		print_Mixed_map_x(map_size - 1 - i);
		myprintf("  |\n+");
	}

	for (i = 0; i < map_size; i++) {
		myprintf("--+");
	}
	myprintf("\n\n");
}*/

/*char read_Mixed_map(char x, char y) {


	 *  wall : W-S-E-N
	 * 		 MSB   LSB


	char wall = 0;

	myprintf("\n\n");

	if (x < map_size - 1) {
		wall |= ((mixed_map_x[x] & (1 << y)) >> y) << 1;
		//myprintf("%d\n", ((mixed_map_x[x] & (1 << y)) >> y) << 1);
	}
	if (x - 1 >= 0) {
		wall |= ((mixed_map_x[x - 1] & (1 << y)) >> y) << 3;
		//myprintf("%d\n", ((mixed_map_x[x - 1] & (1 << y)) >> y) << 3);
	}

	if (y < map_size - 1) {
		wall |= (mixed_map_y[y] & (1 << x)) >> x;
		//myprintf("%d\n", (mixed_map_y[y] & (1 << x)) >> x);
	}
	if (y - 1 >= 0) {
		wall |= ((mixed_map_y[y - 1] & (1 << x)) >> x) << 2;
		//myprintf("%d\n", ((mixed_map_y[y - 1] & (1 << x)) >> x) << 2);
	}
	return wall;

}*/

/*void update_A_dist_map() {
	unsigned char buff_x = 0, buff_y = 0, wall, dist = 0;
	int i, j, k;

	while (a_star_dist_map[pos_x][pos_y] == 255) {
		for (i = 0; i < map_size; i++) {
			for (j = 0; j < map_size; j++) {
				if (a_star_dist_map[i][j] == dist) {
					buff_x = i;
					buff_y = j;
					wall = read_Wall_map(buff_x, buff_y);

					if ((wall & 2) != 2) {
						if (buff_x != (map_size - 1)) {
							if (a_star_dist_map[buff_x + 1][buff_y] == 255) {
								a_star_dist_map[buff_x + 1][buff_y] = dist + 1;
							}
						}
					}
					if ((wall & 8) != 8) {
						if (buff_x != 0) {
							if (a_star_dist_map[buff_x - 1][buff_y] == 255) {
								a_star_dist_map[buff_x - 1][buff_y] = dist + 1;
							}
						}
					}

					if ((wall & 1) != 1) {
						if (buff_y != (map_size - 1)) {
							if (a_star_dist_map[buff_x][buff_y + 1] == 255) {
								a_star_dist_map[buff_x][buff_y + 1] = dist + 1;
							}
						}
					}

					if ((wall & 4) != 4) {
						if (buff_y != 0) {
							if (a_star_dist_map[buff_x][buff_y - 1] == 255) {
								a_star_dist_map[buff_x][buff_y - 1] = dist + 1;
							}
						}
					}
				}
			}
		}
		dist += 1;
	}

}*/

/*char generate_A_path() {

	 * =================
	 * about path[]
	 * 0:forward
	 * 1:right
	 * 2:backward
	 * 3:left
	 * 4:goal
	 * =================
	 * wall : W-S-E-N
	 * 		 MSB   LSB
	 * =================
	 * dir : 0 North
	 * 		 1 East
	 * 		 2 South
	 * 		 3 West
	 * =================
	 *
	char x = pos_x, y = pos_y, dir = direction, rel_dir, wall, dist, min_dist,
			pri_flag;
	int i = 0;

	wall = read_Wall_map(x, y);
	dist = a_star_dist_map[x][y];
	pri_flag = 4;

	if ((wall & 4) == 0) {
		if (y - 1 >= 0) {
			if (a_star_dist_map[x][y - 1] <= dist) {
				dist = a_star_dist_map[x][y - 1];
				min_dist = 2;
				if (min_dist == dir) {
					pri_flag = dir;
				}
			}
		}
	}

	if ((wall & 2) == 0) {
		if (x + 1 < map_size) {
			if (a_star_dist_map[x + 1][y] <= dist) {
				dist = a_star_dist_map[x + 1][y];
				min_dist = 1;
				if (min_dist == dir) {
					pri_flag = dir;
				}
			}
		}
	}

	if ((wall & 8) == 0) {
		if (x - 1 >= 0) {
			if (a_star_dist_map[x - 1][y] <= dist) {
				dist = a_star_dist_map[x - 1][y];
				min_dist = 3;
				if (min_dist == dir) {
					pri_flag = dir;
				}
			}
		}
	}

	if ((wall & 1) == 0) {
		if (y + 1 < map_size) {
			if (a_star_dist_map[x][y + 1] <= dist) {
				dist = a_star_dist_map[x][y + 1];
				min_dist = 0;
				if (min_dist == dir) {
					pri_flag = dir;
				}
			}
		}
	}

	if (pri_flag != 4) {
		min_dist = pri_flag;
	}

	rel_dir = min_dist - dir;
	if (rel_dir < 0) {
		rel_dir += 4;
	}
	return rel_dir;
}*/

/*void initdist_map() {
	int i, j;

	for (i = 0; i < map_size; i++) {
		for (j = 0; j < map_size; j++) {
			dist_map[i][j] = 255;
			a_star_dist_map[i][j] = 255;
		}
	}
	dist_map[goal_x][goal_y] = 0;
	a_star_dist_map[goal_x][goal_y] = 0;
}*/

/*void init_Dist_map() {
	int i, j;

	for (i = 0; i < map_size; i++) {
		for (j = 0; j < map_size; j++) {
			dist_map[i][j] = 255;
		}
	}
	dist_map[goal_x][goal_y] = 0;

}*/

/*void init_A_dist_map() {
	int i, j;

	for (i = 0; i < map_size; i++) {
		for (j = 0; j < map_size; j++) {
			a_star_dist_map[i][j] = 255;
		}
	}
	a_star_dist_map[goal_x][goal_y] = 0;
}*/

/*void print_Dist_map() {
	int i, j;

	myprintf("\n");
	for (i = 0; i < map_size; i++) {
		for (j = 0; j < map_size; j++) {
			myprintf("%3d ", dist_map[j][map_size - 1 - i]);
		}
		myprintf("\n");
	}
}*/

/*
void update_Dist_map() {
	unsigned char buff_x = 0, buff_y = 0, wall, dist = 0;
	int i, j, k;
	mix_map();
	print_Mixed_map();
	while (dist_map[0][0] == 255) {
		for (i = 0; i < map_size; i++) {
			for (j = 0; j < map_size; j++) {
				if (dist_map[i][j] == dist) {
					buff_x = i;
					buff_y = j;
					wall = read_Mixed_map(buff_x, buff_y);

					if ((wall & 2) != 2) {
						if (buff_x != (map_size - 1)) {
							if (dist_map[buff_x + 1][buff_y] == 255) {
								dist_map[buff_x + 1][buff_y] = dist + 1;
							}
						}
					}
					if ((wall & 8) != 8) {
						if (buff_x != 0) {
							if (dist_map[buff_x - 1][buff_y] == 255) {
								dist_map[buff_x - 1][buff_y] = dist + 1;
							}
						}
					}

					if ((wall & 1) != 1) {
						if (buff_y != (map_size - 1)) {
							if (dist_map[buff_x][buff_y + 1] == 255) {
								dist_map[buff_x][buff_y + 1] = dist + 1;
							}
						}
					}

					if ((wall & 4) != 4) {
						if (buff_y != 0) {
							if (dist_map[buff_x][buff_y - 1] == 255) {
								dist_map[buff_x][buff_y - 1] = dist + 1;
							}
						}
					}
				}
			}
		}
		print_Dist_map();
		dist += 1;
	}
}
*/

/*
char read_dist_map(char x, char y) {
	return dist_map[x][y];
}
*/
/*
void generate_Path() {

	 * =================
	 * about path[]
	 * 0:forward
	 * 1:right
	 * 2:backward
	 * 3:left
	 * 4:goal
	 * =================
	 * wall : W-S-E-N
	 * 		 MSB   LSB
	 * =================
	 * dir : 0 North
	 * 		 1 East
	 * 		 2 South
	 * 		 3 West
	 * =================
	 *
	char x = 0, y = 0, dir = direction, rel_dir = direction, wall, dist,
			min_dist, pri_flag;
	int i = 0;

	myprintf("=======\n");

	while (dist_map[x][y] != 0) {
		wall = read_Mixed_map(x, y);
		dist = dist_map[x][y];
		pri_flag = 4;

		if ((wall & 1) == 0) {
			if (y + 1 < map_size) {
				if (dist_map[x][y + 1] <= dist) {
					dist = dist_map[x][y + 1];
					min_dist = 0;
					if (dir == min_dist) {
						pri_flag = dir;
					}
				}

			}
		}
		if ((wall & 2) == 0) {
			if (x + 1 < map_size) {
				if (dist_map[x + 1][y] <= dist) {
					dist = dist_map[x + 1][y];
					min_dist = 1;
					if (dir == min_dist) {
						pri_flag = dir;
					}
				}

			}
		}
		if ((wall & 4) == 0) {
			if (y - 1 >= 0) {
				if (dist_map[x][y - 1] <= dist) {
					dist = dist_map[x][y - 1];
					min_dist = 2;
					if (dir == min_dist) {
						pri_flag = dir;
					}
				}

			}
		}
		if ((wall & 8) == 0) {
			if (x - 1 >= 0) {
				if (dist_map[x - 1][y] <= dist) {
					dist = dist_map[x - 1][y];
					min_dist = 3;
					if (dir == min_dist) {
						pri_flag = dir;
					}
				}

			}
		}

		if (pri_flag != 4) {
			min_dist = pri_flag;
			myprintf("====\n");
		}

		if (min_dist == 0) {
			y += 1;
		} else if (min_dist == 1) {
			x += 1;
		} else if (min_dist == 2) {
			y -= 1;
		} else if (min_dist == 3) {
			x -= 1;
		}

		rel_dir = min_dist - dir;
		if (rel_dir < 0) {
			rel_dir += 4;
		}

		dir = min_dist;
		myprintf("(%d,%d)\n", x, y);
		myprintf("%d\n", rel_dir);
		path[i] = rel_dir;

		i++;
	}
}*/

/*void init_Path() {
	int i;
	for (i = 0; i < map_size * map_size; i++) {
		path[i] = 4;
	}
}*/

int main(void) {
	float v_r = 0;
	float v_l = 0;
	//sta_LED_flag = 0;
	init_sci1();
	initALL();
	//init_Path();
	switch_Motor(off);
	//PE.DRL.BIT.B7 = 0;
/*

	half_block = 93.5;
	full_block = 180;
	r_distance = (int) ((90.0 / 180 * 3.141592) * (spec.tire_dim / 2) + 0.5);
	l_distance = (int) ((90.0 / 180 * 3.141592) * (spec.tire_dim / 2) - 1);
*/

//	initwall_map();
//	initdist_map();

	while (1) {
		while (spec.batt < 11.3) {
			get_Battery_Voltage();
			myprintf("Low_battery\n");
			UX_effect(error);
		}

		drv_Status_LED(Red, off);

		while (PB.DR.BIT.B5 != 0) {
			//batt = get_Battery_Voltage();
			get_Battery_Voltage();
			select_Mode();
			myprintf("batt : %f\n", spec.batt);
			myprintf("sen : %d | %d | %d | %d\n", l_sen.sen, cl_sen.sen,
					cr_sen.sen, r_sen.sen);
			myprintf("mode : %d\n", Switch.rot_sw);
			wait_ms(100);
		}
		wait_ms(100);
		drv_Status_LED(Rst_status_LED, off);
		switch (Switch.rot_sw) {

		case astar:
			spec.sta_LED_flag = 0;
			map.pos_x = 0;
			map.pos_y = 1;
			map.direction = 0;
			spec.run_interruption = 0;
			UX_effect(alart);
			route_index = 1;

			switch_Motor(on);
			mot_app2(spec.half_block, 350, 1500, straight, on);

			while (spec.run_interruption != 1) {
				map.a_dist_map[map.pos_x][map.pos_y] = 255;
				update_Wall_map();
				update_A_dist_map();
				map.tmp_path = generate_A_path();
				init_A_dist_map();

				if (map.tmp_path == 1) {
					map.direction += 1;
					move_Right();

				} else if (map.tmp_path == 3) {
					map.direction += 3;
					move_Left();
				} else if (map.tmp_path == 0) {
					map.direction += 0;
					move_Forward();
				} else if (map.tmp_path == 2) {
					map.direction += 2;
					if (map.wall == 7 || map.wall == 11 || map.wall == 13 || map.wall == 14) {
						move_Backward();
					} else {
						move_Backward_2();
					}
				}
				map.direction %= 4;

				detect_Direction();
				if (map.pos_x == map.goal_x & map.pos_y == map.goal_y) {
					spec.run_interruption = 1;
				}
			}
			update_Wall_map();
			mot_app(spec.half_block, 310, 1500, straight, on);
			wait_ms(300);
			switch_Motor(off);
			spec.sta_LED_flag = 0;
			map.pos_x = 0;
			map.pos_y = 0;
			init_Path();
			init_Dist_map();
			update_Dist_map();
			generate_Path();
			break;

		case run:
			spec.sta_LED_flag = 0;
			map.pos_x = 0;
			map.pos_y = 1;
			map.direction = 0;
			spec.run_interruption = 0;
			UX_effect(alart);
			route_index = 1;

			switch_Motor(on);
			mot_app2(spec.half_block, 310, 1500, straight, on);

			while (map.path[route_index] != 4) {
				update_Wall_map();

				if (map.path[route_index] == 1) {
					map.direction += 1;
					move_Right();

				} else if (map.path[route_index] == 3) {
					map.direction += 3;
					move_Left();
				} else if (map.path[route_index] == 0) {
					map.direction += 0;
					move_Forward();
				} else {
					map.direction += 2;
					move_Backward_2();
				}
				map.direction %= 4;

				detect_Direction();
				route_index += 1;
			}
			update_Wall_map();
			mot_app(spec.half_block, 310, 1500, straight, on);
			wait_ms(300);
			spec.sta_LED_flag = 0;
			map.pos_x = map.goal_x;
			map.pos_y = map.goal_y;
			break;

		case search:
			spec.sta_LED_flag = 0;
			map.pos_x = 0;
			map.pos_y = 1;
			map.direction = 0;
			spec.run_interruption = 0;
			UX_effect(alart);

			switch_Motor(on);
			mot_app2(spec.half_block, 350, 1500, straight, on);

			while (spec.run_interruption != 1) {
				update_Wall_map();

				if (l_sen.sen <= l_sen.non_threshold) {
					map.direction += 3;
					move_Left();
				} else if (cl_sen.sen <= cl_sen.non_threshold) {
					map.direction += 0;
					move_Forward();
				} else if (r_sen.sen <= r_sen.non_threshold) {
					map.direction += 1;
					move_Right();

				} else {
					map.direction += 2;
					move_Backward();
				}
				map.direction %= 4;

				detect_Direction();
				if (map.pos_x == map.goal_x & map.pos_y == map.goal_y) {
					spec.run_interruption = 1;
				}
			}
			update_Wall_map();
			mot_app(spec.half_block, 310, 1500, straight, on);
			wait_ms(300);
			switch_Motor(off);
			spec.sta_LED_flag = 0;
			map.pos_x = 0;
			map.pos_y = 0;
			init_Dist_map();
			update_Dist_map();
			generate_Path();
			break;

		case show_map:

			spec.sta_LED_flag = 0;
			map.direction = 0;
			UX_effect(alart);
			print_Wall_map();
			print_Searched_map();
			init_Path();
			update_Dist_map();
			generate_Path();

			/*
			 sta_LED_flag = 0;
			 direction = 0;
			 UX_effect(alart);
			 switch_Motor(on);
			 mot_app2(half_block, 330, 2000, straight, on);
			 for (k = 0; k < 4; k++) {
			 mot_app2(full_block * 13, 460, 2000, straight, on);
			 mot_app2(full_block, 330, 2000, straight, on);
			 move_Right();
			 mot_app2(full_block * 5, 460, 2000, straight, on);
			 mot_app2(full_block, 330, 2000, straight, on);
			 move_Right();
			 }
			 mot_app(half_block, 310, 2000, straight, on);

			 */
			break;
		case test:

			spec.sta_LED_flag = 1;

			UX_effect(alart);
			switch_Motor(on);
			wait_ms(100);
			mot_app2(spec.half_block, 330, 2000, straight, on);
			mot_app(spec.half_block, 330, 2000, straight, on);
			wait_ms(100);
			switch_Motor(off);

			while (PB.DR.BIT.B5 != 0)
				;
			UX_effect(alart);
			switch_Motor(on);
			wait_ms(100);
			mot_app(spec.r_distance, 310, 2000, right, on);
			wait_ms(100);
			switch_Motor(off);

			while (PB.DR.BIT.B5 != 0)
				;
			UX_effect(alart);
			switch_Motor(on);
			wait_ms(100);
			mot_app(spec.l_distance, 310, 2000, left, on);
			wait_ms(100);
			switch_Motor(off);

			while (PB.DR.BIT.B5 != 0)
				;
			UX_effect(alart);
			switch_Motor(on);
			wait_ms(100);
			mot_app2(spec.half_block, 330, 2000, straight, on);
			mot_app2(spec.full_block * 5, 480, 2000, straight, on);
			mot_app(spec.half_block, 330, 2000, straight, on);
			wait_ms(100);
			switch_Motor(off);

			spec.sta_LED_flag = 0;

			break;
		}

		switch_Motor(off);

	}

}

