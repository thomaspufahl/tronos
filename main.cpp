#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h> // getch()
#include <windows.h>

using namespace std;
#include "rlutil.h"

int main()
{
    /// RENDERIZAR CONSOLA EN TAMAÑO COMPLETO
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    /// CONFIGURACION DEL BUFFER Y LA VENTANA DE LA CONSOLA
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // Cambiar tamaño del búfer
    COORD bufferSize = { (SHORT)150, (SHORT)38 };
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    // Cambiar tamaño de la ventana
    SMALL_RECT windowSize = { 0, 0, (SHORT)(150 - 1), (SHORT)(38 - 1) };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);


    /// RENDERIZAR CONSOLA EN PANTALLA COMPLETO

    SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000); // Alt + Enter

    /// JUGABILIDAD
    const int juego_presupuesto_base = 50000;
    int juego_oro = juego_presupuesto_base;

    const int juego_soldados_base = 0;
    int juego_soldados = juego_soldados_base;

    const int juego_comida_base = 0;
    int juego_comida = juego_comida_base;

    // TIENDA
    const int tienda_valor_x_soldado       = 1000;
    const int tienda_valor_x_comida        = 500;
    const int tienda_valor_x_mejora_pasiva = 10000;

    // articulo: objeto que se compra en la tienda, el valor refiere a la cantidad.
    const int tienda_articulo_soldados        = 1000;
    const int tienda_articulo_comida          = 500;
    const float tienda_articulo_mejora_pasiva = 0.1;

    // BATALLA
    const int batalla_duracion_guerra = 10;
    int batalla_actual = 0;

    // pasiva LANNISTER
    const float batalla_pasiva_base = 0.5;
    float batalla_pasiva_probabilidad = batalla_pasiva_base;

    const float batalla_pasiva_beneficio = 2; // 20% extra en oro ganado
    float batalla_pasiva_nro_random;

    bool batalla_pasiva_eval_activada;

    // combate
    int batalla_aux_oro_ganado            = 0;
    int batalla_aux_comida_gastado;
    int batalla_aux_comida_ganada_pasiva  = 0;
    int batalla_aux_soldados_gastado;

    bool batalla_victoria_eval;

    const int max_perdida_comida = 1000;
    const int max_perdida_soldados = 500;


    // ESTADISTICAS
    int est_batalla_victorias_cantidad   = 0;
    int est_batalla_derrotas_cantidad    = 0;

    int est_batalla_pasiva_uso_cantidad  = 0;
    int est_batalla_pasiva_comida_ganada = 0;

    int est_batalla_oro_ganado           = 0;
    int est_batalla_comida_perdido       = 0;
    int est_batalla_soldados_perdido     = 0;

    int est_tienda_total_gastado_oro     = 0;
    int est_tienda_total_gastado_comida  = 0;
    int est_tienda_total_gastado_soldados= 0;

    /// Auxiliares

    // ingresos
    int opcion_menu=0;
    int opcion_tienda=0;
    int tecla_menu;
    int tecla_tienda;

    // Menuess
    int while_menu_batalla = 1;
    int opcion_menu_batalla = 0;
    int while_menu_principal = 1;
    int while_menu_tienda = 1;

    int aux_tienda_gasto;

    // Generador de numeros aleatorios
    srand(time(0));

    //Logo dimensiones
    const int logo_dimensionY = 30;
    const int logo_dimensionX = 35;

    const int baratheon_logo[logo_dimensionY][logo_dimensionX] =
    {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,2,1,1,1,1,1,1,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,1,0,1,0,1,1,1,1,1,1,1,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,1,1,2,2,2,2,0,0,1,0,2,1,1,1,1,1,1,1},
        {1,2,2,2,2,2,2,2,2,2,2,1,1,1,1,0,0,0,1,1,2,2,1,0,0,0,1,2,1,1,1,1,1,1,1},
        {1,2,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,1,2,0,0,0,1,1,2,1,1,1,1,1,1,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,1,2,2,2,1,1,1,1,1,1,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,0,1,1,0,0,1,2,2,2,2,2,2,1,1,1,1,1,1},
        {1,2,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1},
        {1,2,2,2,2,2,2,2,1,1,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1},
        {1,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1},
        {1,2,2,2,2,2,1,0,2,1,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1},
        {1,2,2,2,2,1,0,1,2,1,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1},
        {1,2,2,2,2,1,1,2,2,2,0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1},
        {0,2,2,2,2,2,2,2,0,0,0,0,1,0,0,0,0,0,0,0,0,1,2,2,2,2,2,2,1,1,1,1,1,2,0},
        {0,2,2,2,2,2,2,1,0,2,2,2,2,2,1,0,0,0,0,0,0,0,2,2,2,2,2,1,1,1,1,1,1,1,0},
        {0,1,2,2,2,2,2,0,0,2,2,2,2,2,2,2,0,0,0,0,0,0,0,1,2,1,2,1,1,1,1,1,1,1,0},
        {0,0,2,2,2,2,2,0,1,2,2,2,2,2,2,2,1,0,0,0,0,0,0,1,2,1,1,1,1,1,1,1,2,0,0},
        {0,0,1,2,2,2,1,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,2,1,1,2,1,1,1,1,1,1,1,0,0},
        {0,0,0,2,1,2,2,1,2,2,2,2,2,2,2,2,1,0,0,0,0,0,1,2,2,1,1,1,1,1,1,2,0,0,0},
        {0,0,0,1,2,1,2,2,2,2,2,2,2,2,2,2,2,0,0,2,1,0,0,0,1,2,1,1,1,1,2,1,0,0,0},
        {0,0,0,0,1,2,1,2,2,2,2,2,2,1,1,1,0,0,0,1,2,2,1,0,1,2,1,1,1,1,1,0,0,0,0},
        {0,0,0,0,0,1,2,1,1,1,2,1,0,0,0,1,1,1,1,1,1,1,2,0,1,1,1,1,2,1,0,0,0,0,0},
        {0,0,0,0,0,0,1,2,1,1,2,1,0,1,1,2,2,2,2,1,1,2,1,0,1,1,1,2,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,1,2,1,1,2,2,2,1,1,1,1,1,1,1,0,0,1,1,1,2,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,1,2,1,1,1,1,1,1,1,1,1,1,1,1,2,2,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,1,1,2,1,1,1,1,1,1,1,1,1,2,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,1,1,2,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

    };

    while(true)
    {
        ///UI
        {
            ///RECUADRO FRASE ----------------------------------------------------------------------------
            rlutil::locate(53,26);  // +9
            rlutil::setBackgroundColor(rlutil::BROWN);
            cout<<"                                             "<<endl;
            rlutil::locate(53,30);  // +9
            rlutil::setBackgroundColor(rlutil::BROWN);
            cout<<"                                             "<<endl;
            for (int i=26; i<30; i++)
            {
                rlutil::setBackgroundColor(rlutil::BROWN);
                rlutil::locate(53,i);  // +9
                cout<<"  "<<endl;
            }
            for (int i=26; i<30; i++)
            {
                rlutil::setBackgroundColor(rlutil::BROWN);
                rlutil::locate(96,i);  // +9
                cout<<"  "<<endl;
            }
            rlutil::setBackgroundColor(rlutil::BLACK);
            rlutil::setColor(rlutil::YELLOW);

            rlutil::locate(57, 28), cout << "OURS IS THE FURY!";  // +9

            /// Se hace esto para pintar nuevamente de negro el fondo y blanco las letras por debajo
            rlutil::setBackgroundColor(rlutil::BLACK);
            rlutil::setColor(rlutil::WHITE);

            ///LOGO GAME OF THRONES ----------------------------------------------------------------------------

            rlutil::locate(27,32);  // +9
            cout << "  _____          __  __ ______    ____  ______   _______ _    _ _____   ____  _   _ ______  _____ " << endl;
            rlutil::locate(27,33);  // +9
            cout << " / ____|   /\\   |  \\/  |  ____|  / __ \\|  ____| |__   __| |  | |  __ \\ / __ \\| \\ | |  ____|/ ____|" << endl;
            rlutil::locate(27,34);  // +9
            cout << "| |  __   /  \\  | \\  / | |__    | |  | | |__       | |  | |__| | |__) | |  | |  \\| | |__  | (___  " << endl;
            rlutil::locate(27,35);  // +9
            cout << "| | |_ | / /\\ \\ | |\\/| |  __|   | |  | |  __|      | |  |  __  |  _  /| |  | | . ` |  __|  \\___ \\ " << endl;
            rlutil::locate(27,36);  // +9
            cout << "| |__| |/ ____ \\| |  | | |____  | |__| | |         | |  | |  | | | \\ \\| |__| | |\\  | |____ ____) |" << endl;
            rlutil::locate(27,37);  // +9
            cout << " \\_____/_/    \\_\\_|  |_|______|  \\____/|_|         |_|  |_|  |_|_|  \\_\\\\____/|_| \\_|______|_____/ " << endl;

            ///LOGOS ESCUDO BARATHEON ----------------------------------------------------------------------------
            for (int i = 0; i < logo_dimensionY; i++)
            {
                for (int j = 0; j < logo_dimensionX; j++)
                {
                    rlutil::locate(8 + j, 2 + i);  // +9
                    switch (baratheon_logo[i][j])
                    {
                    case 1:
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        cout << " ";
                        break;
                    case 2:
                        rlutil::setBackgroundColor(rlutil::YELLOW);
                        cout << " ";
                        break;
                    default:
                        rlutil::setBackgroundColor(rlutil::BLACK);
                        cout << " ";
                    }
                }
            }

            for (int i = 0; i < logo_dimensionY; i++)
            {
                for (int j = 0; j < logo_dimensionX; j++)
                {
                    rlutil::locate(108 + j, 2 + i);  // +9
                    switch (baratheon_logo[i][j])
                    {
                    case 1:
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        cout << " ";
                        break;
                    case 2:
                        rlutil::setBackgroundColor(rlutil::YELLOW);
                        cout << " ";
                        break;
                    default:
                        rlutil::setBackgroundColor(rlutil::BLACK);
                        cout << " ";
                    }
                }
            }

            ///RECUADRO EXTERIOR ----------------------------------------------------------------------------

            rlutil::setBackgroundColor(rlutil::BROWN);
            rlutil::locate(52,3);
            cout<<"                                             "<<endl;

            for (int i=3; i<23; i++)
            {
                rlutil::setBackgroundColor(rlutil::BROWN);
                rlutil::locate(52,i+1);
                cout<<"  "<<endl;
            }
            rlutil::setBackgroundColor(rlutil::BROWN);
            rlutil::locate(54,23);
            cout<<"                                           "<<endl;

            for (int i=2; i<23; i++)
            {
                rlutil::setBackgroundColor(rlutil::BROWN);
                rlutil::locate(97,i+1);
                cout<<"  "<<endl;
            }

            /// RECUADRO "JUEGO DE TRONOS"
            rlutil::setBackgroundColor(rlutil::BROWN);
            rlutil::setColor(rlutil::WHITE);
            rlutil::locate(56,5);
            cout<<"---------------------------------------"<<endl;

            rlutil::setBackgroundColor(rlutil::YELLOW);
            rlutil::setColor(rlutil::WHITE);
            rlutil::locate(56,6);
            cout<<"                                       "<<endl;

            rlutil::setBackgroundColor(rlutil::YELLOW);
            rlutil::setColor(rlutil::BLACK);
            rlutil::locate(60,6);
            cout<<"        JUEGO DE TRONOS        "<<endl;

            rlutil::setBackgroundColor(rlutil::BROWN);
            rlutil::setColor(rlutil::WHITE);
            rlutil::locate(56,7);
            cout<<"---------------------------------------"<<endl;
            rlutil::setBackgroundColor(rlutil::BLACK);

            rlutil::hidecursor();

            /// RECUADRO "baratheon"
            rlutil::setBackgroundColor(rlutil::YELLOW);
            rlutil::setColor(rlutil::BLACK);
            rlutil::locate(56,10);
            cout<<"-------------  BARATHEON  -------------"<<endl;
            rlutil::setBackgroundColor(rlutil::BLACK);
        }
        ///FIN UI

        /// DATOS DEL CLAN
        rlutil::locate(56,12);
        rlutil::setColor(rlutil::YELLOW);
        cout << "     |Batallas realizadas : " << batalla_actual             << endl;
        rlutil::locate(56,14);
        cout << "     |Oro                 : " << juego_oro                  << endl;
        rlutil::locate(56,15);
        cout << "     |Comida              : " << juego_comida               << endl;
        rlutil::locate(56,16);
        cout << "     |Soldados            : " << juego_soldados             << endl;
        rlutil::locate(56,17);
        cout << "     |Pasiva              : " << batalla_pasiva_probabilidad<< endl;
        rlutil::setColor(rlutil::WHITE);


        ///MENU PRINCIPAL DEL JUEGO

        while_menu_principal = true;
        while(while_menu_principal)  //While para mejorar
        {

            /// PINTAR "IR A LA BATALLA"
            if(opcion_menu==0)
            {
                rlutil::setBackgroundColor(rlutil::BROWN);
                rlutil::locate(56,19);
                cout<<" "<<(char)175<<"          IR A LA BATALLA          "<<(char)174<<" ";
            }
            else
            {
                rlutil::setBackgroundColor(rlutil::BLACK);
                rlutil::locate(56,19);
                cout<<"            IR A LA BATALLA            "<<endl;
            }

            /// PINTAR "TIENDA"
            if(opcion_menu==1)
            {
                rlutil::setBackgroundColor(rlutil::BROWN);
                rlutil::locate(56,20);
                cout<<" "<<(char)175<<"              TIENDA               "<<(char)174<<" ";
            }
            else
            {
                rlutil::setBackgroundColor(rlutil::BLACK);
                rlutil::locate(56,20);
                cout<<"                TIENDA                 "<<endl;
            }

            /// PINTAR "SALIR DEL JUEGO"
            if(opcion_menu==2)
            {
                rlutil::setBackgroundColor(rlutil::BROWN);
                rlutil::locate(56,21);
                cout<<" "<<(char)175<<"          SALIR DEL JUEGO          "<<(char)174<<" ";
            }
            else
            {
                rlutil::setBackgroundColor(rlutil::BLACK);
                rlutil::locate(56,21);
                cout<<"            SALIR DEL JUEGO            "<<endl;
            }

            rlutil::setBackgroundColor(rlutil::BLACK);

            /// LOGICA DEL MENU ----------------------------------------------------------------------------

            tecla_menu = rlutil::getkey();
            switch(tecla_menu)
            {
            case 14: //ARRIBA
                opcion_menu--;
                if(opcion_menu<0)
                {
                    opcion_menu=2;
                }
                break;
            case 15: //ABAJO
                opcion_menu++;
                if(opcion_menu>2)
                {
                    opcion_menu=0;
                }
                break;
            case 1: //ENTER
                system("cls");
                while_menu_principal = false;
            }
        }

        switch(opcion_menu)
        {
        case 0: /// MENU BATALLA
        {

            while(true)
            {

                ///VALIDO APTITUD PARA LA GUERRA
                if (batalla_actual >= batalla_duracion_guerra)
                {
                    rlutil::locate(43,15);
                    cout << "No hay mas batallas por luchar soldado..." << endl;
                    rlutil::locate(53,16);
                    cout << "Ya puedes descansar" << endl;
                    rlutil::locate(44,20);
                    system("pause");
                    system("cls");
                    break;
                }
                if(juego_comida < max_perdida_comida)
                {
                    rlutil::locate(43,15);
                    cout << "No tienes la cantidad de comida suficiente para ir a la batalla!" << endl;
                    rlutil::locate(49,16);
                    cout << "Compra mas comida en la tienda y vuelve mas tarde..." << endl;
                    rlutil::locate(55,20);
                    system("pause");
                    system("cls");
                    break;
                }

                if(juego_soldados < max_perdida_soldados)
                {
                    rlutil::locate(43,15);
                    cout << "No tienes la cantidad de soldados suficientes para ir a la batalla!" << endl;
                    rlutil::locate(49,16);
                    cout << "Compra mas soldados en la tienda y vuelve mas tarde..." << endl;
                    rlutil::locate(55,20);
                    system("pause");
                    system("cls");
                    break;
                }


                ///UI
                {
                    ///RECUADRO FRASE ----------------------------------------------------------------------------
                    rlutil::locate(53,26);  // +9
                    rlutil::setBackgroundColor(rlutil::BROWN);
                    cout<<"                                             "<<endl;
                    rlutil::locate(53,30);  // +9
                    rlutil::setBackgroundColor(rlutil::BROWN);
                    cout<<"                                             "<<endl;
                    for (int i=26; i<30; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        rlutil::locate(53,i);  // +9
                        cout<<"  "<<endl;
                    }
                    for (int i=26; i<30; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        rlutil::locate(96,i);  // +9
                        cout<<"  "<<endl;
                    }
                    rlutil::setBackgroundColor(rlutil::BLACK);
                    rlutil::setColor(rlutil::YELLOW);

                    rlutil::locate(57, 28), cout << "OURS IS THE FURY!";  // +9

                    /// Se hace esto para pintar nuevamente de negro el fondo y blanco las letras por debajo
                    rlutil::setBackgroundColor(rlutil::BLACK);
                    rlutil::setColor(rlutil::WHITE);

                    ///LOGO GAME OF THRONES ----------------------------------------------------------------------------

                    rlutil::locate(27,32);  // +9
                    cout << "  _____          __  __ ______    ____  ______   _______ _    _ _____   ____  _   _ ______  _____ " << endl;
                    rlutil::locate(27,33);  // +9
                    cout << " / ____|   /\\   |  \\/  |  ____|  / __ \\|  ____| |__   __| |  | |  __ \\ / __ \\| \\ | |  ____|/ ____|" << endl;
                    rlutil::locate(27,34);  // +9
                    cout << "| |  __   /  \\  | \\  / | |__    | |  | | |__       | |  | |__| | |__) | |  | |  \\| | |__  | (___  " << endl;
                    rlutil::locate(27,35);  // +9
                    cout << "| | |_ | / /\\ \\ | |\\/| |  __|   | |  | |  __|      | |  |  __  |  _  /| |  | | . ` |  __|  \\___ \\ " << endl;
                    rlutil::locate(27,36);  // +9
                    cout << "| |__| |/ ____ \\| |  | | |____  | |__| | |         | |  | |  | | | \\ \\| |__| | |\\  | |____ ____) |" << endl;
                    rlutil::locate(27,37);  // +9
                    cout << " \\_____/_/    \\_\\_|  |_|______|  \\____/|_|         |_|  |_|  |_|_|  \\_\\\\____/|_| \\_|______|_____/ " << endl;

                    ///LOGOS ESCUDO BARATHEON ----------------------------------------------------------------------------
                    for (int i = 0; i < logo_dimensionY; i++)
                    {
                        for (int j = 0; j < logo_dimensionX; j++)
                        {
                            rlutil::locate(8 + j, 2 + i);  // +9
                            switch (baratheon_logo[i][j])
                            {
                            case 1:
                                rlutil::setBackgroundColor(rlutil::BROWN);
                                cout << " ";
                                break;
                            case 2:
                                rlutil::setBackgroundColor(rlutil::YELLOW);
                                cout << " ";
                                break;
                            default:
                                rlutil::setBackgroundColor(rlutil::BLACK);
                                cout << " ";
                            }
                        }
                    }

                    for (int i = 0; i < logo_dimensionY; i++)
                    {
                        for (int j = 0; j < logo_dimensionX; j++)
                        {
                            rlutil::locate(108 + j, 2 + i);  // +9
                            switch (baratheon_logo[i][j])
                            {
                            case 1:
                                rlutil::setBackgroundColor(rlutil::BROWN);
                                cout << " ";
                                break;
                            case 2:
                                rlutil::setBackgroundColor(rlutil::YELLOW);
                                cout << " ";
                                break;
                            default:
                                rlutil::setBackgroundColor(rlutil::BLACK);
                                cout << " ";
                            }
                        }
                    }

                    ///RECUADRO EXTERIOR ----------------------------------------------------------------------------

                    rlutil::setBackgroundColor(rlutil::BROWN);
                    rlutil::locate(52,3);
                    cout<<"                                             "<<endl;

                    for (int i=3; i<23; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        rlutil::locate(52,i+1);
                        cout<<"  "<<endl;
                    }
                    rlutil::setBackgroundColor(rlutil::BROWN);
                    rlutil::locate(54,23);
                    cout<<"                                           "<<endl;

                    for (int i=2; i<23; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        rlutil::locate(97,i+1);
                        cout<<"  "<<endl;
                    }

                    /// RECUADRO "JUEGO DE TRONOS"
                    rlutil::setBackgroundColor(rlutil::BROWN);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(56,5);
                    cout<<"---------------------------------------"<<endl;

                    rlutil::setBackgroundColor(rlutil::YELLOW);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(56,6);
                    cout<<"                                       "<<endl;

                    rlutil::setBackgroundColor(rlutil::YELLOW);
                    rlutil::setColor(rlutil::BLACK);
                    rlutil::locate(60,6);
                    cout<<"        JUEGO DE TRONOS        "<<endl;

                    rlutil::setBackgroundColor(rlutil::BROWN);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(56,7);
                    cout<<"---------------------------------------"<<endl;
                    rlutil::setBackgroundColor(rlutil::BLACK);

                    rlutil::hidecursor();

                    /// RECUADRO "baratheon"
                    rlutil::setBackgroundColor(rlutil::YELLOW);
                    rlutil::setColor(rlutil::BLACK);
                    rlutil::locate(56,10);
                    cout<<"-------------  BARATHEON  -------------"<<endl;
                    rlutil::setBackgroundColor(rlutil::BLACK);
                }
                ///FIN UI


                /// DATOS RECURSOS
                rlutil::setColor(rlutil::WHITE);
                rlutil::locate(56,13);
                cout << "     |Batallas realizadas : " << batalla_actual             << endl;
                rlutil::locate(56,14);
                cout << "     |Oro                 : " << juego_oro                  << endl;
                rlutil::locate(56,15);
                cout << "     |Comida              : " << juego_comida               << endl;
                rlutil::locate(56,16);
                cout << "     |Soldados            : " << juego_soldados             << endl;
                rlutil::locate(56,17);
                cout << "     |Pasiva              : " << batalla_pasiva_probabilidad<< endl;


                while_menu_batalla = 1;
                opcion_menu_batalla = 0;
                while(while_menu_batalla)
                {
                    ///PINTA SI
                    if(opcion_menu_batalla==0)
                    {
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        rlutil::locate(62,20);
                        cout<<" "<<(char)175<<"           Si          "<<(char)174<<" ";
                    }
                    else
                    {
                        rlutil::setBackgroundColor(rlutil::BLACK);
                        rlutil::locate(62,20);
                        cout<<"             Si            "<<endl;
                    }

                    /// PINTA NO
                    if(opcion_menu_batalla==1)
                    {
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        rlutil::locate(62,21);
                        cout<<" "<<(char)175<<"           No          "<<(char)174<<" ";
                    }
                    else
                    {
                        rlutil::setBackgroundColor(rlutil::BLACK);
                        rlutil::locate(62,21);
                        cout<<"             No            "<<endl;
                    }

                    rlutil::setBackgroundColor(rlutil::BLACK);

                    tecla_menu = rlutil::getkey();
                    switch(tecla_menu)
                    {
                    case 14: //ARRIBA
                        opcion_menu_batalla--;
                        if(opcion_menu_batalla<0)
                        {
                            opcion_menu_batalla=1;
                        }
                        break;
                    case 15: //ABAJO
                        opcion_menu_batalla++;
                        if(opcion_menu_batalla>1)
                        {
                            opcion_menu_batalla=0;
                        }
                        break;
                    case 1: //ENTER
                        while_menu_batalla = 0;
                        break;
                    }
                }

                system("cls");

                if (opcion_menu_batalla)
                {
                    break;
                }

                batalla_actual++;

                /*if (batalla_actual == batalla_duracion_guerra)
                {
                    rlutil::locate(64,10);
                    cout << "    ~~ ULTIMA BATALLA ~~ " << batalla_actual << endl;
                }
                else{
                    rlutil::locate(65,10);
                    cout << "~~ Batalla Numero " << batalla_actual << " ~~" << endl;
                }*/

                /// INICIO

                /// PASIVA
                batalla_pasiva_nro_random = (float)(rand() % 100) / 100;

                /*

                 e.g: 0.2 < 0.5
                 => la pasiva se activa ya que tenia un 50% de probabilidades de que el numero se encuentre debajo del 0.5

                 e.g: 0.6 < 0.5
                 => la pasiva no se activa ya que perdio contra el 50%

                */


                if (batalla_pasiva_nro_random < batalla_pasiva_probabilidad)
                {
                    rlutil::setBackgroundColor(rlutil::YELLOW);
                    rlutil::setColor(rlutil::BLACK);
                    rlutil::locate(56,12);
                    cout<<"---------   PASIVA ACTIVADA!  ---------"<<endl;
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::setBackgroundColor(rlutil::BLACK);
                    est_batalla_pasiva_uso_cantidad++;
                    batalla_pasiva_eval_activada = true;

                    //cout << "Si ganas, obtendras un " << batalla_pasiva_beneficio * 100 << "% de comida!" << endl;
                }
                else
                {
                    batalla_pasiva_eval_activada = false;
                }

                // LOGICA DE BATALLA
                if(juego_soldados < 10000) batalla_victoria_eval = (float)(rand() % 100) / 100 < 0.25 ? true : false;
                else if(juego_soldados < 20000) batalla_victoria_eval = (float)(rand() % 100) / 100 < 0.35 ? true : false;
                else batalla_victoria_eval = (float)(rand() % 100) / 100 < 0.5 ? true : false;



                if (batalla_victoria_eval)
                {
                    rlutil::setBackgroundColor(rlutil::YELLOW);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(56,10);
                    cout<<"----    HAZ GANADO LA BATALLA!!!   ----"<<endl;
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::setBackgroundColor(rlutil::BLACK);

                    batalla_aux_oro_ganado         = rand() % 10000;
                    batalla_aux_comida_gastado     = rand() % 500;
                    batalla_aux_soldados_gastado   = rand() % 100;

                    if (batalla_pasiva_eval_activada)
                    {
                        // Multiplico la comida que gastaria por el beneficio de la pasiva y sumo
                        batalla_aux_comida_ganada_pasiva = batalla_pasiva_beneficio * batalla_aux_comida_gastado;
                        juego_comida += batalla_aux_comida_ganada_pasiva;
                    }
                    else
                    {
                        batalla_aux_comida_ganada_pasiva = 0;
                        juego_comida -= batalla_aux_comida_gastado;
                    }

                    juego_oro += batalla_aux_oro_ganado;

                    est_batalla_pasiva_comida_ganada += batalla_aux_comida_ganada_pasiva;
                    est_batalla_comida_perdido       += batalla_aux_comida_gastado;
                    est_batalla_soldados_perdido     += batalla_aux_soldados_gastado;
                    est_batalla_oro_ganado           += batalla_aux_oro_ganado;
                    est_batalla_victorias_cantidad ++;

                }
                else
                {
                    rlutil::setBackgroundColor(rlutil::YELLOW);
                    rlutil::setColor(rlutil::BLACK);
                    rlutil::locate(56,10);
                    cout<<"----   HAZ PERDIDO LA BATALLA!!!   ----"<<endl;
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::setBackgroundColor(rlutil::BLACK);

                    batalla_aux_comida_gastado   = rand() % max_perdida_comida;
                    batalla_aux_soldados_gastado = rand() % max_perdida_soldados;

                    est_batalla_comida_perdido    += batalla_aux_comida_gastado;
                    est_batalla_soldados_perdido  += batalla_aux_soldados_gastado;
                    est_batalla_derrotas_cantidad ++;

                    juego_comida -= batalla_aux_comida_gastado;

                }

                ///UI
                {
                    ///RECUADRO FRASE ----------------------------------------------------------------------------
                    rlutil::locate(53,26);  // +9
                    rlutil::setBackgroundColor(rlutil::BROWN);
                    cout<<"                                             "<<endl;
                    rlutil::locate(53,30);  // +9
                    rlutil::setBackgroundColor(rlutil::BROWN);
                    cout<<"                                             "<<endl;
                    for (int i=26; i<30; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        rlutil::locate(53,i);  // +9
                        cout<<"  "<<endl;
                    }
                    for (int i=26; i<30; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        rlutil::locate(96,i);  // +9
                        cout<<"  "<<endl;
                    }
                    rlutil::setBackgroundColor(rlutil::BLACK);
                    rlutil::setColor(rlutil::YELLOW);

                    rlutil::locate(57, 28), cout << "OURS IS THE FURY!";  // +9

                    /// Se hace esto para pintar nuevamente de negro el fondo y blanco las letras por debajo
                    rlutil::setBackgroundColor(rlutil::BLACK);
                    rlutil::setColor(rlutil::WHITE);

                    ///LOGO GAME OF THRONES ----------------------------------------------------------------------------

                    rlutil::locate(27,32);  // +9
                    cout << "  _____          __  __ ______    ____  ______   _______ _    _ _____   ____  _   _ ______  _____ " << endl;
                    rlutil::locate(27,33);  // +9
                    cout << " / ____|   /\\   |  \\/  |  ____|  / __ \\|  ____| |__   __| |  | |  __ \\ / __ \\| \\ | |  ____|/ ____|" << endl;
                    rlutil::locate(27,34);  // +9
                    cout << "| |  __   /  \\  | \\  / | |__    | |  | | |__       | |  | |__| | |__) | |  | |  \\| | |__  | (___  " << endl;
                    rlutil::locate(27,35);  // +9
                    cout << "| | |_ | / /\\ \\ | |\\/| |  __|   | |  | |  __|      | |  |  __  |  _  /| |  | | . ` |  __|  \\___ \\ " << endl;
                    rlutil::locate(27,36);  // +9
                    cout << "| |__| |/ ____ \\| |  | | |____  | |__| | |         | |  | |  | | | \\ \\| |__| | |\\  | |____ ____) |" << endl;
                    rlutil::locate(27,37);  // +9
                    cout << " \\_____/_/    \\_\\_|  |_|______|  \\____/|_|         |_|  |_|  |_|_|  \\_\\\\____/|_| \\_|______|_____/ " << endl;

                    ///LOGOS ESCUDO BARATHEON ----------------------------------------------------------------------------
                    for (int i = 0; i < logo_dimensionY; i++)
                    {
                        for (int j = 0; j < logo_dimensionX; j++)
                        {
                            rlutil::locate(8 + j, 2 + i);  // +9
                            switch (baratheon_logo[i][j])
                            {
                            case 1:
                                rlutil::setBackgroundColor(rlutil::BROWN);
                                cout << " ";
                                break;
                            case 2:
                                rlutil::setBackgroundColor(rlutil::YELLOW);
                                cout << " ";
                                break;
                            default:
                                rlutil::setBackgroundColor(rlutil::BLACK);
                                cout << " ";
                            }
                        }
                    }

                    for (int i = 0; i < logo_dimensionY; i++)
                    {
                        for (int j = 0; j < logo_dimensionX; j++)
                        {
                            rlutil::locate(108 + j, 2 + i);  // +9
                            switch (baratheon_logo[i][j])
                            {
                            case 1:
                                rlutil::setBackgroundColor(rlutil::BROWN);
                                cout << " ";
                                break;
                            case 2:
                                rlutil::setBackgroundColor(rlutil::YELLOW);
                                cout << " ";
                                break;
                            default:
                                rlutil::setBackgroundColor(rlutil::BLACK);
                                cout << " ";
                            }
                        }
                    }

                    ///RECUADRO EXTERIOR ----------------------------------------------------------------------------

                    rlutil::setBackgroundColor(rlutil::BROWN);
                    rlutil::locate(52,3);
                    cout<<"                                             "<<endl;

                    for (int i=3; i<23; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        rlutil::locate(52,i+1);
                        cout<<"  "<<endl;
                    }
                    rlutil::setBackgroundColor(rlutil::BROWN);
                    rlutil::locate(54,23);
                    cout<<"                                           "<<endl;

                    for (int i=2; i<23; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        rlutil::locate(97,i+1);
                        cout<<"  "<<endl;
                    }

                    /// RECUADRO "JUEGO DE TRONOS"
                    rlutil::setBackgroundColor(rlutil::BROWN);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(56,5);
                    cout<<"---------------------------------------"<<endl;

                    rlutil::setBackgroundColor(rlutil::YELLOW);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(56,6);
                    cout<<"                                       "<<endl;

                    rlutil::setBackgroundColor(rlutil::YELLOW);
                    rlutil::setColor(rlutil::BLACK);
                    rlutil::locate(60,6);
                    cout<<"        JUEGO DE TRONOS        "<<endl;

                    rlutil::setBackgroundColor(rlutil::BROWN);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(56,7);
                    cout<<"---------------------------------------"<<endl;
                    rlutil::setBackgroundColor(rlutil::BLACK);

                    rlutil::hidecursor();

                    /// RECUADRO "baratheon"
                    rlutil::setBackgroundColor(rlutil::YELLOW);
                    rlutil::setColor(rlutil::BLACK);
                    rlutil::locate(56,10);
                    cout<<"-------------  BARATHEON  -------------"<<endl;
                    rlutil::setBackgroundColor(rlutil::BLACK);
                }
                ///FIN UI

                rlutil::hidecursor();

                /// DATOS GUERRA
                rlutil::locate(56,14);
                rlutil::setColor(rlutil::WHITE);
                cout << "    |Oro ganado en batalla : " << batalla_aux_oro_ganado << endl;
                rlutil::locate(56,15);
                cout << "    |Soldados perdidos     : " << batalla_aux_soldados_gastado     << endl;

                if(batalla_pasiva_eval_activada && batalla_victoria_eval)
                {
                    rlutil::locate(56,16);
                    cout << "    |Comida ganada pasiva  : " << batalla_aux_comida_ganada_pasiva << endl;
                }
                else if(batalla_pasiva_eval_activada)
                {
                    rlutil::locate(56,16);
                    cout << "    |Comida ganada pasiva  : " << batalla_aux_comida_ganada_pasiva << endl;
                    rlutil::locate(56,17);
                    cout << "    |Comida perdida        : " << batalla_aux_comida_gastado       << endl;
                }
                else{
                    rlutil::locate(56,16);
                    cout << "    |Comida perdida        : " << batalla_aux_comida_gastado       << endl;
                }

                //Descuento los recursos perdidos
                juego_soldados -= batalla_aux_soldados_gastado;

                batalla_aux_oro_ganado = 0;
                batalla_aux_comida_ganada_pasiva  = 0;
                batalla_aux_comida_gastado     = 0;
                batalla_aux_soldados_gastado   = 0;

                /// CIERRE

                rlutil::locate(56,21);
                rlutil::setBackgroundColor(rlutil::YELLOW);
                rlutil::setColor(rlutil::BLACK);
                cout<<"   Presione una tecla para continuar   ";
                rlutil::setBackgroundColor(rlutil::BLACK);
                rlutil::setColor(rlutil::WHITE);
                getch();
                system("cls");

                }
                break;
            }

            case 1: /// MENU TIENDA
            {

                opcion_tienda = 0;
                while(opcion_tienda!=4)
                {

                    ///UI
                    {
                        ///RECUADRO FRASE ----------------------------------------------------------------------------
                        rlutil::locate(53,26);  // +9
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        cout<<"                                             "<<endl;
                        rlutil::locate(53,30);  // +9
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        cout<<"                                             "<<endl;
                        for (int i=26; i<30; i++)
                        {
                            rlutil::setBackgroundColor(rlutil::BROWN);
                            rlutil::locate(53,i);  // +9
                            cout<<"  "<<endl;
                        }
                        for (int i=26; i<30; i++)
                        {
                            rlutil::setBackgroundColor(rlutil::BROWN);
                            rlutil::locate(96,i);  // +9
                            cout<<"  "<<endl;
                        }
                        rlutil::setBackgroundColor(rlutil::BLACK);
                        rlutil::setColor(rlutil::YELLOW);

                        rlutil::locate(57, 28), cout << "OURS IS THE FURY!";  // +9

                        /// Se hace esto para pintar nuevamente de negro el fondo y blanco las letras por debajo
                        rlutil::setBackgroundColor(rlutil::BLACK);
                        rlutil::setColor(rlutil::WHITE);

                        ///LOGO GAME OF THRONES ----------------------------------------------------------------------------

                        rlutil::locate(27,32);  // +9
                        cout << "  _____          __  __ ______    ____  ______   _______ _    _ _____   ____  _   _ ______  _____ " << endl;
                        rlutil::locate(27,33);  // +9
                        cout << " / ____|   /\\   |  \\/  |  ____|  / __ \\|  ____| |__   __| |  | |  __ \\ / __ \\| \\ | |  ____|/ ____|" << endl;
                        rlutil::locate(27,34);  // +9
                        cout << "| |  __   /  \\  | \\  / | |__    | |  | | |__       | |  | |__| | |__) | |  | |  \\| | |__  | (___  " << endl;
                        rlutil::locate(27,35);  // +9
                        cout << "| | |_ | / /\\ \\ | |\\/| |  __|   | |  | |  __|      | |  |  __  |  _  /| |  | | . ` |  __|  \\___ \\ " << endl;
                        rlutil::locate(27,36);  // +9
                        cout << "| |__| |/ ____ \\| |  | | |____  | |__| | |         | |  | |  | | | \\ \\| |__| | |\\  | |____ ____) |" << endl;
                        rlutil::locate(27,37);  // +9
                        cout << " \\_____/_/    \\_\\_|  |_|______|  \\____/|_|         |_|  |_|  |_|_|  \\_\\\\____/|_| \\_|______|_____/ " << endl;

                        ///LOGOS ESCUDO BARATHEON ----------------------------------------------------------------------------
                        for (int i = 0; i < logo_dimensionY; i++)
                        {
                            for (int j = 0; j < logo_dimensionX; j++)
                            {
                                rlutil::locate(8 + j, 2 + i);  // +9
                                switch (baratheon_logo[i][j])
                                {
                                case 1:
                                    rlutil::setBackgroundColor(rlutil::BROWN);
                                    cout << " ";
                                    break;
                                case 2:
                                    rlutil::setBackgroundColor(rlutil::YELLOW);
                                    cout << " ";
                                    break;
                                default:
                                    rlutil::setBackgroundColor(rlutil::BLACK);
                                    cout << " ";
                                }
                            }
                        }

                        for (int i = 0; i < logo_dimensionY; i++)
                        {
                            for (int j = 0; j < logo_dimensionX; j++)
                            {
                                rlutil::locate(108 + j, 2 + i);  // +9
                                switch (baratheon_logo[i][j])
                                {
                                case 1:
                                    rlutil::setBackgroundColor(rlutil::BROWN);
                                    cout << " ";
                                    break;
                                case 2:
                                    rlutil::setBackgroundColor(rlutil::YELLOW);
                                    cout << " ";
                                    break;
                                default:
                                    rlutil::setBackgroundColor(rlutil::BLACK);
                                    cout << " ";
                                }
                            }
                        }

                        ///RECUADRO EXTERIOR ----------------------------------------------------------------------------

                        rlutil::setBackgroundColor(rlutil::BROWN);
                        rlutil::locate(52,3);
                        cout<<"                                             "<<endl;

                        for (int i=3; i<23; i++)
                        {
                            rlutil::setBackgroundColor(rlutil::BROWN);
                            rlutil::locate(52,i+1);
                            cout<<"  "<<endl;
                        }
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        rlutil::locate(54,23);
                        cout<<"                                           "<<endl;

                        for (int i=2; i<23; i++)
                        {
                            rlutil::setBackgroundColor(rlutil::BROWN);
                            rlutil::locate(97,i+1);
                            cout<<"  "<<endl;
                        }

                        /// RECUADRO "JUEGO DE TRONOS"
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        rlutil::setColor(rlutil::WHITE);
                        rlutil::locate(56,5);
                        cout<<"---------------------------------------"<<endl;

                        rlutil::setBackgroundColor(rlutil::YELLOW);
                        rlutil::setColor(rlutil::WHITE);
                        rlutil::locate(56,6);
                        cout<<"                                       "<<endl;

                        rlutil::setBackgroundColor(rlutil::YELLOW);
                        rlutil::setColor(rlutil::BLACK);
                        rlutil::locate(60,6);
                        cout<<"        JUEGO DE TRONOS        "<<endl;

                        rlutil::setBackgroundColor(rlutil::BROWN);
                        rlutil::setColor(rlutil::WHITE);
                        rlutil::locate(56,7);
                        cout<<"---------------------------------------"<<endl;
                        rlutil::setBackgroundColor(rlutil::BLACK);

                        rlutil::hidecursor();

                        /// RECUADRO "baratheon"
                        rlutil::setBackgroundColor(rlutil::YELLOW);
                        rlutil::setColor(rlutil::BLACK);
                        rlutil::locate(56,10);
                        cout<<"-------------  BARATHEON  -------------"<<endl;
                        rlutil::setBackgroundColor(rlutil::BLACK);
                    }
                    ///FIN UI


                    /// DATOS DEL CLAN ----------------------------------------------------------------------------

                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(56,12);
                    cout << "     |Oro                 : " << juego_oro                  << endl;
                    rlutil::locate(56,13);
                    cout << "     |Comida              : " << juego_comida               << endl;
                    rlutil::locate(56,14);
                    cout << "     |Soldados            : " << juego_soldados             << endl;
                    rlutil::locate(56,15);
                    cout << "     |Pasiva              : " << batalla_pasiva_probabilidad<< endl;
                    rlutil::setColor(rlutil::WHITE);

                    /// PINTAR "SOLDADOS" ----------------------------------------------------------------------------
                    while_menu_tienda = 1;
                    while(while_menu_tienda)
                    {

                        if(opcion_tienda==0)
                        {
                            rlutil::setBackgroundColor(rlutil::BROWN);
                            rlutil::locate(56,18);
                            cout<<" "<<(char)175<<"  SOLDADOS  $"<<tienda_valor_x_soldado<< " x "  <<tienda_articulo_soldados<< " UNIDADES  "<<(char)174<<" ";
                        }
                        else
                        {
                            rlutil::setBackgroundColor(rlutil::BLACK);
                            rlutil::locate(56,18);
                            cout<<"    SOLDADOS  $"<<tienda_valor_x_soldado<< " x "  <<tienda_articulo_soldados<< " UNIDADES    "<<endl;
                        }

                        /// PINTAR "COMIDA" ----------------------------------------------------------------------------

                        if(opcion_tienda==1)
                        {
                            rlutil::setBackgroundColor(rlutil::BROWN);
                            rlutil::locate(56,19);
                            cout<<" "<<(char)175<<"    COMIDA  $" << tienda_valor_x_comida         << " x "  << tienda_articulo_comida          << " UNIDADES    "<<(char)174<<" ";
                        }
                        else
                        {
                            rlutil::setBackgroundColor(rlutil::BLACK);
                            rlutil::locate(56,19);
                            cout<<"      COMIDA  $" << tienda_valor_x_comida         << " x "  << tienda_articulo_comida          << " UNIDADES      "<<endl;
                        }

                        /// PINTAR "MEJORA PASIVA" ----------------------------------------------------------------------------

                        if(opcion_tienda==2)
                        {
                            rlutil::setBackgroundColor(rlutil::BROWN);
                            rlutil::locate(56,20);
                            cout<<" "<<(char)175<<"   MEJORAR PASIVA  $" << tienda_valor_x_mejora_pasiva  << " x +" << tienda_articulo_mejora_pasiva   << "   "<<(char)174<<" ";
                        }
                        else
                        {
                            rlutil::setBackgroundColor(rlutil::BLACK);
                            rlutil::locate(56,20);
                            cout<<"     MEJORAR PASIVA  $" << tienda_valor_x_mejora_pasiva  << " x +" << tienda_articulo_mejora_pasiva   << "     "<<endl;
                        }

                        /// PINTAR "VOLVER MENU PRINCIPAL" ----------------------------------------------------------------------------

                        if(opcion_tienda==3)
                        {
                            rlutil::setBackgroundColor(rlutil::BROWN);
                            rlutil::locate(56,21);
                            cout<<" "<<(char)175<<"     VOLVER AL MENU PRINCIPAL      "<<(char)174<<" ";
                        }
                        else
                        {
                            rlutil::setBackgroundColor(rlutil::BLACK);
                            rlutil::locate(56,21);
                            ///   "-------------    TIENDA    ------------"
                            cout<<"       VOLVER AL MENU PRINCIPAL        "<<endl;
                        }
                        rlutil::setBackgroundColor(rlutil::BLACK);

                        /// LOGICA DE LA TIENDA ----------------------------------------------------------------------------

                        tecla_tienda = rlutil::getkey();
                        switch(tecla_tienda)
                        {
                        case 14: //ARRIBA
                            opcion_tienda--;
                            if(opcion_tienda<0)
                            {
                                opcion_tienda=3;
                            }
                            break;
                        case 15: //ABAJO
                            opcion_tienda++;
                            if(opcion_tienda>3)
                            {
                                opcion_tienda=0;
                            }
                            break;
                        case 1: //ENTER
                            while_menu_tienda = 0;
                            break;
                        }
                    }
                    switch(opcion_tienda)
                    {

                    case 0: /// SOLDADOS
                    {
                        if (juego_oro >= tienda_valor_x_soldado)
                        {
                            aux_tienda_gasto = tienda_valor_x_soldado;

                            juego_soldados += tienda_articulo_soldados;
                            juego_oro      -= aux_tienda_gasto;

                            est_tienda_total_gastado_oro      += aux_tienda_gasto;
                            est_tienda_total_gastado_soldados += aux_tienda_gasto;
                        }
                        else
                        {
                            cout << "No tienes oro suficiente!" << endl;

                            cout << endl;

                            system("pause");
                        }

                        break;
                    }
                    case 1: /// COMIDA
                    {
                        if (juego_oro >= tienda_valor_x_comida)
                        {
                            aux_tienda_gasto = tienda_valor_x_comida;

                            juego_comida += tienda_articulo_comida;
                            juego_oro    -= aux_tienda_gasto;

                            est_tienda_total_gastado_oro    += aux_tienda_gasto;
                            est_tienda_total_gastado_comida += aux_tienda_gasto;
                        }
                        else
                        {
                            cout << "No tienes oro suficiente!" << endl;

                            cout << endl;

                            cout << "Presiona cualquier tecla para volver al menu.";
                            system("pause");
                        }

                        break;
                    }
                    case 2: /// MEJORA
                    {
                        if (juego_oro >= tienda_valor_x_mejora_pasiva)
                        {
                            batalla_pasiva_probabilidad += tienda_articulo_mejora_pasiva;
                            juego_oro                   -= tienda_valor_x_mejora_pasiva;
                        }
                        else
                        {
                            cout << "No tienes oro suficiente!" << endl;

                            cout << endl;

                            cout << "Presiona cualquier tecla para volver al menu.";
                            system("pause");
                        }

                        break;
                    }
                    case 3: /// VOLVER AL MENU PRINCIPAL
                        opcion_tienda = 4;
                        system("cls");

                }///TERMINA SWITCH OPCION TIENDA

                }///TERMINA WHILE DE TIENDA
                break;
            }
            case 2: /// SALIR DEL JUEGO
            {
                return 0;
            }

        } ///TERMINA SWITHC DEL MENU PRINCIPAL
    }/// TERMINA WHILE DE MENU PRINCIPAL
}

