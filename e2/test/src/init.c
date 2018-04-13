/*
 * init.c
 *
 *  Created on: 2017/10/01
 *      Author: shts
 */

#include "iodefine.h"
#include "init.h"
#include "util.h"
#include "CMT.h"
#include "sensor.h"

#define round(A)((int)(A + 0.5))

//SPC spec;
extern SEN r_sen, cr_sen, l_sen, cl_sen;
MOT r_motor, l_motor;

//void init_IO(void) {
//	PFC.PEIORL.BIT.B0 = 1; //R_Clock
//	PFC.PEIORL.BIT.B1 = 1; //R_CW/CCW
//	PFC.PEIORL.BIT.B2 = 1; //reset_motor
//	PFC.PEIORL.BIT.B3 = 1; //excitation_enable
//	PFC.PEIORL.BIT.B4 = 1; //L_Clock
//	PFC.PEIORL.BIT.B5 = 1; //L_CW/CCW
//	PFC.PEIORL.BIT.B6 = 1; //LED_yellow
//	PFC.PEIORL.BIT.B7 = 1; //buzzer
//	PFC.PEIORL.BIT.B8 = 1; //LED_red
//	PFC.PEIORL.BIT.B9 = 1; //sensor_right
//	PFC.PEIORL.BIT.B10 = 1; //rotary_encoder_LED
//	PFC.PEIORL.BIT.B11 = 1; //sensor_center_right
//	PFC.PEIORL.BIT.B12 = 1; //sensor_left
//	PFC.PEIORL.BIT.B13 = 1; //sensor_center_left
//	PFC.PEIORL.BIT.B14 = 0; //rotary_encoder_a
//	PFC.PEIORL.BIT.B15 = 0; //rotary_encoder_b
//
//	PFC.PBIORL.BIT.B5 = 0; //select_switch
//	/*
//	 PFC.PECRL4.BIT.PE13MD = 1; //sensor_center_left
//	 PFC.PECRL4.BIT.PE12MD = 1; //sensor_left
//	 PFC.PECRL3.BIT.PE11MD = 1; //sensor_center_right
//	 PFC.PECRL3.BIT.PE10MD = 0; //I/O
//	 PFC.PECRL3.BIT.PE9MD = 1; //sensor_right
//	 */
//	PFC.PECRL4.BIT.PE13MD = 0; //sensor_center_left
//	PFC.PECRL4.BIT.PE12MD = 0; //sensor_left
//	PFC.PECRL3.BIT.PE11MD = 0; //sensor_center_right
//	PFC.PECRL3.BIT.PE10MD = 0; //green_LED
//	PFC.PECRL3.BIT.PE9MD = 0; //sensor_right
//	PFC.PECRL3.BIT.PE8MD = 0; //red_LED
//	PFC.PECRL2.BIT.PE7MD = 0; //buzzer
//	PFC.PECRL2.BIT.PE6MD = 0; //yellow_LED
//	PFC.PECRL2.BIT.PE5MD = 0; //L_CW/CCW
//	PFC.PECRL2.BIT.PE4MD = 1; //L_motor clock
//	PFC.PECRL1.BIT.PE3MD = 0; //excitation_enable
//	PFC.PECRL1.BIT.PE2MD = 0; //reset_motor
//	PFC.PECRL1.BIT.PE1MD = 0; //R_CW/CCW
//	PFC.PECRL1.BIT.PE0MD = 1; //R_motor clock
//}

//void initSPEC() {
//	spec.tread = 90.0;
//	spec.tire_dim = 90.0;
//	spec.step_dist = 0.408407;
//	spec.vel_min = 200;
//	spec.tar_vel_min = 250;
//	/*
//	 spec.sen_min_CR = 29;
//	 spec.sen_min_R = 28;
//	 spec.sen_min_CL = 80;
//	 spec.sen_min_L = 20;
//
//	 spec.sen_max_CR = 230;
//	 spec.sen_max_R = 871;
//	 spec.sen_max_CL = 800;
//	 spec.sen_max_L = 990;
//
//	 spec.sen_ref_R = 30;
//	 spec.sen_ref_L = 12;
//	 spec.sen_ref_wall_R = 15;
//	 spec.sen_ref_wall_L = 10;
//	 spec.sen_diff_threshold = 5;
//
//	 spec.sen_ref_non_CR = 10;
//	 spec.sen_ref_non_R = 10;
//	 spec.sen_ref_non_CL = 20;
//	 spec.sen_ref_non_L = 10;
//	 */
//	spec.pwm_base_clock = 6250 - 1; //(interrupt duty : 1ms(@6.25MHz))
//	spec.pwm_half_clock = round((6250 - 1) / 2);
//}

//void init_Sensor(void) {
//	r_sen.non_threshold = 110;
//	cr_sen.non_threshold = 110;
//	cl_sen.non_threshold = 110;
//	l_sen.non_threshold = 110;
//
//	r_sen.diff_threshold = 2;
//	cr_sen.diff_threshold = 657;
//	cl_sen.diff_threshold = 434;
//	l_sen.diff_threshold = 2;
//
//	r_sen.ref_wall = 421;
//	l_sen.ref_wall = 240;
//	cr_sen.ref_wall = 802 - 400;
//	cl_sen.ref_wall = 948 - 400;
//
//	for (n = 0; n < 9; n++) {
//		r_sen.old[n] = 0;
//		cr_sen.old[n] = 0;
//		l_sen.old[n] = 0;
//		cl_sen.old[n] = 0;
//	}
//}

void initMOT(void) {
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
}

//void initCMT(void) {	//CMT割込の設定
//	STB.CR4.BIT._CMT = 0;		//CMTスタンバイ解除
//	//  (1)コンペアマッチタイマスタートレジスタ（CMSTR）
//	CMT.CMSTR.BIT.STR0 = 0;		// ステータスレジスタ　0：カウント停止, 1：カウント開始
//	CMT.CMSTR.BIT.STR1 = 0;
//	//  (2)コンペアマッチタイマコントロール／ステータスレジスタ（CMCSR）
//	CMT0.CMCSR.BIT.CMIE = 1;    //割り込みイネーブル許可
//	CMT0.CMCSR.BIT.CKS = 0;     //1/8
//	CMT0.CMCSR.BIT.CMF = 0;     //フラグをクリア
//	CMT0.CMCOR = 3125 - 1;  //割り込み周期(1ms)
//	INTC.IPRJ.BIT._CMT0 = 0xf;  //割り込み優先度(15)
//	CMT.CMSTR.BIT.STR0 = 1;		// ステータスレジスタ 1：カウント開始
//
//	CMT1.CMCSR.BIT.CMIE = 1;    //割り込みイネーブル許可
//	CMT1.CMCSR.BIT.CKS = 0;     //1/8
//	CMT1.CMCSR.BIT.CMF = 0;     //フラグをクリア
//	CMT1.CMCOR = 3125 - 1;  //割り込み周期(1ms)
//	INTC.IPRJ.BIT._CMT1 = 0xe;  //割り込み優先度(15)
//	CMT.CMSTR.BIT.STR1 = 1;		// ステータスレジスタ 1：カウント開始
//}

/*
 void initMTU_S(void){
 STB.CR4.BIT._MTU2 = 0; //スタンバイ解除
 MTU2.TSTR.BYTE = 0x00;	//タイマストップ


 MTU23.TCR.BIT.TPSC = 1; //set clock 25/4=6.25MHz(0.16us)
 MTU23.TCR.BIT.CCLR = 1; //TGRAのコンペアマッチでTCNTクリア

 MTU23.TCNT = 0; // reset counter
 MTU24.TCNT = 0;

 MTU23.TGRA = 6250 - 1; // set sensor's PWM clock
 MTU23.TGRB = 6250 - 1; // set sensor's PWM duty
 MTU24.TGRA = 6250 - 1; // set sensor's PWM duty
 MTU24.TGRB = 6250 - 1; // set sensor's PWM duty

 MTU2.TOCR1.BIT.PSYE = 1; //toggle enable
 MTU2.TOCR1.BIT.OLSN = 0; // set low-High mode
 MTU2.TOCR1.BIT.OLSP = 1; // set low-High mode

 MTU23.TMDR.BIT.MD = 8; //reset sync PWM mode

 MTU2.TOER.BYTE = 0xFF; // enable PWM output

 MTU23.TIER.BIT.TGIEA = 1; //enable interrupt
 INTC.IPRE.BIT._MTU23G = 0xC; //set interrupt priority

 }
 */

void initMTU(void) {
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

	MTU20.TGRA = 6250 - 1; // set PWM clock
	MTU20.TGRB = round((6250 - 1) / 2); // set duty

	MTU21.TGRA = 6250 - 1; // set PWM clock
	MTU21.TGRB = round((6250 - 1) / 2); // set duty
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

void initMOTOR(void) {
	PE.DRL.BIT.B2 = 0; //reset (0 : off, 1 : on)
	PE.DRL.BIT.B3 = 0; //excitation_enable (1 : on, 0 : off)

	PE.DRL.BIT.B1 = 0; //R_CW/CCW(0 : forward, 1 : backward)
	PE.DRL.BIT.B5 = 1; //L_CW/CCW(1 : forward, 0 : backward)

	PE.DRL.BIT.B0 = 1; //R_Clock
	PE.DRL.BIT.B4 = 1; //L_Clock

	PE.DRL.BIT.B2 = 0; //reset(0 : off, 1 : on)
	PE.DRL.BIT.B3 = 1; //excitation_enable (1 : on, 0 : off)
}

//void initAD(void) {
//
//	/*
//	ch0 :  	AD0.ADCSR.BIT.CH = 0;
//	ch1 : 	AD0.ADCSR.BIT.CH = 1;
//	ch2 :	AD0.ADCSR.BIT.CH = 2;
//	ch3 :	AD0.ADCSR.BIT.CH = 3;
//
//	ch4 :  	AD1.ADCSR.BIT.CH = 0;
//	ch5 : 	AD1.ADCSR.BIT.CH = 1;
//	ch6 :	AD1.ADCSR.BIT.CH = 2;
//	ch7 :	AD1.ADCSR.BIT.CH = 3;
//	 */
//
//	STB.CR4.BIT._AD0 = 0;		//AD0のスタンバイを解除
//	STB.CR4.BIT._AD1 = 0;		//AD1のスタンバイを解除
//
//	AD0.ADCR.BIT.ADST = 0;		//AD停止
//	AD0.ADCSR.BIT.ADCS = 0;		//サイクルスキャンしない
//	AD0.ADCSR.BIT.TRGE = 0;		//トリガーイネーブル無効
//	AD0.ADCSR.BIT.CKSL = 0;		//周辺回路動作クロック
//	AD0.ADCSR.BIT.ADIE = 0;		//割込み禁止
//	AD0.ADCSR.BIT.ADM = 0;		//シングルスキャン
//	AD0.ADCSR.BIT.CH = 0;		//ch0選択
//
//	AD1.ADCR.BIT.ADST = 0;		//AD停止
//	AD1.ADCSR.BIT.ADCS = 0;		//サイクルスキャンしない
//	AD1.ADCSR.BIT.TRGE = 0;		//トリガイネーブル無効
//	AD1.ADCSR.BIT.CKSL = 0;		//周辺動作クロック
//	AD1.ADCSR.BIT.ADIE = 0;		//割込み禁止
//	AD1.ADCSR.BIT.ADM = 0;		//シングルスキャン
//	AD1.ADCSR.BIT.CH = 0;		//ch4選択
//}


//void initCPU(void) {
//	CPG.FRQCR.BIT.IFC = 1;                //Iφ / 2 = 50Mhz(初期値/4)
//	CPG.FRQCR.BIT.BFC = 3;        // Bφ / 4 = 25MHz
//	CPG.FRQCR.BIT._PFC = 3;    // Pφ / 4 = 25MHz
//	CPG.FRQCR.BIT.MPFC = 3;    // MPφ / 4 = 25MHz
//}

void initALL(void) {
	init_CPU();
	init_IO();
	init_Spec();
	init_Buzzer();
	init_Sensor();
	initMOT();
	init_CMT();
	init_LED();
	//initMTU_S();
	initMTU();
	initMOTOR();
	init_ADC();
}
