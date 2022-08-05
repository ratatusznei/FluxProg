#ifndef _INTERFACE_HPP_
#define _INTERFACE_HPP_

#include <allegro5/allegro.h> //biblioteca matriz do allegro
#include <allegro5/allegro_color.h> //biblioteca para rotinas de cor
#include <allegro5/allegro_primitives.h> //biblioteca para desenho de formas geométricas
#include <allegro5/allegro_image.h> // biblioteca para importação e desenho de imagens
#include <allegro5/allegro_native_dialog.h> //biblioteca para uso de diálogos nativos
#include <allegro5/allegro_font.h> // biblioteca para escrita/ uso de fontes
#include <allegro5/allegro_ttf.h> // biblioteca para uso de fontes TrueType
#include <iostream> // usada para debugging
#include <cmath>
#include <fstream>
#include <cstring>
#include <signal.h>
#include "Block.hpp"
#include "fluxProgCte.h"
#include <libgen.h>


using namespace std;

class Interface {
    ALLEGRO_COLOR black, scroll_bar_color;
    ALLEGRO_COLOR backgroud_color;
    ALLEGRO_COLOR white, strange_color;
    ALLEGRO_COLOR primary_menu_color, blocks_menu_color, functions_menu_color, sensors_menu_color, numbers_menu_color, new_menu_color;
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_BITMAP *play_button, *play_button_selected, *pause_button, *pause_button_selected;
    ALLEGRO_BITMAP *stop_button, *stop_button_selected, *save_button, *save_button_selected;
    ALLEGRO_BITMAP *load_button, *load_button_selected, *save_as_button, *save_as_button_selected;
    ALLEGRO_BITMAP *vrep_button, *vrep_button_selected, *bluetooth_button, *bluetooth_button_selected, *mini_menu[6];
    ALLEGRO_BITMAP *WALK_FOWARD_ACTION, *TURN_LEFT_ACTION, *TURN_RIGHT_ACTION;
    ALLEGRO_BITMAP *MICRO_WALK_FOWARD, *MICRO_TURN_LEFT, *MICRO_TURN_RIGHT;
    ALLEGRO_BITMAP *NUMBER[10], *logic_true, *logic_false;
    ALLEGRO_BITMAP *MICRO_NUMBER[10];
    ALLEGRO_BITMAP *DECISION_BLOCK[4], *BLACK_SENSOR_FUNCTION, *BLACK_SENSOR_1_FUNCTION, *BLACK_SENSOR_2_FUNCTION, *BLACK_SENSOR_3_FUNCTION, *BLACK_SENSOR_4_FUNCTION;
    ALLEGRO_BITMAP *BLACK_SENSOR_5_FUNCTION, *COLOR_SENSOR_FUNCTION, *COLOR_SENSOR_1_FUNCTION, *COLOR_SENSOR_2_FUNCTION, *ULTRASONIC_SENSOR_FUNCTION;
    ALLEGRO_BITMAP *ULTRASONIC_SENSOR_1_FUNCTION, *ULTRASONIC_SENSOR_2_FUNCTION, *ULTRASONIC_SENSOR_3_FUNCTION;
    ALLEGRO_BITMAP *MICRO_BLACK_SENSOR1, *MICRO_BLACK_SENSOR2, *MICRO_BLACK_SENSOR3, *MICRO_BLACK_SENSOR4, *MICRO_BLACK_SENSOR5;
    ALLEGRO_BITMAP *MICRO_COLOR_SENSOR1, *MICRO_COLOR_SENSOR2, *MICRO_ULTRASONIC_SENSOR1, *MICRO_ULTRASONIC_SENSOR2, *MICRO_ULTRASONIC_SENSOR3;
    ALLEGRO_BITMAP *END_BLOCK_IMG[4], *FUNCTION_BLOCK[4], *LOOP_BLOCK_IMG[4], *MERGE_BLOCK_IMG[4], *START_BLOCK_IMG[4];
    ALLEGRO_BITMAP *POINT[2];
    ALLEGRO_BITMAP *trash;
    ALLEGRO_BITMAP *arrow;
    ALLEGRO_FONT *font;
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_EVENT events;
    int mouseX, mouseY, mouseZ, mouseZaux;
    bool inicialize_mouseZ;
    int menu_selected;
    int mouse_aux_x, mouse_aux_y; // usado para arrastar as imagens;
    bool black_sensor_menu_selected, color_sensor_menu_selected, ultrasonic_sensor_menu_selected;
    bool number_menu_selected;
    bool dragging_number_0, dragging_number_1, dragging_number_2, dragging_number_3, dragging_number_4;
    bool dragging_number_5, dragging_number_6, dragging_number_7, dragging_number_8, dragging_number_9;
    bool dragging_black_sensor1, dragging_black_sensor2, dragging_black_sensor3, dragging_black_sensor4, dragging_black_sensor5;
    bool dragging_color_sensor1, dragging_color_sensor2;
    bool dragging_ultrasonic_sensor1, dragging_ultrasonic_sensor2, dragging_ultrasonic_sensor3;
    bool dragging_walk_foward, dragging_turn_left, dragging_turn_right;
    bool sub_menu;
    int menu1_X_limit;
    int menu1_Y_limit;
    int menu_connect_X_begin;
    int menu_connect_X_limit;
    int menu2_X_limit;
    int menu_blocks_Y_begin;
    int menu_blocks_Y_limit;
    int menu_sensors_Y_begin;
    int menu_sensors_Y_limit;
    int menu_actions_Y_begin;
    int menu_actions_Y_limit;
    int menu_extra_Y_begin;
    int menu_extra_Y_limit;
    int temporary_line_X, temporary_line_Y;
    bool drawing_line;
    int block_selected;
    int number_of_selected_out;
    Block** blocks_list_to_print;
    int menu_click;
    bool executing;
    bool connected_simulator;
    bool connected_robot;
    bool executing_fluxogram;
    string program_path;
    int scroll_bar_x, scroll_bar_y;
    //Imagens V3
    ALLEGRO_BITMAP *NEW_MINI_MENU_IMG[4];
    ALLEGRO_BITMAP *ATTRIBUTION_BLOCK_IMG[4], *MATH_BLOCK_IMG[4], *NEW_CONDITIONAL_BLOCK_IMG[4], *WHILE_BLOCK_IMG[4];
    ALLEGRO_BITMAP *BOOL_VAR_IMG[10], *INT_VAR_IMG[10], *LOGIC_OPERATOR_IMG[6], *MATH_OPERATOR_IMG[5];
    ALLEGRO_BITMAP *MICRO_BOOL_VAR_IMG[10], *MICRO_INT_VAR_IMG[10], *MICRO_LOGIC_OPERATOR_IMG[6], *MICRO_MATH_OPERATOR_IMG[5];
    ALLEGRO_BITMAP *SLOT_NUMBER[10];
    //v3
    bool int_menu_selected, bool_menu_selected, logic_menu_selected, math_menu_selected;
    //v3 dragging
    bool dragging_int_A, dragging_int_B, dragging_int_C, dragging_int_D, dragging_int_E;
    bool dragging_int_F, dragging_int_G, dragging_int_H, dragging_int_I, dragging_int_J;
    bool dragging_bool_A, dragging_bool_B, dragging_bool_C, dragging_bool_D, dragging_bool_E;
    bool dragging_bool_F, dragging_bool_G, dragging_bool_H, dragging_bool_I, dragging_bool_J;
    bool dragging_equal, dragging_greater_equal, dragging_less_equal, dragging_greater, dragging_less, dragging_unequal;
    bool dragging_add, dragging_div, dragging_exponent, dragging_mult, dragging_sub;
    int** int_var_list;
    bool** bool_var_list;
    //v3 limit
    int menu_var_and_symbols_y_begin;
    int menu_var_and_symbols_y_limit;
    int menu_new_blocks_y_begin;
    int menu_new_blocks_y_limit;

    void load_bitmap(ALLEGRO_BITMAP **bitmap, const char *adress);
    void print_primary_menu();
    void print_secondary_menu();
    void print_function_block(Block *b);
    void print_end_block(Block *b);
    void print_start_block(Block *b);
    void print_loop_block(Block *b);
    void print_decision_block(Block *b);
    void print_merge_block(Block *b);
    void load_program_images();
    void destroy_program_images();
    void reset_dragging_variables();
    void check_dragging();
    void draw_dragging();
    void check_mouse_on_menus();
    void print_list_of_blocks();
    void check_mouse_on_points(Block *b);
    void draw_temporary_line();
    bool check_colisions();
    void draw_lines();
    void delete_connections();
    void draw_everything();
    bool check_enable_menu();
    void check_drop_arrow();
    void check_drag_block_or_begin_arrow();
    void check_menu_selected();
    void check_scrolling();
    void draw_scroll_bar();
    //v3
    void print_attribution_block(Block *b);
    void print_math_block(Block *b);
    void print_new_conditional_block(Block *b);
    void print_while_block(Block *b);

public:

    Interface(Block** _blocks_list_to_print, string _program_path, int** _int_var_list, bool** _bool_var_list);
    ~Interface();
    void draw();
    int getMenuClick();
    int getMouseX();
    int getMouseY();
    bool getExecuting();
    int callMessage(int i);
    int getImageHeight(int i);
    int getImageWidth(int i);
    void setConnectedSimulator(bool c);
    void setConnectedRobot(bool c);
    void setExecutingFluxogram(bool e);
    void reset_scrollbar();
    char* save_file_window();
    char* open_file_window();


};
#endif
