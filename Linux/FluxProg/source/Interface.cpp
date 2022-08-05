#include "Interface.hpp"

Interface :: Interface(Block** _blocks_list_to_print, string _program_path, int** _int_var_list, bool** _bool_var_list) {

    //variavel que diz se o programa ainda está executando ou não
    executing = true;
    program_path = _program_path;
    connected_robot = false;
    connected_simulator = false;
    executing_fluxogram = false;

    //inicializa o allegro, dá msg de erro caso falhe alguma inicializacao
    if(!al_init()) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do Allegro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;
    }
    if(!al_init_image_addon()) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do Allegro_image", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;
    }
    if(!al_init_primitives_addon()) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do Allegro_primitives", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;;
    }
    if(!al_init_native_dialog_addon()) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do Allegro_native_dialog", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;;
    }
    if(!al_install_keyboard()) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do Allegro_keyboard", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;;
    }
    if(!al_install_mouse()) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do Allegro_mouse", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;;
    }
    al_init_font_addon();
    /*
    if(!al_init_font_addon()) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do Allegro_font", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;;
    }
    */
    if(!al_init_ttf_addon()) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do Allegro_ttf", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;;
    }

    //CRIA A JANELA DO PROGRAMA E SETA AS VARIÁVEIS DA JANELA
    //al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_MAXIMIZED);
    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
    display = al_create_display(display_width, display_height);
    al_set_window_position(display, 40, 40);
    al_set_window_title(display, "Fluxprog");
    al_clear_to_color(al_map_rgb(0, 0, 0));
    //al_set_window_constraints(display, 670, 630, 5000, 5000);
    //al_apply_window_constraints(display, true);

    timer = al_create_timer(0.5);
    if(!timer) {
       al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do timer", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
       executing = false;;
    }

    //inicialização das cores
    black = al_map_rgb(0, 0, 0);
    backgroud_color = al_map_rgb(128, 128, 128);
    white = al_map_rgb(255, 255, 255);
    strange_color = al_map_rgb(0, 255, 255);
    primary_menu_color = al_map_rgb(176, 175, 175);
    blocks_menu_color = al_map_rgb(255, 0, 0);
    functions_menu_color = al_map_rgb(255, 0, 0);
    sensors_menu_color = al_map_rgb(0, 0, 255);
    scroll_bar_color = al_map_rgb(74, 62, 62);
    numbers_menu_color = al_map_rgb(255, 124, 71);
    new_menu_color = al_map_rgb(0, 128, 0);

    reset_dragging_variables();

    black_sensor_menu_selected = false;
    color_sensor_menu_selected = false;
    ultrasonic_sensor_menu_selected = false;
    number_menu_selected = false;
    sub_menu = false;
    drawing_line = false;
    temporary_line_X = 0;
    temporary_line_Y = 0;
    inicialize_mouseZ = false;
    scroll_bar_x = al_get_display_width(display)-roll_bar_width;
    scroll_bar_y = scroll_bar_y_begin;

    //v3 selected
    int_menu_selected = false;
    bool_menu_selected = false;
    logic_menu_selected = false;
    math_menu_selected = false;

    // checa se o display foi inicializado corretamente, se não foi dá msg de erro
    if(!display) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do display", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;;
    }

    // carrega todas as imagens usadas no programa, se alguma não for carregada dá msg de erro
    #ifdef _WIN32
        al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
    #endif // _WIN32
    load_program_images();

    //carrega a fonte, dá msg de erro caso não consiga ser carregada
    string address = program_path;
    address = address + "/../../GUI_files/OpenSans-Regular.ttf";
    #ifdef _WIN32
        #ifdef WINDOWS_USER
            address = program_path + "GUI_files/OpenSans-Regular.ttf";
        #endif // _WIN32
    #endif
    //cout << address <<endl;
    font = al_load_font(address.c_str(), 10, 0);
    if(!font) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao da fonte", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;;
    }

    blocks_list_to_print = _blocks_list_to_print;
    //v3
    int_var_list = _int_var_list;
    bool_var_list = _bool_var_list;

}
Interface :: ~Interface() {

    //destroi os objetos usados pelo allegro
	destroy_program_images();

	al_destroy_font(font);

    al_destroy_timer(timer);

	al_shutdown_image_addon();
	al_shutdown_primitives_addon();
	al_shutdown_native_dialog_addon();
	al_shutdown_font_addon();
	al_shutdown_ttf_addon();
    al_destroy_display(display);

}

void Interface :: draw() {
    menu_click = 0;

    //avisa o allegro sobre a mudança de tamanho do display para que as variáveis sejam atualizadas
    al_acknowledge_resize(display);
    //seta a cor de fundo da tela
    al_clear_to_color(backgroud_color);

    //desenha
    draw_everything();

    //atualiza o display
    al_flip_display();

    //cria a lista com eventos
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
    //espera pelo próximo evento
    al_wait_for_event(event_queue, &events);

    if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        //se a janela for fechada o programa para de executar
        executing = false;
    }

    if(events.type == ALLEGRO_EVENT_MOUSE_AXES) {
        //atualiza as variaveis com as posições do mouse
        mouseX = events.mouse.x;
        mouseY = events.mouse.y;

        if(mouseY > al_get_bitmap_height(play_button)) {
            if(inicialize_mouseZ == false) {
                mouseZ = events.mouse.z;
                mouseZaux = events.mouse.z;
                inicialize_mouseZ = true;
            }
            if(mouseZaux == events.mouse.z) {
                mouseZ = 0;
            }
            if(mouseZaux > events.mouse.z) {
                    mouseZ = -8;
                    mouseZaux = events.mouse.z;
            }
            if (mouseZaux < events.mouse.z){
                mouseZ = 8;
                mouseZaux = events.mouse.z;
            }

            bool at_limit = false;
            //cout<<"mouseZ: "<<mouseZ<<endl;
            if((scroll_bar_y >= scroll_bar_y_begin) && (scroll_bar_y <= (al_get_display_height(display)-al_get_bitmap_height(trash) - roll_bar_height))){
                scroll_bar_y = scroll_bar_y - (mouseZ/2);
                at_limit = false;
            }
            if(scroll_bar_y < scroll_bar_y_begin) {
                scroll_bar_y = scroll_bar_y_begin;
                at_limit = true;
            }
            if(scroll_bar_y >= (al_get_display_height(display)-al_get_bitmap_height(trash) - roll_bar_height)) {
                scroll_bar_y = al_get_display_height(display)-al_get_bitmap_height(trash) - roll_bar_height;
                at_limit = true;
            }
            if(at_limit == false) {
                check_scrolling();
            }
        }
        //coxambre pra arrumar o tamanho da janela
        if(al_get_display_width(display) < 670) {
            al_resize_display(display, 670, al_get_display_height(display));
        }
    }

    if(events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

        if(events.mouse.button == 1) {
            check_drop_arrow();

            check_drag_block_or_begin_arrow();

            check_menu_selected();
        }
    }

    if(events.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        if(events.mouse.button == 1) {
            if(check_enable_menu() == true) {
                cout<<"ALLEGRO_EVENT_MOUSE_BUTTON_UP - Valor = "<<menu_selected<<endl;
                if(menu_selected == PLAY) {
                    //cout<<"play"<<endl;
                    menu_click = PLAY;
                }
                if(menu_selected == PAUSE) {
                    //cout<<"pause"<<endl;
                    menu_click = PAUSE;
                }
                if(menu_selected == STOP) {
                    //cout<<"stop"<<endl;
                    menu_click = STOP;
                }
                if(menu_selected == SAVE) {
                    cout<<"save"<<endl;
                    menu_click = SAVE;
                }
                if(menu_selected == LOAD) {
                    cout<<"load"<<endl;
                    menu_click = LOAD;
                }
                if(menu_selected == SAVE_AS) {
                    cout<<"save_as"<<endl;
                    menu_click = SAVE_AS;
                }
                if(menu_selected == PHYSICAL_ROBOT) {
                    //cout<<"bluetooth"<<endl;
                    menu_click = PHYSICAL_ROBOT;
                }
                if(menu_selected == VIRTUAL_ROBOT) {
                    //cout<<"vrep"<<endl;
                    menu_click = VIRTUAL_ROBOT;
                }
                if(menu_selected == BLACK_TAPE_SENSOR_MENU) {
                    black_sensor_menu_selected = true;
                }
                if(menu_selected == SENSOR_COLOR_MENU) {
                    color_sensor_menu_selected = true;
                }
                if(menu_selected == ULTRASONIC_SENSOR_MENU) {
                    ultrasonic_sensor_menu_selected = true;
                }
                if(menu_selected == N_LOOP_BLOCK) {
                    cout<<"draw() - number_menu_selected = true"<<endl;
                    number_menu_selected = true;
                }
                if(menu_selected == T_LOGIC_BLOCK) {
                    cout<<"verdadeiro"<<endl;
                }
                if(menu_selected == F_LOGIC_BLOCK) {
                    cout<<"falso"<<endl;
                }
                //v3
                if(menu_selected == INT_MENU) {
                    cout<<"draw() - int_menu_selected = true"<<endl;
                    int_menu_selected = true;
                }
                if(menu_selected == BOOL_MENU) {
                    cout<<"draw() - bool_menu_selected = true"<<endl;
                    bool_menu_selected = true;
                }
                if(menu_selected == LOGIC_AND_MATH_MENU) {
                    cout<<"draw() - logic_menu_selected = true"<<endl;
                    logic_menu_selected = true;
                }
                if(menu_selected == LOGIC_AND_MATH_MENU) {
                    //math_menu_selected = true;
                }
            }
            //testa se soltou o bloco na lixeira
            for(int i=0; i<valor_maximo_blocos; i++) {
                if(blocks_list_to_print[i] != NULL) {
                    if(blocks_list_to_print[i]->getDragging() == true) {
                        if((mouseX > (al_get_display_width(display)-al_get_bitmap_width(trash))) && (mouseX < al_get_display_width(display)) && (mouseY > (al_get_display_height(display)-al_get_bitmap_height(trash))) && (mouseY < al_get_display_height(display))) {
                            //remove_block(blocks_list_to_print[i]);
                            blocks_list_to_print[i]->setDelete(true);
                        } else {
                            blocks_list_to_print[i]->setDragging(false);
                        }
                    }
                }
            }
            //checa se soltou alguma funçao ou sensor
            check_dragging();

            reset_dragging_variables();
        }

    }

    if(events.type == ALLEGRO_EVENT_KEY_DOWN) {

        //cout<<"tecla apertada: " << events.keyboard.keycode <<endl;
        if((events.keyboard.keycode == 77) || (events.keyboard.keycode == 90)){
            if(drawing_line == false) {
                for(int i=0; i<valor_maximo_blocos; i++) {
                    if(blocks_list_to_print[i] != NULL) {
                        if(blocks_list_to_print[i]->getSelected() == true) {
                            //remove_block(blocks_list_to_print[i]);
                            blocks_list_to_print[i]->setDelete(true);
                        }
                    }
                }
            }
            delete_connections();
        }
    }

    if(events.type == ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY) {
        menu_selected = 0;
        mouseX = 0;
        mouseY = 0;
    }

    while(check_colisions());

    al_destroy_event_queue(event_queue);
}

void Interface :: load_bitmap(ALLEGRO_BITMAP **bitmap, const char *adress) {
    string address = program_path;
    address = address + adress;
    #ifdef _WIN32
        #ifdef WINDOWS_USER
            size_t pos = address.find("../", 0);
            while(pos != string::npos)
            {
                address.replace(pos, 3, "");
                pos = address.find("../", pos);
            }
        #endif
    #endif //_WIN32
    //se não achar a imagem no diretorio especificado dá msg de erro e para a execução
    if(!al_load_bitmap(address.c_str())) {
        const char* str1 = "Não encontrou imagem: ";
        char * str = (char *) malloc(1 + strlen(str1)+ strlen(adress));
        strcpy(str, str1);
        strcat(str, adress);
        al_show_native_message_box(display, "Fluxprog", "ERRO", str, "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;
    } else {
        *bitmap = al_load_bitmap(address.c_str());
    }
}
void Interface :: print_primary_menu() {
    //desenha o retangulo no qual ficam os botões de opções
    al_draw_filled_rectangle(0, 0, al_get_display_width(display), (4 + al_get_bitmap_height(play_button)), primary_menu_color);

    //desenha os botões do menu e checa a posição do mouse para ver se estão selecionados ou não
    if((mouseX < 14 + al_get_bitmap_width(play_button)) && (mouseX > 14) && (mouseY >= 2) && (mouseY <= 2+ al_get_bitmap_height(play_button))) {
        al_draw_bitmap(play_button_selected, 14 + 0*al_get_bitmap_width(play_button), 2, 0);
        menu_selected = PLAY;
        cout<<"print_primary_menu() - menu_selected = "<<menu_selected<<endl;
    } else {
        al_draw_bitmap(play_button, 14 + 0*al_get_bitmap_width(play_button), 2, 0);
    }
    if((mouseX < 14 + 2*al_get_bitmap_width(pause_button)) && (mouseX > 14 + al_get_bitmap_width(pause_button)) && (mouseY >= 2) && (mouseY <= 2+ al_get_bitmap_height(pause_button))) {
        al_draw_bitmap(pause_button_selected, 14 + 1*al_get_bitmap_width(pause_button), 2, 0);
        menu_selected = PAUSE;
        cout<<"print_primary_menu() - menu_selected = "<<menu_selected<<endl;
    } else {
        al_draw_bitmap(pause_button, 14 + 1*al_get_bitmap_width(pause_button), 2, 0);
    }
    if((mouseX < 14 + 3*al_get_bitmap_width(stop_button)) && (mouseX > 14 + 2*al_get_bitmap_width(stop_button)) && (mouseY >= 2) && (mouseY <= 2+ al_get_bitmap_height(stop_button))) {
        al_draw_bitmap(stop_button_selected, 14 + 2*al_get_bitmap_width(stop_button), 2, 0);
        menu_selected = STOP;
        cout<<"print_primary_menu() - menu_selected = "<<menu_selected<<endl;
    } else {
        al_draw_bitmap(stop_button, 14 + 2*al_get_bitmap_width(stop_button), 2, 0);
    }
    if((mouseX < 14 + 4*al_get_bitmap_width(save_button)) && (mouseX > 14 + 3*al_get_bitmap_width(save_button)) && (mouseY >= 2) && (mouseY <= 2+ al_get_bitmap_height(save_button))) {
        al_draw_bitmap(save_button_selected, 14 + 3*al_get_bitmap_width(save_button), 2, 0);
        menu_selected = SAVE;
        cout<<"print_primary_menu() - menu_selected = "<<menu_selected<<endl;
    } else {
        al_draw_bitmap(save_button, 14 + 3*al_get_bitmap_width(save_button), 2, 0);
    }
    if((mouseX < 14 + 5*al_get_bitmap_width(load_button)) && (mouseX > 14 + 4*al_get_bitmap_width(load_button)) && (mouseY >= 2) && (mouseY <= 2+ al_get_bitmap_height(load_button))) {
        al_draw_bitmap(save_as_button_selected, 14 + 4*al_get_bitmap_width(save_as_button), 2, 0);
        menu_selected = SAVE_AS;
        cout<<"print_primary_menu() - menu_selected = "<<menu_selected<<endl;
    } else {
        al_draw_bitmap(save_as_button, 14 + 4*al_get_bitmap_width(load_button), 2, 0);
    }
    if((mouseX < 14 + 6*al_get_bitmap_width(save_as_button)) && (mouseX > 14 + 5*al_get_bitmap_width(save_as_button)) && (mouseY >= 2) && (mouseY <= 2+ al_get_bitmap_height(save_as_button))) {
        al_draw_bitmap(load_button_selected, 14 + 5*al_get_bitmap_width(load_button), 2, 0);
        menu_selected = LOAD;
        cout<<"print_primary_menu() - menu_selected = "<<menu_selected<<endl;
    } else {
        al_draw_bitmap(load_button, 14 + 5*al_get_bitmap_width(save_as_button), 2, 0);
    }

    //desenha o botão de conexão ao v-rep e bluetooth acompanhando o lado direito da tela
    if((mouseX < al_get_display_width(display) + 10 - al_get_bitmap_width(vrep_button) - roll_bar_width) && (mouseX > al_get_display_width(display) - 73.5 - al_get_bitmap_width(bluetooth_button) - roll_bar_width) && (mouseY >= 2) && (mouseY <= 2+ al_get_bitmap_height(bluetooth_button))) {
        al_draw_bitmap(bluetooth_button_selected, al_get_display_width(display) - 73.5 - al_get_bitmap_width(bluetooth_button) - roll_bar_width, 2, 0);
        menu_selected = PHYSICAL_ROBOT;
        cout<<"print_primary_menu() - menu_selected = "<<menu_selected<<endl;
    } else {
        al_draw_bitmap(bluetooth_button, al_get_display_width(display) - 73.5 - al_get_bitmap_width(bluetooth_button) - roll_bar_width, 2, 0);
    }
    if((mouseX < al_get_display_width(display) - roll_bar_width) && (mouseX > al_get_display_width(display) + 10 - al_get_bitmap_width(vrep_button) - roll_bar_width) && (mouseY >= 2) && (mouseY <= 2+ al_get_bitmap_height(vrep_button))) {
        al_draw_bitmap(vrep_button_selected, al_get_display_width(display) + 10 - al_get_bitmap_width(vrep_button) - roll_bar_width, 2, 0);
        menu_selected = VIRTUAL_ROBOT;
        cout<<"print_primary_menu() - menu_selected = "<<menu_selected<<endl;
    } else {
        al_draw_bitmap(vrep_button, al_get_display_width(display) + 10 - al_get_bitmap_width(vrep_button) - roll_bar_width, 2, 0);
    }

}
void Interface :: print_secondary_menu() {

    int blocks_menu_Y = (al_get_bitmap_height(play_button)+4);
    int sensors_menu_Y = blocks_menu_Y + 6*al_get_bitmap_height(mini_menu[0]);
    int actions_menu_Y = sensors_menu_Y + 3 * al_get_bitmap_height(WALK_FOWARD_ACTION)+10;
    int extra_menu_Y = actions_menu_Y + 3 * al_get_bitmap_height(WALK_FOWARD_ACTION)+10;
    //v3
    int new_menu_Y = extra_menu_Y + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION)+10;

    //desenha o retangulo no qual ficam os botões de opções
    al_draw_filled_rectangle(0, blocks_menu_Y, al_get_bitmap_width(mini_menu[0])+5, blocks_menu_Y + 6*al_get_bitmap_height(mini_menu[0])+5, primary_menu_color);
    al_draw_filled_rectangle(0, sensors_menu_Y, al_get_bitmap_width(mini_menu[0])+5, sensors_menu_Y + 3 * al_get_bitmap_height(WALK_FOWARD_ACTION)+10, sensors_menu_color);
    al_draw_filled_rectangle(0, actions_menu_Y, al_get_bitmap_width(mini_menu[0])+5, actions_menu_Y + 3 * al_get_bitmap_height(WALK_FOWARD_ACTION)+10, functions_menu_color);
    al_draw_filled_rectangle(0, extra_menu_Y, al_get_bitmap_width(mini_menu[0])+5, extra_menu_Y + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION)+10, numbers_menu_color);
    //v3
    al_draw_filled_rectangle(0, new_menu_Y, al_get_bitmap_width(mini_menu[0])+5, new_menu_Y + 7 * al_get_bitmap_height(WALK_FOWARD_ACTION)+10, new_menu_color);

    //desenha todos os botões que criam blocos
    for (int i = 0; i < 6; i++) {
        al_draw_bitmap(mini_menu[i], 0, blocks_menu_Y + i * al_get_bitmap_height(mini_menu[i]), 0);
    }
    //desenha botoes de sensores
    al_draw_bitmap(BLACK_SENSOR_FUNCTION, 0, sensors_menu_Y + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    al_draw_bitmap(COLOR_SENSOR_FUNCTION, 0, sensors_menu_Y + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    al_draw_bitmap(ULTRASONIC_SENSOR_FUNCTION, 0, sensors_menu_Y + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    //desenha botoes de acao
    al_draw_bitmap(WALK_FOWARD_ACTION, 0, actions_menu_Y + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    al_draw_bitmap(TURN_LEFT_ACTION, 0, actions_menu_Y + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    al_draw_bitmap(TURN_RIGHT_ACTION, 0, actions_menu_Y + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    //desenha botoes extras
    al_draw_bitmap(NUMBER[2], 0, extra_menu_Y + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    //al_draw_bitmap(logic_true, 0, extra_menu_Y + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    //al_draw_bitmap(logic_false, 0, extra_menu_Y + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);

    //desenha botoes v3
    al_draw_bitmap(INT_VAR_IMG[0], 0, new_menu_Y + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    al_draw_bitmap(BOOL_VAR_IMG[0], 0, new_menu_Y + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    al_draw_bitmap(LOGIC_OPERATOR_IMG[0], 0, new_menu_Y + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    //al_draw_bitmap(MATH_OPERATOR_IMG[0], 0, new_menu_Y + 3 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    al_draw_bitmap(NEW_MINI_MENU_IMG[0], 0, new_menu_Y + 3 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    al_draw_bitmap(NEW_MINI_MENU_IMG[1], 0, new_menu_Y + 4 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    al_draw_bitmap(NEW_MINI_MENU_IMG[2], 0, new_menu_Y + 5 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    al_draw_bitmap(NEW_MINI_MENU_IMG[3], 0, new_menu_Y + 6 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);

    //variaveis usadas para facilitar a impressao das imagens
    int black_sensor_submenu_X = al_get_bitmap_width(mini_menu[0]) + 5;
    int black_sensor_submenu_Y = sensors_menu_Y + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION)+5;
    int color_sensor_submenu_X = al_get_bitmap_width(mini_menu[0]) + 5;
    int color_sensor_submenu_Y = sensors_menu_Y + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION)+5;
    int ultrasonic_sensor_submenu_X = al_get_bitmap_width(mini_menu[0]) + 5;
    int ultrasonic_sensor_submenu_Y = sensors_menu_Y + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION)+5;

    //imprime submenus
    if(black_sensor_menu_selected == true) {
        sub_menu = true;
        al_draw_bitmap(BLACK_SENSOR_1_FUNCTION, black_sensor_submenu_X + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION), black_sensor_submenu_Y, 0);
        al_draw_bitmap(BLACK_SENSOR_2_FUNCTION, black_sensor_submenu_X + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION), black_sensor_submenu_Y, 0);
        al_draw_bitmap(BLACK_SENSOR_3_FUNCTION, black_sensor_submenu_X + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION), black_sensor_submenu_Y, 0);
        al_draw_bitmap(BLACK_SENSOR_4_FUNCTION, black_sensor_submenu_X + 3 * al_get_bitmap_height(WALK_FOWARD_ACTION), black_sensor_submenu_Y, 0);
        al_draw_bitmap(BLACK_SENSOR_5_FUNCTION, black_sensor_submenu_X + 4 * al_get_bitmap_height(WALK_FOWARD_ACTION), black_sensor_submenu_Y, 0);
        for(int i=0; i<5; i++) {
            // checa se o mouse está sobre o menu de blocos e desenha um retangulo selecionando o botao que o mouse está sobre
            if((mouseX > black_sensor_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION)) && (mouseX < black_sensor_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION))) {
                if((mouseY > black_sensor_submenu_Y) && (mouseY < black_sensor_submenu_Y + 5 + al_get_bitmap_height(mini_menu[0]))) {
                    al_draw_rectangle(black_sensor_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION), black_sensor_submenu_Y, black_sensor_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION), black_sensor_submenu_Y + al_get_bitmap_height(mini_menu[0]), black, 3);
                    menu_selected = 24 + i;
                    cout<<"print_secondary_menu() - menu_selected = "<<menu_selected<<endl;
                }
            }
        }
    }

    if(color_sensor_menu_selected == true) {
        sub_menu = true;
        al_draw_bitmap(COLOR_SENSOR_1_FUNCTION, color_sensor_submenu_X + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION), color_sensor_submenu_Y, 0);
        al_draw_bitmap(COLOR_SENSOR_2_FUNCTION, color_sensor_submenu_X + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION), color_sensor_submenu_Y, 0);
        for(int i=0; i<2; i++) {
            // checa se o mouse está sobre o menu de blocos e desenha um retangulo selecionando o botao que o mouse está sobre
            if((mouseX > color_sensor_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION)) && (mouseX < color_sensor_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION))) {
                if((mouseY > color_sensor_submenu_Y) && (mouseY < color_sensor_submenu_Y+5 + al_get_bitmap_height(mini_menu[0]))) {
                    al_draw_rectangle(color_sensor_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION), color_sensor_submenu_Y, color_sensor_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION), color_sensor_submenu_Y + al_get_bitmap_height(mini_menu[0]), black, 3);
                    menu_selected = 29 + i;
                    cout<<"print_secondary_menu() - menu_selected = "<<menu_selected<<endl;
                }
            }
        }
    }

    if(ultrasonic_sensor_menu_selected == true) {
        sub_menu = true;
        al_draw_bitmap(ULTRASONIC_SENSOR_1_FUNCTION, ultrasonic_sensor_submenu_X + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION), ultrasonic_sensor_submenu_Y, 0);
        al_draw_bitmap(ULTRASONIC_SENSOR_2_FUNCTION, ultrasonic_sensor_submenu_X + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION), ultrasonic_sensor_submenu_Y, 0);
        al_draw_bitmap(ULTRASONIC_SENSOR_3_FUNCTION, ultrasonic_sensor_submenu_X + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION), ultrasonic_sensor_submenu_Y, 0);
        for(int i=0; i<3; i++) {
            // checa se o mouse está sobre o menu de blocos e desenha um retangulo selecionando o botao que o mouse está sobre
            if((mouseX > ultrasonic_sensor_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION)) && (mouseX < ultrasonic_sensor_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION))) {
                if((mouseY > ultrasonic_sensor_submenu_Y) && (mouseY < ultrasonic_sensor_submenu_Y+5 + al_get_bitmap_height(mini_menu[0]))) {
                    al_draw_rectangle(ultrasonic_sensor_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION), ultrasonic_sensor_submenu_Y, ultrasonic_sensor_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION), ultrasonic_sensor_submenu_Y + al_get_bitmap_height(mini_menu[0]), black, 3);
                    menu_selected = 31 + i;
                    cout<<"print_secondary_menu() - menu_selected = "<<menu_selected<<endl;
                }
            }
        }
    }
    //v3 anotacoes: os numeros 4, 9, 19 e 29 sao o offset do retangulo desenhado ao redor dos grupos de botoes
    for(int i=0; i<6; i++) {
        // checa se o mouse está sobre o menu de blocos e desenha um retangulo selecionando o botao que o mouse está sobre
        if((mouseX > 0) && (mouseX < al_get_bitmap_width(WALK_FOWARD_ACTION))) {
            if((mouseY > al_get_bitmap_height(play_button) + 4 + i*al_get_bitmap_height(mini_menu[0])) && (mouseY < al_get_bitmap_height(play_button) + 4 + (i+1)*al_get_bitmap_height(mini_menu[0]))) {
                al_draw_rectangle(0, al_get_bitmap_height(play_button) + 4 + i*al_get_bitmap_height(mini_menu[0]), al_get_bitmap_width(mini_menu[0]), al_get_bitmap_height(play_button)+4 + (i+1)*al_get_bitmap_height(mini_menu[0]), black, 3);
                menu_selected = 9 + i;
                cout<<"print_secondary_menu() - menu_selected = "<<menu_selected<<endl;
            }
        }
    }

    for(int i=6; i<9; i++) {
        // checa se o mouse está sobre o menu de sensores e desenha um retangulo selecionando o botao que o mouse está sobre
        if((mouseX > 0) && (mouseX < al_get_bitmap_width(WALK_FOWARD_ACTION))) {
            if((mouseY > al_get_bitmap_height(play_button) + 9 + i*al_get_bitmap_height(mini_menu[0])) && (mouseY < al_get_bitmap_height(play_button) + 9 + (i+1)*al_get_bitmap_height(mini_menu[0]))) {
                al_draw_rectangle(0, al_get_bitmap_height(play_button) + 9 + i*al_get_bitmap_height(mini_menu[0]), al_get_bitmap_width(mini_menu[0]), al_get_bitmap_height(play_button)+ 9 + (i+1)*al_get_bitmap_height(mini_menu[0]), black, 3);
                menu_selected = 9 + i;
                cout<<"print_secondary_menu() - menu_selected = "<<menu_selected<<endl;
            }
        }
    }

    for(int i=9; i<12; i++) {
        // checa se o mouse está sobre o menu e desenha um retangulo selecionando o botao que o mouse está sobre
        if((mouseX > 0) && (mouseX < al_get_bitmap_width(WALK_FOWARD_ACTION))) {
            if((mouseY > al_get_bitmap_height(play_button) + 19 + i*al_get_bitmap_height(mini_menu[0])) && (mouseY < al_get_bitmap_height(play_button) + 19 + (i+1)*al_get_bitmap_height(mini_menu[0]))) {
                al_draw_rectangle(0, al_get_bitmap_height(play_button) + 19 + i*al_get_bitmap_height(mini_menu[0]), al_get_bitmap_width(mini_menu[0]), al_get_bitmap_height(play_button)+ 19 + (i+1)*al_get_bitmap_height(mini_menu[0]), black, 3);
                menu_selected = 9 + i;
                cout<<"print_secondary_menu() - menu_selected = "<<menu_selected<<endl;
            }
        }
    }

    for(int i=12; i<13; i++) {
        // checa se o mouse está sobre o menu e desenha um retangulo selecionando o botao que o mouse está sobre
        if((mouseX > 0) && (mouseX < al_get_bitmap_width(WALK_FOWARD_ACTION))) {
            if((mouseY > al_get_bitmap_height(play_button) + 29 + i*al_get_bitmap_height(mini_menu[0])) && (mouseY < al_get_bitmap_height(play_button) + 29 + (i+1)*al_get_bitmap_height(mini_menu[0]))) {
                al_draw_rectangle(0, al_get_bitmap_height(play_button) + 29 + i*al_get_bitmap_height(mini_menu[0]), al_get_bitmap_width(mini_menu[0]), al_get_bitmap_height(play_button)+ 29 + (i+1)*al_get_bitmap_height(mini_menu[0]), black, 3);
                menu_selected = 9 + i;
                cout<<"print_secondary_menu() - menu_selected = "<<menu_selected<<endl;
            }
        }
    }
    //v3
    for(int i=13; i<20; i++) {
        // checa se o mouse está sobre o NOVO menu e desenha um retangulo selecionando o botao que o mouse está sobre
        if((mouseX > 0) && (mouseX < al_get_bitmap_width(WALK_FOWARD_ACTION))) {
            if((mouseY > al_get_bitmap_height(play_button) + 39 + i*al_get_bitmap_height(mini_menu[0])) && (mouseY < al_get_bitmap_height(play_button) + 39 + (i+1)*al_get_bitmap_height(mini_menu[0]))) {
                al_draw_rectangle(0, al_get_bitmap_height(play_button) + 39 + i*al_get_bitmap_height(mini_menu[0]), al_get_bitmap_width(mini_menu[0]), al_get_bitmap_height(play_button)+ 39 + (i+1)*al_get_bitmap_height(mini_menu[0]), black, 3);
                menu_selected = 9 + i + 53; //53 e o offset das definicoes do v3
                cout<<"print_secondary_menu() - menu_selected = "<<menu_selected<<endl;
            }
        }
    }

    //variaveis usadas para facilitar a impressao das imagens
    int number_submenu_X = al_get_bitmap_width(mini_menu[0]) + 5;
    int number_submenu_Y = extra_menu_Y + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION)+5;

    //desenha submenu de numberos
    if(number_menu_selected == true) {
        sub_menu = true;
        al_draw_bitmap(NUMBER[0], number_submenu_X + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION), number_submenu_Y, 0);
        al_draw_bitmap(NUMBER[1], number_submenu_X + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION), number_submenu_Y, 0);
        al_draw_bitmap(NUMBER[2], number_submenu_X + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION), number_submenu_Y, 0);
        al_draw_bitmap(NUMBER[3], number_submenu_X + 3 * al_get_bitmap_height(WALK_FOWARD_ACTION), number_submenu_Y, 0);
        al_draw_bitmap(NUMBER[4], number_submenu_X + 4 * al_get_bitmap_height(WALK_FOWARD_ACTION), number_submenu_Y, 0);
        al_draw_bitmap(NUMBER[5], number_submenu_X + 5 * al_get_bitmap_height(WALK_FOWARD_ACTION), number_submenu_Y, 0);
        al_draw_bitmap(NUMBER[6], number_submenu_X + 6 * al_get_bitmap_height(WALK_FOWARD_ACTION), number_submenu_Y, 0);
        al_draw_bitmap(NUMBER[7], number_submenu_X + 7 * al_get_bitmap_height(WALK_FOWARD_ACTION), number_submenu_Y, 0);
        al_draw_bitmap(NUMBER[8], number_submenu_X + 8 * al_get_bitmap_height(WALK_FOWARD_ACTION), number_submenu_Y, 0);
        al_draw_bitmap(NUMBER[9], number_submenu_X + 9 * al_get_bitmap_height(WALK_FOWARD_ACTION), number_submenu_Y, 0);
        for(int i=0; i<10; i++) {
            // checa se o mouse está sobre o menu de blocos e desenha um retangulo selecionando o botao que o mouse está sobre
            if((mouseX > number_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION)) && (mouseX < number_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION))) {
                if((mouseY > number_submenu_Y) && (mouseY < number_submenu_Y+ 5 + al_get_bitmap_height(mini_menu[0]))) {
                    al_draw_rectangle(number_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION), number_submenu_Y, number_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION), number_submenu_Y + al_get_bitmap_height(mini_menu[0]), black, 3);
                    menu_selected = 34 + i;
                    cout<<"print_secondary_menu() - menu_selected = "<<menu_selected<<endl;
                }
            }
        }
    }

    //desenha submenus de variaveis e operacoes v3
    int int_var_submenu_X = al_get_bitmap_width(mini_menu[0]) + 5;
    int int_var_submenu_Y = new_menu_Y + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION)+5;
    int bool_var_submenu_X = al_get_bitmap_width(mini_menu[0]) + 5;
    int bool_var_submenu_Y = new_menu_Y + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION)+5;
    int logic_submenu_X = al_get_bitmap_width(mini_menu[0]) + 5;
    int logic_submenu_Y = new_menu_Y + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION)+5;
    //int math_submenu_X = al_get_bitmap_width(mini_menu[0]) + 5;
    //int math_submenu_Y = new_menu_Y + 7 * al_get_bitmap_height(WALK_FOWARD_ACTION)+5;

    if(int_menu_selected == true) {
        sub_menu = true;
        al_draw_bitmap(INT_VAR_IMG[0], int_var_submenu_X + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION), int_var_submenu_Y, 0);
        al_draw_bitmap(INT_VAR_IMG[1], int_var_submenu_X + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION), int_var_submenu_Y, 0);
        al_draw_bitmap(INT_VAR_IMG[2], int_var_submenu_X + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION), int_var_submenu_Y, 0);
        al_draw_bitmap(INT_VAR_IMG[3], int_var_submenu_X + 3 * al_get_bitmap_height(WALK_FOWARD_ACTION), int_var_submenu_Y, 0);
        al_draw_bitmap(INT_VAR_IMG[4], int_var_submenu_X + 4 * al_get_bitmap_height(WALK_FOWARD_ACTION), int_var_submenu_Y, 0);
        al_draw_bitmap(INT_VAR_IMG[5], int_var_submenu_X + 5 * al_get_bitmap_height(WALK_FOWARD_ACTION), int_var_submenu_Y, 0);
        al_draw_bitmap(INT_VAR_IMG[6], int_var_submenu_X + 6 * al_get_bitmap_height(WALK_FOWARD_ACTION), int_var_submenu_Y, 0);
        al_draw_bitmap(INT_VAR_IMG[7], int_var_submenu_X + 7 * al_get_bitmap_height(WALK_FOWARD_ACTION), int_var_submenu_Y, 0);
        al_draw_bitmap(INT_VAR_IMG[8], int_var_submenu_X + 8 * al_get_bitmap_height(WALK_FOWARD_ACTION), int_var_submenu_Y, 0);
        al_draw_bitmap(INT_VAR_IMG[9], int_var_submenu_X + 9 * al_get_bitmap_height(WALK_FOWARD_ACTION), int_var_submenu_Y, 0);
        for(int i=0; i<10; i++) {
            // checa se o mouse está sobre o menu de INT e desenha um retangulo selecionando o botao que o mouse está sobre
            if((mouseX > int_var_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION)) && (mouseX < int_var_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION))) {
                if((mouseY > int_var_submenu_Y) && (mouseY < int_var_submenu_Y+ 5 + al_get_bitmap_height(mini_menu[0]))) {
                    al_draw_rectangle(int_var_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION), int_var_submenu_Y, int_var_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION), int_var_submenu_Y + al_get_bitmap_height(mini_menu[0]), black, 3);
                    menu_selected = 44 + i;
                    cout<<"print_secondary_menu() - menu_selected = "<<menu_selected<<endl;
                }
            }
        }
    }

    if(bool_menu_selected == true) {
        sub_menu = true;
        al_draw_bitmap(BOOL_VAR_IMG[0], bool_var_submenu_X + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION), bool_var_submenu_Y, 0);
        al_draw_bitmap(BOOL_VAR_IMG[1], bool_var_submenu_X + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION), bool_var_submenu_Y, 0);
        al_draw_bitmap(BOOL_VAR_IMG[2], bool_var_submenu_X + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION), bool_var_submenu_Y, 0);
        al_draw_bitmap(BOOL_VAR_IMG[3], bool_var_submenu_X + 3 * al_get_bitmap_height(WALK_FOWARD_ACTION), bool_var_submenu_Y, 0);
        al_draw_bitmap(BOOL_VAR_IMG[4], bool_var_submenu_X + 4 * al_get_bitmap_height(WALK_FOWARD_ACTION), bool_var_submenu_Y, 0);
        al_draw_bitmap(BOOL_VAR_IMG[5], bool_var_submenu_X + 5 * al_get_bitmap_height(WALK_FOWARD_ACTION), bool_var_submenu_Y, 0);
        al_draw_bitmap(BOOL_VAR_IMG[6], bool_var_submenu_X + 6 * al_get_bitmap_height(WALK_FOWARD_ACTION), bool_var_submenu_Y, 0);
        al_draw_bitmap(BOOL_VAR_IMG[7], bool_var_submenu_X + 7 * al_get_bitmap_height(WALK_FOWARD_ACTION), bool_var_submenu_Y, 0);
        al_draw_bitmap(BOOL_VAR_IMG[8], bool_var_submenu_X + 8 * al_get_bitmap_height(WALK_FOWARD_ACTION), bool_var_submenu_Y, 0);
        al_draw_bitmap(BOOL_VAR_IMG[9], bool_var_submenu_X + 9 * al_get_bitmap_height(WALK_FOWARD_ACTION), bool_var_submenu_Y, 0);
        for(int i=0; i<10; i++) {
            // checa se o mouse está sobre o menu de INT e desenha um retangulo selecionando o botao que o mouse está sobre
            if((mouseX > bool_var_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION)) && (mouseX < bool_var_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION))) {
                if((mouseY > bool_var_submenu_Y) && (mouseY < bool_var_submenu_Y+ 5 + al_get_bitmap_height(mini_menu[0]))) {
                    al_draw_rectangle(bool_var_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION), bool_var_submenu_Y, bool_var_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION), bool_var_submenu_Y + al_get_bitmap_height(mini_menu[0]), black, 3);
                    menu_selected = 54 + i;
                    cout<<"print_secondary_menu() - menu_selected = "<<menu_selected<<endl;
                }
            }
        }
    }
    //v3: operadores matematicos foram imbutidos no mesmo menu dos operadores logicos
    if(logic_menu_selected == true) {
        sub_menu = true;
        al_draw_bitmap(LOGIC_OPERATOR_IMG[0], logic_submenu_X + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION), logic_submenu_Y, 0);
        al_draw_bitmap(LOGIC_OPERATOR_IMG[3], logic_submenu_X + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION), logic_submenu_Y, 0);
        al_draw_bitmap(LOGIC_OPERATOR_IMG[4], logic_submenu_X + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION), logic_submenu_Y, 0);
        al_draw_bitmap(LOGIC_OPERATOR_IMG[1], logic_submenu_X + 3 * al_get_bitmap_height(WALK_FOWARD_ACTION), logic_submenu_Y, 0);
        al_draw_bitmap(LOGIC_OPERATOR_IMG[2], logic_submenu_X + 4 * al_get_bitmap_height(WALK_FOWARD_ACTION), logic_submenu_Y, 0);
        al_draw_bitmap(LOGIC_OPERATOR_IMG[5], logic_submenu_X + 5 * al_get_bitmap_height(WALK_FOWARD_ACTION), logic_submenu_Y, 0);
        al_draw_bitmap(MATH_OPERATOR_IMG[0], logic_submenu_X + 6 * al_get_bitmap_height(WALK_FOWARD_ACTION), logic_submenu_Y, 0);
        al_draw_bitmap(MATH_OPERATOR_IMG[1], logic_submenu_X + 7 * al_get_bitmap_height(WALK_FOWARD_ACTION), logic_submenu_Y, 0);
        al_draw_bitmap(MATH_OPERATOR_IMG[2], logic_submenu_X + 8 * al_get_bitmap_height(WALK_FOWARD_ACTION), logic_submenu_Y, 0);
        al_draw_bitmap(MATH_OPERATOR_IMG[3], logic_submenu_X + 9 * al_get_bitmap_height(WALK_FOWARD_ACTION), logic_submenu_Y, 0);
        al_draw_bitmap(MATH_OPERATOR_IMG[4], logic_submenu_X + 10 * al_get_bitmap_height(WALK_FOWARD_ACTION), logic_submenu_Y, 0);
        for(int i=0; i<11; i++) {
            // checa se o mouse está sobre o menu de logica ou matematica e desenha um retangulo selecionando o botao que o mouse está sobre
            if((mouseX > logic_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION)) && (mouseX < logic_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION))) {
                if((mouseY > logic_submenu_Y) && (mouseY < logic_submenu_Y+ 5 + al_get_bitmap_height(mini_menu[0]))) {
                    al_draw_rectangle(logic_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION), logic_submenu_Y, logic_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION), logic_submenu_Y + al_get_bitmap_height(mini_menu[0]), black, 3);
                    menu_selected = 64 + i;
                    cout<<"print_secondary_menu() - menu_selected = "<<menu_selected<<endl;
                }
            }
        }
    }

    //if(math_menu_selected == true) {
    //    sub_menu = true;
    //    al_draw_bitmap(MATH_OPERATOR_IMG[0], math_submenu_X + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION), math_submenu_Y, 0);
    //    al_draw_bitmap(MATH_OPERATOR_IMG[1], math_submenu_X + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION), math_submenu_Y, 0);
    //    al_draw_bitmap(MATH_OPERATOR_IMG[2], math_submenu_X + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION), math_submenu_Y, 0);
    //    al_draw_bitmap(MATH_OPERATOR_IMG[3], math_submenu_X + 3 * al_get_bitmap_height(WALK_FOWARD_ACTION), math_submenu_Y, 0);
    //    al_draw_bitmap(MATH_OPERATOR_IMG[4], math_submenu_X + 4 * al_get_bitmap_height(WALK_FOWARD_ACTION), math_submenu_Y, 0);
    //    for(int i=0; i<5; i++) {
    //        // checa se o mouse está sobre o menu de matematica e desenha um retangulo selecionando o botao que o mouse está sobre
    //        if((mouseX > math_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION)) && (mouseX < math_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION))) {
    //            if((mouseY > math_submenu_Y) && (mouseY < math_submenu_Y+ 5 + al_get_bitmap_height(mini_menu[0]))) {
    //                al_draw_rectangle(math_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION), math_submenu_Y, math_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION), math_submenu_Y + al_get_bitmap_height(mini_menu[0]), black, 3);
    //                menu_selected = 70 + i;
    //            }
    //        }
    //    }
    //}

    al_draw_bitmap(trash, al_get_display_width(display)-al_get_bitmap_width(trash), al_get_display_height(display)-al_get_bitmap_height(trash), 0);

}
void Interface :: print_function_block(Block *b) {
    if((mouseX > b->getX()) && (mouseX < (b->getX() + b->getWidth())) && (mouseY > b->getY()) && (mouseY < (b->getY() + b->getHeight()))) {
        if(black_sensor_menu_selected || color_sensor_menu_selected || ultrasonic_sensor_menu_selected || number_menu_selected || int_menu_selected || bool_menu_selected || logic_menu_selected) {
            al_draw_bitmap(FUNCTION_BLOCK[0], b->getX(), b->getY(), 0);
        } else {
            al_draw_bitmap(FUNCTION_BLOCK[1], b->getX(), b->getY(), 0);
        }

    } else if(b->getSelected() == true) {
        al_draw_bitmap(FUNCTION_BLOCK[2], b->getX(), b->getY(), 0);
    } else if(b->getExecuting() == true) {
        al_draw_bitmap(FUNCTION_BLOCK[3], b->getX(), b->getY(), 0);
    } else {
        al_draw_bitmap(FUNCTION_BLOCK[0], b->getX(), b->getY(), 0);
    }

    //testa qual o tipo de sensor da comparação para imprimir a imagem certa
    switch(b->getFunction()) {
        case MOVE_FORWARD:
            al_draw_bitmap(MICRO_WALK_FOWARD, b->getX()+32, b->getY()+7, 0);
            break;
        case TURN_LEFT:
            al_draw_bitmap(MICRO_TURN_LEFT, b->getX()+32, b->getY()+7, 0);
            break;
        case TURN_RIGHT:
            al_draw_bitmap(MICRO_TURN_RIGHT, b->getX()+32, b->getY()+7, 0);
            break;
    }

    //desenha as bolinhas onde são ligadas as linhas
    if((mouseX > b->getX()+40) && (mouseX < (b->getX() + 53)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
        al_draw_bitmap(POINT[1], b->getX()+40, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+40, b->getY()+35, 0);
    } else if((mouseX > b->getX()+40) && (mouseX < (b->getX() + 53)) && (mouseY > b->getY()+35) && (mouseY < (b->getY()+48))) {
        al_draw_bitmap(POINT[0], b->getX()+40, b->getY()-5, 0);
        al_draw_bitmap(POINT[1], b->getX()+40, b->getY()+35, 0);
    } else {
        al_draw_bitmap(POINT[0], b->getX()+40, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+40, b->getY()+35, 0);
    }
}
void Interface :: print_end_block(Block *b) {
    if((mouseX > b->getX()) && (mouseX < (b->getX() + b->getWidth())) && (mouseY > b->getY()) && (mouseY < (b->getY() + b->getHeight()))) {
        if(black_sensor_menu_selected || color_sensor_menu_selected || ultrasonic_sensor_menu_selected || number_menu_selected || int_menu_selected || bool_menu_selected || logic_menu_selected) {
            al_draw_bitmap(END_BLOCK_IMG[0], b->getX(), b->getY(), 0);
        } else {
            al_draw_bitmap(END_BLOCK_IMG[1], b->getX(), b->getY(), 0);
        }
    } else if(b->getSelected() == true) {
        al_draw_bitmap(END_BLOCK_IMG[2], b->getX(), b->getY(), 0);
    } else if(b->getExecuting() == true) {
        al_draw_bitmap(END_BLOCK_IMG[3], b->getX(), b->getY(), 0);
    } else {
        al_draw_bitmap(END_BLOCK_IMG[0], b->getX(), b->getY(), 0);
    }
    //desenha as bolinhas onde são ligadas as linhas
    if((mouseX > b->getX()+37) && (mouseX < (b->getX() + 50)) && (mouseY > b->getY()-5) && (mouseY < (b->getY() + 8))) {
        al_draw_bitmap(POINT[1], b->getX()+37, b->getY()-5, 0);
    } else {
        al_draw_bitmap(POINT[0], b->getX()+37, b->getY()-5, 0);
    }
}
void Interface :: print_start_block(Block *b) {
    if((mouseX > b->getX()) && (mouseX < (b->getX() + b->getWidth())) && (mouseY > b->getY()) && (mouseY < (b->getY() + b->getHeight()))) {
        if(black_sensor_menu_selected || color_sensor_menu_selected || ultrasonic_sensor_menu_selected || number_menu_selected || int_menu_selected || bool_menu_selected || logic_menu_selected) {
            al_draw_bitmap(START_BLOCK_IMG[0], b->getX(), b->getY(), 0);
        } else {
            al_draw_bitmap(START_BLOCK_IMG[1], b->getX(), b->getY(), 0);
        }
    } else if(b->getSelected() == true) {
        al_draw_bitmap(START_BLOCK_IMG[2], b->getX(), b->getY(), 0);
    } else if(b->getExecuting() == true) {
        al_draw_bitmap(START_BLOCK_IMG[3], b->getX(), b->getY(), 0);
    } else {
        al_draw_bitmap(START_BLOCK_IMG[0], b->getX(), b->getY(), 0);
    }
    //desenha as bolinhas onde são ligadas as linhas
    if((mouseX > b->getX()+37) && (mouseX < (b->getX() + 50)) && (mouseY > b->getY()+27) && (mouseY < (b->getY() + 40))) {
        al_draw_bitmap(POINT[1], b->getX()+37, b->getY()+27, 0);
    } else {
        al_draw_bitmap(POINT[0], b->getX()+37, b->getY()+27, 0);
    }
}
void Interface :: print_loop_block(Block *b) {
    if((mouseX > b->getX()) && (mouseX < (b->getX() + b->getWidth())) && (mouseY > b->getY()) && (mouseY < (b->getY() + b->getHeight()))) {
        if(black_sensor_menu_selected || color_sensor_menu_selected || ultrasonic_sensor_menu_selected || number_menu_selected || int_menu_selected || bool_menu_selected || logic_menu_selected) {
            al_draw_bitmap(LOOP_BLOCK_IMG[0], b->getX(), b->getY(), 0);
        } else {
            al_draw_bitmap(LOOP_BLOCK_IMG[1], b->getX(), b->getY(), 0);
        }
    } else if(b->getSelected() == true) {
        al_draw_bitmap(LOOP_BLOCK_IMG[2], b->getX(), b->getY(), 0);
    } else if(b->getExecuting() == true) {
        al_draw_bitmap(LOOP_BLOCK_IMG[3], b->getX(), b->getY(), 0);
    } else {
        al_draw_bitmap(LOOP_BLOCK_IMG[0], b->getX(), b->getY(), 0);
    }
    //desenha unidade
    //al_draw_bitmap(MICRO_NUMBER[b->getUnit()], b->getX()+43, b->getY()+42, 0);
    //desenha dezena
    //al_draw_bitmap(MICRO_NUMBER[b->getTen()], b->getX()+28, b->getY()+42, 0);

    al_draw_bitmap(MICRO_NUMBER[b->getUnit()], b->getX()+35, b->getY()+42, 0);

    //desenha as bolinhas onde são ligadas as linhas
    if((mouseX > b->getX()+35) && (mouseX < (b->getX() + 48)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
        al_draw_bitmap(POINT[1], b->getX()+35, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+35, b->getY()+81, 0);
        al_draw_bitmap(POINT[0], b->getX()+72, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+72, b->getY()+81, 0);
    } else if((mouseX > b->getX()+35) && (mouseX < (b->getX() + 48)) && (mouseY > b->getY()+81) && (mouseY < (b->getY()+94))) {
        al_draw_bitmap(POINT[0], b->getX()+35, b->getY()-5, 0);
        al_draw_bitmap(POINT[1], b->getX()+35, b->getY()+81, 0);
        al_draw_bitmap(POINT[0], b->getX()+72, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+72, b->getY()+81, 0);
    } else if((mouseX > b->getX()+72) && (mouseX < (b->getX() + 85)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
        al_draw_bitmap(POINT[0], b->getX()+35, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+35, b->getY()+81, 0);
        al_draw_bitmap(POINT[1], b->getX()+72, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+72, b->getY()+81, 0);
    } else if((mouseX > b->getX()+72) && (mouseX < (b->getX() + 85)) && (mouseY > b->getY()+81) && (mouseY < (b->getY()+94))) {
        al_draw_bitmap(POINT[0], b->getX()+35, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+35, b->getY()+81, 0);
        al_draw_bitmap(POINT[0], b->getX()+72, b->getY()-5, 0);
        al_draw_bitmap(POINT[1], b->getX()+72, b->getY()+81, 0);
    } else {
        al_draw_bitmap(POINT[0], b->getX()+35, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+35, b->getY()+81, 0);
        al_draw_bitmap(POINT[0], b->getX()+72, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+72, b->getY()+81, 0);
    }
}
void Interface :: print_decision_block(Block *b) {

    //desenha o bloco
    if((mouseX > b->getX()) && (mouseX < (b->getX() + b->getWidth())) && (mouseY > b->getY()) && (mouseY < (b->getY() + b->getHeight()))) {
        if(black_sensor_menu_selected || color_sensor_menu_selected || ultrasonic_sensor_menu_selected || number_menu_selected || int_menu_selected || bool_menu_selected || logic_menu_selected) {
            al_draw_bitmap(DECISION_BLOCK[0], b->getX(), b->getY(), 0);
        } else {
            al_draw_bitmap(DECISION_BLOCK[1], b->getX(), b->getY(), 0);
        }
    } else if(b->getSelected() == true) {
        al_draw_bitmap(DECISION_BLOCK[2], b->getX(), b->getY(), 0);
    } else if(b->getExecuting() == true) {
        al_draw_bitmap(DECISION_BLOCK[3], b->getX(), b->getY(), 0);
    } else {
        al_draw_bitmap(DECISION_BLOCK[0], b->getX(), b->getY(), 0);
    }
    //testa qual o tipo de sensor da comparação para imprimir a imagem certa
    switch(b->getTypeOfSensor()) {
        case BLACK_SENSOR_1:
            al_draw_bitmap(MICRO_BLACK_SENSOR1, b->getX()+47, b->getY()+19, 0);
            break;

        case BLACK_SENSOR_2:
            al_draw_bitmap(MICRO_BLACK_SENSOR2, b->getX()+47, b->getY()+19, 0);
            break;
        case BLACK_SENSOR_3:
            al_draw_bitmap(MICRO_BLACK_SENSOR3, b->getX()+47, b->getY()+19, 0);
            break;
        case BLACK_SENSOR_4:
            al_draw_bitmap(MICRO_BLACK_SENSOR4, b->getX()+47, b->getY()+19, 0);
            break;
        case BLACK_SENSOR_5:
            al_draw_bitmap(MICRO_BLACK_SENSOR5, b->getX()+47, b->getY()+19, 0);
            break;
        case COLOR_SENSOR_1:
            al_draw_bitmap(MICRO_COLOR_SENSOR1, b->getX()+47, b->getY()+19, 0);
            break;
        case COLOR_SENSOR_2:
            al_draw_bitmap(MICRO_COLOR_SENSOR2, b->getX()+47, b->getY()+19, 0);
            break;
        case ULTRASONIC_SENSOR_1:
            al_draw_bitmap(MICRO_ULTRASONIC_SENSOR1, b->getX()+47, b->getY()+19, 0);
            break;
        case ULTRASONIC_SENSOR_2:
            al_draw_bitmap(MICRO_ULTRASONIC_SENSOR2, b->getX()+47, b->getY()+19, 0);
            break;
        case ULTRASONIC_SENSOR_3:
            al_draw_bitmap(MICRO_ULTRASONIC_SENSOR3, b->getX()+47, b->getY()+19, 0);
            break;
    }

    //desenha as bolinhas onde são ligadas as linhas
    if((mouseX > b->getX()+53) && (mouseX < (b->getX() + 66)) && (mouseY > b->getY()-6) && (mouseY < (b->getY()+7))) {
        al_draw_bitmap(POINT[1], b->getX()+53, b->getY()-6, 0);
        al_draw_bitmap(POINT[0], b->getX()+53, b->getY()+62, 0);
        al_draw_bitmap(POINT[0], b->getX()+112, b->getY()+28, 0);
    } else if((mouseX > b->getX()+53) && (mouseX < (b->getX() + 66)) && (mouseY > b->getY()+62) && (mouseY < (b->getY()+75))) {
        al_draw_bitmap(POINT[0], b->getX()+53, b->getY()-6, 0);
        al_draw_bitmap(POINT[1], b->getX()+53, b->getY()+62, 0);
        al_draw_bitmap(POINT[0], b->getX()+112, b->getY()+28, 0);
    } else if((mouseX > b->getX()+112) && (mouseX < (b->getX() + 125)) && (mouseY > b->getY()+28) && (mouseY < (b->getY()+41))) {
        al_draw_bitmap(POINT[0], b->getX()+53, b->getY()-6, 0);
        al_draw_bitmap(POINT[0], b->getX()+53, b->getY()+62, 0);
        al_draw_bitmap(POINT[1], b->getX()+112, b->getY()+28, 0);
    } else {
        al_draw_bitmap(POINT[0], b->getX()+53, b->getY()-6, 0);
        al_draw_bitmap(POINT[0], b->getX()+53, b->getY()+62, 0);
        al_draw_bitmap(POINT[0], b->getX()+112, b->getY()+28, 0);
    }
}
void Interface :: print_merge_block(Block *b) {
    if((mouseX > b->getX()) && (mouseX < (b->getX() + b->getWidth())) && (mouseY > b->getY()) && (mouseY < (b->getY() + b->getHeight()))) {
        if(black_sensor_menu_selected || color_sensor_menu_selected || ultrasonic_sensor_menu_selected || number_menu_selected || int_menu_selected || bool_menu_selected || logic_menu_selected) {
            al_draw_bitmap(MERGE_BLOCK_IMG[0], b->getX(), b->getY(), 0);
        } else {
            al_draw_bitmap(MERGE_BLOCK_IMG[1], b->getX(), b->getY(), 0);
        }
    } else if(b->getSelected() == true) {
        al_draw_bitmap(MERGE_BLOCK_IMG[2], b->getX(), b->getY(), 0);
    } else if(b->getExecuting() == true) {
        al_draw_bitmap(MERGE_BLOCK_IMG[3], b->getX(), b->getY(), 0);
    } else {
        al_draw_bitmap(MERGE_BLOCK_IMG[0], b->getX(), b->getY(), 0);
    }
    //desenha as bolinhas onde são ligadas as linhas
    if((mouseX > b->getX()-5) && (mouseX < (b->getX() + 8)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
        al_draw_bitmap(POINT[1], b->getX()-5, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+22, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+8, b->getY()+17, 0);
    } else if((mouseX > b->getX()+22) && (mouseX < (b->getX() + 35)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
        al_draw_bitmap(POINT[0], b->getX()-5, b->getY()-5, 0);
        al_draw_bitmap(POINT[1], b->getX()+22, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+8, b->getY()+17, 0);
    } else if((mouseX > b->getX()+8) && (mouseX < (b->getX() + 21)) && (mouseY > b->getY()+17) && (mouseY < (b->getY()+30))) {
        al_draw_bitmap(POINT[0], b->getX()-5, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+22, b->getY()-5, 0);
        al_draw_bitmap(POINT[1], b->getX()+8, b->getY()+17, 0);
    } else {
        al_draw_bitmap(POINT[0], b->getX()-5, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+22, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+8, b->getY()+17, 0);
    }
}
//v3
void Interface :: print_attribution_block(Block *b) {
    if((mouseX > b->getX()) && (mouseX < (b->getX() + b->getWidth())) && (mouseY > b->getY()) && (mouseY < (b->getY() + b->getHeight()))) {
        if(black_sensor_menu_selected || color_sensor_menu_selected || ultrasonic_sensor_menu_selected || number_menu_selected || int_menu_selected || bool_menu_selected || logic_menu_selected) {
            al_draw_bitmap(ATTRIBUTION_BLOCK_IMG[0], b->getX(), b->getY(), 0);
        } else {
            al_draw_bitmap(ATTRIBUTION_BLOCK_IMG[1], b->getX(), b->getY(), 0);
        }
    } else if(b->getSelected() == true) {
        al_draw_bitmap(ATTRIBUTION_BLOCK_IMG[2], b->getX(), b->getY(), 0);
    } else if(b->getExecuting() == true) {
        al_draw_bitmap(ATTRIBUTION_BLOCK_IMG[3], b->getX(), b->getY(), 0);
    } else {
        al_draw_bitmap(ATTRIBUTION_BLOCK_IMG[0], b->getX(), b->getY(), 0);
    }
    int i = 0;
    while (i < 10){
        if(b->returnIntVar_1() == int_var_list[i]){
            al_draw_bitmap(MICRO_INT_VAR_IMG[i], b->getX()+23, b->getY()+8, 0);
            break;
        }
        if(b->returnBoolVar_1() == bool_var_list[i]){
            al_draw_bitmap(MICRO_BOOL_VAR_IMG[i], b->getX()+23, b->getY()+8, 0);
            break;
        }
        i = i + 1;
    }
    i = 0;
    while (i < 10){
        if(b->returnIntVar_2() == int_var_list[i]){
            al_draw_bitmap(MICRO_INT_VAR_IMG[i], b->getX()+113, b->getY()+8, 0);
            break;
        }
        if(b->returnBoolVar_2() == bool_var_list[i]){
            al_draw_bitmap(MICRO_BOOL_VAR_IMG[i], b->getX()+113, b->getY()+8, 0);
            break;
        }
        if(b->returnStaticValue_2() != 999){
            al_draw_bitmap(SLOT_NUMBER[b->returnStaticValue_2()], b->getX()+113, b->getY()+8, 0);
            break;
        }
        i = i + 1;
    }
    i = 0;
    if((mouseX > b->getX()+74) && (mouseX < (b->getX() + 86)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+6))) {
        al_draw_bitmap(POINT[1], b->getX()+75, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+75, b->getY()+37, 0);
    } else if((mouseX > b->getX()+74) && (mouseX < (b->getX() + 86)) && (mouseY > b->getY()+36) && (mouseY < (b->getY()+47))) {
        al_draw_bitmap(POINT[0], b->getX()+75, b->getY()-5, 0);
        al_draw_bitmap(POINT[1], b->getX()+75, b->getY()+37, 0);
    } else {
        al_draw_bitmap(POINT[0], b->getX()+75, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+75, b->getY()+37, 0);
    }
}
void Interface :: print_math_block(Block *b) {
    if((mouseX > b->getX()) && (mouseX < (b->getX() + b->getWidth())) && (mouseY > b->getY()) && (mouseY < (b->getY() + b->getHeight()))) {
        if(black_sensor_menu_selected || color_sensor_menu_selected || ultrasonic_sensor_menu_selected || number_menu_selected || int_menu_selected || bool_menu_selected || logic_menu_selected) {
            al_draw_bitmap(MATH_BLOCK_IMG[0], b->getX(), b->getY(), 0);
        } else {
            al_draw_bitmap(MATH_BLOCK_IMG[1], b->getX(), b->getY(), 0);
        }
    } else if(b->getSelected() == true) {
        al_draw_bitmap(MATH_BLOCK_IMG[2], b->getX(), b->getY(), 0);
    } else if(b->getExecuting() == true) {
        al_draw_bitmap(MATH_BLOCK_IMG[3], b->getX(), b->getY(), 0);
    } else {
        al_draw_bitmap(MATH_BLOCK_IMG[0], b->getX(), b->getY(), 0);
    }
    int i = 0;
    while (i < 10){
        if(b->returnIntVar_1() == int_var_list[i]){
            al_draw_bitmap(MICRO_INT_VAR_IMG[i], b->getX()+12, b->getY()+8, 0);
            break;
        }
        if(b->returnBoolVar_1() == bool_var_list[i]){
            al_draw_bitmap(MICRO_BOOL_VAR_IMG[i], b->getX()+12, b->getY()+8, 0);
            break;
        }
        i = i + 1;
    }
    i = 0;
    while (i < 10){
        if(b->returnIntVar_2() == int_var_list[i]){
            al_draw_bitmap(MICRO_INT_VAR_IMG[i], b->getX()+102, b->getY()+8, 0);
            break;
        }
        if(b->returnBoolVar_2() == bool_var_list[i]){
            al_draw_bitmap(MICRO_BOOL_VAR_IMG[i], b->getX()+102, b->getY()+8, 0);
            break;
        }
        if(b->returnStaticValue_2() != 999){
            al_draw_bitmap(SLOT_NUMBER[b->returnStaticValue_2()], b->getX()+102, b->getY()+8, 0);
            break;
        }
        i = i + 1;
    }
    i = 0;
    while (i < 10){
        if(b->returnIntVar_4() == int_var_list[i]){
            al_draw_bitmap(MICRO_INT_VAR_IMG[i], b->getX()+190, b->getY()+8, 0);
            break;
        }
        if(b->returnBoolVar_4() == bool_var_list[i]){
            al_draw_bitmap(MICRO_BOOL_VAR_IMG[i], b->getX()+190, b->getY()+8, 0);
            break;
        }
        if(b->returnStaticValue_4() != 999){
            al_draw_bitmap(SLOT_NUMBER[b->returnStaticValue_4()], b->getX()+190, b->getY()+8, 0);
            break;
        }
        i = i + 1;
    }
    i = 0;
    while (i < 6){
        if(b->returnLogicSymbol_3() != 999){
            al_draw_bitmap(MICRO_LOGIC_OPERATOR_IMG[b->returnLogicSymbol_3()], b->getX()+146, b->getY()+8, 0);
            break;
        }
        i = i + 1;
    }
    i = 0;
    while (i < 5){
        if(b->returnMathSymbol_3() != 999){
            al_draw_bitmap(MICRO_MATH_OPERATOR_IMG[b->returnMathSymbol_3()], b->getX()+146, b->getY()+8, 0);
            break;
        }
        i = i + 1;
    }
    i = 0;
    if((mouseX > b->getX()+108) && (mouseX < (b->getX() + 120)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+6))) {
        al_draw_bitmap(POINT[1], b->getX()+109, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+109, b->getY()+37, 0);
    } else if((mouseX > b->getX()+108) && (mouseX < (b->getX() + 120)) && (mouseY > b->getY()+36) && (mouseY < (b->getY()+47))) {
        al_draw_bitmap(POINT[0], b->getX()+109, b->getY()-5, 0);
        al_draw_bitmap(POINT[1], b->getX()+109, b->getY()+37, 0);
    } else {
        al_draw_bitmap(POINT[0], b->getX()+109, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+109, b->getY()+37, 0);
    }
}
void Interface :: print_new_conditional_block(Block *b) {
    if((mouseX > b->getX()) && (mouseX < (b->getX() + b->getWidth())) && (mouseY > b->getY()) && (mouseY < (b->getY() + b->getHeight()))) {
        if(black_sensor_menu_selected || color_sensor_menu_selected || ultrasonic_sensor_menu_selected || number_menu_selected || int_menu_selected || bool_menu_selected || logic_menu_selected) {
            al_draw_bitmap(NEW_CONDITIONAL_BLOCK_IMG[0], b->getX(), b->getY(), 0);
        } else {
            al_draw_bitmap(NEW_CONDITIONAL_BLOCK_IMG[1], b->getX(), b->getY(), 0);
        }
    } else if(b->getSelected() == true) {
        al_draw_bitmap(NEW_CONDITIONAL_BLOCK_IMG[2], b->getX(), b->getY(), 0);
    } else if(b->getExecuting() == true) {
        al_draw_bitmap(NEW_CONDITIONAL_BLOCK_IMG[3], b->getX(), b->getY(), 0);
    } else {
        al_draw_bitmap(NEW_CONDITIONAL_BLOCK_IMG[0], b->getX(), b->getY(), 0);
    }
    int i = 0;
    while (i < 10){
        if(b->returnIntVar_1() == int_var_list[i]){
            al_draw_bitmap(MICRO_INT_VAR_IMG[i], b->getX()+23, b->getY()+8, 0);
            break;
        }
        if(b->returnBoolVar_1() == bool_var_list[i]){
            al_draw_bitmap(MICRO_BOOL_VAR_IMG[i], b->getX()+23, b->getY()+8, 0);
            break;
        }
        i = i + 1;
    }
    i = 0;
    while (i < 10){
        if(b->returnIntVar_3() == int_var_list[i]){
            al_draw_bitmap(MICRO_INT_VAR_IMG[i], b->getX()+113, b->getY()+8, 0);
            break;
        }
        if(b->returnBoolVar_3() == bool_var_list[i]){
            al_draw_bitmap(MICRO_BOOL_VAR_IMG[i], b->getX()+113, b->getY()+8, 0);
            break;
        }
        if(b->returnStaticValue_3() != 999){
            al_draw_bitmap(MICRO_NUMBER[b->returnStaticValue_3()], b->getX()+113, b->getY()+8, 0);
            break;
        }
        i = i + 1;
    }
    i = 0;
    while (i < 6){
        if(b->returnLogicSymbol_2() != 999){
            al_draw_bitmap(MICRO_LOGIC_OPERATOR_IMG[b->returnLogicSymbol_2()], b->getX()+68, b->getY()+8, 0);
            break;
        }
        i = i + 1;
    }
    i = 0;
    if((mouseX > b->getX()+74) && (mouseX < (b->getX() + 86)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+6))) {
        al_draw_bitmap(POINT[1], b->getX()+75, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+30, b->getY()+37, 0);
        al_draw_bitmap(POINT[0], b->getX()+121, b->getY()+37, 0);
    } else if((mouseX > b->getX()+29) && (mouseX < (b->getX() + 41)) && (mouseY > b->getY()+36) && (mouseY < (b->getY()+47))) {
        al_draw_bitmap(POINT[0], b->getX()+75, b->getY()-5, 0);
        al_draw_bitmap(POINT[1], b->getX()+30, b->getY()+37, 0);
        al_draw_bitmap(POINT[0], b->getX()+121, b->getY()+37, 0);
    } else if((mouseX > b->getX()+120) && (mouseX < (b->getX() + 132)) && (mouseY > b->getY()+36) && (mouseY < (b->getY()+47))) {
        al_draw_bitmap(POINT[0], b->getX()+75, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+30, b->getY()+37, 0);
        al_draw_bitmap(POINT[1], b->getX()+121, b->getY()+37, 0);
    } else {
        al_draw_bitmap(POINT[0], b->getX()+75, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+30, b->getY()+37, 0);
        al_draw_bitmap(POINT[0], b->getX()+121, b->getY()+37, 0);
    }
}
void Interface :: print_while_block(Block *b) {
    if((mouseX > b->getX()) && (mouseX < (b->getX() + b->getWidth())) && (mouseY > b->getY()) && (mouseY < (b->getY() + b->getHeight()))) {
        if(black_sensor_menu_selected || color_sensor_menu_selected || ultrasonic_sensor_menu_selected || number_menu_selected || int_menu_selected || bool_menu_selected || logic_menu_selected) {
            al_draw_bitmap(WHILE_BLOCK_IMG[0], b->getX(), b->getY(), 0);
        } else {
            al_draw_bitmap(WHILE_BLOCK_IMG[1], b->getX(), b->getY(), 0);
        }
    } else if(b->getSelected() == true) {
        al_draw_bitmap(WHILE_BLOCK_IMG[2], b->getX(), b->getY(), 0);
    } else if(b->getExecuting() == true) {
        al_draw_bitmap(WHILE_BLOCK_IMG[3], b->getX(), b->getY(), 0);
    } else {
        al_draw_bitmap(WHILE_BLOCK_IMG[0], b->getX(), b->getY(), 0);
    }
    int i = 0;
    while (i < 10){
        if(b->returnIntVar_1() == int_var_list[i]){
            al_draw_bitmap(MICRO_INT_VAR_IMG[i], b->getX()+23, b->getY()+8, 0);
            break;
        }
        if(b->returnBoolVar_1() == bool_var_list[i]){
            al_draw_bitmap(MICRO_BOOL_VAR_IMG[i], b->getX()+23, b->getY()+8, 0);
            break;
        }
        i = i + 1;
    }
    i = 0;
    while (i < 10){
        if(b->returnIntVar_3() == int_var_list[i]){
            al_draw_bitmap(MICRO_INT_VAR_IMG[i], b->getX()+113, b->getY()+8, 0);
            break;
        }
        if(b->returnBoolVar_3() == bool_var_list[i]){
            al_draw_bitmap(MICRO_BOOL_VAR_IMG[i], b->getX()+113, b->getY()+8, 0);
            break;
        }
        if(b->returnStaticValue_3() != 999){
            al_draw_bitmap(SLOT_NUMBER[b->returnStaticValue_3()], b->getX()+113, b->getY()+8, 0);
            break;
        }
        i = i + 1;
    }
    i = 0;
    while (i < 6){
        if(b->returnLogicSymbol_2() != 999){
            al_draw_bitmap(MICRO_LOGIC_OPERATOR_IMG[b->returnLogicSymbol_2()], b->getX()+68, b->getY()+8, 0);
            break;
        }
        i = i + 1;
    }
    i = 0;
    if((mouseX > b->getX()+74) && (mouseX < (b->getX() + 86)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+6))) {
        al_draw_bitmap(POINT[1], b->getX()+75, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+75, b->getY()+37, 0);
        al_draw_bitmap(POINT[0], b->getX()+155, b->getY()+2, 0);
        al_draw_bitmap(POINT[0], b->getX()+155, b->getY()+29, 0);
    } else if((mouseX > b->getX()+74) && (mouseX < (b->getX() + 86)) && (mouseY > b->getY()+36) && (mouseY < (b->getY()+47))) {
        al_draw_bitmap(POINT[0], b->getX()+75, b->getY()-5, 0);
        al_draw_bitmap(POINT[1], b->getX()+75, b->getY()+37, 0);
        al_draw_bitmap(POINT[0], b->getX()+155, b->getY()+2, 0);
        al_draw_bitmap(POINT[0], b->getX()+155, b->getY()+29, 0);
    } else if((mouseX > b->getX()+154) && (mouseX < (b->getX() + 165)) && (mouseY > b->getY()+1) && (mouseY < (b->getY()+12))) {
        al_draw_bitmap(POINT[0], b->getX()+75, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+75, b->getY()+37, 0);
        al_draw_bitmap(POINT[1], b->getX()+155, b->getY()+2, 0);
        al_draw_bitmap(POINT[0], b->getX()+155, b->getY()+29, 0);
    } else if((mouseX > b->getX()+154) && (mouseX < (b->getX() + 165)) && (mouseY > b->getY()+28) && (mouseY < (b->getY()+40))) {
        al_draw_bitmap(POINT[0], b->getX()+75, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+75, b->getY()+37, 0);
        al_draw_bitmap(POINT[0], b->getX()+155, b->getY()+2, 0);
        al_draw_bitmap(POINT[1], b->getX()+155, b->getY()+29, 0);
    } else {
        al_draw_bitmap(POINT[0], b->getX()+75, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+75, b->getY()+37, 0);
        al_draw_bitmap(POINT[0], b->getX()+155, b->getY()+2, 0);
        al_draw_bitmap(POINT[0], b->getX()+155, b->getY()+29, 0);
    }
}
void Interface :: load_program_images() {
    //load_bitmap(&menu, "../../GUI_files/images/menu.png");

    load_bitmap(&play_button, "../../GUI_files/images/menu_buttons/play_button.png");
    load_bitmap(&play_button_selected, "../../GUI_files/images/menu_buttons/play_button_selected.png");
    load_bitmap(&pause_button, "../../GUI_files/images/menu_buttons/pause_button.png");
    load_bitmap(&pause_button_selected, "../../GUI_files/images/menu_buttons/pause_button_selected.png");
    load_bitmap(&stop_button, "../../GUI_files/images/menu_buttons/stop_button.png");
    load_bitmap(&stop_button_selected, "../../GUI_files/images/menu_buttons/stop_button_selected.png");
    load_bitmap(&save_button, "../../GUI_files/images/menu_buttons/save_button.png");
    load_bitmap(&save_button_selected, "../../GUI_files/images/menu_buttons/save_button_selected.png");
    load_bitmap(&load_button, "../../GUI_files/images/menu_buttons/load_button.png");
    load_bitmap(&load_button_selected, "../../GUI_files/images/menu_buttons/load_button_selected.png");
    load_bitmap(&save_as_button, "../../GUI_files/images/menu_buttons/save_as_button.png");
    load_bitmap(&save_as_button_selected, "../../GUI_files/images/menu_buttons/save_as_button_selected.png");
    load_bitmap(&vrep_button, "../../GUI_files/images/menu_buttons/vrep_button.png");
    load_bitmap(&vrep_button_selected, "../../GUI_files/images/menu_buttons/vrep_button_selected.png");
    load_bitmap(&bluetooth_button, "../../GUI_files/images/menu_buttons/bluetooth_button.png");
    load_bitmap(&bluetooth_button_selected, "../../GUI_files/images/menu_buttons/bluetooth_button_selected.png");

    load_bitmap(&mini_menu[0], "../../GUI_files/images/blocks/decision_block/mini_decision_block.png");
    load_bitmap(&mini_menu[1], "../../GUI_files/images/blocks/function_block/mini_function_block.png");
    load_bitmap(&mini_menu[2], "../../GUI_files/images/blocks/start_block/mini_start_block.png");
    load_bitmap(&mini_menu[3], "../../GUI_files/images/blocks/end_block/mini_end_block.png");
    load_bitmap(&mini_menu[4], "../../GUI_files/images/blocks/merge_block/mini_merge_block.png");
    load_bitmap(&mini_menu[5], "../../GUI_files/images/blocks/loop_block/mini_loop_block.png");
    load_bitmap(&WALK_FOWARD_ACTION, "../../GUI_files/images/functions/mini_action_walk_foward.png");
    load_bitmap(&MICRO_WALK_FOWARD, "../../GUI_files/images/functions/micro_action_walk_foward.png");
    load_bitmap(&TURN_LEFT_ACTION, "../../GUI_files/images/functions/mini_action_turn_left.png");
    load_bitmap(&MICRO_TURN_LEFT, "../../GUI_files/images/functions/micro_action_turn_left.png");
    load_bitmap(&TURN_RIGHT_ACTION, "../../GUI_files/images/functions/mini_action_turn_right.png");
    load_bitmap(&MICRO_TURN_RIGHT, "../../GUI_files/images/functions/micro_action_turn_right.png");
    load_bitmap(&logic_true, "../../GUI_files/images/functions/mini_logic_true.png");
    load_bitmap(&logic_false, "../../GUI_files/images/functions/mini_logic_false.png");
    load_bitmap(&POINT[0], "../../GUI_files/images/point.png");
    load_bitmap(&POINT[1], "../../GUI_files/images/hpoint.png");
    load_bitmap(&arrow, "../../GUI_files/images/arrow.png");

    load_bitmap(&DECISION_BLOCK[0], "../../GUI_files/images/blocks/decision_block/decision_block.png");
    load_bitmap(&DECISION_BLOCK[1], "../../GUI_files/images/blocks/decision_block/decision_block_mouse.png");
    load_bitmap(&DECISION_BLOCK[2], "../../GUI_files/images/blocks/decision_block/decision_block_selected.png");
    load_bitmap(&DECISION_BLOCK[3], "../../GUI_files/images/blocks/decision_block/decision_block_executing.png");
    load_bitmap(&END_BLOCK_IMG[0], "../../GUI_files/images/blocks/end_block/end_block.png");
    load_bitmap(&END_BLOCK_IMG[1], "../../GUI_files/images/blocks/end_block/end_block_mouse.png");
    load_bitmap(&END_BLOCK_IMG[2], "../../GUI_files/images/blocks/end_block/end_block_selected.png");
    load_bitmap(&END_BLOCK_IMG[3], "../../GUI_files/images/blocks/end_block/end_block_executing.png");
    load_bitmap(&FUNCTION_BLOCK[0], "../../GUI_files/images/blocks/function_block/function_block.png");
    load_bitmap(&FUNCTION_BLOCK[1], "../../GUI_files/images/blocks/function_block/function_block_mouse.png");
    load_bitmap(&FUNCTION_BLOCK[2], "../../GUI_files/images/blocks/function_block/function_block_selected.png");
    load_bitmap(&FUNCTION_BLOCK[3], "../../GUI_files/images/blocks/function_block/function_block_executing.png");
    load_bitmap(&LOOP_BLOCK_IMG[0], "../../GUI_files/images/blocks/loop_block/loop_block2_1.png");
    load_bitmap(&LOOP_BLOCK_IMG[1], "../../GUI_files/images/blocks/loop_block/loop_block2_1_mouse.png");
    load_bitmap(&LOOP_BLOCK_IMG[2], "../../GUI_files/images/blocks/loop_block/loop_block2_1_selected.png");
    load_bitmap(&LOOP_BLOCK_IMG[3], "../../GUI_files/images/blocks/loop_block/loop_block2_1_executing.png");
    load_bitmap(&MERGE_BLOCK_IMG[0], "../../GUI_files/images/blocks/merge_block/merge_block.png");
    load_bitmap(&MERGE_BLOCK_IMG[1], "../../GUI_files/images/blocks/merge_block/merge_block_mouse.png");
    load_bitmap(&MERGE_BLOCK_IMG[2], "../../GUI_files/images/blocks/merge_block/merge_block_selected.png");
    load_bitmap(&MERGE_BLOCK_IMG[3], "../../GUI_files/images/blocks/merge_block/merge_block_executing.png");
    load_bitmap(&START_BLOCK_IMG[0], "../../GUI_files/images/blocks/start_block/start_block.png");
    load_bitmap(&START_BLOCK_IMG[1], "../../GUI_files/images/blocks/start_block/start_block_mouse.png");
    load_bitmap(&START_BLOCK_IMG[2], "../../GUI_files/images/blocks/start_block/start_block_selected.png");
    load_bitmap(&START_BLOCK_IMG[3], "../../GUI_files/images/blocks/start_block/start_block_executing.png");
    load_bitmap(&BLACK_SENSOR_FUNCTION, "../../GUI_files/images/blocks/black_sensor_block/mini_black_sensor.png");
    load_bitmap(&BLACK_SENSOR_1_FUNCTION, "../../GUI_files/images/blocks/black_sensor_block/mini_black_sensor1.png");
    load_bitmap(&BLACK_SENSOR_2_FUNCTION, "../../GUI_files/images/blocks/black_sensor_block/mini_black_sensor2.png");
    load_bitmap(&BLACK_SENSOR_3_FUNCTION, "../../GUI_files/images/blocks/black_sensor_block/mini_black_sensor3.png");
    load_bitmap(&BLACK_SENSOR_4_FUNCTION, "../../GUI_files/images/blocks/black_sensor_block/mini_black_sensor4.png");
    load_bitmap(&BLACK_SENSOR_5_FUNCTION, "../../GUI_files/images/blocks/black_sensor_block/mini_black_sensor5.png");
    load_bitmap(&MICRO_BLACK_SENSOR1, "../../GUI_files/images/blocks/black_sensor_block/micro_black_sensor1.png");
    load_bitmap(&MICRO_BLACK_SENSOR2, "../../GUI_files/images/blocks/black_sensor_block/micro_black_sensor2.png");
    load_bitmap(&MICRO_BLACK_SENSOR3, "../../GUI_files/images/blocks/black_sensor_block/micro_black_sensor3.png");
    load_bitmap(&MICRO_BLACK_SENSOR4, "../../GUI_files/images/blocks/black_sensor_block/micro_black_sensor4.png");
    load_bitmap(&MICRO_BLACK_SENSOR5, "../../GUI_files/images/blocks/black_sensor_block/micro_black_sensor5.png");
    load_bitmap(&COLOR_SENSOR_FUNCTION, "../../GUI_files/images/blocks/color_sensor_block/mini_color_sensor.png");
    load_bitmap(&COLOR_SENSOR_1_FUNCTION, "../../GUI_files/images/blocks/color_sensor_block/mini_color_sensor1.png");
    load_bitmap(&COLOR_SENSOR_2_FUNCTION, "../../GUI_files/images/blocks/color_sensor_block/mini_color_sensor2.png");
    load_bitmap(&MICRO_COLOR_SENSOR1, "../../GUI_files/images/blocks/color_sensor_block/micro_color_sensor1.png");
    load_bitmap(&MICRO_COLOR_SENSOR2, "../../GUI_files/images/blocks/color_sensor_block/micro_color_sensor2.png");
    load_bitmap(&ULTRASONIC_SENSOR_FUNCTION, "../../GUI_files/images/blocks/ultrasonic_sensor_block/mini_ultrasonic_sensor.png");
    load_bitmap(&ULTRASONIC_SENSOR_1_FUNCTION, "../../GUI_files/images/blocks/ultrasonic_sensor_block/mini_ultrasonic_sensor1.png");
    load_bitmap(&ULTRASONIC_SENSOR_2_FUNCTION, "../../GUI_files/images/blocks/ultrasonic_sensor_block/mini_ultrasonic_sensor2.png");
    load_bitmap(&ULTRASONIC_SENSOR_3_FUNCTION, "../../GUI_files/images/blocks/ultrasonic_sensor_block/mini_ultrasonic_sensor3.png");
    load_bitmap(&MICRO_ULTRASONIC_SENSOR1, "../../GUI_files/images/blocks/ultrasonic_sensor_block/micro_ultrasonic_sensor1.png");
    load_bitmap(&MICRO_ULTRASONIC_SENSOR2, "../../GUI_files/images/blocks/ultrasonic_sensor_block/micro_ultrasonic_sensor2.png");
    load_bitmap(&MICRO_ULTRASONIC_SENSOR3, "../../GUI_files/images/blocks/ultrasonic_sensor_block/micro_ultrasonic_sensor3.png");

    load_bitmap(&NUMBER[0], "../../GUI_files/images/functions/mini_number_0.png");
    load_bitmap(&NUMBER[1], "../../GUI_files/images/functions/mini_number_1.png");
    load_bitmap(&NUMBER[2], "../../GUI_files/images/functions/mini_number_2.png");
    load_bitmap(&NUMBER[3], "../../GUI_files/images/functions/mini_number_3.png");
    load_bitmap(&NUMBER[4], "../../GUI_files/images/functions/mini_number_4.png");
    load_bitmap(&NUMBER[5], "../../GUI_files/images/functions/mini_number_5.png");
    load_bitmap(&NUMBER[6], "../../GUI_files/images/functions/mini_number_6.png");
    load_bitmap(&NUMBER[7], "../../GUI_files/images/functions/mini_number_7.png");
    load_bitmap(&NUMBER[8], "../../GUI_files/images/functions/mini_number_8.png");
    load_bitmap(&NUMBER[9], "../../GUI_files/images/functions/mini_number_9.png");

    load_bitmap(&MICRO_NUMBER[0], "../../GUI_files/images/functions/micro_number_0.png");
    load_bitmap(&MICRO_NUMBER[1], "../../GUI_files/images/functions/micro_number_1.png");
    load_bitmap(&MICRO_NUMBER[2], "../../GUI_files/images/functions/micro_number_2.png");
    load_bitmap(&MICRO_NUMBER[3], "../../GUI_files/images/functions/micro_number_3.png");
    load_bitmap(&MICRO_NUMBER[4], "../../GUI_files/images/functions/micro_number_4.png");
    load_bitmap(&MICRO_NUMBER[5], "../../GUI_files/images/functions/micro_number_5.png");
    load_bitmap(&MICRO_NUMBER[6], "../../GUI_files/images/functions/micro_number_6.png");
    load_bitmap(&MICRO_NUMBER[7], "../../GUI_files/images/functions/micro_number_7.png");
    load_bitmap(&MICRO_NUMBER[8], "../../GUI_files/images/functions/micro_number_8.png");
    load_bitmap(&MICRO_NUMBER[9], "../../GUI_files/images/functions/micro_number_9.png");

    load_bitmap(&trash, "../../GUI_files/images/trash_icon.png");

    //Imagens V3
    load_bitmap(&NEW_MINI_MENU_IMG[0], "../../GUI_files/images/blocks/attribution_block/mini_attribution_block.png");
    load_bitmap(&NEW_MINI_MENU_IMG[1], "../../GUI_files/images/blocks/math_block/mini_math_block.png");
    load_bitmap(&NEW_MINI_MENU_IMG[2], "../../GUI_files/images/blocks/new_condition_block/mini_condition_block.png");
    load_bitmap(&NEW_MINI_MENU_IMG[3], "../../GUI_files/images/blocks/while_block/mini_while_block.png");

    load_bitmap(&ATTRIBUTION_BLOCK_IMG[0], "../../GUI_files/images/blocks/attribution_block/attribution_block.png");
    load_bitmap(&ATTRIBUTION_BLOCK_IMG[1], "../../GUI_files/images/blocks/attribution_block/attribution_block_executing.png");
    load_bitmap(&ATTRIBUTION_BLOCK_IMG[2], "../../GUI_files/images/blocks/attribution_block/attribution_block_mouse.png");
    load_bitmap(&ATTRIBUTION_BLOCK_IMG[3], "../../GUI_files/images/blocks/attribution_block/attribution_block_selected.png");

    load_bitmap(&MATH_BLOCK_IMG[0], "../../GUI_files/images/blocks/math_block/math_block.png");
    load_bitmap(&MATH_BLOCK_IMG[1], "../../GUI_files/images/blocks/math_block/math_block_executing.png");
    load_bitmap(&MATH_BLOCK_IMG[2], "../../GUI_files/images/blocks/math_block/math_block_mouse.png");
    load_bitmap(&MATH_BLOCK_IMG[3], "../../GUI_files/images/blocks/math_block/math_block_selected.png");

    load_bitmap(&NEW_CONDITIONAL_BLOCK_IMG[0], "../../GUI_files/images/blocks/new_condition_block/condition_block.png");
    load_bitmap(&NEW_CONDITIONAL_BLOCK_IMG[1], "../../GUI_files/images/blocks/new_condition_block/condition_block_executing.png");
    load_bitmap(&NEW_CONDITIONAL_BLOCK_IMG[2], "../../GUI_files/images/blocks/new_condition_block/condition_block_mouse.png");
    load_bitmap(&NEW_CONDITIONAL_BLOCK_IMG[3], "../../GUI_files/images/blocks/new_condition_block/condition_block_selected.png");

    load_bitmap(&WHILE_BLOCK_IMG[0], "../../GUI_files/images/blocks/while_block/while_block.png");
    load_bitmap(&WHILE_BLOCK_IMG[1], "../../GUI_files/images/blocks/while_block/while_block_executing.png");
    load_bitmap(&WHILE_BLOCK_IMG[2], "../../GUI_files/images/blocks/while_block/while_block_mouse.png");
    load_bitmap(&WHILE_BLOCK_IMG[3], "../../GUI_files/images/blocks/while_block/while_block_selected.png");

    load_bitmap(&BOOL_VAR_IMG[0], "../../GUI_files/images/variables_and_operators/boolean_A.png");
    load_bitmap(&BOOL_VAR_IMG[1], "../../GUI_files/images/variables_and_operators/boolean_B.png");
    load_bitmap(&BOOL_VAR_IMG[2], "../../GUI_files/images/variables_and_operators/boolean_C.png");
    load_bitmap(&BOOL_VAR_IMG[3], "../../GUI_files/images/variables_and_operators/boolean_D.png");
    load_bitmap(&BOOL_VAR_IMG[4], "../../GUI_files/images/variables_and_operators/boolean_E.png");
    load_bitmap(&BOOL_VAR_IMG[5], "../../GUI_files/images/variables_and_operators/boolean_F.png");
    load_bitmap(&BOOL_VAR_IMG[6], "../../GUI_files/images/variables_and_operators/boolean_G.png");
    load_bitmap(&BOOL_VAR_IMG[7], "../../GUI_files/images/variables_and_operators/boolean_H.png");
    load_bitmap(&BOOL_VAR_IMG[8], "../../GUI_files/images/variables_and_operators/boolean_I.png");
    load_bitmap(&BOOL_VAR_IMG[9], "../../GUI_files/images/variables_and_operators/boolean_J.png");

    load_bitmap(&MICRO_BOOL_VAR_IMG[0], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_boolean_A.png");
    load_bitmap(&MICRO_BOOL_VAR_IMG[1], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_boolean_B.png");
    load_bitmap(&MICRO_BOOL_VAR_IMG[2], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_boolean_C.png");
    load_bitmap(&MICRO_BOOL_VAR_IMG[3], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_boolean_D.png");
    load_bitmap(&MICRO_BOOL_VAR_IMG[4], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_boolean_E.png");
    load_bitmap(&MICRO_BOOL_VAR_IMG[5], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_boolean_F.png");
    load_bitmap(&MICRO_BOOL_VAR_IMG[6], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_boolean_G.png");
    load_bitmap(&MICRO_BOOL_VAR_IMG[7], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_boolean_H.png");
    load_bitmap(&MICRO_BOOL_VAR_IMG[8], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_boolean_I.png");
    load_bitmap(&MICRO_BOOL_VAR_IMG[9], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_boolean_J.png");

    load_bitmap(&INT_VAR_IMG[0], "../../GUI_files/images/variables_and_operators/integer_A.png");
    load_bitmap(&INT_VAR_IMG[1], "../../GUI_files/images/variables_and_operators/integer_B.png");
    load_bitmap(&INT_VAR_IMG[2], "../../GUI_files/images/variables_and_operators/integer_C.png");
    load_bitmap(&INT_VAR_IMG[3], "../../GUI_files/images/variables_and_operators/integer_D.png");
    load_bitmap(&INT_VAR_IMG[4], "../../GUI_files/images/variables_and_operators/integer_E.png");
    load_bitmap(&INT_VAR_IMG[5], "../../GUI_files/images/variables_and_operators/integer_F.png");
    load_bitmap(&INT_VAR_IMG[6], "../../GUI_files/images/variables_and_operators/integer_G.png");
    load_bitmap(&INT_VAR_IMG[7], "../../GUI_files/images/variables_and_operators/integer_H.png");
    load_bitmap(&INT_VAR_IMG[8], "../../GUI_files/images/variables_and_operators/integer_I.png");
    load_bitmap(&INT_VAR_IMG[9], "../../GUI_files/images/variables_and_operators/integer_J.png");

    load_bitmap(&MICRO_INT_VAR_IMG[0], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_integer_A.png");
    load_bitmap(&MICRO_INT_VAR_IMG[1], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_integer_B.png");
    load_bitmap(&MICRO_INT_VAR_IMG[2], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_integer_C.png");
    load_bitmap(&MICRO_INT_VAR_IMG[3], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_integer_D.png");
    load_bitmap(&MICRO_INT_VAR_IMG[4], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_integer_E.png");
    load_bitmap(&MICRO_INT_VAR_IMG[5], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_integer_F.png");
    load_bitmap(&MICRO_INT_VAR_IMG[6], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_integer_G.png");
    load_bitmap(&MICRO_INT_VAR_IMG[7], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_integer_H.png");
    load_bitmap(&MICRO_INT_VAR_IMG[8], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_integer_I.png");
    load_bitmap(&MICRO_INT_VAR_IMG[9], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_integer_J.png");

    load_bitmap(&LOGIC_OPERATOR_IMG[0], "../../GUI_files/images/variables_and_operators/logic_equal.png");
    load_bitmap(&LOGIC_OPERATOR_IMG[1], "../../GUI_files/images/variables_and_operators/logic_greater.png");
    load_bitmap(&LOGIC_OPERATOR_IMG[2], "../../GUI_files/images/variables_and_operators/logic_less.png");
    load_bitmap(&LOGIC_OPERATOR_IMG[3], "../../GUI_files/images/variables_and_operators/logic_greater_equal.png");
    load_bitmap(&LOGIC_OPERATOR_IMG[4], "../../GUI_files/images/variables_and_operators/logic_less_equal.png");
    load_bitmap(&LOGIC_OPERATOR_IMG[5], "../../GUI_files/images/variables_and_operators/logic_unequal.png");

    load_bitmap(&MICRO_LOGIC_OPERATOR_IMG[0], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_logic_equal.png");
    load_bitmap(&MICRO_LOGIC_OPERATOR_IMG[2], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_logic_greater.png");
    load_bitmap(&MICRO_LOGIC_OPERATOR_IMG[4], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_logic_less.png");
    load_bitmap(&MICRO_LOGIC_OPERATOR_IMG[3], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_logic_greater_equal.png");
    load_bitmap(&MICRO_LOGIC_OPERATOR_IMG[5], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_logic_less_equal.png");
    load_bitmap(&MICRO_LOGIC_OPERATOR_IMG[1], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_logic_unequal.png");

    load_bitmap(&MATH_OPERATOR_IMG[0], "../../GUI_files/images/variables_and_operators/operator_add.png");
    load_bitmap(&MATH_OPERATOR_IMG[1], "../../GUI_files/images/variables_and_operators/operator_div.png");
    load_bitmap(&MATH_OPERATOR_IMG[2], "../../GUI_files/images/variables_and_operators/operator_exponent.png");
    load_bitmap(&MATH_OPERATOR_IMG[3], "../../GUI_files/images/variables_and_operators/operator_mult.png");
    load_bitmap(&MATH_OPERATOR_IMG[4], "../../GUI_files/images/variables_and_operators/operator_sub.png");

    load_bitmap(&MICRO_MATH_OPERATOR_IMG[0], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_operator_add.png");
    load_bitmap(&MICRO_MATH_OPERATOR_IMG[3], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_operator_div.png");
    load_bitmap(&MICRO_MATH_OPERATOR_IMG[4], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_operator_exponent.png");
    load_bitmap(&MICRO_MATH_OPERATOR_IMG[2], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_operator_mult.png");
    load_bitmap(&MICRO_MATH_OPERATOR_IMG[1], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/micro_operator_sub.png");

    load_bitmap(&SLOT_NUMBER[0], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/number_0.png");
    load_bitmap(&SLOT_NUMBER[1], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/number_1.png");
    load_bitmap(&SLOT_NUMBER[2], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/number_2.png");
    load_bitmap(&SLOT_NUMBER[3], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/number_3.png");
    load_bitmap(&SLOT_NUMBER[4], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/number_4.png");
    load_bitmap(&SLOT_NUMBER[5], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/number_5.png");
    load_bitmap(&SLOT_NUMBER[6], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/number_6.png");
    load_bitmap(&SLOT_NUMBER[7], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/number_7.png");
    load_bitmap(&SLOT_NUMBER[8], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/number_8.png");
    load_bitmap(&SLOT_NUMBER[9], "../../GUI_files/images/variables_and_operators/micro_variables_and_operators/number_9.png");
}
void Interface :: destroy_program_images() {
    al_destroy_bitmap(play_button);
    al_destroy_bitmap(play_button_selected);
	al_destroy_bitmap(pause_button);
    al_destroy_bitmap(pause_button_selected);
	al_destroy_bitmap(stop_button);
    al_destroy_bitmap(stop_button_selected);
	al_destroy_bitmap(save_button);
    al_destroy_bitmap(save_button_selected);
    al_destroy_bitmap(save_as_button);
    al_destroy_bitmap(save_as_button_selected);
    al_destroy_bitmap(load_button);
    al_destroy_bitmap(load_button_selected);
    al_destroy_bitmap(vrep_button);
    al_destroy_bitmap(vrep_button_selected);
	al_destroy_bitmap(bluetooth_button);
    al_destroy_bitmap(bluetooth_button_selected);
    for(int i=0; i<6; i++) {
        al_destroy_bitmap(mini_menu[i]);
    }
    al_destroy_bitmap(WALK_FOWARD_ACTION);
    al_destroy_bitmap(MICRO_WALK_FOWARD);
    al_destroy_bitmap(TURN_LEFT_ACTION);
    al_destroy_bitmap(MICRO_TURN_LEFT);
    al_destroy_bitmap(TURN_RIGHT_ACTION);
    al_destroy_bitmap(MICRO_TURN_RIGHT);
    al_destroy_bitmap(logic_true);
    al_destroy_bitmap(logic_false);
    al_destroy_bitmap(POINT[0]);
	al_destroy_bitmap(POINT[1]);
    al_destroy_bitmap(arrow);
    al_destroy_bitmap(BLACK_SENSOR_FUNCTION);
    al_destroy_bitmap(BLACK_SENSOR_1_FUNCTION);
    al_destroy_bitmap(BLACK_SENSOR_2_FUNCTION);
    al_destroy_bitmap(BLACK_SENSOR_3_FUNCTION);
    al_destroy_bitmap(BLACK_SENSOR_4_FUNCTION);
    al_destroy_bitmap(BLACK_SENSOR_5_FUNCTION);
    al_destroy_bitmap(MICRO_BLACK_SENSOR1);
    al_destroy_bitmap(MICRO_BLACK_SENSOR2);
    al_destroy_bitmap(MICRO_BLACK_SENSOR3);
    al_destroy_bitmap(MICRO_BLACK_SENSOR4);
    al_destroy_bitmap(MICRO_BLACK_SENSOR5);
    al_destroy_bitmap(COLOR_SENSOR_FUNCTION);
    al_destroy_bitmap(COLOR_SENSOR_1_FUNCTION);
    al_destroy_bitmap(COLOR_SENSOR_2_FUNCTION);
    al_destroy_bitmap(MICRO_COLOR_SENSOR1);
    al_destroy_bitmap(MICRO_COLOR_SENSOR2);
    al_destroy_bitmap(ULTRASONIC_SENSOR_FUNCTION);
    al_destroy_bitmap(ULTRASONIC_SENSOR_1_FUNCTION);
    al_destroy_bitmap(ULTRASONIC_SENSOR_2_FUNCTION);
    al_destroy_bitmap(ULTRASONIC_SENSOR_3_FUNCTION);
    al_destroy_bitmap(MICRO_ULTRASONIC_SENSOR1);
    al_destroy_bitmap(MICRO_ULTRASONIC_SENSOR2);
    al_destroy_bitmap(MICRO_ULTRASONIC_SENSOR3);

    al_destroy_bitmap(trash);

    for(int i=0; i<4; i++) {
        al_destroy_bitmap(DECISION_BLOCK[i]);
    }
    for(int i=0; i<4; i++) {
        al_destroy_bitmap(END_BLOCK_IMG[i]);
    }
    for(int i=0; i<4; i++) {
        al_destroy_bitmap(FUNCTION_BLOCK[i]);
    }
    for(int i=0; i<4; i++) {
        al_destroy_bitmap(LOOP_BLOCK_IMG[i]);
    }
    for(int i=0; i<4; i++) {
        al_destroy_bitmap(MERGE_BLOCK_IMG[i]);
    }
    for(int i=0; i<4; i++) {
        al_destroy_bitmap(START_BLOCK_IMG[i]);
    }
    for(int i=0; i<10; i++) {
        al_destroy_bitmap(NUMBER[i]);
    }
    for(int i=0; i<10; i++) {
        al_destroy_bitmap(MICRO_NUMBER[i]);
    }
}
void Interface :: reset_dragging_variables() {
    dragging_black_sensor1 = false;
    dragging_black_sensor2 = false;
    dragging_black_sensor3 = false;
    dragging_black_sensor4 = false;
    dragging_black_sensor5 = false;
    dragging_color_sensor1 = false;
    dragging_color_sensor2 = false;
    dragging_ultrasonic_sensor1 = false;
    dragging_ultrasonic_sensor2 = false;
    dragging_ultrasonic_sensor3 = false;
    dragging_walk_foward = false;
    dragging_turn_left = false;
    dragging_turn_right = false;
    dragging_number_0 = false;
    dragging_number_1 = false;
    dragging_number_2 = false;
    dragging_number_3 = false;
    dragging_number_4 = false;
    dragging_number_5 = false;
    dragging_number_6 = false;
    dragging_number_7 = false;
    dragging_number_8 = false;
    dragging_number_9 = false;
    dragging_int_A = false;
    dragging_int_B = false;
    dragging_int_C = false;
    dragging_int_D = false;
    dragging_int_E = false;
    dragging_int_F = false;
    dragging_int_G = false;
    dragging_int_H = false;
    dragging_int_I = false;
    dragging_int_J = false;
    dragging_bool_A = false;
    dragging_bool_B = false;
    dragging_bool_C = false;
    dragging_bool_D = false;
    dragging_bool_E = false;
    dragging_bool_F = false;
    dragging_bool_G = false;
    dragging_bool_H = false;
    dragging_bool_I = false;
    dragging_bool_J = false;
    dragging_equal = false; 
    dragging_greater_equal = false; 
    dragging_less_equal = false; 
    dragging_greater = false; 
    dragging_less = false; 
    dragging_unequal = false;
    dragging_add = false;
    dragging_div = false;
    dragging_exponent = false; 
    dragging_mult = false;
    dragging_sub = false;
}
void Interface :: check_dragging() {

    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i]->getType() == CONDITIONAL_BLOCK) {
                if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth())) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + blocks_list_to_print[i]->getHeight()))) {
                    //cout<<"soltou sobre o bloco certo"<<endl;
                    if(dragging_black_sensor1 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(BLACK_SENSOR_1);
                    } else if(dragging_black_sensor2 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(BLACK_SENSOR_2);
                    } else if(dragging_black_sensor3 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(BLACK_SENSOR_3);
                    } else if(dragging_black_sensor4 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(BLACK_SENSOR_4);
                    } else if(dragging_black_sensor5 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(BLACK_SENSOR_5);
                    } else if(dragging_color_sensor1 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(COLOR_SENSOR_1);
                    } else if(dragging_color_sensor2 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(COLOR_SENSOR_2);
                    } else if(dragging_ultrasonic_sensor1 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(ULTRASONIC_SENSOR_1);
                    } else if(dragging_ultrasonic_sensor2 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(ULTRASONIC_SENSOR_2);
                    } else if(dragging_ultrasonic_sensor3 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(ULTRASONIC_SENSOR_3);
                    } else if((dragging_walk_foward == true) || (dragging_turn_left == true) || (dragging_turn_right == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Essa ação não pode ser usado como entrada no bloco de decisão", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    }  else if((dragging_number_0 == true) || (dragging_number_1 == true) || (dragging_number_2 == true) || (dragging_number_3 == true) || (dragging_number_4 == true) || (dragging_number_5 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Um numero não pode ser usado como entrada no bloco de decisão", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_number_6 == true) || (dragging_number_7 == true) || (dragging_number_8 == true) || (dragging_number_9 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Um numero não pode ser usado como entrada no bloco de decisão", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    }
                }
            }
            if(blocks_list_to_print[i]->getType() == ACTION_BLOCK) {
                if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth())) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + blocks_list_to_print[i]->getHeight()))) {
                    //cout<<"soltou sobre o bloco certo"<<endl;
                    if((dragging_black_sensor1 == true) || (dragging_black_sensor2 == true) || (dragging_black_sensor3 == true)  || (dragging_black_sensor4 == true) || (dragging_black_sensor5 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "O sensor de fita preta não pode ser usado como entrada no bloco de função", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    }  else if((dragging_color_sensor1 == true) || (dragging_color_sensor2 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "O sensor de cor não pode ser usado como entrada no bloco de função", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_ultrasonic_sensor1 == true) || (dragging_ultrasonic_sensor2 == true) || (dragging_ultrasonic_sensor3 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "O sensor de ultrassom não pode ser usado como entrada no bloco de função", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_number_0 == true) || (dragging_number_1 == true) || (dragging_number_2 == true) || (dragging_number_3 == true) || (dragging_number_4 == true) || (dragging_number_5 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Um numero não pode ser usado como entrada no bloco de função", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_number_6 == true) || (dragging_number_7 == true) || (dragging_number_8 == true) || (dragging_number_9 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Um numero não pode ser usado como entrada no bloco de função", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if(dragging_walk_foward == true) {
                        blocks_list_to_print[i]->setFunction(MOVE_FORWARD);
                    } else if(dragging_turn_left == true) {
                        blocks_list_to_print[i]->setFunction(TURN_LEFT);
                    } else if(dragging_turn_right == true) {
                        blocks_list_to_print[i]->setFunction(TURN_RIGHT);
                    }
                }
            }
            if(blocks_list_to_print[i]->getType() == LOOP_BLOCK) {
                if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth())) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + blocks_list_to_print[i]->getHeight()))) {
                    //cout<<"soltou sobre o bloco certo"<<endl;
                    if((dragging_black_sensor1 == true) || (dragging_black_sensor2 == true) || (dragging_black_sensor3 == true)  || (dragging_black_sensor4 == true) || (dragging_black_sensor5 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "O sensor de fita preta não pode ser usado como entrada no bloco de loop", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    }  else if((dragging_color_sensor1 == true) || (dragging_color_sensor2 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "O sensor de cor não pode ser usado como entrada no bloco de loop", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_ultrasonic_sensor1 == true) || (dragging_ultrasonic_sensor2 == true) || (dragging_ultrasonic_sensor3 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "O sensor de ultrassom não pode ser usado como entrada no bloco de loop", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_walk_foward == true) || (dragging_turn_left == true) || (dragging_turn_right == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Uma ação não pode ser usado como entrada no bloco de loop", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if(dragging_number_0 == true) {
                        blocks_list_to_print[i]->setValue(0);
                    } else if(dragging_number_1 == true) {
                        blocks_list_to_print[i]->setValue(1);
                    } else if(dragging_number_2 == true) {
                        blocks_list_to_print[i]->setValue(2);
                    } else if(dragging_number_3 == true) {
                        blocks_list_to_print[i]->setValue(3);
                    } else if(dragging_number_4 == true) {
                        blocks_list_to_print[i]->setValue(4);
                    } else if(dragging_number_5 == true) {
                        blocks_list_to_print[i]->setValue(5);
                    } else if(dragging_number_6 == true) {
                        blocks_list_to_print[i]->setValue(6);
                    } else if(dragging_number_7 == true) {
                        blocks_list_to_print[i]->setValue(7);
                    } else if(dragging_number_8 == true) {
                        blocks_list_to_print[i]->setValue(8);
                    } else if(dragging_number_9 == true) {
                        blocks_list_to_print[i]->setValue(9);
                    }
                }
            }
            //v3
            if(blocks_list_to_print[i]->getType() == ATTRIBUTION_BLOCK){
                //PRIMEIRO SLOT ATTR
                if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() - (blocks_list_to_print[i]->getWidth()/2))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + blocks_list_to_print[i]->getHeight()))) {
                    //Apenas variaveis no primeiro slot do ATTRIBUTION_BLOCK
                    if((dragging_black_sensor1 == true) || (dragging_black_sensor2 == true) || (dragging_black_sensor3 == true)  || (dragging_black_sensor4 == true) || (dragging_black_sensor5 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_color_sensor1 == true) || (dragging_color_sensor2 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_ultrasonic_sensor1 == true) || (dragging_ultrasonic_sensor2 == true) || (dragging_ultrasonic_sensor3 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_number_0 == true) || (dragging_number_1 == true) || (dragging_number_2 == true) || (dragging_number_3 == true) || (dragging_number_4 == true) || (dragging_number_5 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_number_6 == true) || (dragging_number_7 == true) || (dragging_number_8 == true) || (dragging_number_9 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_walk_foward == true) || (dragging_turn_left == true) || (dragging_turn_right == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_equal == true) || (dragging_unequal == true) || (dragging_greater == true) || (dragging_greater_equal == true) || (dragging_less == true) || (dragging_less_equal == true)) {
                        //erro logic para o bloco de ATTRIBUTION
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_add == true) || (dragging_sub == true) || (dragging_mult == true) || (dragging_div == true) || (dragging_exponent == true)) {
                        //erro math para o bloco de ATTRIBUTION
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } 
                    //Variaveis INT
                    else if(dragging_int_A == true){
                        blocks_list_to_print[i]->setFirstSlotAttr(1, int_var_list[0], NULL);
                    } else if(dragging_int_B == true){
                        blocks_list_to_print[i]->setFirstSlotAttr(1, int_var_list[1], NULL);
                    } else if(dragging_int_C == true){
                        blocks_list_to_print[i]->setFirstSlotAttr(1, int_var_list[2], NULL);
                    } else if(dragging_int_D == true){
                        blocks_list_to_print[i]->setFirstSlotAttr(1, int_var_list[3], NULL);
                    } else if(dragging_int_E == true){
                        blocks_list_to_print[i]->setFirstSlotAttr(1, int_var_list[4], NULL);
                    } else if(dragging_int_F == true){
                        blocks_list_to_print[i]->setFirstSlotAttr(1, int_var_list[5], NULL);
                    } else if(dragging_int_G == true){
                        blocks_list_to_print[i]->setFirstSlotAttr(1, int_var_list[6], NULL);
                    } else if(dragging_int_H == true){
                        blocks_list_to_print[i]->setFirstSlotAttr(1, int_var_list[7], NULL);
                    } else if(dragging_int_I == true){
                        blocks_list_to_print[i]->setFirstSlotAttr(1, int_var_list[8], NULL);
                    } else if(dragging_int_J == true){
                        blocks_list_to_print[i]->setFirstSlotAttr(1, int_var_list[9], NULL);
                    } 
                    //Variaveis BOOL
                    else if(dragging_bool_A == true){
                        blocks_list_to_print[i]->setFirstSlotAttr(2, NULL, bool_var_list[0]);
                    } else if(dragging_bool_B == true){  
                        blocks_list_to_print[i]->setFirstSlotAttr(2, NULL, bool_var_list[1]);
                    } else if(dragging_bool_C == true){  
                        blocks_list_to_print[i]->setFirstSlotAttr(2, NULL, bool_var_list[2]);
                    } else if(dragging_bool_D == true){  
                        blocks_list_to_print[i]->setFirstSlotAttr(2, NULL, bool_var_list[3]);
                    } else if(dragging_bool_E == true){  
                        blocks_list_to_print[i]->setFirstSlotAttr(2, NULL, bool_var_list[4]);
                    } else if(dragging_bool_F == true){  
                        blocks_list_to_print[i]->setFirstSlotAttr(2, NULL, bool_var_list[5]);
                    } else if(dragging_bool_G == true){  
                        blocks_list_to_print[i]->setFirstSlotAttr(2, NULL, bool_var_list[6]);
                    } else if(dragging_bool_H == true){  
                        blocks_list_to_print[i]->setFirstSlotAttr(2, NULL, bool_var_list[7]);
                    } else if(dragging_bool_I == true){  
                        blocks_list_to_print[i]->setFirstSlotAttr(2, NULL, bool_var_list[8]);
                    } else if(dragging_bool_J == true){  
                        blocks_list_to_print[i]->setFirstSlotAttr(2, NULL, bool_var_list[9]);
                    }
                }
                //SEGUNDO SLOT ATTR
                if((mouseX > blocks_list_to_print[i]->getX() + (blocks_list_to_print[i]->getWidth()/2)) && (mouseX < (blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth())) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + blocks_list_to_print[i]->getHeight()))) {
                    //Apenas variaveis, valores, e leitura de sensores no segundo slot
                    if((dragging_walk_foward == true) || (dragging_turn_left == true) || (dragging_turn_right == true)) {
                        //erro para o bloco de ATTRIBUTION
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_equal == true) || (dragging_unequal == true) || (dragging_greater == true) || (dragging_greater_equal == true) || (dragging_less == true) || (dragging_less_equal == true)) {
                        //erro logic para o bloco de ATTRIBUTION
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_add == true) || (dragging_sub == true) || (dragging_mult == true) || (dragging_div == true) || (dragging_exponent == true)) {
                        //erro math para o bloco de ATTRIBUTION
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } 
                    //correto, implementar sensores
                    //valores dos sensores só podem ser atribuidos a uma variavel atraves do bloco de ATTR
                    else if((dragging_black_sensor1 == true) || (dragging_black_sensor2 == true) || (dragging_black_sensor3 == true)  || (dragging_black_sensor4 == true) || (dragging_black_sensor5 == true)){
                        //Correto para o Attribution Block para ambas as variaveis int e bool, implementar por ultimo
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Correto, porem não finalizado.", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_color_sensor1 == true) || (dragging_color_sensor2 == true)){
                        //Correto para o Attribution Block para ambas as variaveis int e bool, implementar por ultimo
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Correto, porem não finalizado.", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_ultrasonic_sensor1 == true) || (dragging_ultrasonic_sensor2 == true) || (dragging_ultrasonic_sensor3 == true)) {
                        //Correto para o Attribution Block para ambas as variaveis int e bool, implementar por ultimo
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Correto, porem não finalizado.", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    }/* else if((dragging_number_0 == true) || (dragging_number_1 == true) || (dragging_number_2 == true) || (dragging_number_3 == true) || (dragging_number_4 == true) || (dragging_number_5 == true)) {
                        //Correto para o Attribution Block para ambas as variaveis int e bool, implementar
                        
                    } else if((dragging_number_6 == true) || (dragging_number_7 == true) || (dragging_number_8 == true) || (dragging_number_9 == true)) {
                        //Correto para o Attribution Block para ambas as variaveis int e bool, implementar
                        
                    } else if((dragging_int_A == true) || (dragging_int_B == true) || (dragging_int_C == true) || (dragging_int_D == true) || (dragging_int_E == true)) {
                        //Correto, variavel int
                        
                    } else if((dragging_int_F == true) || (dragging_int_G == true) || (dragging_int_H == true) || (dragging_int_I == true) || (dragging_int_J == true)) {
                        //Correto, variavel int
                        
                    } else if((dragging_bool_A == true) || (dragging_bool_B == true) || (dragging_bool_C == true) || (dragging_bool_D == true) || (dragging_bool_E == true)) {
                        //Correto, variavel bool
                        
                    } else if((dragging_bool_F == true) || (dragging_bool_G == true) || (dragging_bool_H == true) || (dragging_bool_I == true) || (dragging_bool_J == true)) {
                        //Correto, variavel bool
                        
                    }*/ 
                    //Variaveis INT
                    else if(dragging_int_A == true){
                        blocks_list_to_print[i]->setSecondSlotAttr(1, int_var_list[0], NULL, 0);
                    } else if(dragging_int_B == true){
                        blocks_list_to_print[i]->setSecondSlotAttr(1, int_var_list[1], NULL, 0);
                    } else if(dragging_int_C == true){
                        blocks_list_to_print[i]->setSecondSlotAttr(1, int_var_list[2], NULL, 0);
                    } else if(dragging_int_D == true){
                        blocks_list_to_print[i]->setSecondSlotAttr(1, int_var_list[3], NULL, 0);
                    } else if(dragging_int_E == true){
                        blocks_list_to_print[i]->setSecondSlotAttr(1, int_var_list[4], NULL, 0);
                    } else if(dragging_int_F == true){
                        blocks_list_to_print[i]->setSecondSlotAttr(1, int_var_list[5], NULL, 0);
                    } else if(dragging_int_G == true){
                        blocks_list_to_print[i]->setSecondSlotAttr(1, int_var_list[6], NULL, 0);
                    } else if(dragging_int_H == true){
                        blocks_list_to_print[i]->setSecondSlotAttr(1, int_var_list[7], NULL, 0);
                    } else if(dragging_int_I == true){
                        blocks_list_to_print[i]->setSecondSlotAttr(1, int_var_list[8], NULL, 0);
                    } else if(dragging_int_J == true){
                        blocks_list_to_print[i]->setSecondSlotAttr(1, int_var_list[9], NULL, 0);
                    } 
                    //Variaveis BOOL
                    else if(dragging_bool_A == true){
                        blocks_list_to_print[i]->setSecondSlotAttr(2, NULL, bool_var_list[0], 0);
                    } else if(dragging_bool_B == true){  
                        blocks_list_to_print[i]->setSecondSlotAttr(2, NULL, bool_var_list[1], 0);
                    } else if(dragging_bool_C == true){  
                        blocks_list_to_print[i]->setSecondSlotAttr(2, NULL, bool_var_list[2], 0);
                    } else if(dragging_bool_D == true){  
                        blocks_list_to_print[i]->setSecondSlotAttr(2, NULL, bool_var_list[3], 0);
                    } else if(dragging_bool_E == true){  
                        blocks_list_to_print[i]->setSecondSlotAttr(2, NULL, bool_var_list[4], 0);
                    } else if(dragging_bool_F == true){  
                        blocks_list_to_print[i]->setSecondSlotAttr(2, NULL, bool_var_list[5], 0);
                    } else if(dragging_bool_G == true){  
                        blocks_list_to_print[i]->setSecondSlotAttr(2, NULL, bool_var_list[6], 0);
                    } else if(dragging_bool_H == true){  
                        blocks_list_to_print[i]->setSecondSlotAttr(2, NULL, bool_var_list[7], 0);
                    } else if(dragging_bool_I == true){  
                        blocks_list_to_print[i]->setSecondSlotAttr(2, NULL, bool_var_list[8], 0);
                    } else if(dragging_bool_J == true){  
                        blocks_list_to_print[i]->setSecondSlotAttr(2, NULL, bool_var_list[9], 0);
                    } 
                    //Valores estaticos
                    else if(dragging_number_0 == true) {
                        blocks_list_to_print[i]->setSecondSlotAttr(3, NULL, NULL, 0);
                    } else if(dragging_number_1 == true) {
                        blocks_list_to_print[i]->setSecondSlotAttr(3, NULL, NULL, 1);
                    } else if(dragging_number_2 == true) {
                        blocks_list_to_print[i]->setSecondSlotAttr(3, NULL, NULL, 2);
                    } else if(dragging_number_3 == true) {
                        blocks_list_to_print[i]->setSecondSlotAttr(3, NULL, NULL, 3);
                    } else if(dragging_number_4 == true) {
                        blocks_list_to_print[i]->setSecondSlotAttr(3, NULL, NULL, 4);
                    } else if(dragging_number_5 == true) {
                        blocks_list_to_print[i]->setSecondSlotAttr(3, NULL, NULL, 5);
                    } else if(dragging_number_6 == true) {
                        blocks_list_to_print[i]->setSecondSlotAttr(3, NULL, NULL, 6);
                    } else if(dragging_number_7 == true) {
                        blocks_list_to_print[i]->setSecondSlotAttr(3, NULL, NULL, 7);
                    } else if(dragging_number_8 == true) {
                        blocks_list_to_print[i]->setSecondSlotAttr(3, NULL, NULL, 8);
                    } else if(dragging_number_9 == true) {
                        blocks_list_to_print[i]->setSecondSlotAttr(3, NULL, NULL, 9);
                    }
                }
            }
            if(blocks_list_to_print[i]->getType() == MATH_BLOCK){
                //PRIMEIRO SLOT MATH
                if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + (1 * blocks_list_to_print[i]->getWidth()/5))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + blocks_list_to_print[i]->getHeight()))) {
                    //Apenas variaveis no primeiro slot do MATH_BLOCK
                    if((dragging_black_sensor1 == true) || (dragging_black_sensor2 == true) || (dragging_black_sensor3 == true)  || (dragging_black_sensor4 == true) || (dragging_black_sensor5 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_color_sensor1 == true) || (dragging_color_sensor2 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_ultrasonic_sensor1 == true) || (dragging_ultrasonic_sensor2 == true) || (dragging_ultrasonic_sensor3 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_number_0 == true) || (dragging_number_1 == true) || (dragging_number_2 == true) || (dragging_number_3 == true) || (dragging_number_4 == true) || (dragging_number_5 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_number_6 == true) || (dragging_number_7 == true) || (dragging_number_8 == true) || (dragging_number_9 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_walk_foward == true) || (dragging_turn_left == true) || (dragging_turn_right == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_equal == true) || (dragging_unequal == true) || (dragging_greater == true) || (dragging_greater_equal == true) || (dragging_less == true) || (dragging_less_equal == true)) {
                        //erro logic para o bloco de MATH
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_add == true) || (dragging_sub == true) || (dragging_mult == true) || (dragging_div == true) || (dragging_exponent == true)) {
                        //erro math para o bloco de MATH
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    //Variaveis INT
                    else if(dragging_int_A == true){
                        blocks_list_to_print[i]->setFirstSlotMath(1, int_var_list[0], NULL);
                    } else if(dragging_int_B == true){
                        blocks_list_to_print[i]->setFirstSlotMath(1, int_var_list[1], NULL);
                    } else if(dragging_int_C == true){
                        blocks_list_to_print[i]->setFirstSlotMath(1, int_var_list[2], NULL);
                    } else if(dragging_int_D == true){
                        blocks_list_to_print[i]->setFirstSlotMath(1, int_var_list[3], NULL);
                    } else if(dragging_int_E == true){
                        blocks_list_to_print[i]->setFirstSlotMath(1, int_var_list[4], NULL);
                    } else if(dragging_int_F == true){
                        blocks_list_to_print[i]->setFirstSlotMath(1, int_var_list[5], NULL);
                    } else if(dragging_int_G == true){
                        blocks_list_to_print[i]->setFirstSlotMath(1, int_var_list[6], NULL);
                    } else if(dragging_int_H == true){
                        blocks_list_to_print[i]->setFirstSlotMath(1, int_var_list[7], NULL);
                    } else if(dragging_int_I == true){
                        blocks_list_to_print[i]->setFirstSlotMath(1, int_var_list[8], NULL);
                    } else if(dragging_int_J == true){
                        blocks_list_to_print[i]->setFirstSlotMath(1, int_var_list[9], NULL);
                    } 
                    //Variaveis BOOL
                    else if(dragging_bool_A == true){
                        blocks_list_to_print[i]->setFirstSlotMath(2, NULL, bool_var_list[0]);
                    } else if(dragging_bool_B == true){  
                        blocks_list_to_print[i]->setFirstSlotMath(2, NULL, bool_var_list[1]);
                    } else if(dragging_bool_C == true){  
                        blocks_list_to_print[i]->setFirstSlotMath(2, NULL, bool_var_list[2]);
                    } else if(dragging_bool_D == true){  
                        blocks_list_to_print[i]->setFirstSlotMath(2, NULL, bool_var_list[3]);
                    } else if(dragging_bool_E == true){  
                        blocks_list_to_print[i]->setFirstSlotMath(2, NULL, bool_var_list[4]);
                    } else if(dragging_bool_F == true){  
                        blocks_list_to_print[i]->setFirstSlotMath(2, NULL, bool_var_list[5]);
                    } else if(dragging_bool_G == true){  
                        blocks_list_to_print[i]->setFirstSlotMath(2, NULL, bool_var_list[6]);
                    } else if(dragging_bool_H == true){  
                        blocks_list_to_print[i]->setFirstSlotMath(2, NULL, bool_var_list[7]);
                    } else if(dragging_bool_I == true){  
                        blocks_list_to_print[i]->setFirstSlotMath(2, NULL, bool_var_list[8]);
                    } else if(dragging_bool_J == true){  
                        blocks_list_to_print[i]->setFirstSlotMath(2, NULL, bool_var_list[9]);
                    }
                }
                //SEGUNDO SLOT MATH
                if((mouseX > blocks_list_to_print[i]->getX() + (2 * blocks_list_to_print[i]->getWidth()/5)) && (mouseX < (blocks_list_to_print[i]->getX() + (3 * blocks_list_to_print[i]->getWidth()/5))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + blocks_list_to_print[i]->getHeight()))) {
                    //Apenas variaveis e valores estaticos no slot do MATH_BLOCK
                    if((dragging_walk_foward == true) || (dragging_turn_left == true) || (dragging_turn_right == true)) {
                        //erro comando no MATH_BLOCK
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_equal == true) || (dragging_unequal == true) || (dragging_greater == true) || (dragging_greater_equal == true) || (dragging_less == true) || (dragging_less_equal == true)) {
                        //erro operando no slot incorreto
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_add == true) || (dragging_sub == true) || (dragging_mult == true) || (dragging_div == true) || (dragging_exponent == true)) {
                        //erro operando no slot incorreto
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } 
                    //MATH_BLOCK nao lida com sensores
                    else if((dragging_black_sensor1 == true) || (dragging_black_sensor2 == true) || (dragging_black_sensor3 == true)  || (dragging_black_sensor4 == true) || (dragging_black_sensor5 == true)){
                        //erro sensor no MATH_BLOCK
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_color_sensor1 == true) || (dragging_color_sensor2 == true)){
                        //erro sensor no MATH_BLOCK
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_ultrasonic_sensor1 == true) || (dragging_ultrasonic_sensor2 == true) || (dragging_ultrasonic_sensor3 == true)) {
                        //erro sensor no MATH_BLOCK
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } 
                    //Variaveis INT
                    else if(dragging_int_A == true){
                        blocks_list_to_print[i]->setSecondSlotMath(1, int_var_list[0], NULL, 0);
                    } else if(dragging_int_B == true){
                        blocks_list_to_print[i]->setSecondSlotMath(1, int_var_list[1], NULL, 0);
                    } else if(dragging_int_C == true){
                        blocks_list_to_print[i]->setSecondSlotMath(1, int_var_list[2], NULL, 0);
                    } else if(dragging_int_D == true){
                        blocks_list_to_print[i]->setSecondSlotMath(1, int_var_list[3], NULL, 0);
                    } else if(dragging_int_E == true){
                        blocks_list_to_print[i]->setSecondSlotMath(1, int_var_list[4], NULL, 0);
                    } else if(dragging_int_F == true){
                        blocks_list_to_print[i]->setSecondSlotMath(1, int_var_list[5], NULL, 0);
                    } else if(dragging_int_G == true){
                        blocks_list_to_print[i]->setSecondSlotMath(1, int_var_list[6], NULL, 0);
                    } else if(dragging_int_H == true){
                        blocks_list_to_print[i]->setSecondSlotMath(1, int_var_list[7], NULL, 0);
                    } else if(dragging_int_I == true){
                        blocks_list_to_print[i]->setSecondSlotMath(1, int_var_list[8], NULL, 0);
                    } else if(dragging_int_J == true){
                        blocks_list_to_print[i]->setSecondSlotMath(1, int_var_list[9], NULL, 0);
                    } 
                    //Variaveis BOOL
                    else if(dragging_bool_A == true){
                        blocks_list_to_print[i]->setSecondSlotMath(2, NULL, bool_var_list[0], 0);
                    } else if(dragging_bool_B == true){  
                        blocks_list_to_print[i]->setSecondSlotMath(2, NULL, bool_var_list[1], 0);
                    } else if(dragging_bool_C == true){  
                        blocks_list_to_print[i]->setSecondSlotMath(2, NULL, bool_var_list[2], 0);
                    } else if(dragging_bool_D == true){  
                        blocks_list_to_print[i]->setSecondSlotMath(2, NULL, bool_var_list[3], 0);
                    } else if(dragging_bool_E == true){  
                        blocks_list_to_print[i]->setSecondSlotMath(2, NULL, bool_var_list[4], 0);
                    } else if(dragging_bool_F == true){  
                        blocks_list_to_print[i]->setSecondSlotMath(2, NULL, bool_var_list[5], 0);
                    } else if(dragging_bool_G == true){  
                        blocks_list_to_print[i]->setSecondSlotMath(2, NULL, bool_var_list[6], 0);
                    } else if(dragging_bool_H == true){  
                        blocks_list_to_print[i]->setSecondSlotMath(2, NULL, bool_var_list[7], 0);
                    } else if(dragging_bool_I == true){  
                        blocks_list_to_print[i]->setSecondSlotMath(2, NULL, bool_var_list[8], 0);
                    } else if(dragging_bool_J == true){  
                        blocks_list_to_print[i]->setSecondSlotMath(2, NULL, bool_var_list[9], 0);
                    } 
                    //Valores estaticos
                    else if(dragging_number_0 == true) {
                        blocks_list_to_print[i]->setSecondSlotMath(3, NULL, NULL, 0);
                    } else if(dragging_number_1 == true) {
                        blocks_list_to_print[i]->setSecondSlotMath(3, NULL, NULL, 1);
                    } else if(dragging_number_2 == true) {
                        blocks_list_to_print[i]->setSecondSlotMath(3, NULL, NULL, 2);
                    } else if(dragging_number_3 == true) {
                        blocks_list_to_print[i]->setSecondSlotMath(3, NULL, NULL, 3);
                    } else if(dragging_number_4 == true) {
                        blocks_list_to_print[i]->setSecondSlotMath(3, NULL, NULL, 4);
                    } else if(dragging_number_5 == true) {
                        blocks_list_to_print[i]->setSecondSlotMath(3, NULL, NULL, 5);
                    } else if(dragging_number_6 == true) {
                        blocks_list_to_print[i]->setSecondSlotMath(3, NULL, NULL, 6);
                    } else if(dragging_number_7 == true) {
                        blocks_list_to_print[i]->setSecondSlotMath(3, NULL, NULL, 7);
                    } else if(dragging_number_8 == true) {
                        blocks_list_to_print[i]->setSecondSlotMath(3, NULL, NULL, 8);
                    } else if(dragging_number_9 == true) {
                        blocks_list_to_print[i]->setSecondSlotMath(3, NULL, NULL, 9);
                    }
                }
                //TERCEIRO SLOT MATH
                if((mouseX > blocks_list_to_print[i]->getX() + (3 * blocks_list_to_print[i]->getWidth()/5)) && (mouseX < (blocks_list_to_print[i]->getX() + (4 * blocks_list_to_print[i]->getWidth()/5))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + blocks_list_to_print[i]->getHeight()))) {
                    //Apenas operandos no slot do MATH_BLOCK
                    if((dragging_black_sensor1 == true) || (dragging_black_sensor2 == true) || (dragging_black_sensor3 == true)  || (dragging_black_sensor4 == true) || (dragging_black_sensor5 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_color_sensor1 == true) || (dragging_color_sensor2 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_ultrasonic_sensor1 == true) || (dragging_ultrasonic_sensor2 == true) || (dragging_ultrasonic_sensor3 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_number_0 == true) || (dragging_number_1 == true) || (dragging_number_2 == true) || (dragging_number_3 == true) || (dragging_number_4 == true) || (dragging_number_5 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_number_6 == true) || (dragging_number_7 == true) || (dragging_number_8 == true) || (dragging_number_9 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_walk_foward == true) || (dragging_turn_left == true) || (dragging_turn_right == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } 
                    //incorreto, variaveis no slot de operandos
                    else if((dragging_int_A == true) || (dragging_int_B == true) || (dragging_int_C == true) || (dragging_int_D == true) || (dragging_int_E == true)) {
                        //incorreto, variaveis no slot de operandos
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_int_F == true) || (dragging_int_G == true) || (dragging_int_H == true) || (dragging_int_I == true) || (dragging_int_J == true)) {
                        //incorreto, variaveis no slot de operandos
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_bool_A == true) || (dragging_bool_B == true) || (dragging_bool_C == true) || (dragging_bool_D == true) || (dragging_bool_E == true)) {
                        //incorreto, variaveis no slot de operandos
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_bool_F == true) || (dragging_bool_G == true) || (dragging_bool_H == true) || (dragging_bool_I == true) || (dragging_bool_J == true)) {
                        //incorreto, variaveis no slot de operandos
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } 
                    //Operadores matematicos
                    else if(dragging_add == true){
                        blocks_list_to_print[i]->setThirdSlotMath(4, 0);
                    } else if(dragging_sub == true){
                        blocks_list_to_print[i]->setThirdSlotMath(4, 1);
                    } else if(dragging_mult == true){
                        blocks_list_to_print[i]->setThirdSlotMath(4, 2);
                    } else if(dragging_div == true){
                        blocks_list_to_print[i]->setThirdSlotMath(4, 3);
                    } else if(dragging_exponent == true){
                        blocks_list_to_print[i]->setThirdSlotMath(4, 4);
                    }
                    //Operadores logicos 
                    else if(dragging_equal == true){
                        blocks_list_to_print[i]->setThirdSlotMath(5, 0);
                    } else if(dragging_unequal == true){
                        blocks_list_to_print[i]->setThirdSlotMath(5, 1);
                    } else if(dragging_greater == true){
                        blocks_list_to_print[i]->setThirdSlotMath(5, 2);
                    } else if(dragging_greater_equal == true){
                        blocks_list_to_print[i]->setThirdSlotMath(5, 3);
                    } else if(dragging_less == true){
                        blocks_list_to_print[i]->setThirdSlotMath(5, 4);
                    } else if(dragging_less_equal == true){
                        blocks_list_to_print[i]->setThirdSlotMath(5, 5);
                    } 
                }
                //QUARTO SLOT MATH
                if((mouseX > blocks_list_to_print[i]->getX() + (4 * blocks_list_to_print[i]->getWidth()/5)) && (mouseX < (blocks_list_to_print[i]->getX() + (5 * blocks_list_to_print[i]->getWidth()/5))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + blocks_list_to_print[i]->getHeight()))) {
                    //Apenas variaveis e valores estaticos no slot do MATH_BLOCK
                    if((dragging_walk_foward == true) || (dragging_turn_left == true) || (dragging_turn_right == true)) {
                        //erro comando no MATH_BLOCK
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_equal == true) || (dragging_unequal == true) || (dragging_greater == true) || (dragging_greater_equal == true) || (dragging_less == true) || (dragging_less_equal == true)) {
                        //erro operando no slot incorreto
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_add == true) || (dragging_sub == true) || (dragging_mult == true) || (dragging_div == true) || (dragging_exponent == true)) {
                        //erro operando no slot incorreto
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } 
                    //MATH_BLOCK nao lida com sensores
                    else if((dragging_black_sensor1 == true) || (dragging_black_sensor2 == true) || (dragging_black_sensor3 == true)  || (dragging_black_sensor4 == true) || (dragging_black_sensor5 == true)){
                        //erro sensor no MATH_BLOCK
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_color_sensor1 == true) || (dragging_color_sensor2 == true)){
                        //erro sensor no MATH_BLOCK
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_ultrasonic_sensor1 == true) || (dragging_ultrasonic_sensor2 == true) || (dragging_ultrasonic_sensor3 == true)) {
                        //erro sensor no MATH_BLOCK
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } 
                    //Variaveis INT
                    else if(dragging_int_A == true){
                        blocks_list_to_print[i]->setFourthSlotMath(1, int_var_list[0], NULL, 0);
                    } else if(dragging_int_B == true){
                        blocks_list_to_print[i]->setFourthSlotMath(1, int_var_list[1], NULL, 0);
                    } else if(dragging_int_C == true){
                        blocks_list_to_print[i]->setFourthSlotMath(1, int_var_list[2], NULL, 0);
                    } else if(dragging_int_D == true){
                        blocks_list_to_print[i]->setFourthSlotMath(1, int_var_list[3], NULL, 0);
                    } else if(dragging_int_E == true){
                        blocks_list_to_print[i]->setFourthSlotMath(1, int_var_list[4], NULL, 0);
                    } else if(dragging_int_F == true){
                        blocks_list_to_print[i]->setFourthSlotMath(1, int_var_list[5], NULL, 0);
                    } else if(dragging_int_G == true){
                        blocks_list_to_print[i]->setFourthSlotMath(1, int_var_list[6], NULL, 0);
                    } else if(dragging_int_H == true){
                        blocks_list_to_print[i]->setFourthSlotMath(1, int_var_list[7], NULL, 0);
                    } else if(dragging_int_I == true){
                        blocks_list_to_print[i]->setFourthSlotMath(1, int_var_list[8], NULL, 0);
                    } else if(dragging_int_J == true){
                        blocks_list_to_print[i]->setFourthSlotMath(1, int_var_list[9], NULL, 0);
                    } 
                    //Variaveis BOOL
                    else if(dragging_bool_A == true){
                        blocks_list_to_print[i]->setFourthSlotMath(2, NULL, bool_var_list[0], 0);
                    } else if(dragging_bool_B == true){  
                        blocks_list_to_print[i]->setFourthSlotMath(2, NULL, bool_var_list[1], 0);
                    } else if(dragging_bool_C == true){  
                        blocks_list_to_print[i]->setFourthSlotMath(2, NULL, bool_var_list[2], 0);
                    } else if(dragging_bool_D == true){  
                        blocks_list_to_print[i]->setFourthSlotMath(2, NULL, bool_var_list[3], 0);
                    } else if(dragging_bool_E == true){  
                        blocks_list_to_print[i]->setFourthSlotMath(2, NULL, bool_var_list[4], 0);
                    } else if(dragging_bool_F == true){  
                        blocks_list_to_print[i]->setFourthSlotMath(2, NULL, bool_var_list[5], 0);
                    } else if(dragging_bool_G == true){  
                        blocks_list_to_print[i]->setFourthSlotMath(2, NULL, bool_var_list[6], 0);
                    } else if(dragging_bool_H == true){  
                        blocks_list_to_print[i]->setFourthSlotMath(2, NULL, bool_var_list[7], 0);
                    } else if(dragging_bool_I == true){  
                        blocks_list_to_print[i]->setFourthSlotMath(2, NULL, bool_var_list[8], 0);
                    } else if(dragging_bool_J == true){  
                        blocks_list_to_print[i]->setFourthSlotMath(2, NULL, bool_var_list[9], 0);
                    } else if(dragging_number_0 == true) {
                        //erro, ZERO
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Evite usar o Zero neste Slot.", "Hmm..", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if(dragging_number_1 == true) {
                        blocks_list_to_print[i]->setFourthSlotMath(3, NULL, NULL, 1);
                    } else if(dragging_number_2 == true) {
                        blocks_list_to_print[i]->setFourthSlotMath(3, NULL, NULL, 2);
                    } else if(dragging_number_3 == true) {
                        blocks_list_to_print[i]->setFourthSlotMath(3, NULL, NULL, 3);
                    } else if(dragging_number_4 == true) {
                        blocks_list_to_print[i]->setFourthSlotMath(3, NULL, NULL, 4);
                    } else if(dragging_number_5 == true) {
                        blocks_list_to_print[i]->setFourthSlotMath(3, NULL, NULL, 5);
                    } else if(dragging_number_6 == true) {
                        blocks_list_to_print[i]->setFourthSlotMath(3, NULL, NULL, 6);
                    } else if(dragging_number_7 == true) {
                        blocks_list_to_print[i]->setFourthSlotMath(3, NULL, NULL, 7);
                    } else if(dragging_number_8 == true) {
                        blocks_list_to_print[i]->setFourthSlotMath(3, NULL, NULL, 8);
                    } else if(dragging_number_9 == true) {
                        blocks_list_to_print[i]->setFourthSlotMath(3, NULL, NULL, 9);
                    }
                }
            }
            if(blocks_list_to_print[i]->getType() == NEW_CONDITIONAL_BLOCK){
                //PRIMEIRO SLOT CONDITIONAL
                if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + (1 * blocks_list_to_print[i]->getWidth()/3))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + blocks_list_to_print[i]->getHeight()))) {
                    //Apenas variaveis no primeiro slot do NEW_CONDITIONAL_BLOCK
                    if((dragging_black_sensor1 == true) || (dragging_black_sensor2 == true) || (dragging_black_sensor3 == true)  || (dragging_black_sensor4 == true) || (dragging_black_sensor5 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_color_sensor1 == true) || (dragging_color_sensor2 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_ultrasonic_sensor1 == true) || (dragging_ultrasonic_sensor2 == true) || (dragging_ultrasonic_sensor3 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_number_0 == true) || (dragging_number_1 == true) || (dragging_number_2 == true) || (dragging_number_3 == true) || (dragging_number_4 == true) || (dragging_number_5 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_number_6 == true) || (dragging_number_7 == true) || (dragging_number_8 == true) || (dragging_number_9 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_walk_foward == true) || (dragging_turn_left == true) || (dragging_turn_right == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_equal == true) || (dragging_unequal == true) || (dragging_greater == true) || (dragging_greater_equal == true) || (dragging_less == true) || (dragging_less_equal == true)) {
                        //erro logic para o bloco de CONDITIONAL
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_add == true) || (dragging_sub == true) || (dragging_mult == true) || (dragging_div == true) || (dragging_exponent == true)) {
                        //erro math para o bloco de CONDITIONAL
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } 
                    //Variaveis INT
                    else if(dragging_int_A == true){
                        blocks_list_to_print[i]->setFirstSlotConditional(1, int_var_list[0], NULL);
                    } else if(dragging_int_B == true){
                        blocks_list_to_print[i]->setFirstSlotConditional(1, int_var_list[1], NULL);
                    } else if(dragging_int_C == true){
                        blocks_list_to_print[i]->setFirstSlotConditional(1, int_var_list[2], NULL);
                    } else if(dragging_int_D == true){
                        blocks_list_to_print[i]->setFirstSlotConditional(1, int_var_list[3], NULL);
                    } else if(dragging_int_E == true){
                        blocks_list_to_print[i]->setFirstSlotConditional(1, int_var_list[4], NULL);
                    } else if(dragging_int_F == true){
                        blocks_list_to_print[i]->setFirstSlotConditional(1, int_var_list[5], NULL);
                    } else if(dragging_int_G == true){
                        blocks_list_to_print[i]->setFirstSlotConditional(1, int_var_list[6], NULL);
                    } else if(dragging_int_H == true){
                        blocks_list_to_print[i]->setFirstSlotConditional(1, int_var_list[7], NULL);
                    } else if(dragging_int_I == true){
                        blocks_list_to_print[i]->setFirstSlotConditional(1, int_var_list[8], NULL);
                    } else if(dragging_int_J == true){
                        blocks_list_to_print[i]->setFirstSlotConditional(1, int_var_list[9], NULL);
                    } 
                    //Variaveis BOOL
                    else if(dragging_bool_A == true){
                        blocks_list_to_print[i]->setFirstSlotConditional(2, NULL, bool_var_list[0]);
                    } else if(dragging_bool_B == true){  
                        blocks_list_to_print[i]->setFirstSlotConditional(2, NULL, bool_var_list[1]);
                    } else if(dragging_bool_C == true){  
                        blocks_list_to_print[i]->setFirstSlotConditional(2, NULL, bool_var_list[2]);
                    } else if(dragging_bool_D == true){  
                        blocks_list_to_print[i]->setFirstSlotConditional(2, NULL, bool_var_list[3]);
                    } else if(dragging_bool_E == true){  
                        blocks_list_to_print[i]->setFirstSlotConditional(2, NULL, bool_var_list[4]);
                    } else if(dragging_bool_F == true){  
                        blocks_list_to_print[i]->setFirstSlotConditional(2, NULL, bool_var_list[5]);
                    } else if(dragging_bool_G == true){  
                        blocks_list_to_print[i]->setFirstSlotConditional(2, NULL, bool_var_list[6]);
                    } else if(dragging_bool_H == true){  
                        blocks_list_to_print[i]->setFirstSlotConditional(2, NULL, bool_var_list[7]);
                    } else if(dragging_bool_I == true){  
                        blocks_list_to_print[i]->setFirstSlotConditional(2, NULL, bool_var_list[8]);
                    } else if(dragging_bool_J == true){  
                        blocks_list_to_print[i]->setFirstSlotConditional(2, NULL, bool_var_list[9]);
                    }
                }
                //SEGUNDO SLOT CONDITIONAL
                if((mouseX > blocks_list_to_print[i]->getX() + (1 * blocks_list_to_print[i]->getWidth()/3)) && (mouseX < (blocks_list_to_print[i]->getX() + (2 * blocks_list_to_print[i]->getWidth()/3))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + blocks_list_to_print[i]->getHeight()))) {
                    //Apenas operandos no slot do NEW_CONDITIONAL_BLOCK
                    if((dragging_black_sensor1 == true) || (dragging_black_sensor2 == true) || (dragging_black_sensor3 == true)  || (dragging_black_sensor4 == true) || (dragging_black_sensor5 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_color_sensor1 == true) || (dragging_color_sensor2 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_ultrasonic_sensor1 == true) || (dragging_ultrasonic_sensor2 == true) || (dragging_ultrasonic_sensor3 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_number_0 == true) || (dragging_number_1 == true) || (dragging_number_2 == true) || (dragging_number_3 == true) || (dragging_number_4 == true) || (dragging_number_5 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_number_6 == true) || (dragging_number_7 == true) || (dragging_number_8 == true) || (dragging_number_9 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_walk_foward == true) || (dragging_turn_left == true) || (dragging_turn_right == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } 
                    //incorreto, variaveis no slot de operandos
                    else if((dragging_int_A == true) || (dragging_int_B == true) || (dragging_int_C == true) || (dragging_int_D == true) || (dragging_int_E == true)) {
                        //incorreto, variaveis no slot de operandos
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_int_F == true) || (dragging_int_G == true) || (dragging_int_H == true) || (dragging_int_I == true) || (dragging_int_J == true)) {
                        //incorreto, variaveis no slot de operandos
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_bool_A == true) || (dragging_bool_B == true) || (dragging_bool_C == true) || (dragging_bool_D == true) || (dragging_bool_E == true)) {
                        //incorreto, variaveis no slot de operandos
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_bool_F == true) || (dragging_bool_G == true) || (dragging_bool_H == true) || (dragging_bool_I == true) || (dragging_bool_J == true)) {
                        //incorreto, variaveis no slot de operandos
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_add == true) || (dragging_sub == true) || (dragging_mult == true) || (dragging_div == true) || (dragging_exponent == true)) {
                        //erro math para o bloco de CONDITIONAL
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } 
                    //Operadores logicos 
                    else if(dragging_equal == true){
                        blocks_list_to_print[i]->setSecondSlotConditional(0);
                    } else if(dragging_unequal == true){
                        blocks_list_to_print[i]->setSecondSlotConditional(1);
                    } else if(dragging_greater == true){
                        blocks_list_to_print[i]->setSecondSlotConditional(2);
                    } else if(dragging_greater_equal == true){
                        blocks_list_to_print[i]->setSecondSlotConditional(3);
                    } else if(dragging_less == true){
                        blocks_list_to_print[i]->setSecondSlotConditional(4);
                    } else if(dragging_less_equal == true){
                        blocks_list_to_print[i]->setSecondSlotConditional(5);
                    } 
                }
                //TERCEIRO SLOT CONDITIONAL
                if((mouseX > blocks_list_to_print[i]->getX() + (2 * blocks_list_to_print[i]->getWidth()/3)) && (mouseX < (blocks_list_to_print[i]->getX() + (3 * blocks_list_to_print[i]->getWidth()/3))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + blocks_list_to_print[i]->getHeight()))) {
                    //Apenas variaveis e valores estaticos no slot do NEW_CONDITIONAL_BLOCK
                    if((dragging_walk_foward == true) || (dragging_turn_left == true) || (dragging_turn_right == true)) {
                        //erro comando no NEW_CONDITIONAL_BLOCK
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_equal == true) || (dragging_unequal == true) || (dragging_greater == true) || (dragging_greater_equal == true) || (dragging_less == true) || (dragging_less_equal == true)) {
                        //erro operando no slot incorreto
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_add == true) || (dragging_sub == true) || (dragging_mult == true) || (dragging_div == true) || (dragging_exponent == true)) {
                        //erro operando no slot incorreto
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } 
                    //CONDITIONAL_BLOCK nao lida com sensores
                    else if((dragging_black_sensor1 == true) || (dragging_black_sensor2 == true) || (dragging_black_sensor3 == true)  || (dragging_black_sensor4 == true) || (dragging_black_sensor5 == true)){
                        //erro sensor no CONDITIONAL_BLOCK
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_color_sensor1 == true) || (dragging_color_sensor2 == true)){
                        //erro sensor no CONDITIONAL_BLOCK
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_ultrasonic_sensor1 == true) || (dragging_ultrasonic_sensor2 == true) || (dragging_ultrasonic_sensor3 == true)) {
                        //erro sensor no CONDITIONAL_BLOCK
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } 
                    //Variaveis INT
                    else if(dragging_int_A == true){
                        blocks_list_to_print[i]->setThirdSlotConditional(1, int_var_list[0], NULL, 0);
                    } else if(dragging_int_B == true){
                        blocks_list_to_print[i]->setThirdSlotConditional(1, int_var_list[1], NULL, 0);
                    } else if(dragging_int_C == true){
                        blocks_list_to_print[i]->setThirdSlotConditional(1, int_var_list[2], NULL, 0);
                    } else if(dragging_int_D == true){
                        blocks_list_to_print[i]->setThirdSlotConditional(1, int_var_list[3], NULL, 0);
                    } else if(dragging_int_E == true){
                        blocks_list_to_print[i]->setThirdSlotConditional(1, int_var_list[4], NULL, 0);
                    } else if(dragging_int_F == true){
                        blocks_list_to_print[i]->setThirdSlotConditional(1, int_var_list[5], NULL, 0);
                    } else if(dragging_int_G == true){
                        blocks_list_to_print[i]->setThirdSlotConditional(1, int_var_list[6], NULL, 0);
                    } else if(dragging_int_H == true){
                        blocks_list_to_print[i]->setThirdSlotConditional(1, int_var_list[7], NULL, 0);
                    } else if(dragging_int_I == true){
                        blocks_list_to_print[i]->setThirdSlotConditional(1, int_var_list[8], NULL, 0);
                    } else if(dragging_int_J == true){
                        blocks_list_to_print[i]->setThirdSlotConditional(1, int_var_list[9], NULL, 0);
                    } 
                    //Variaveis BOOL
                    else if(dragging_bool_A == true){
                        blocks_list_to_print[i]->setThirdSlotConditional(2, NULL, bool_var_list[0], 0);
                    } else if(dragging_bool_B == true){  
                        blocks_list_to_print[i]->setThirdSlotConditional(2, NULL, bool_var_list[1], 0);
                    } else if(dragging_bool_C == true){  
                        blocks_list_to_print[i]->setThirdSlotConditional(2, NULL, bool_var_list[2], 0);
                    } else if(dragging_bool_D == true){  
                        blocks_list_to_print[i]->setThirdSlotConditional(2, NULL, bool_var_list[3], 0);
                    } else if(dragging_bool_E == true){  
                        blocks_list_to_print[i]->setThirdSlotConditional(2, NULL, bool_var_list[4], 0);
                    } else if(dragging_bool_F == true){  
                        blocks_list_to_print[i]->setThirdSlotConditional(2, NULL, bool_var_list[5], 0);
                    } else if(dragging_bool_G == true){  
                        blocks_list_to_print[i]->setThirdSlotConditional(2, NULL, bool_var_list[6], 0);
                    } else if(dragging_bool_H == true){  
                        blocks_list_to_print[i]->setThirdSlotConditional(2, NULL, bool_var_list[7], 0);
                    } else if(dragging_bool_I == true){  
                        blocks_list_to_print[i]->setThirdSlotConditional(2, NULL, bool_var_list[8], 0);
                    } else if(dragging_bool_J == true){  
                        blocks_list_to_print[i]->setThirdSlotConditional(2, NULL, bool_var_list[9], 0);
                    } else if(dragging_number_0 == true) {
                        //erro, ZERO
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Evite usar o Zero neste Slot.", "Hmm..", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if(dragging_number_1 == true) {
                        blocks_list_to_print[i]->setThirdSlotConditional(3, NULL, NULL, 1);
                    } else if(dragging_number_2 == true) {
                        blocks_list_to_print[i]->setThirdSlotConditional(3, NULL, NULL, 2);
                    } else if(dragging_number_3 == true) {
                        blocks_list_to_print[i]->setThirdSlotConditional(3, NULL, NULL, 3);
                    } else if(dragging_number_4 == true) {
                        blocks_list_to_print[i]->setThirdSlotConditional(3, NULL, NULL, 4);
                    } else if(dragging_number_5 == true) {
                        blocks_list_to_print[i]->setThirdSlotConditional(3, NULL, NULL, 5);
                    } else if(dragging_number_6 == true) {
                        blocks_list_to_print[i]->setThirdSlotConditional(3, NULL, NULL, 6);
                    } else if(dragging_number_7 == true) {
                        blocks_list_to_print[i]->setThirdSlotConditional(3, NULL, NULL, 7);
                    } else if(dragging_number_8 == true) {
                        blocks_list_to_print[i]->setThirdSlotConditional(3, NULL, NULL, 8);
                    } else if(dragging_number_9 == true) {
                        blocks_list_to_print[i]->setThirdSlotConditional(3, NULL, NULL, 9);
                    }
                }
            }
            if(blocks_list_to_print[i]->getType() == WHILE_BLOCK){
                //PRIMEIRO SLOT WHILE
                if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + (1 * blocks_list_to_print[i]->getWidth()/3))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + blocks_list_to_print[i]->getHeight()))) {
                    //Apenas variaveis no primeiro slot do WHILE_BLOCK
                    if((dragging_black_sensor1 == true) || (dragging_black_sensor2 == true) || (dragging_black_sensor3 == true)  || (dragging_black_sensor4 == true) || (dragging_black_sensor5 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_color_sensor1 == true) || (dragging_color_sensor2 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_ultrasonic_sensor1 == true) || (dragging_ultrasonic_sensor2 == true) || (dragging_ultrasonic_sensor3 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_number_0 == true) || (dragging_number_1 == true) || (dragging_number_2 == true) || (dragging_number_3 == true) || (dragging_number_4 == true) || (dragging_number_5 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_number_6 == true) || (dragging_number_7 == true) || (dragging_number_8 == true) || (dragging_number_9 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_walk_foward == true) || (dragging_turn_left == true) || (dragging_turn_right == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_equal == true) || (dragging_unequal == true) || (dragging_greater == true) || (dragging_greater_equal == true) || (dragging_less == true) || (dragging_less_equal == true)) {
                        //erro logic para o bloco de WHILE
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_add == true) || (dragging_sub == true) || (dragging_mult == true) || (dragging_div == true) || (dragging_exponent == true)) {
                        //erro math para o bloco de WHILE
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } 
                    //Variaveis INT
                    else if(dragging_int_A == true){
                        blocks_list_to_print[i]->setFirstSlotWhile(1, int_var_list[0], NULL);
                    } else if(dragging_int_B == true){
                        blocks_list_to_print[i]->setFirstSlotWhile(1, int_var_list[1], NULL);
                    } else if(dragging_int_C == true){
                        blocks_list_to_print[i]->setFirstSlotWhile(1, int_var_list[2], NULL);
                    } else if(dragging_int_D == true){
                        blocks_list_to_print[i]->setFirstSlotWhile(1, int_var_list[3], NULL);
                    } else if(dragging_int_E == true){
                        blocks_list_to_print[i]->setFirstSlotWhile(1, int_var_list[4], NULL);
                    } else if(dragging_int_F == true){
                        blocks_list_to_print[i]->setFirstSlotWhile(1, int_var_list[5], NULL);
                    } else if(dragging_int_G == true){
                        blocks_list_to_print[i]->setFirstSlotWhile(1, int_var_list[6], NULL);
                    } else if(dragging_int_H == true){
                        blocks_list_to_print[i]->setFirstSlotWhile(1, int_var_list[7], NULL);
                    } else if(dragging_int_I == true){
                        blocks_list_to_print[i]->setFirstSlotWhile(1, int_var_list[8], NULL);
                    } else if(dragging_int_J == true){
                        blocks_list_to_print[i]->setFirstSlotWhile(1, int_var_list[9], NULL);
                    } 
                    //Variaveis BOOL
                    else if(dragging_bool_A == true){
                        blocks_list_to_print[i]->setFirstSlotWhile(2, NULL, bool_var_list[0]);
                    } else if(dragging_bool_B == true){  
                        blocks_list_to_print[i]->setFirstSlotWhile(2, NULL, bool_var_list[1]);
                    } else if(dragging_bool_C == true){  
                        blocks_list_to_print[i]->setFirstSlotWhile(2, NULL, bool_var_list[2]);
                    } else if(dragging_bool_D == true){  
                        blocks_list_to_print[i]->setFirstSlotWhile(2, NULL, bool_var_list[3]);
                    } else if(dragging_bool_E == true){  
                        blocks_list_to_print[i]->setFirstSlotWhile(2, NULL, bool_var_list[4]);
                    } else if(dragging_bool_F == true){  
                        blocks_list_to_print[i]->setFirstSlotWhile(2, NULL, bool_var_list[5]);
                    } else if(dragging_bool_G == true){  
                        blocks_list_to_print[i]->setFirstSlotWhile(2, NULL, bool_var_list[6]);
                    } else if(dragging_bool_H == true){  
                        blocks_list_to_print[i]->setFirstSlotWhile(2, NULL, bool_var_list[7]);
                    } else if(dragging_bool_I == true){  
                        blocks_list_to_print[i]->setFirstSlotWhile(2, NULL, bool_var_list[8]);
                    } else if(dragging_bool_J == true){  
                        blocks_list_to_print[i]->setFirstSlotWhile(2, NULL, bool_var_list[9]);
                    }
                }
                //SEGUNDO SLOT WHILE
                if((mouseX > blocks_list_to_print[i]->getX() + (1 * blocks_list_to_print[i]->getWidth()/3)) && (mouseX < (blocks_list_to_print[i]->getX() + (2 * blocks_list_to_print[i]->getWidth()/3))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + blocks_list_to_print[i]->getHeight()))) {
                    //Apenas operandos no slot do WHILE_BLOCK
                    if((dragging_black_sensor1 == true) || (dragging_black_sensor2 == true) || (dragging_black_sensor3 == true)  || (dragging_black_sensor4 == true) || (dragging_black_sensor5 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_color_sensor1 == true) || (dragging_color_sensor2 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_ultrasonic_sensor1 == true) || (dragging_ultrasonic_sensor2 == true) || (dragging_ultrasonic_sensor3 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_number_0 == true) || (dragging_number_1 == true) || (dragging_number_2 == true) || (dragging_number_3 == true) || (dragging_number_4 == true) || (dragging_number_5 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_number_6 == true) || (dragging_number_7 == true) || (dragging_number_8 == true) || (dragging_number_9 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_walk_foward == true) || (dragging_turn_left == true) || (dragging_turn_right == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } 
                    //incorreto, variaveis no slot de operandos
                    else if((dragging_int_A == true) || (dragging_int_B == true) || (dragging_int_C == true) || (dragging_int_D == true) || (dragging_int_E == true)) {
                        //incorreto, variaveis no slot de operandos
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_int_F == true) || (dragging_int_G == true) || (dragging_int_H == true) || (dragging_int_I == true) || (dragging_int_J == true)) {
                        //incorreto, variaveis no slot de operandos
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_bool_A == true) || (dragging_bool_B == true) || (dragging_bool_C == true) || (dragging_bool_D == true) || (dragging_bool_E == true)) {
                        //incorreto, variaveis no slot de operandos
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_bool_F == true) || (dragging_bool_G == true) || (dragging_bool_H == true) || (dragging_bool_I == true) || (dragging_bool_J == true)) {
                        //incorreto, variaveis no slot de operandos
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_add == true) || (dragging_sub == true) || (dragging_mult == true) || (dragging_div == true) || (dragging_exponent == true)) {
                        //erro math para o bloco de WHILE
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } 
                    //Operadores logicos 
                    else if(dragging_equal == true){
                        blocks_list_to_print[i]->setSecondSlotWhile(0);
                    } else if(dragging_unequal == true){
                        blocks_list_to_print[i]->setSecondSlotWhile(1);
                    } else if(dragging_greater == true){
                        blocks_list_to_print[i]->setSecondSlotWhile(2);
                    } else if(dragging_greater_equal == true){
                        blocks_list_to_print[i]->setSecondSlotWhile(3);
                    } else if(dragging_less == true){
                        blocks_list_to_print[i]->setSecondSlotWhile(4);
                    } else if(dragging_less_equal == true){
                        blocks_list_to_print[i]->setSecondSlotWhile(5);
                    } 
                }
                //TERCEIRO SLOT WHILE
                if((mouseX > blocks_list_to_print[i]->getX() + (2 * blocks_list_to_print[i]->getWidth()/3)) && (mouseX < (blocks_list_to_print[i]->getX() + (3 * blocks_list_to_print[i]->getWidth()/3))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + blocks_list_to_print[i]->getHeight()))) {
                    //Apenas variaveis e valores estaticos no slot do WHILE_BLOCK
                    if((dragging_walk_foward == true) || (dragging_turn_left == true) || (dragging_turn_right == true)) {
                        //erro comando no WHILE_BLOCK
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_equal == true) || (dragging_unequal == true) || (dragging_greater == true) || (dragging_greater_equal == true) || (dragging_less == true) || (dragging_less_equal == true)) {
                        //erro operando no slot incorreto
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_add == true) || (dragging_sub == true) || (dragging_mult == true) || (dragging_div == true) || (dragging_exponent == true)) {
                        //erro operando no slot incorreto
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } 
                    //WHILE_BLOCK nao lida com sensores
                    else if((dragging_black_sensor1 == true) || (dragging_black_sensor2 == true) || (dragging_black_sensor3 == true)  || (dragging_black_sensor4 == true) || (dragging_black_sensor5 == true)){
                        //erro sensor no WHILE_BLOCK
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_color_sensor1 == true) || (dragging_color_sensor2 == true)){
                        //erro sensor no WHILE_BLOCK
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_ultrasonic_sensor1 == true) || (dragging_ultrasonic_sensor2 == true) || (dragging_ultrasonic_sensor3 == true)) {
                        //erro sensor no WHILE_BLOCK
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Definir Erro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } 
                    //Variaveis INT
                    else if(dragging_int_A == true){
                        blocks_list_to_print[i]->setThirdSlotWhile(1, int_var_list[0], NULL, 0);
                    } else if(dragging_int_B == true){
                        blocks_list_to_print[i]->setThirdSlotWhile(1, int_var_list[1], NULL, 0);
                    } else if(dragging_int_C == true){
                        blocks_list_to_print[i]->setThirdSlotWhile(1, int_var_list[2], NULL, 0);
                    } else if(dragging_int_D == true){
                        blocks_list_to_print[i]->setThirdSlotWhile(1, int_var_list[3], NULL, 0);
                    } else if(dragging_int_E == true){
                        blocks_list_to_print[i]->setThirdSlotWhile(1, int_var_list[4], NULL, 0);
                    } else if(dragging_int_F == true){
                        blocks_list_to_print[i]->setThirdSlotWhile(1, int_var_list[5], NULL, 0);
                    } else if(dragging_int_G == true){
                        blocks_list_to_print[i]->setThirdSlotWhile(1, int_var_list[6], NULL, 0);
                    } else if(dragging_int_H == true){
                        blocks_list_to_print[i]->setThirdSlotWhile(1, int_var_list[7], NULL, 0);
                    } else if(dragging_int_I == true){
                        blocks_list_to_print[i]->setThirdSlotWhile(1, int_var_list[8], NULL, 0);
                    } else if(dragging_int_J == true){
                        blocks_list_to_print[i]->setThirdSlotWhile(1, int_var_list[9], NULL, 0);
                    } 
                    //Variaveis BOOL
                    else if(dragging_bool_A == true){
                        blocks_list_to_print[i]->setThirdSlotWhile(2, NULL, bool_var_list[0], 0);
                    } else if(dragging_bool_B == true){  
                        blocks_list_to_print[i]->setThirdSlotWhile(2, NULL, bool_var_list[1], 0);
                    } else if(dragging_bool_C == true){  
                        blocks_list_to_print[i]->setThirdSlotWhile(2, NULL, bool_var_list[2], 0);
                    } else if(dragging_bool_D == true){  
                        blocks_list_to_print[i]->setThirdSlotWhile(2, NULL, bool_var_list[3], 0);
                    } else if(dragging_bool_E == true){  
                        blocks_list_to_print[i]->setThirdSlotWhile(2, NULL, bool_var_list[4], 0);
                    } else if(dragging_bool_F == true){  
                        blocks_list_to_print[i]->setThirdSlotWhile(2, NULL, bool_var_list[5], 0);
                    } else if(dragging_bool_G == true){  
                        blocks_list_to_print[i]->setThirdSlotWhile(2, NULL, bool_var_list[6], 0);
                    } else if(dragging_bool_H == true){  
                        blocks_list_to_print[i]->setThirdSlotWhile(2, NULL, bool_var_list[7], 0);
                    } else if(dragging_bool_I == true){  
                        blocks_list_to_print[i]->setThirdSlotWhile(2, NULL, bool_var_list[8], 0);
                    } else if(dragging_bool_J == true){  
                        blocks_list_to_print[i]->setThirdSlotWhile(2, NULL, bool_var_list[9], 0);
                    } else if(dragging_number_0 == true) {
                        //erro, ZERO
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Evite usar o Zero neste Slot.", "Hmm..", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if(dragging_number_1 == true) {
                        blocks_list_to_print[i]->setThirdSlotWhile(3, NULL, NULL, 1);
                    } else if(dragging_number_2 == true) {
                        blocks_list_to_print[i]->setThirdSlotWhile(3, NULL, NULL, 2);
                    } else if(dragging_number_3 == true) {
                        blocks_list_to_print[i]->setThirdSlotWhile(3, NULL, NULL, 3);
                    } else if(dragging_number_4 == true) {
                        blocks_list_to_print[i]->setThirdSlotWhile(3, NULL, NULL, 4);
                    } else if(dragging_number_5 == true) {
                        blocks_list_to_print[i]->setThirdSlotWhile(3, NULL, NULL, 5);
                    } else if(dragging_number_6 == true) {
                        blocks_list_to_print[i]->setThirdSlotWhile(3, NULL, NULL, 6);
                    } else if(dragging_number_7 == true) {
                        blocks_list_to_print[i]->setThirdSlotWhile(3, NULL, NULL, 7);
                    } else if(dragging_number_8 == true) {
                        blocks_list_to_print[i]->setThirdSlotWhile(3, NULL, NULL, 8);
                    } else if(dragging_number_9 == true) {
                        blocks_list_to_print[i]->setThirdSlotWhile(3, NULL, NULL, 9);
                    }
                }
            }
        }
    }
}
void Interface :: draw_dragging() {
    if(dragging_black_sensor1 == true) {
        al_draw_bitmap(BLACK_SENSOR_1_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_black_sensor2 == true) {
        al_draw_bitmap(BLACK_SENSOR_2_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_black_sensor3 == true) {
        al_draw_bitmap(BLACK_SENSOR_3_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_black_sensor4 == true) {
        al_draw_bitmap(BLACK_SENSOR_4_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_black_sensor5 == true) {
        al_draw_bitmap(BLACK_SENSOR_5_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_color_sensor1 == true) {
        al_draw_bitmap(COLOR_SENSOR_1_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_color_sensor2 == true) {
        al_draw_bitmap(COLOR_SENSOR_2_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_ultrasonic_sensor1 == true) {
        al_draw_bitmap(ULTRASONIC_SENSOR_1_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_ultrasonic_sensor2 == true) {
        al_draw_bitmap(ULTRASONIC_SENSOR_2_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_ultrasonic_sensor3 == true) {
        al_draw_bitmap(ULTRASONIC_SENSOR_3_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_walk_foward == true) {
        al_draw_bitmap(WALK_FOWARD_ACTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_turn_left == true) {
        al_draw_bitmap(TURN_LEFT_ACTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_turn_right == true) {
        al_draw_bitmap(TURN_RIGHT_ACTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_number_0 == true) {
        al_draw_bitmap(NUMBER[0], mouseX-20, mouseY-20, 0);
    }
    if(dragging_number_1 == true) {
        al_draw_bitmap(NUMBER[1], mouseX-20, mouseY-20, 0);
    }
    if(dragging_number_2 == true) {
        al_draw_bitmap(NUMBER[2], mouseX-20, mouseY-20, 0);
    }
    if(dragging_number_3 == true) {
        al_draw_bitmap(NUMBER[3], mouseX-20, mouseY-20, 0);
    }
    if(dragging_number_4 == true) {
        al_draw_bitmap(NUMBER[4], mouseX-20, mouseY-20, 0);
    }
    if(dragging_number_5 == true) {
        al_draw_bitmap(NUMBER[5], mouseX-20, mouseY-20, 0);
    }
    if(dragging_number_6 == true) {
        al_draw_bitmap(NUMBER[6], mouseX-20, mouseY-20, 0);
    }
    if(dragging_number_7 == true) {
        al_draw_bitmap(NUMBER[7], mouseX-20, mouseY-20, 0);
    }
    if(dragging_number_8 == true) {
        al_draw_bitmap(NUMBER[8], mouseX-20, mouseY-20, 0);
    }
    if(dragging_number_9 == true) {
        al_draw_bitmap(NUMBER[9], mouseX-20, mouseY-20, 0);
    }
    //v3
    if(dragging_int_A == true) {
        al_draw_bitmap(INT_VAR_IMG[0], mouseX-20, mouseY-20, 0);
    }
    if(dragging_int_B == true) {
        al_draw_bitmap(INT_VAR_IMG[1], mouseX-20, mouseY-20, 0);
    }
    if(dragging_int_C == true) {
        al_draw_bitmap(INT_VAR_IMG[2], mouseX-20, mouseY-20, 0);
    }
    if(dragging_int_D == true) {
        al_draw_bitmap(INT_VAR_IMG[3], mouseX-20, mouseY-20, 0);
    }
    if(dragging_int_E == true) {
        al_draw_bitmap(INT_VAR_IMG[4], mouseX-20, mouseY-20, 0);
    }
    if(dragging_int_F == true) {
        al_draw_bitmap(INT_VAR_IMG[5], mouseX-20, mouseY-20, 0);
    }
    if(dragging_int_G == true) {
        al_draw_bitmap(INT_VAR_IMG[6], mouseX-20, mouseY-20, 0);
    }
    if(dragging_int_H == true) {
        al_draw_bitmap(INT_VAR_IMG[7], mouseX-20, mouseY-20, 0);
    }
    if(dragging_int_I == true) {
        al_draw_bitmap(INT_VAR_IMG[8], mouseX-20, mouseY-20, 0);
    }
    if(dragging_int_J == true) {
        al_draw_bitmap(INT_VAR_IMG[9], mouseX-20, mouseY-20, 0);
    }
    if(dragging_bool_A == true) {
        al_draw_bitmap(BOOL_VAR_IMG[0], mouseX-20, mouseY-20, 0);
    }
    if(dragging_bool_B == true) {
        al_draw_bitmap(BOOL_VAR_IMG[1], mouseX-20, mouseY-20, 0);
    }
    if(dragging_bool_C == true) {
        al_draw_bitmap(BOOL_VAR_IMG[2], mouseX-20, mouseY-20, 0);
    }
    if(dragging_bool_D == true) {
        al_draw_bitmap(BOOL_VAR_IMG[3], mouseX-20, mouseY-20, 0);
    }
    if(dragging_bool_E == true) {
        al_draw_bitmap(BOOL_VAR_IMG[4], mouseX-20, mouseY-20, 0);
    }
    if(dragging_bool_F == true) {
        al_draw_bitmap(BOOL_VAR_IMG[5], mouseX-20, mouseY-20, 0);
    }
    if(dragging_bool_G == true) {
        al_draw_bitmap(BOOL_VAR_IMG[6], mouseX-20, mouseY-20, 0);
    }
    if(dragging_bool_H == true) {
        al_draw_bitmap(BOOL_VAR_IMG[7], mouseX-20, mouseY-20, 0);
    }
    if(dragging_bool_I == true) {
        al_draw_bitmap(BOOL_VAR_IMG[8], mouseX-20, mouseY-20, 0);
    }
    if(dragging_bool_J == true) {
        al_draw_bitmap(BOOL_VAR_IMG[9], mouseX-20, mouseY-20, 0);
    }
    if(dragging_equal == true) {
        al_draw_bitmap(LOGIC_OPERATOR_IMG[0], mouseX-20, mouseY-20, 0);
    }
    if(dragging_greater == true) {
        al_draw_bitmap(LOGIC_OPERATOR_IMG[1], mouseX-20, mouseY-20, 0);
    }
    if(dragging_less == true) {
        al_draw_bitmap(LOGIC_OPERATOR_IMG[2], mouseX-20, mouseY-20, 0);
    }
    if(dragging_greater_equal == true) {
        al_draw_bitmap(LOGIC_OPERATOR_IMG[3], mouseX-20, mouseY-20, 0);
    }
    if(dragging_less_equal == true) {
        al_draw_bitmap(LOGIC_OPERATOR_IMG[4], mouseX-20, mouseY-20, 0);
    }
    if(dragging_unequal == true) {
        al_draw_bitmap(LOGIC_OPERATOR_IMG[5], mouseX-20, mouseY-20, 0);
    }
    if(dragging_add == true) {
        al_draw_bitmap(MATH_OPERATOR_IMG[0], mouseX-20, mouseY-20, 0);
    }
    if(dragging_div == true) {
        al_draw_bitmap(MATH_OPERATOR_IMG[1], mouseX-20, mouseY-20, 0);
    }
    if(dragging_exponent == true) {
        al_draw_bitmap(MATH_OPERATOR_IMG[2], mouseX-20, mouseY-20, 0);
    }
    if(dragging_mult == true) {
        al_draw_bitmap(MATH_OPERATOR_IMG[3], mouseX-20, mouseY-20, 0);
    }
    if(dragging_sub == true) {
        al_draw_bitmap(MATH_OPERATOR_IMG[4], mouseX-20, mouseY-20, 0);
    }
}
void Interface :: check_mouse_on_menus() {
    menu1_X_limit = 14 + 6*al_get_bitmap_width(play_button);
    menu1_Y_limit = 4 + al_get_bitmap_height(play_button);

    menu_connect_X_begin = al_get_display_width(display) - 73.5 - al_get_bitmap_width(bluetooth_button) - roll_bar_width;
    menu_connect_X_limit = al_get_display_width(display) - roll_bar_width;

    menu2_X_limit = al_get_bitmap_width(WALK_FOWARD_ACTION);

    menu_blocks_Y_begin = 4 + al_get_bitmap_height(play_button);
    menu_blocks_Y_limit = menu1_Y_limit + 6*al_get_bitmap_height(WALK_FOWARD_ACTION);

    menu_sensors_Y_begin = menu_blocks_Y_limit;
    menu_sensors_Y_limit = menu_sensors_Y_begin + 3*al_get_bitmap_height(WALK_FOWARD_ACTION)+10;

    menu_actions_Y_begin = menu_sensors_Y_limit;
    menu_actions_Y_limit = menu_actions_Y_begin + 3*al_get_bitmap_height(WALK_FOWARD_ACTION)+10;

    menu_extra_Y_begin = menu_actions_Y_limit;
    menu_extra_Y_limit = menu_extra_Y_begin + 1*al_get_bitmap_height(WALK_FOWARD_ACTION)+10;

    //v3
    menu_var_and_symbols_y_begin = menu_extra_Y_limit;
    menu_var_and_symbols_y_limit = menu_var_and_symbols_y_begin + 3*al_get_bitmap_height(WALK_FOWARD_ACTION)+10;

    menu_new_blocks_y_begin = menu_var_and_symbols_y_limit;
    menu_new_blocks_y_limit = menu_var_and_symbols_y_limit + 4*al_get_bitmap_height(WALK_FOWARD_ACTION)+10;


    //se o mouse não está sobre os menus seta a variável de teste para zero
    if(mouseY < menu1_Y_limit) {
        if((mouseX > 14) && (mouseX < menu1_X_limit)) {
            //menu 1
        } else if((mouseX < menu_connect_X_limit) && (mouseX > menu_connect_X_begin)) {
            //menu conectar
        } else {
            //cout<<"check_mouse_on_menus() - caso 1 - menu_selected = "<<menu_selected<<endl;
            menu_selected = 0;
        }
    } else if(mouseX < menu2_X_limit) {
        if((mouseY > menu_blocks_Y_begin) && (mouseY < menu_blocks_Y_limit)) {
            //menu de blocos
        } else if((mouseY > menu_sensors_Y_begin) && (mouseY < menu_sensors_Y_limit)) {
            //menu de sensores
        } else if((mouseY > menu_actions_Y_begin) && (mouseY < menu_actions_Y_limit)) {
            //menu de acoes
        } else if((mouseY > menu_extra_Y_begin) && (mouseY < menu_extra_Y_limit)) {
            //menu extra
        } 
        //v3
        else if((mouseY > menu_var_and_symbols_y_begin) && (mouseY < menu_var_and_symbols_y_limit)) {
            //menu variaveis e simbolos
        } 
        else if((mouseY > menu_new_blocks_y_begin) && (mouseY < menu_new_blocks_y_limit)) {
            //menu blocos novos
        } 
        else {
            //cout<<"check_mouse_on_menus() - caso 2 - menu_selected = "<<menu_selected<<endl;
            //cout<<"check_mouse_on_menus() - caso 2 - menu_extra_Y_begin = "<<menu_extra_Y_begin<<endl;
            //cout<<"check_mouse_on_menus() - caso 2 - menu_var_and_symbols_y_begin = "<<menu_var_and_symbols_y_begin<<endl;
            //cout<<"check_mouse_on_menus() - caso 2 - menu_new_blocks_y_begin = "<<menu_new_blocks_y_begin<<endl;
            menu_selected = 0;
        }
    } else if((black_sensor_menu_selected == true) && ((mouseX < 6*menu2_X_limit) && (mouseY > menu_sensors_Y_begin) && (mouseY < (menu_sensors_Y_begin+al_get_bitmap_height(WALK_FOWARD_ACTION))))) {
        //submenu sensor de fita
    } else if((color_sensor_menu_selected == true) && ((mouseX < 3*menu2_X_limit) && (mouseY > (menu_sensors_Y_begin+al_get_bitmap_width(WALK_FOWARD_ACTION))) && (mouseY < (menu_sensors_Y_begin+2*al_get_bitmap_height(WALK_FOWARD_ACTION))))) {
        //submenu sensor de cor
    } else if((ultrasonic_sensor_menu_selected == true) && ((mouseX < 4*menu2_X_limit) && (mouseY > (menu_sensors_Y_begin+2*al_get_bitmap_width(WALK_FOWARD_ACTION))) && (mouseY < (menu_sensors_Y_begin+3*al_get_bitmap_height(WALK_FOWARD_ACTION))))) {
        //submenu sensor de ultrassom
    } else if((number_menu_selected == true) && ((mouseX < 11*menu2_X_limit) && (mouseY > (menu_extra_Y_begin+0*al_get_bitmap_width(WALK_FOWARD_ACTION))) && (mouseY < (menu_extra_Y_begin+1*al_get_bitmap_height(WALK_FOWARD_ACTION))))) {
        //submenu sensor de numero
        //cout<<"check_mouse_on_menus() - submenu sensor de numero"<<endl;
    } 
    //v3
    else if((int_menu_selected == true) && ((mouseX < 10*menu2_X_limit) && (mouseY > (menu_var_and_symbols_y_begin+0*al_get_bitmap_width(WALK_FOWARD_ACTION))) && (mouseY < (menu_var_and_symbols_y_begin+1*al_get_bitmap_height(WALK_FOWARD_ACTION))))) {
        //submenu variaveis int
        //cout<<"check_mouse_on_menus() - submenu variaveis int"<<endl;
    } 
    else if((bool_menu_selected == true) && ((mouseX < 10*menu2_X_limit) && (mouseY > (menu_var_and_symbols_y_begin+1*al_get_bitmap_width(WALK_FOWARD_ACTION))) && (mouseY < (menu_var_and_symbols_y_begin+2*al_get_bitmap_height(WALK_FOWARD_ACTION))))) {
        //submenu variaveis bool
        //cout<<"check_mouse_on_menus() - submenu variaveis bool"<<endl;
    } 
    else if((logic_menu_selected == true) && ((mouseX < 12*menu2_X_limit) && (mouseY > (menu_var_and_symbols_y_begin+2*al_get_bitmap_width(WALK_FOWARD_ACTION))) && (mouseY < (menu_var_and_symbols_y_begin+3*al_get_bitmap_height(WALK_FOWARD_ACTION))))) {
        //submenu logica e matematica
        //cout<<"check_mouse_on_menus() - submenu logica e matematica"<<endl;
    } 
    else {
        //cout<<"check_mouse_on_menus()- caso 3 - menu_selected = "<<menu_selected<<endl;
        menu_selected = 0;
    }
}
void Interface :: print_list_of_blocks() {
    for(int i=0; i<valor_maximo_blocos; i++) {
        //testa se na posição do array existe mesmo um bloco
        if(blocks_list_to_print[i] != NULL) {
            //cout<<"print_list_of_blocks() - Print Block: "<<blocks_list_to_print[i]<<endl;
            if(blocks_list_to_print[i]->getType() == ACTION_BLOCK) {
                //cout<<"print_list_of_blocks() - ACTION_BLOCK: "<<blocks_list_to_print[i]<<endl;
                print_function_block(blocks_list_to_print[i]);
            }
            if(blocks_list_to_print[i]->getType() == END_BLOCK) {
                //cout<<"print_list_of_blocks() - END_BLOCK: "<<blocks_list_to_print[i]<<endl;
                print_end_block(blocks_list_to_print[i]);
            }
            if(blocks_list_to_print[i]->getType() == START_BLOCK) {
                //cout<<"print_list_of_blocks() - START_BLOCK: "<<blocks_list_to_print[i]<<endl;
                print_start_block(blocks_list_to_print[i]);
            }
            if(blocks_list_to_print[i]->getType() == LOOP_BLOCK) {
                //cout<<"print_list_of_blocks() - LOOP_BLOCK: "<<blocks_list_to_print[i]<<endl;
                print_loop_block(blocks_list_to_print[i]);
            }
            if(blocks_list_to_print[i]->getType() == CONDITIONAL_BLOCK) {
                //cout<<"print_list_of_blocks() - CONDITIONAL_BLOCK: "<<blocks_list_to_print[i]<<endl;
                print_decision_block(blocks_list_to_print[i]);
            }
            if(blocks_list_to_print[i]->getType() == MERGE_BLOCK) {
                //cout<<"print_list_of_blocks() - MERGE_BLOCK: "<<blocks_list_to_print[i]<<endl;
                print_merge_block(blocks_list_to_print[i]);
            }
            //v3
            if(blocks_list_to_print[i]->getType() == ATTRIBUTION_BLOCK) {
                //cout<<"print_list_of_blocks() - ATTRIBUTION_BLOCK: "<<blocks_list_to_print[i]<<endl;
                print_attribution_block(blocks_list_to_print[i]);
            }
            if(blocks_list_to_print[i]->getType() == MATH_BLOCK) {
                //cout<<"print_list_of_blocks() - MATH_BLOCK: "<<blocks_list_to_print[i]<<endl;
                print_math_block(blocks_list_to_print[i]);
            }
            if(blocks_list_to_print[i]->getType() == NEW_CONDITIONAL_BLOCK) {
                //cout<<"print_list_of_blocks() - NEW_CONDITIONAL_BLOCK: "<<blocks_list_to_print[i]<<endl;
                print_new_conditional_block(blocks_list_to_print[i]);
            }
            if(blocks_list_to_print[i]->getType() == WHILE_BLOCK) {
                //cout<<"print_list_of_blocks() - WHILE_BLOCK: "<<blocks_list_to_print[i]<<endl;
                print_while_block(blocks_list_to_print[i]);
            }
            //se o block estiver sendo arrastado, atualiza as cordenadas
            if(blocks_list_to_print[i]->getDragging() == true) {
                blocks_list_to_print[i]->setX(mouseX - mouse_aux_x);
                blocks_list_to_print[i]->setY(mouseY - mouse_aux_y);
            } else if(blocks_list_to_print[i]->getX() <= menu2_X_limit) {
                blocks_list_to_print[i]->setX(menu2_X_limit + 20);
            }
            if((blocks_list_to_print[i]->getY() <= menu1_Y_limit) && (blocks_list_to_print[i]->getDragging() == true)){
                blocks_list_to_print[i]->setY(menu1_Y_limit);
            }
        }
    }
}
void Interface :: check_mouse_on_points(Block *b) {
    //função
    if(b->getType() == ACTION_BLOCK) {
        if((mouseX > b->getX()+40) && (mouseX < (b->getX() + 53)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
            //cout<<"clicou na entrada do bloco de funcao"<<endl;
            b->setIn1Selected(true);
            b->setOut1Selected(false);
        } else if((mouseX > b->getX()+40) && (mouseX < (b->getX() + 53)) && (mouseY > b->getY()+35) && (mouseY < (b->getY()+48))) {
            //cout<<"clicou na saida do bloco de funcao"<<endl;
            b->setIn1Selected(false);
            b->setOut1Selected(true);
        } else {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
        }
    }
    //fim
    if(b->getType() == END_BLOCK) {
        if((mouseX > b->getX()+37) && (mouseX < (b->getX() + 50)) && (mouseY > b->getY()-5) && (mouseY < (b->getY() + 8))) {
            //cout<<"clicou na entrada do bloco de fim"<<endl;
            b->setIn1Selected(true);
        } else {
            b->setIn1Selected(false);
        }
    }
    //inicio
    if(b->getType() == START_BLOCK) {
        if((mouseX > b->getX()+37) && (mouseX < (b->getX() + 50)) && (mouseY > b->getY()+27) && (mouseY < (b->getY() + 40))) {
            b->setOut1Selected(true);
        } else {
            b->setOut1Selected(false);
        }
    }
    //loop
    if(b->getType() == LOOP_BLOCK) {
        if((mouseX > b->getX()+35) && (mouseX < (b->getX() + 48)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
            b->setIn1Selected(true);
            b->setOut1Selected(false);
            b->setIn2Selected(false);
            b->setOut2Selected(false);
        } else if((mouseX > b->getX()+35) && (mouseX < (b->getX() + 48)) && (mouseY > b->getY()+81) && (mouseY < (b->getY()+94))) {
            b->setIn1Selected(false);
            b->setOut1Selected(true);
            b->setIn2Selected(false);
            b->setOut2Selected(false);
        } else if((mouseX > b->getX()+72) && (mouseX < (b->getX() + 85)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setIn2Selected(false);
            b->setOut2Selected(true);
        } else if((mouseX > b->getX()+72) && (mouseX < (b->getX() + 85)) && (mouseY > b->getY()+81) && (mouseY < (b->getY()+94))) {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setIn2Selected(true);
            b->setOut2Selected(false);
        } else {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setIn2Selected(false);
            b->setOut2Selected(false);
        }
    }
    //decisao
    if(b->getType() == CONDITIONAL_BLOCK) {

        if((mouseX > b->getX()+53) && (mouseX < (b->getX() + 66)) && (mouseY > b->getY()-6) && (mouseY < (b->getY()+7))) {
            b->setIn1Selected(true);
            b->setOut1Selected(false);
            b->setOut2Selected(false);
        } else if((mouseX > b->getX()+53) && (mouseX < (b->getX() + 66)) && (mouseY > b->getY()+62) && (mouseY < (b->getY()+75))) {
            b->setIn1Selected(false);
            b->setOut1Selected(true);
            b->setOut2Selected(false);
        } else if((mouseX > b->getX()+112) && (mouseX < (b->getX() + 125)) && (mouseY > b->getY()+28) && (mouseY < (b->getY()+41))) {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setOut2Selected(true);
        } else {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setOut2Selected(false);
        }
    }
    //junção
    if(b->getType() == MERGE_BLOCK) {

        if((mouseX > b->getX()-5) && (mouseX < (b->getX() + 8)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
            b->setIn1Selected(true);
            b->setOut1Selected(false);
            b->setIn2Selected(false);
        } else if((mouseX > b->getX()+22) && (mouseX < (b->getX() + 35)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setIn2Selected(true);
        } else if((mouseX > b->getX()+8) && (mouseX < (b->getX() + 21)) && (mouseY > b->getY()+17) && (mouseY < (b->getY()+30))) {
            b->setIn1Selected(false);
            b->setOut1Selected(true);
            b->setIn2Selected(false);
        } else {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setIn2Selected(false);
        }
    }
    //v3
    if(b->getType() == ATTRIBUTION_BLOCK) {
        if((mouseX > b->getX()+74) && (mouseX < (b->getX() + 86)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+6))) {
            b->setIn1Selected(true);
            b->setOut1Selected(false);
        } else if((mouseX > b->getX()+74) && (mouseX < (b->getX() + 86)) && (mouseY > b->getY()+36) && (mouseY < (b->getY()+47))) {
            b->setIn1Selected(false);
            b->setOut1Selected(true);
        } else {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
        }
    }
    if(b->getType() == MATH_BLOCK) {
        if((mouseX > b->getX()+108) && (mouseX < (b->getX() + 120)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+6))) {
            b->setIn1Selected(true);
            b->setOut1Selected(false);
        } else if((mouseX > b->getX()+108) && (mouseX < (b->getX() + 120)) && (mouseY > b->getY()+36) && (mouseY < (b->getY()+47))) {
            b->setIn1Selected(false);
            b->setOut1Selected(true);
        } else {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
        }
    }
    if(b->getType() == NEW_CONDITIONAL_BLOCK) {
        if((mouseX > b->getX()+74) && (mouseX < (b->getX() + 86)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+6))) {
            b->setIn1Selected(true);
            b->setOut1Selected(false);
            b->setOut2Selected(false);
        } else if((mouseX > b->getX()+29) && (mouseX < (b->getX() + 41)) && (mouseY > b->getY()+36) && (mouseY < (b->getY()+47))) {
            b->setIn1Selected(false);
            b->setOut1Selected(true);
            b->setOut2Selected(false);
        } else if((mouseX > b->getX()+120) && (mouseX < (b->getX() + 132)) && (mouseY > b->getY()+36) && (mouseY < (b->getY()+47))) {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setOut2Selected(true);
        } else {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setOut2Selected(false);
        }
    }
    if(b->getType() == WHILE_BLOCK) {
        if((mouseX > b->getX()+74) && (mouseX < (b->getX() + 86)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+6))) {
            b->setIn1Selected(true);
            b->setOut1Selected(false);
            b->setIn2Selected(false);
            b->setOut2Selected(false);
        } else if((mouseX > b->getX()+74) && (mouseX < (b->getX() + 86)) && (mouseY > b->getY()+36) && (mouseY < (b->getY()+47))) {
            b->setIn1Selected(false);
            b->setOut1Selected(true);
            b->setIn2Selected(false);
            b->setOut2Selected(false);
        } else if((mouseX > b->getX()+154) && (mouseX < (b->getX() + 165)) && (mouseY > b->getY()+1) && (mouseY < (b->getY()+12))) {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setIn2Selected(false);
            b->setOut2Selected(true);
        } else if((mouseX > b->getX()+154) && (mouseX < (b->getX() + 165)) && (mouseY > b->getY()+28) && (mouseY < (b->getY()+40))) {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setIn2Selected(true);
            b->setOut2Selected(false);
        } else {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setIn2Selected(false);
            b->setOut2Selected(false);
        }
    }
}
void Interface :: draw_temporary_line() {
    if(drawing_line == true) {
        float angulo = -(atan2((mouseX - temporary_line_X), (mouseY - temporary_line_Y)));
        al_draw_line(temporary_line_X, temporary_line_Y, mouseX, mouseY, primary_menu_color, 2);
        al_draw_rotated_bitmap(arrow, (al_get_bitmap_width(arrow))/2, (al_get_bitmap_height(arrow))/2, mouseX, mouseY, angulo, 0);
    }
}
void Interface :: delete_connections() {
    if(drawing_line == true) {
        if(blocks_list_to_print[block_selected] != NULL) {
            if(number_of_selected_out == 1) {
                int aux=0;
                //procura bloco que tem relação
                for(int i=0; i<valor_maximo_blocos; i++) {
                    if(blocks_list_to_print[i] != NULL) {
                        if(blocks_list_to_print[i] == blocks_list_to_print[block_selected]->getNext1()) {
                            //achou o bloco
                            aux = i;
                        }
                    }
                }
                blocks_list_to_print[block_selected]->setNext1(NULL);
                if(blocks_list_to_print[aux]->getPrevious1() == blocks_list_to_print[block_selected]) {
                    blocks_list_to_print[aux]->setPrevious1(NULL);
                } else {
                    blocks_list_to_print[aux]->setPrevious2(NULL);
                }
            } else {
                int aux=0;
                //procura bloco que tem relação
                for(int i=0; i<valor_maximo_blocos; i++) {
                    if(blocks_list_to_print[i] != NULL) {
                        if(blocks_list_to_print[i] == blocks_list_to_print[block_selected]->getNext2()) {
                            //achou o bloco
                            aux = i;
                        }
                    }
                }
                blocks_list_to_print[block_selected]->setNext2(NULL);
                if(blocks_list_to_print[aux]->getPrevious1() == blocks_list_to_print[block_selected]) {
                    blocks_list_to_print[aux]->setPrevious1(NULL);
                } else {
                    blocks_list_to_print[aux]->setPrevious2(NULL);
                }
            }
        }
        drawing_line = false;
    }
}
void Interface :: draw_everything() {
    if(black_sensor_menu_selected || color_sensor_menu_selected || ultrasonic_sensor_menu_selected || number_menu_selected || int_menu_selected || bool_menu_selected || logic_menu_selected) {
        //percorre toda a lista de impressão dos blocos
        print_list_of_blocks();
        //desenha ligações entre blocos
        draw_lines();
        //imprime menu
        print_primary_menu();
        //imprime menu de blocos
        print_secondary_menu();
        //checa se o mouse está sobre os menus, para setar a variável de controle do menu_selected
        check_mouse_on_menus();
    } else {
        //imprime menu de blocos
        print_secondary_menu();
        //checa se o mouse está sobre os menus, para setar a variável de controle do menu_selected
        check_mouse_on_menus();
        //percorre toda a lista de impressão dos blocos
        print_list_of_blocks();
        //desenha ligações entre blocos
        draw_lines();
        //imprime menu
        print_primary_menu();
    }

    //desenha objetos sendo arrastados
    draw_dragging();
    //desenha linha de ligação
    draw_temporary_line();

    if(connected_simulator == true) {
        int x_init = al_get_display_width(display) + 10 - al_get_bitmap_width(vrep_button) - roll_bar_width;
        int y_init = 2;
        int x_end = x_init + al_get_bitmap_width(vrep_button);
        int y_end = y_init + al_get_bitmap_height(vrep_button);
        al_draw_rectangle(x_init, y_init, x_end, y_end, strange_color, 2);
    }
    if(connected_robot == true) {
        int x_init = al_get_display_width(display) - 73.5 - al_get_bitmap_width(bluetooth_button) - roll_bar_width;
        int y_init = 2;
        int x_end = x_init + al_get_bitmap_width(bluetooth_button);
        int y_end = y_init + al_get_bitmap_height(bluetooth_button);
        al_draw_rectangle(x_init, y_init, x_end, y_end, strange_color, 2);
    }

    if(executing_fluxogram == true) {
        int x_init = 14 + 0*al_get_bitmap_width(play_button);
        int y_init = 2;
        int x_end = x_init + al_get_bitmap_width(play_button);
        int y_end = y_init + al_get_bitmap_height(play_button);
        al_draw_rectangle(x_init, y_init, x_end, y_end, strange_color, 2);
    }

    draw_scroll_bar();
}
bool Interface :: check_colisions() {
    int selected_block;
    //procura o bloco selecionado
    for(int i = 0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i]->getSelected() == true) {
                //cout<<"encontrou o selecionado"<<endl;
                selected_block = i;
                break;
            } else {
                selected_block = -1;
            }
        }
    }
    //percorre todos os blocos
    for(int i=0; i<valor_maximo_blocos; i++) {
        //testa se tem algum selecionado
        if(selected_block != -1) {
            //testa se os blocos são válidos
            if((blocks_list_to_print[i] != NULL) && (blocks_list_to_print[selected_block] != NULL)) {
                //exclui o bloco selecionado do teste para n testar colisão dele com ele mesmo
                if(i != selected_block) {
                    //teste se está sendo arrastado, pois o teste de colisão ocorre quando não está sendo arrastado
                    if(blocks_list_to_print[selected_block]->getDragging() == false) {
                        int selected_block_1_x = blocks_list_to_print[selected_block]->getX();
                        int selected_block_1_y = blocks_list_to_print[selected_block]->getY();
                        int selected_block_2_y = blocks_list_to_print[selected_block]->getY()+blocks_list_to_print[selected_block]->getHeight();

                        int selected_block_3_x = blocks_list_to_print[selected_block]->getX()+blocks_list_to_print[selected_block]->getWidth();
                        int selected_block_3_y = blocks_list_to_print[selected_block]->getY();
                        int selected_block_4_y = blocks_list_to_print[selected_block]->getY()+blocks_list_to_print[selected_block]->getHeight();

                        int begin_x = blocks_list_to_print[i]->getX();
                        int begin_y = blocks_list_to_print[i]->getY();
                        int limit_x = blocks_list_to_print[i]->getX()+blocks_list_to_print[i]->getWidth();
                        int limit_y = blocks_list_to_print[i]->getY()+blocks_list_to_print[i]->getHeight();
                        //casos 1 e 2
                        if(selected_block_1_x >= begin_x) {
                            if(selected_block_1_x < limit_x) {
                                //caso 1
                                if(selected_block_1_y >= begin_y) {
                                    if(selected_block_1_y < limit_y) {
                                        //cout<<"realizou operação de colisão caso 1"<<endl;
                                        blocks_list_to_print[selected_block]->setX(blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() + 10);
                                        return true;
                                    }
                                }
                                //caso 2
                                if(selected_block_2_y >= begin_y) {
                                    if(selected_block_2_y < limit_y) {
                                        //cout<<"realizou operação de colisão caso 2"<<endl;
                                        blocks_list_to_print[selected_block]->setX(blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() + 10);
                                        return true;
                                    }
                                }
                            }
                        }
                        //casos 3 e 4
                        if(selected_block_3_x >= begin_x) {
                            if(selected_block_3_x < limit_x) {
                                //caso 3
                                if(selected_block_3_y >= begin_y) {
                                    if(selected_block_3_y < limit_y) {
                                        //cout<<"realizou operação de colisão caso 3"<<endl;
                                        blocks_list_to_print[selected_block]->setX(blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() + 10);
                                        return true;
                                    }
                                }
                                //caso 4
                                if(selected_block_4_y >= begin_y) {
                                    if(selected_block_4_y < limit_y) {
                                        //cout<<"realizou operação de colisão caso 4"<<endl;
                                        blocks_list_to_print[selected_block]->setX(blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() + 10);
                                        return true;
                                    }
                                }
                            }
                        }
                        //caso 5, 6 e 7
                        if(selected_block_1_x < begin_x) {
                            if(selected_block_3_x > limit_x) {
                                //caso 5
                                if(selected_block_1_y < begin_y) {
                                    if(selected_block_2_y > limit_y) {
                                        //cout<<"realizou operação de colisão caso 5"<<endl;
                                        blocks_list_to_print[selected_block]->setX(blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() + 10);
                                        return true;
                                    }
                                }
                                //caso 6
                                if(selected_block_1_y > begin_y) {
                                    if(selected_block_1_y < limit_y) {
                                        //cout<<"realizou operação de colisão caso 6"<<endl;
                                        blocks_list_to_print[selected_block]->setX(blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() + 10);
                                        return true;
                                    }
                                }
                                //caso 7
                                if(selected_block_2_y > begin_y) {
                                    if(selected_block_2_y < limit_y) {
                                        //cout<<"realizou operação de colisão caso 7"<<endl;
                                        blocks_list_to_print[selected_block]->setX(blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() + 10);
                                        return true;
                                    }
                                }
                            }
                        }
                        //caso 8 e 9
                        if(selected_block_1_y < begin_y) {
                            if(selected_block_2_y > limit_y) {
                                //caso 8
                                if(selected_block_1_x > begin_x) {
                                    if(selected_block_1_x < limit_x) {
                                        //cout<<"realizou operação de colisão caso 8"<<endl;
                                        blocks_list_to_print[selected_block]->setX(blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() + 10);
                                        return true;
                                    }
                                }
                                //caso 9
                                if(selected_block_3_x > begin_x) {
                                    if(selected_block_3_x < limit_x) {
                                        //cout<<"realizou operação de colisão caso 9"<<endl;
                                        blocks_list_to_print[selected_block]->setX(blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() + 10);
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}
void Interface :: draw_lines() {
    bool special_case = false;

    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            //caso especial
            if(blocks_list_to_print[i]->getNext1() != NULL) {
                if(blocks_list_to_print[i]->getNext2() != NULL) {
                    if(blocks_list_to_print[i]->getNext1()->getPrevious1() != NULL) {
                        if(blocks_list_to_print[i]->getNext1()->getPrevious2() != NULL) {
                            if(blocks_list_to_print[i]->getNext2()->getPrevious1() != NULL) {
                                if(blocks_list_to_print[i]->getNext2()->getPrevious2() != NULL) {
                                    if(blocks_list_to_print[i]->getNext1()->getPrevious1() == blocks_list_to_print[i]) {
                                        if(blocks_list_to_print[i]->getNext1()->getPrevious2() == blocks_list_to_print[i]) {
                                            if(blocks_list_to_print[i]->getNext2()->getPrevious1() == blocks_list_to_print[i]) {
                                                if(blocks_list_to_print[i]->getNext2()->getPrevious2() == blocks_list_to_print[i]) {
                                                    int start_x = blocks_list_to_print[i]->getPointOut1X();
                                                    int start_y = blocks_list_to_print[i]->getPointOut1Y();
                                                    int end_x = blocks_list_to_print[i]->getNext1()->getPointIn1X();
                                                    int end_y = blocks_list_to_print[i]->getNext1()->getPointIn1Y();
                                                    float angulo = -(atan2((end_x - start_x), (end_y - start_y)));
                                                    al_draw_line(start_x, start_y, end_x, end_y, primary_menu_color, 2);
                                                    al_draw_rotated_bitmap(arrow, (al_get_bitmap_width(arrow))/2, (al_get_bitmap_height(arrow))/2, end_x, end_y, angulo, 0);
                                                    start_x = blocks_list_to_print[i]->getPointOut2X();
                                                    start_y = blocks_list_to_print[i]->getPointOut2Y();
                                                    end_x = blocks_list_to_print[i]->getNext2()->getPointIn2X();
                                                    end_y = blocks_list_to_print[i]->getNext2()->getPointIn2Y();
                                                    angulo = -(atan2((end_x - start_x), (end_y - start_y)));
                                                    al_draw_line(start_x, start_y, end_x, end_y, primary_menu_color, 2);
                                                    al_draw_rotated_bitmap(arrow, (al_get_bitmap_width(arrow))/2, (al_get_bitmap_height(arrow))/2, end_x, end_y, angulo, 0);
                                                    special_case = true;
                                                }
                                            }
                                        } else {
                                            special_case = false;
                                        }
                                    } else {
                                        special_case = false;
                                    }
                                } else {
                                    special_case = false;
                                }
                            } else {
                                special_case = false;
                            }
                        } else {
                            special_case = false;
                        }
                    } else {
                        special_case = false;
                    }
                } else {
                    special_case = false;
                }
            } else {
                special_case = false;
            }
            if(special_case == false) {
                if(blocks_list_to_print[i]->getNext1() != NULL) {
                    if(blocks_list_to_print[i]->getNext1()->getPrevious1() != NULL) {
                        if(blocks_list_to_print[i]->getNext1()->getPrevious1() == blocks_list_to_print[i]) {
                            int start_x = blocks_list_to_print[i]->getPointOut1X();
                            int start_y = blocks_list_to_print[i]->getPointOut1Y();
                            int end_x = blocks_list_to_print[i]->getNext1()->getPointIn1X();
                            int end_y = blocks_list_to_print[i]->getNext1()->getPointIn1Y();
                            float angulo = -(atan2((end_x - start_x), (end_y - start_y)));
                            al_draw_line(start_x, start_y, end_x, end_y, primary_menu_color, 2);
                            al_draw_rotated_bitmap(arrow, (al_get_bitmap_width(arrow))/2, (al_get_bitmap_height(arrow))/2, end_x, end_y, angulo, 0);
                        }
                    }
                    if(blocks_list_to_print[i]->getNext1()->getPrevious2() != NULL) {
                        if(blocks_list_to_print[i]->getNext1()->getPrevious2() == blocks_list_to_print[i]) {
                            int start_x = blocks_list_to_print[i]->getPointOut1X();
                            int start_y = blocks_list_to_print[i]->getPointOut1Y();
                            int end_x = blocks_list_to_print[i]->getNext1()->getPointIn2X();
                            int end_y = blocks_list_to_print[i]->getNext1()->getPointIn2Y();
                            float angulo = -(atan2((end_x - start_x), (end_y - start_y)));
                            al_draw_line(start_x, start_y, end_x, end_y, primary_menu_color, 2);
                            al_draw_rotated_bitmap(arrow, (al_get_bitmap_width(arrow))/2, (al_get_bitmap_height(arrow))/2, end_x, end_y, angulo, 0);
                        }
                    }
                }
                if(blocks_list_to_print[i]->getNext2() != NULL) {
                    if(blocks_list_to_print[i]->getNext2()->getPrevious1() != NULL) {
                        if(blocks_list_to_print[i]->getNext2()->getPrevious1() == blocks_list_to_print[i]) {
                            int start_x = blocks_list_to_print[i]->getPointOut2X();
                            int start_y = blocks_list_to_print[i]->getPointOut2Y();
                            int end_x = blocks_list_to_print[i]->getNext2()->getPointIn1X();
                            int end_y = blocks_list_to_print[i]->getNext2()->getPointIn1Y();
                            float angulo = -(atan2((end_x - start_x), (end_y - start_y)));
                            al_draw_line(start_x, start_y, end_x, end_y, primary_menu_color, 2);
                            al_draw_rotated_bitmap(arrow, (al_get_bitmap_width(arrow))/2, (al_get_bitmap_height(arrow))/2, end_x, end_y, angulo, 0);
                        }
                    }
                    if(blocks_list_to_print[i]->getNext2()->getPrevious2() != NULL) {
                        if(blocks_list_to_print[i]->getNext2()->getPrevious2() == blocks_list_to_print[i]) {
                            int start_x = blocks_list_to_print[i]->getPointOut2X();
                            int start_y = blocks_list_to_print[i]->getPointOut2Y();
                            int end_x = blocks_list_to_print[i]->getNext2()->getPointIn2X();
                            int end_y = blocks_list_to_print[i]->getNext2()->getPointIn2Y();
                            float angulo = -(atan2((end_x - start_x), (end_y - start_y)));
                            al_draw_line(start_x, start_y, end_x, end_y, primary_menu_color, 2);
                            al_draw_rotated_bitmap(arrow, (al_get_bitmap_width(arrow))/2, (al_get_bitmap_height(arrow))/2, end_x, end_y, angulo, 0);
                        }
                    }
                }
            }
        }
    }
}
void Interface :: check_drop_arrow() {
    //checa se a flecha foi solta na entrada de algum bloco
    if(drawing_line == true) {
        for(int i=0; i<valor_maximo_blocos; i++) {
            if((blocks_list_to_print[i] != NULL) && (block_selected != i)){

                check_mouse_on_points(blocks_list_to_print[i]);
                if(blocks_list_to_print[block_selected] != NULL) {
                    if(blocks_list_to_print[i]->getIn1Selected() == true) {
                        if(number_of_selected_out == 1) {
                            //testa se não havia alguma ligação antes
                            if(blocks_list_to_print[block_selected]->getNext1() != NULL) {
                                //descobre se é o previous1 ou o 2
                                if(blocks_list_to_print[block_selected]->getNext1()->getPrevious1() == blocks_list_to_print[block_selected]) {
                                    blocks_list_to_print[block_selected]->getNext1()->setPrevious1(NULL);
                                } else {
                                    blocks_list_to_print[block_selected]->getNext1()->setPrevious2(NULL);
                                }
                            }
                            blocks_list_to_print[block_selected]->setNext1(blocks_list_to_print[i]);
                            blocks_list_to_print[i]->setPrevious1(blocks_list_to_print[block_selected]);
                            //teste se não está setando o mesmo bloco nas duas saídas
                            if(blocks_list_to_print[block_selected]->getNext1() == blocks_list_to_print[block_selected]->getNext2()) {
                                blocks_list_to_print[block_selected]->getNext2()->setPrevious2(NULL);
                                blocks_list_to_print[block_selected]->setNext2(NULL);
                            }
                        } else {
                            //testa se não havia alguma ligação antes
                            if(blocks_list_to_print[block_selected]->getNext2() != NULL) {
                                //descobre se é o previous1 ou o 2
                                if(blocks_list_to_print[block_selected]->getNext2()->getPrevious1() == blocks_list_to_print[block_selected]) {
                                    blocks_list_to_print[block_selected]->getNext2()->setPrevious1(NULL);
                                } else {
                                    blocks_list_to_print[block_selected]->getNext2()->setPrevious2(NULL);
                                }
                            }
                            blocks_list_to_print[block_selected]->setNext2(blocks_list_to_print[i]);
                            blocks_list_to_print[i]->setPrevious1(blocks_list_to_print[block_selected]);
                            //teste se não está setando o mesmo bloco nas duas saídas
                            if(blocks_list_to_print[block_selected]->getNext1() == blocks_list_to_print[block_selected]->getNext2()) {
                                blocks_list_to_print[block_selected]->getNext1()->setPrevious2(NULL);
                                blocks_list_to_print[block_selected]->setNext1(NULL);
                            }
                        }

                    } else if(blocks_list_to_print[i]->getIn2Selected() == true) {
                        if(number_of_selected_out == 1) {
                            //testa se não havia alguma ligação antes
                            if(blocks_list_to_print[block_selected]->getNext1() != NULL) {
                                //descobre se é o previous1 ou o 2
                                if(blocks_list_to_print[block_selected]->getNext1()->getPrevious1() == blocks_list_to_print[block_selected]) {
                                    blocks_list_to_print[block_selected]->getNext1()->setPrevious1(NULL);
                                } else {
                                    blocks_list_to_print[block_selected]->getNext1()->setPrevious2(NULL);
                                }
                            }
                            blocks_list_to_print[block_selected]->setNext1(blocks_list_to_print[i]);
                            blocks_list_to_print[i]->setPrevious2(blocks_list_to_print[block_selected]);
                            //teste se não está setando o mesmo bloco nas duas saídas
                            if(blocks_list_to_print[block_selected]->getNext1() == blocks_list_to_print[block_selected]->getNext2()) {
                                blocks_list_to_print[block_selected]->getNext2()->setPrevious1(NULL);
                                blocks_list_to_print[block_selected]->setNext2(NULL);
                            }
                        } else {
                            //testa se não havia alguma ligação antes
                            if(blocks_list_to_print[block_selected]->getNext2() != NULL) {
                                //descobre se é o previous1 ou o 2
                                if(blocks_list_to_print[block_selected]->getNext2()->getPrevious1() == blocks_list_to_print[block_selected]) {
                                    blocks_list_to_print[block_selected]->getNext2()->setPrevious1(NULL);
                                } else {
                                    blocks_list_to_print[block_selected]->getNext2()->setPrevious2(NULL);
                                }
                            }
                            blocks_list_to_print[block_selected]->setNext2(blocks_list_to_print[i]);
                            blocks_list_to_print[i]->setPrevious2(blocks_list_to_print[block_selected]);
                            //teste se não está setando o mesmo bloco nas duas saídas
                            if(blocks_list_to_print[block_selected]->getNext1() == blocks_list_to_print[block_selected]->getNext2()) {
                                blocks_list_to_print[block_selected]->getNext1()->setPrevious1(NULL);
                                blocks_list_to_print[block_selected]->setNext1(NULL);
                            }
                        }
                    }
                }
            }
        }
    }
}
void Interface :: check_drag_block_or_begin_arrow() {
    bool line_test = drawing_line;
    //testa se pressionou o mouse sobre algum bloco existente,
    //se sim seta a variável dizendo que o bloco está selecionado para poder arrastá-lo
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {

            check_mouse_on_points(blocks_list_to_print[i]);
            //checa se clicou nas bolinhas de saida dos blocos, se sim, seta as variáveis para desenhar as linhas de ligação
            if(blocks_list_to_print[i]->getOut1Selected() == true) {
                temporary_line_X = mouseX;
                temporary_line_Y = mouseY;
                drawing_line = true;
                block_selected = i;
                number_of_selected_out = 1;
            } else if(blocks_list_to_print[i]->getOut2Selected() == true) {
                temporary_line_X = mouseX;
                temporary_line_Y = mouseY;
                drawing_line = true;
                block_selected = i;
                number_of_selected_out = 2;
            } else if(drawing_line == line_test) {
                temporary_line_X = 0;
                temporary_line_Y = 0;
                drawing_line = false;
            }
            //checa se clicou sobre os blocos, se sim, seta as variáveis de seleção e seta as variáveis para poder arrastar
            if(black_sensor_menu_selected || color_sensor_menu_selected || ultrasonic_sensor_menu_selected || number_menu_selected || int_menu_selected || bool_menu_selected || logic_menu_selected) {

            } else if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX()+blocks_list_to_print[i]->getWidth())) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY()+blocks_list_to_print[i]->getHeight()))) {
                if((blocks_list_to_print[i]->getIn1Selected() == false) && (blocks_list_to_print[i]->getOut1Selected() == false) && (blocks_list_to_print[i]->getIn2Selected() == false) && (blocks_list_to_print[i]->getOut2Selected() == false)) {
                    blocks_list_to_print[i]->setDragging(true);
                    blocks_list_to_print[i]->setSelected(true);
                    mouse_aux_x = mouseX-blocks_list_to_print[i]->getX();
                    mouse_aux_y = mouseY-blocks_list_to_print[i]->getY();
                }
            } else {
                blocks_list_to_print[i]->setSelected(false);
            }
        }
    }
}
void Interface :: check_menu_selected() {
    // caso tenha clicado sobre um dos menus de ação ou sensor, seta as variáveis para arrastar com o mouse.
    switch(menu_selected) {
        case 18:
            dragging_walk_foward = true;
            break;
        case 19:
            dragging_turn_left = true;
            break;
        case 20:
            dragging_turn_right = true;
            break;
        case 24:
            dragging_black_sensor1 = true;
            break;
        case 25:
            dragging_black_sensor2 = true;
            break;
        case 26:
            dragging_black_sensor3 = true;
            break;
        case 27:
            dragging_black_sensor4 = true;
            break;
        case 28:
            dragging_black_sensor5 = true;
            break;
        case 29:
            dragging_color_sensor1 = true;
            break;
        case 30:
            dragging_color_sensor2 = true;
            break;
        case 31:
            dragging_ultrasonic_sensor1 = true;
            break;
        case 32:
            dragging_ultrasonic_sensor2 = true;
            break;
        case 33:
            dragging_ultrasonic_sensor3 = true;
            break;
        case 34:
            dragging_number_0 = true;
            break;
        case 35:
            dragging_number_1 = true;
            break;
        case 36:
            dragging_number_2 = true;
            break;
        case 37:
            dragging_number_3 = true;
            break;
        case 38:
            dragging_number_4 = true;
            break;
        case 39:
            dragging_number_5 = true;
            break;
        case 40:
            dragging_number_6 = true;
            break;
        case 41:
            dragging_number_7 = true;
            break;
        case 42:
            dragging_number_8 = true;
            break;
        case 43:
            dragging_number_9 = true;
            break;
        case 44: // INT
            dragging_int_A = true;
            break;    
        case 45:
            dragging_int_B = true;
            break;    
        case 46:
            dragging_int_C = true;
            break;    
        case 47:
            dragging_int_D = true;
            break;    
        case 48:
            dragging_int_E = true;
            break;    
        case 49:
            dragging_int_F = true;
            break;    
        case 50:
            dragging_int_G = true;
            break;    
        case 51:
            dragging_int_H = true;
            break;    
        case 52:
            dragging_int_I = true;
            break;    
        case 53:
            dragging_int_J = true;
            break;    
        case 54: // BOOL
            dragging_bool_A = true;
            break;    
        case 55: 
            dragging_bool_B = true;
            break;    
        case 56: 
            dragging_bool_C = true;
            break;    
        case 57: 
            dragging_bool_D = true;
            break;    
        case 58: 
            dragging_bool_E = true;
            break;    
        case 59: 
            dragging_bool_F = true;
            break;    
        case 60: 
            dragging_bool_G = true;
            break;    
        case 61: 
            dragging_bool_H = true;
            break;    
        case 62: 
            dragging_bool_I = true;
            break;    
        case 63: 
            dragging_bool_J = true;
            break;
        case 64: //LOGIC
            dragging_equal = true;
            break;    
        case 65: 
            dragging_greater_equal = true;
            break;    
        case 66: 
            dragging_less_equal = true;
            break;    
        case 67: 
            dragging_greater = true;
            break;    
        case 68: 
            dragging_less = true;
            break;    
        case 69: 
            dragging_unequal = true;
            break;    
        case 70: //MATH
            dragging_add = true;
            break;
        case 71: 
            dragging_div = true;
            break;  
        case 72: 
            dragging_exponent = true;
            break;  
        case 73: 
            dragging_mult = true;
            break;  
        case 74: 
            dragging_sub = true;
            break;  
    }
    if(menu_selected == CONDITIONAL_BLOCK) {
        mouse_aux_x = 60;
        mouse_aux_y = 35;
        menu_click = CONDITIONAL_BLOCK;
        cout<<"menu_selected == CONDITIONAL_BLOCK"<<endl;
    }
    if(menu_selected == ACTION_BLOCK) {
        mouse_aux_x = 45;
        mouse_aux_y = 20;
        menu_click = ACTION_BLOCK;
        cout<<"menu_selected == ACTION_BLOCK"<<endl;
    }
    if(menu_selected == START_BLOCK) {
        mouse_aux_x = 40;
        mouse_aux_y = 15;
        menu_click = START_BLOCK;
        cout<<"menu_selected == START_BLOCK"<<endl;
    }
    if(menu_selected == END_BLOCK) {
        mouse_aux_x = 40;
        mouse_aux_y = 15;
        menu_click = END_BLOCK;
        cout<<"menu_selected == END_BLOCK"<<endl;
    }
    if(menu_selected == MERGE_BLOCK) {
        mouse_aux_x = 15;
        mouse_aux_y = 10;
        menu_click = MERGE_BLOCK;
        cout<<"menu_selected == MERGE_BLOCK"<<endl;
    }
    if(menu_selected == LOOP_BLOCK) {
        mouse_aux_x = 30;
        mouse_aux_y = 40;
        menu_click = LOOP_BLOCK;
        cout<<"menu_selected == LOOP_BLOCK"<<endl;
    }
    //v3
    if(menu_selected == ATTRIBUTION_BLOCK) {
        mouse_aux_x = 80;
        mouse_aux_y = 20;
        menu_click = ATTRIBUTION_BLOCK;
        cout<<"menu_selected == ATTRIBUTION_BLOCK"<<endl;
    }
    if(menu_selected == MATH_BLOCK) {
        mouse_aux_x = 110;
        mouse_aux_y = 20;
        menu_click = MATH_BLOCK;
        cout<<"menu_selected == MATH_BLOCK"<<endl;
    }
    if(menu_selected == NEW_CONDITIONAL_BLOCK) {
        mouse_aux_x = 80;
        mouse_aux_y = 20;
        menu_click = NEW_CONDITIONAL_BLOCK;
        cout<<"menu_selected == NEW_CONDITIONAL_BLOCK"<<endl;
    }
    if(menu_selected == WHILE_BLOCK) {
        mouse_aux_x = 80;
        mouse_aux_y = 20;
        menu_click = WHILE_BLOCK;
        cout<<"menu_selected == WHILE_BLOCK"<<endl;
    }

    //se o sub menu estava aberto e clicou fora, fecha o sub menu
    if(menu_selected != BLACK_TAPE_SENSOR_MENU) {
        black_sensor_menu_selected = false;
    }
    if(menu_selected != SENSOR_COLOR_MENU) {
        color_sensor_menu_selected = false;
    }
    if(menu_selected != ULTRASONIC_SENSOR_MENU) {
        ultrasonic_sensor_menu_selected = false;
    }
    if(menu_selected != N_LOOP_BLOCK) {
        number_menu_selected = false;
    }
    if(menu_selected != INT_MENU) {
        int_menu_selected = false;
    }
    if(menu_selected != BOOL_MENU) {
        bool_menu_selected = false;
    }
    if(menu_selected != LOGIC_AND_MATH_MENU) {
        logic_menu_selected = false;
    }
}
int Interface :: getMenuClick() {
    //cout<<"getMenuClick() = "<<menu_click<<endl;
    return menu_click;
}
int Interface :: getMouseX() {
    return mouseX;
}
int Interface :: getMouseY() {
    return mouseY;
}
bool Interface :: getExecuting() {
    return executing;
}
int Interface :: callMessage(int i) {
    switch(i) {
        case 1:
            al_show_native_message_box(display, "Fluxprog", "ERRO", "Deve existir um, e apenas um, bloco de início.", "Ok", ALLEGRO_MESSAGEBOX_WARN);
            break;
        case 2:
            al_show_native_message_box(display, "Fluxprog", "ERRO", "Deve existir pelo menos um bloco de fim.", "Ok", ALLEGRO_MESSAGEBOX_WARN);
            break;
        case 3:
            al_show_native_message_box(display, "Fluxprog", "ERRO", "Todos os blocos devem estar interligados.", "Ok", ALLEGRO_MESSAGEBOX_WARN);
            break;
        case 4:
            al_show_native_message_box(display, "Fluxprog", "ERRO", "Todos os blocos devem estar com as suas funções ou sensores determinados.", "Ok", ALLEGRO_MESSAGEBOX_WARN);
            break;
        case 5:
            al_show_native_message_box(display, "Fluxprog", "ERRO", "Falha ao abrir o programa BackEnd", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
            break;
        case 6:
            al_show_native_message_box(display, "Fluxprog", "ERRO", "Bluetooth não encontrado", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
            break;
        case 7:
            al_show_native_message_box(display, "Fluxprog", "ERRO", "V-Rep não encontrado", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
            break;
        case 8:
            al_show_native_message_box(display, "Fluxprog", " ", "Conectado ao V-REP com sucesso", "Ok", 0);
            break;
        case 9:
            al_show_native_message_box(display, "Fluxprog", " ", "Conectado ao robô físico com sucesso", "Ok", 0);
            break;
        case 10:
            al_show_native_message_box(display, "Fluxprog", "ERRO", "Reinicie a simulação do V-Rep e tente novamente", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
            break;
        case 11:
            al_show_native_message_box(display, "Fluxprog", "ERRO", "Troque a porta onde o bluetooth está conectado e tente novamente", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
            break;
        case 12:
            al_show_native_message_box(display, "Fluxprog", " ", "Desconectado ao V-Rep", "Ok", 0);
            break;
        case 13:
            al_show_native_message_box(display, "Fluxprog", " ", "Desconectado ao Robo", "Ok", 0);
            break;
        case 14:
            al_show_native_message_box(display, "Fluxprog", "ERRO", "O programa não está conectado.", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
            break;
        case 15:
            al_show_native_message_box(display, "Fluxprog", " ", "Arquivo salvo com sucesso", "Ok", 0);
            break;
        case 16:
            al_show_native_message_box(display, "Fluxprog", " ", "Falha ao carregar o fluxograma", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
            break;
        case 17:
            al_show_native_message_box(display, "Fluxprog", "ERRO", "Conexão Perdida.\n Reinicie a simulação do V-Rep e reconecte", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
            break;
        case 18:
            al_show_native_message_box(display, "Fluxprog", "ERRO", "Conexão Perdida.\n Verifique as condições do robô e reconecte", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
            break;
        case 19:
            al_show_native_message_box(display, "Fluxprog", "ERRO", "O Robô Colidiu!", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
            break;
        case 20:
            int k = al_show_native_message_box(display, "Fluxprog", "ERRO", "Deseja que os blocos que não estão interligados sejam excluídos?", NULL, ALLEGRO_MESSAGEBOX_YES_NO);
            if(k == 1) {
                return 0;
            } else if(k == 2) {
                return 1;
            }

            break;
    }
    return 1;
}
int Interface :: getImageHeight(int i) {
    int height = 0;
    switch(i) {
        case 1:
            height = al_get_bitmap_height(DECISION_BLOCK[0]);
            break;
        case 2:
            height = al_get_bitmap_height(FUNCTION_BLOCK[0]);
            break;
        case 3:
            height = al_get_bitmap_height(START_BLOCK_IMG[0]);
            break;
        case 4:
            height = al_get_bitmap_height(END_BLOCK_IMG[0]);
            break;
        case 5:
            height = al_get_bitmap_height(MERGE_BLOCK_IMG[0]);
            break;
        case 6:
            height = al_get_bitmap_height(LOOP_BLOCK_IMG[0]);
            break;
        case 7:
            height = al_get_bitmap_height(ATTRIBUTION_BLOCK_IMG[0]);
            break;
        case 8:
            height = al_get_bitmap_height(MATH_BLOCK_IMG[0]);
            break;
        case 9:
            height = al_get_bitmap_height(NEW_CONDITIONAL_BLOCK_IMG[0]);
            break;
        case 10:
            height = al_get_bitmap_height(WHILE_BLOCK_IMG[0]);
            break;
    }
    return height;
}
int Interface :: getImageWidth(int i) {
    int width = 0;
    switch(i) {
        case 1:
            width = al_get_bitmap_width(DECISION_BLOCK[0]);
            break;
        case 2:
            width = al_get_bitmap_width(FUNCTION_BLOCK[0]);
            break;
        case 3:
            width = al_get_bitmap_width(START_BLOCK_IMG[0]);
            break;
        case 4:
            width = al_get_bitmap_width(END_BLOCK_IMG[0]);
            break;
        case 5:
            width = al_get_bitmap_width(MERGE_BLOCK_IMG[0]);
            break;
        case 6:
            width = al_get_bitmap_width(LOOP_BLOCK_IMG[0]);
            break;
        case 7:
            width = al_get_bitmap_width(ATTRIBUTION_BLOCK_IMG[0]);
            break;
        case 8:
            width = al_get_bitmap_width(MATH_BLOCK_IMG[0]);
            break;
        case 9:
            width = al_get_bitmap_width(NEW_CONDITIONAL_BLOCK_IMG[0]);
            break;
        case 10:
            width = al_get_bitmap_width(WHILE_BLOCK_IMG[0]);
            break;
    }
    return width;
}
void Interface :: setConnectedSimulator(bool c) {
    connected_simulator = c;
}
void Interface :: setConnectedRobot(bool c) {
    connected_robot = c;
}
void Interface :: setExecutingFluxogram(bool e) {
    executing_fluxogram = e;
}
bool Interface :: check_enable_menu() {
    if(dragging_number_0 || dragging_number_1 || dragging_number_2 || dragging_number_3 || dragging_number_4) {
        return false;
    } else if(dragging_number_5 || dragging_number_6 || dragging_number_7 || dragging_number_8 || dragging_number_9) {
        return false;
    } else if(dragging_black_sensor1 || dragging_black_sensor2 || dragging_black_sensor3 || dragging_black_sensor4 || dragging_black_sensor5) {
        return false;
    } else if(dragging_color_sensor1 || dragging_color_sensor2) {
        return false;
    } else if(dragging_ultrasonic_sensor1 || dragging_ultrasonic_sensor2 || dragging_ultrasonic_sensor3) {
        return false;
    } else if(dragging_walk_foward || dragging_turn_left || dragging_turn_right) {
        return false;
    } else if(dragging_int_A || dragging_int_B || dragging_int_C || dragging_int_D || dragging_int_E) {
        return false;
    } else if(dragging_int_F || dragging_int_G || dragging_int_H || dragging_int_I || dragging_int_J) {
        return false;
    } else if(dragging_bool_A || dragging_bool_B || dragging_bool_C || dragging_bool_D || dragging_bool_E) {
        return false;
    } else if(dragging_bool_F || dragging_bool_G || dragging_bool_H || dragging_bool_I || dragging_bool_J) {
        return false;
    } else if(dragging_greater || dragging_greater_equal || dragging_less || dragging_less_equal || dragging_equal || dragging_unequal) {
        return false;
    } else if(dragging_sub || dragging_add || dragging_mult || dragging_div || dragging_exponent) {
        return false;
    } else {
        for(int i=0; i<valor_maximo_blocos; i++) {
            if(blocks_list_to_print[i] != NULL) {
                if(blocks_list_to_print[i]->getDragging() == true) {
                    return false;
                }
            }
        }
    }
    return true;
}
void Interface :: check_scrolling() {
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            int old_y = blocks_list_to_print[i]->getY();
            blocks_list_to_print[i]->setY(old_y + mouseZ);
        }
    }
}
void Interface :: draw_scroll_bar() {
    al_draw_filled_rectangle(al_get_display_width(display)-roll_bar_width, scroll_bar_y, al_get_display_width(display), scroll_bar_y+roll_bar_height, scroll_bar_color);
}
void Interface :: reset_scrollbar() {
    scroll_bar_x = al_get_display_width(display)-roll_bar_width;
    scroll_bar_y = scroll_bar_y_begin;
}
char* Interface :: save_file_window() {
    char* path = new char[100];
    strcpy(path, ".fluxpg");
    ALLEGRO_FILECHOOSER *select_file = al_create_native_file_dialog(path, "Salvar Fluxograma","*.fluxpg*" , ALLEGRO_FILECHOOSER_SAVE);
    al_show_native_file_dialog(display, select_file);
    if(al_get_native_file_dialog_path(select_file, 0) != NULL) {
        strcpy(path, al_get_native_file_dialog_path(select_file, 0));
        return path;
    }

    return NULL;
}
char* Interface :: open_file_window() {
    char* path = new char[100];
    strcpy(path, ".fluxpg");
    ALLEGRO_FILECHOOSER *select_file = al_create_native_file_dialog(path, "Abrir Fluxograma","*.fluxpg*" , ALLEGRO_FILECHOOSER_FILE_MUST_EXIST);
    al_show_native_file_dialog(display, select_file);
    if(al_get_native_file_dialog_path(select_file, 0) != NULL) {
        strcpy(path, al_get_native_file_dialog_path(select_file, 0));
        return path;
    }

    return NULL;
}
