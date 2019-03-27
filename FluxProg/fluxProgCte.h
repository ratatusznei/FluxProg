#ifndef _FLUX_PROG_CTE_H_
#define _FLUX_PROG_CTE_H_

//acesso e criacao de memoria
#define NOME_DA_MEMORIA "memoria"
#define NOME_DA_MEMORIA2 "memoria2"
#define TAMANHO_DA_MEMORIA 65536
#define NOME_DO_INT_NA_MEMORIA1 "comando1"
#define NOME_DO_INT_NA_MEMORIA2 "comando2"
#define SENSOR_VISAO "reading_VS"
#define SENSOR_ULTRASSOM "readin_U"
#define POSICAO_DETECTADA "detectedObjet_U"
#define POSICAO "linPosition"
#define ANGULAR "angPosition"
#define BLUETOOTH_ENABLE "bluetooth"

//fisicas
#define N_ULTRASONIC 3
#define N_BLACK_TAPE_SENSOR 5
#define N_COLOR_SENSOR 2

//Fluxprog
#define PLAY 1
#define PAUSE 2
#define STOP 3
#define SAVE 4
#define LOAD 5
#define SAVE_AS 6
#define PHYSICAL_ROBOT 7
#define VIRTUAL_ROBOT 8
#define CONDITIONAL_BLOCK 9
#define ACTION_BLOCK 10
#define START_BLOCK 11
#define END_BLOCK 12
#define MERGE_BLOCK 13
#define LOOP_BLOCK 14
#define BLACK_TAPE_SENSOR_MENU 15
#define SENSOR_COLOR_MENU 16
#define ULTRASONIC_SENSOR_MENU 17
#define MOVE_FORWARD_BLOCK 18
#define TURN_LEFT_BLOCK 19
#define TURN_RIGHT_BLOCK 20
#define N_LOOP_BLOCK 21
#define T_LOGIC_BLOCK 22
#define F_LOGIC_BLOCK 23

//communicacao entre back e front end
#define CLOSE_PROGRAM -10
#define ERROR -5
#define READY 1
#define EXECUTING 0
#define MOVE_FORWARD 8
#define TURN_LEFT 4
#define TURN_RIGHT 6
#define N_ULTRASONIC 3
#define N_BLACK_TAPE_SENSOR 5
#define N_COLOR_SENSOR 2
#endif