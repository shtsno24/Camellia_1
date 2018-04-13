/*
 * CMT.c
 *
 *  Created on: 2017/09/17
 *      Author: shts
 */

#include "iodefine.h"
#include "CMT.h"

CMT_01 tim;

void init_CMT(void) {	//CMT割込の設定
	STB.CR4.BIT._CMT = 0;		//CMTスタンバイ解除
	//  (1)コンペアマッチタイマスタートレジスタ（CMSTR）
	CMT.CMSTR.BIT.STR0 = 0;		// ステータスレジスタ　0：カウント停止, 1：カウント開始
	CMT.CMSTR.BIT.STR1 = 0;
	//  (2)コンペアマッチタイマコントロール／ステータスレジスタ（CMCSR）
	CMT0.CMCSR.BIT.CMIE = 1;    //割り込みイネーブル許可
	CMT0.CMCSR.BIT.CKS = 0;     //1/8
	CMT0.CMCSR.BIT.CMF = 0;     //フラグをクリア
	CMT0.CMCOR = 3125 - 1;  //割り込み周期(1ms)
	INTC.IPRJ.BIT._CMT0 = 0xf;  //割り込み優先度(15)
	CMT.CMSTR.BIT.STR0 = 1;		// ステータスレジスタ 1：カウント開始

	CMT1.CMCSR.BIT.CMIE = 1;    //割り込みイネーブル許可
	CMT1.CMCSR.BIT.CKS = 0;     //1/8
	CMT1.CMCSR.BIT.CMF = 0;     //フラグをクリア
	CMT1.CMCOR = 3125 - 1;  //割り込み周期(1ms)
	INTC.IPRJ.BIT._CMT1 = 0xe;  //割り込み優先度(15)
	CMT.CMSTR.BIT.STR1 = 1;		// ステータスレジスタ 1：カウント開始
}

void interrupt_cmt0() {
// write this function to interrupt_handlers.c
	CMT0.CMCSR.BIT.CMF = 0;
	tim.count_cmt_0 += 1;
}
void interrupt_cmt1() {
// write this function to interrupt_handlers.c
	CMT1.CMCSR.BIT.CMF = 0;
	tim.count_cmt_1 += 1;
}

void wait_ms(int t) {
	tim.count_cmt_0 = 0;
	CMT0.CMCNT = 0;
	while (tim.count_cmt_0 < t)
		;
}

