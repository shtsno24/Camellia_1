/*
 * MTU.c
 *
 *  Created on: 2017/09/17
 *      Author: shts
 */
#include "iodefine.h"
#include "util.h"
#include "MTU.h"
#include "motor.h"

#define round(A)((int)(A + 0.5))

extern SPC spec;
extern MOT r_motor,l_motor;

void init_MTU(void) {

	STB.CR4.BIT._MTU2 = 0; //スタンバイ解除
	MTU2.TSTR.BYTE = 0x00;	//タイマストップ

	MTU20.TIER.BIT.TGIEA = 1; //enable interrupt
	MTU21.TIER.BIT.TGIEA = 1; //enable interrupt
	//	MTU22.TIER.BIT.TGIEA = 1; //enable interrupt
	//	MTU23.TIER.BIT.TGIEA = 1; //enable interrupt
	//	MTU23.TIER.BIT.TGIEC = 1; //enable interrupt

	INTC.IPRD.BIT._MTU20G = 0xF; //set interrupt priority
	INTC.IPRD.BIT._MTU21G = 0xE; //set interrupt priority
	//	INTC.IPRE.BIT._MTU22G = 0xD; //set interrupt priority
	//	INTC.IPRE.BIT._MTU23G = 0xC; //set interrupt priority

	MTU20.TCR.BIT.TPSC = 1; //set clock 25/4=6.25MHz(0.16us)
	MTU21.TCR.BIT.TPSC = 1; //set clock 25/4=6.25MHz(0.16us)
	//	MTU22.TCR.BIT.TPSC = 1; //set clock 25/4=6.25MHz(0.16us)
	//	MTU23.TCR.BIT.TPSC = 1; //set clock 25/4=6.25MHz(0.16us)

	MTU20.TCNT = 0;	// reset counter
	MTU21.TCNT = 0;
	MTU22.TCNT = 0;
	MTU23.TCNT = 0;

	MTU20.TCR.BIT.CCLR = 1; //TGRAのコンペアマッチでTCNTクリア
	MTU20.TIOR.BIT.IOA = 1; //0→0
	MTU20.TIOR.BIT.IOB = 2; //0→1

	MTU21.TCR.BIT.CCLR = 1; //TGRAのコンペアマッチでTCNTクリア
	MTU21.TIOR.BIT.IOA = 1; //0→0
	MTU21.TIOR.BIT.IOB = 2; //0→1
	/*
	 MTU22.TCR.BIT.CCLR = 1; //TGRAのコンペアマッチでTCNTクリア
	 MTU22.TIOR.BIT.IOA = 1; //0→0
	 MTU22.TIOR.BIT.IOB = 2; //0→1

	 MTU23.TCR.BIT.CCLR = 1; //TGRAのコンペアマッチでTCNTクリア
	 MTU23.TIOR.BIT.IOA = 1; //0→0
	 MTU23.TIOR.BIT.IOB = 2; //0→1
	 MTU23.TIOR.BIT.IOC = 1; //0→0
	 MTU23.TIOR.BIT.IOD = 2; //0→1
	 */
	MTU2.TOER.BYTE = 0xFF; // permission of PWM output

	MTU20.TGRA = spec.pwm_base_clock; // set PWM clock
	MTU20.TGRB = spec.pwm_half_clock; // set duty

	MTU21.TGRA = spec.pwm_base_clock; // set PWM clock
	MTU21.TGRB = spec.pwm_half_clock; // set duty
	/*
	 MTU22.TGRA = 6250 - 1;; // set PWM clock
	 MTU22.TGRB = round((6250 - 1) / 2); // set duty

	 MTU23.TGRA = 6250 - 1; // set sensor's PWM clock
	 MTU23.TGRB = round((6250 - 1) / 2); // set sensor's PWM duty
	 MTU23.TGRC = 6250 - 1; // set PWM clock
	 MTU23.TGRD = round((6250 - 1) / 2); // set duty
	 */
	MTU20.TMDR.BIT.MD = 2; //PWMモード1
	MTU21.TMDR.BIT.MD = 2; //PWMモード1
	//	MTU22.TMDR.BIT.MD = 2; //PWMモード1
	//	MTU23.TMDR.BIT.MD = 2; //PWMモード1

	/*
	 MTU2.TSTR.BIT.CST0 = 1; //start MTU20's timer
	 MTU2.TSTR.BIT.CST1 = 1; //start MTU21's timer
	 MTU2.TSTR.BIT.CST2 = 1; //start MTU22's timer
	 MTU2.TSTR.BIT.CST3 = 1; //start MTU23's timer
	 */
}

void stop_MTU(char cst) {
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

void start_MTU(char cst) {
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

void change_Duty_MTU20(void) {
	/*
	 * this function operates R_motor
	 * */

	//stopMTU(cst0); //stop count
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
		//stop_MTU(cst0);
	} else {
		start_MTU(cst0);
	}

}

void change_Duty_MTU21(void) {
	/*
	 * this function operates L_motor
	 */

	//stopMTU(cst1); //stop count
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
		//stop_MTU(cst1);
	} else {
		start_MTU(cst1);
	}

}
