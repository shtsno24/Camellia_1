/*
 * motor.h
 *
 *  Created on: 2018/04/14
 *      Author: shts
 */

#ifndef MOTOR_H_
#define MOTOR_H_

enum motor {
	R_motor = 0, L_motor = 1
};

enum turn {
	back = 0, left = 1, right = 2, straight = 3,
};

typedef struct mot {
	int cnt; //ステップ数
	int duty; //デューティー

	float vel; //現在の速度
	float acc; //現在の設定加速度
	float max_vel; //最大設定速度
	float min_vel; //最小設定速度
	float max_acc; //最大設定加速度
	float min_acc; //最小設定加速度
	float tar_vel; //現在の設定速度
	float dist; //距離

	unsigned char rot_dir_flag; //回転方向
	unsigned char stop_flag;
	unsigned char end_flag;

} MOT;

void init_Motor(void);

void drv_Motor(int, int, int, char, int, int);

void switch_Motor(char);

void mot_app(int, int, int, char, char);

void mot_app2(int, int, int, char, char);

void move_Left(void);

void move_Right(void);

void move_Forward(void);

void move_Backward(void);

void move_Backward_2(void);

#endif /* MOTOR_H_ */