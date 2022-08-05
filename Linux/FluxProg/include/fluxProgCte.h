#ifndef _FLUX_PROG_CTE_H_
#define _FLUX_PROG_CTE_H_

#define WINDOWS_USER //comentar se estiver desenvolvendo no windows
//acesso e criacao de memoria
#define MEMORY_BLOCK "memory"
#define MEMORY_BLOCK_SIZE 65536
#define MEMORY_COMMAND "command"
#define MEMORY_ABSTRACTION "abstraction_level"
#define MEMORY_FEEDBACK "feedback"
#define MEMORY_VISION_SENSOR "reading_VS"
#define MEMORY_ULTRASONIC_SENSOR "readin_US"
#define MEMORY_ROBOT_TYPE "robot_type"

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

//definicoes Fluxprog v3
#define INT_MENU 75
#define BOOL_MENU 76
#define LOGIC_AND_MATH_MENU 77
//#define MATH_MENU 78
#define ATTRIBUTION_BLOCK 78
#define MATH_BLOCK 79
#define NEW_CONDITIONAL_BLOCK 80
#define WHILE_BLOCK 81

#define BLACK_SENSOR_1 1
#define BLACK_SENSOR_2 2
#define BLACK_SENSOR_3 3
#define BLACK_SENSOR_4 4
#define BLACK_SENSOR_5 5
#define COLOR_SENSOR_1 6
#define COLOR_SENSOR_2 7
#define ULTRASONIC_SENSOR_1 8
#define ULTRASONIC_SENSOR_2 9
#define ULTRASONIC_SENSOR_3 10

//communicacao entre back e front end
#define CLOSE_PROGRAM -10
#define CONNECT_ERROR -5
#define CONNECT -1
#define READY 1
#define EXECUTING 0
#define MOVE_FORWARD 8
#define TURN_LEFT 4
#define TURN_RIGHT 6
#define N_ULTRASONIC 3
#define N_BLACK_TAPE_SENSOR 5
#define N_COLOR_SENSOR 2
#define HIGH_ABSTRACTION 0
#define MID_ABSTRACTION 1
#define LOW_ABSTRACTION 2
#define COLISION -2

#define valor_maximo_blocos 100

#define scroll_bar_y_begin 84
//#define display_width 670
#define display_width 800
#define display_height 925
#define roll_bar_width 20
#define roll_bar_height 30

#define TIMEOUT 5.0

#endif
