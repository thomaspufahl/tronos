#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;
#include "rlutil.h"

int main()
{
    /// RENDERIZAR CONSOLA EN TAMA�O COMPLETO
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

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

    const float batalla_pasiva_beneficio = 0.2; // 20% extra en oro ganado
    float batalla_pasiva_nro_random;

    bool batalla_pasiva_eval_activada;

    // combate
    int batalla_aux_oro_ganado_parcial;
    int batalla_aux_oro_ganado_pasiva;
    int batalla_aux_oro_ganado_total;
    int batalla_aux_comida_gastado;
    int batalla_aux_soldados_gastado;

    bool batalla_victoria_eval;
    float batalla_victoria_nro_random;
    float batalla_victoria_probabilidad;


    // ESTADISTICAS
    int est_batalla_victorias_cantidad   = 0;
    int est_batalla_derrotas_cantidad    = 0;

    int est_batalla_pasiva_uso_cantidad  = 0;
    int est_batalla_pasiva_oro_ganado    = 0;
    int est_batalla_pasiva_comida_ganado = 0;

    int est_batalla_oro_ganado           = 0;
    int est_batalla_oro_perdido          = 0;
    int est_batalla_comida_ganado        = 0;
    int est_batalla_comida_perdido       = 0;
    int est_batalla_soldados_ganado      = 0;
    int est_batalla_soldados_perdido     = 0;

    int est_tienda_total_gastado_oro     = 0;
    int est_tienda_total_gastado_comida  = 0;
    int est_tienda_total_gastado_soldados= 0;

    /// Auxiliares

    // ingresos
    int cin_opcion_menu;
    int cin_opcion_tienda;
    int cin_batalla_deseas_continuar;
    int opcion_menu=0;
    int opcion_tienda=0;
    int tecla_menu;
    int tecla_tienda;

    // validaciones
    bool eval_deseas_continuar;

    // indices
    const int idx_opcion_menu_batalla = 1;
    const int idx_opcion_menu_tienda  = 2;
    const int idx_opcion_menu_volver  = 3;

    const int idx_opcion_tienda_soldados      = 1;
    const int idx_opcion_tienda_comida        = 2;
    const int idx_opcion_tienda_mejora_pasiva = 3;
    const int idx_opcion_tienda_volver        = 4;

    int aux_tienda_gasto;

    // Generador de numeros aleatorios
    srand(time(0));

    //Logo dimensiones
    const int logo_dimensionY = 30;
    const int logo_dimensionX = 27;

    const int lannister_logo[30][27] =
    {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,2,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,3,3,2,2,2,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,3,2,1,2,2,3,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,3,2,2,3,3,1,2,1,1,1,1,1,1,1},
        {1,1,1,1,1,2,3,2,1,1,1,1,1,2,2,2,2,2,3,3,1,1,1,1,1,1,1},
        {1,1,1,1,1,2,2,2,2,1,2,2,3,2,2,2,2,3,3,3,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,2,2,2,3,2,3,3,2,2,2,1,2,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,3,2,2,2,2,2,2,2,3,1,2,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,3,2,3,2,2,2,2,1,1,2,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,3,1,1,2,2,2,2,2,3,2,1,1,3,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,3,3,2,2,2,1,2,2,2,2,3,1,1,3,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,2,2,2,1,1,1,1,3,2,2,2,1,1,2,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,3,1,1,1,1,2,2,3,2,2,2,1,2,1,1,1,1,1,1,1},
        {3,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,3,3,2,1,1,1,1,1,1,3},
        {3,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,3,1,1,1,1,1,1,1,1,3},
        {3,1,1,1,1,1,1,1,1,1,1,2,2,1,3,2,2,1,1,1,1,1,1,1,1,1,3},
        {3,3,1,1,1,1,1,1,1,1,1,2,2,2,1,2,2,2,1,1,1,1,1,1,1,3,3},
        {3,3,1,1,1,1,1,1,1,1,2,2,2,1,1,1,3,2,3,1,1,1,1,1,1,3,3},
        {3,3,3,1,1,1,1,3,2,2,2,1,3,1,1,1,1,2,2,3,1,1,1,1,3,3,3},
        {3,3,3,1,1,1,1,1,3,2,3,1,1,1,1,1,1,2,2,1,1,1,1,1,3,3,3},
        {3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,2,2,2,3,1,1,1,1,3,3,3,3},
        {3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,3,3,3,3,3},
        {3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3},
        {3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3},
        {3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3},
        {3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3},
        {3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3},
        {3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3},
    };

    /// MENU
    while(true)
    {
        ///LANNISTER ESLOGAN
        rlutil::locate(44,25);
        rlutil::setBackgroundColor(rlutil::RED);
        cout<<"                                             "<<endl;
        rlutil::locate(44,29);
        rlutil::setBackgroundColor(rlutil::RED);
        cout<<"                                             "<<endl;
        for (int i=25; i<29; i++)
        {
            rlutil::setBackgroundColor(rlutil::RED);
            rlutil::locate(44,i);
            cout<<"  "<<endl;
        }
        for (int i=25; i<29; i++)
        {
            rlutil::setBackgroundColor(rlutil::RED);
            rlutil::locate(87,i);
            cout<<"  "<<endl;
        }
        rlutil::setBackgroundColor(rlutil::BLACK);
        rlutil::setColor(rlutil::YELLOW);

        rlutil::locate(48, 27), cout << "Un Lannister siempre paga sus deudas!";

        /// Se hace esto para pintar nuevamente de negro el fondo y blanco las letras por debajo
        rlutil::setBackgroundColor(rlutil::BLACK);
        rlutil::setColor(rlutil::WHITE);

        rlutil::locate(19,32);
        cout << "  _____          __  __ ______    ____  ______   _______ _    _ _____   ____  _   _ ______  _____ " << endl;
        rlutil::locate(19,33);
        cout << " / ____|   /\\   |  \\/  |  ____|  / __ \\|  ____| |__   __| |  | |  __ \\ / __ \\| \\ | |  ____|/ ____|" << endl;
        rlutil::locate(19,34);
        cout << "| |  __   /  \\  | \\  / | |__    | |  | | |__       | |  | |__| | |__) | |  | |  \\| | |__  | (___  " << endl;
        rlutil::locate(19,35);
        cout << "| | |_ | / /\\ \\ | |\\/| |  __|   | |  | |  __|      | |  |  __  |  _  /| |  | | . ` |  __|  \\___ \\ " << endl;
        rlutil::locate(19,36);
        cout << "| |__| |/ ____ \\| |  | | |____  | |__| | |         | |  | |  | | | \\ \\| |__| | |\\  | |____ ____) |" << endl;
        rlutil::locate(19,37);
        cout << " \\_____/_/    \\_\\_|  |_|______|  \\____/|_|         |_|  |_|  |_|_|  \\_\\\\____/|_| \\_|______|_____/ " << endl;


        ///LOGOS
        for (int i = 0; i < logo_dimensionY; i++)
        {
            for (int j = 0; j < logo_dimensionX; j++)
            {
                rlutil::locate(10 + j, 1 + i);
                switch (lannister_logo[i][j])
                {
                case 1:
                    rlutil::setBackgroundColor(rlutil::RED);
                    cout << " ";
                    break;
                case 2:
                    rlutil::setBackgroundColor(rlutil::YELLOW);
                    cout << " ";
                    break;
                case 3:
                    rlutil::setBackgroundColor(rlutil::BROWN);
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
                rlutil::locate(95 + j, 1 + i);
                switch (lannister_logo[i][j])
                {
                case 1:
                    rlutil::setBackgroundColor(rlutil::RED);
                    cout << " ";
                    break;
                case 2:
                    rlutil::setBackgroundColor(rlutil::YELLOW);
                    cout << " ";
                    break;
                case 3:
                    rlutil::setBackgroundColor(rlutil::BROWN);
                    cout << " ";
                    break;
                default:
                    rlutil::setBackgroundColor(rlutil::BLACK);
                    cout << " ";
                }
            }
        }

        ///RECUADRO EXTERIOR ----------------------------------------------------------------------------

        rlutil::setBackgroundColor(rlutil::RED);
        rlutil::locate(43,2);
        cout<<"                                             "<<endl;


        for (int i=3; i<23; i++)
        {
            rlutil::setBackgroundColor(rlutil::RED);
            rlutil::locate(43,i);
            cout<<"  "<<endl;
        }
        rlutil::setBackgroundColor(rlutil::RED);
        rlutil::locate(45,22);
        cout<<"                                           "<<endl;
        for (int i=2; i<23; i++)
        {
            rlutil::setBackgroundColor(rlutil::RED);
            rlutil::locate(88,i);
            cout<<"  "<<endl;
        }

        ///RECUADRO "JUEGO DE TRONOS" ----------------------------------------------------------------------------

        rlutil::setBackgroundColor(rlutil::RED);
        rlutil::setColor(rlutil::WHITE);
        rlutil::locate(47,4);
        cout<<"---------------------------------------"<<endl;


        rlutil::setBackgroundColor(rlutil::BROWN);
        rlutil::setColor(rlutil::WHITE);
        rlutil::locate(47,5);
        cout<<"                                       "<<endl;


        rlutil::setBackgroundColor(rlutil::YELLOW);
        rlutil::setColor(rlutil::BLACK);
        rlutil::locate(51,5);
        cout<<"        JUEGO DE TRONOS        "<<endl;


        rlutil::setBackgroundColor(rlutil::RED);
        rlutil::setColor(rlutil::WHITE);
        rlutil::locate(47,6);
        cout<<"---------------------------------------"<<endl;
        rlutil::setBackgroundColor(rlutil::BLACK);

        rlutil::hidecursor();

        ///RECUADRO "LANNISTER" ----------------------------------------------------------------------------

        rlutil::setBackgroundColor(rlutil::YELLOW);
        rlutil::setColor(rlutil::BLACK);
        rlutil::locate(47,9);
        cout<<"-------------  LANNISTER  -------------"<<endl;
        rlutil::setBackgroundColor(rlutil::BLACK);

        /// DATOS DEL CLAN ----------------------------------------------------------------------------

        rlutil::locate(47,11);
        rlutil::setColor(rlutil::WHITE);
        cout << "     |Batallas realizadas : " << batalla_actual             << endl;
        rlutil::locate(47,13);
        cout << "     |Oro                 : " << juego_oro                  << endl;
        rlutil::locate(47,14);
        cout << "     |Comida              : " << juego_comida               << endl;
        rlutil::locate(47,15);
        cout << "     |Soldados            : " << juego_soldados             << endl;
        rlutil::locate(47,16);
        cout << "     |Pasiva              : " << batalla_pasiva_probabilidad<< endl;
        rlutil::setColor(rlutil::WHITE);

        /// PINTAR "IR A LA BATALLA" ----------------------------------------------------------------------------

        if(opcion_menu==0)
        {
            rlutil::setBackgroundColor(rlutil::BROWN);
            rlutil::locate(47,18);
            cout<<" "<<(char)175<<"          IR A LA BATALLA          "<<(char)174<<" ";
        }
        else
        {
            rlutil::setBackgroundColor(rlutil::BLACK);
            rlutil::locate(47,18);
            cout<<"            IR A LA BATALLA            "<<endl;
        }

        /// PINTAR "TIENDA" ----------------------------------------------------------------------------

        if(opcion_menu==1)
        {
            rlutil::setBackgroundColor(rlutil::BROWN);
            rlutil::locate(47,19);
            cout<<" "<<(char)175<<"              TIENDA               "<<(char)174<<" ";
        }
        else
        {
            rlutil::setBackgroundColor(rlutil::BLACK);
            rlutil::locate(47,19);
            cout<<"                TIENDA                 "<<endl;
        }

        /// PINTAR "SALIR DEL JUEGO" ----------------------------------------------------------------------------

        if(opcion_menu==2)
        {
            rlutil::setBackgroundColor(rlutil::BROWN);
            rlutil::locate(47,20);
            cout<<" "<<(char)175<<"          SALIR DEL JUEGO          "<<(char)174<<" ";
        }
        else
        {
            rlutil::setBackgroundColor(rlutil::BLACK);
            rlutil::locate(47,20);
            cout<<"            SALIR DEL JUEGO            "<<endl;
        }
        rlutil::setBackgroundColor(rlutil::BLACK);

        /// LOGICA DEL MENU ----------------------------------------------------------------------------

        tecla_menu= rlutil::getkey();
        switch(tecla_menu)
        {
        case 14: //ARRIBA
            rlutil::locate(50,10+opcion_menu);
            cout<<" "<<endl;
            opcion_menu--;
            if(opcion_menu<0)
            {
                opcion_menu=2;
            }
            break;
        case 15: //ABAJO
            rlutil::locate(50,10+opcion_menu);
            cout<<" "<<endl;
            opcion_menu++;
            if(opcion_menu>2)
            {
                opcion_menu=0;
            }
            break;
        case 1: //ENTER
            system("cls");
            switch(opcion_menu)
            {
            case 0: /// MENU BATALLA
            {
                if (batalla_actual > batalla_duracion_guerra)
                {
                    cout << "No hay mas batallas por luchar, puedes descansar!" << endl;

                    cout << endl;

                    cout << "Presiona cualquier tecla para volver al menu.";
                    system("pause");
                    system("cls");
                    break;
                }

                cout << "Soldados: "                << juego_soldados              << endl;
                cout << "Comida: "                  << juego_comida                << endl;
                cout << "Probabilidad de pasiva: "  << batalla_pasiva_probabilidad << endl;

                cout << endl;

                //? considerar usar 1 y 0 para no usar el ternario
                cout << "Deseas continuar? 1(SI) / 2(NO)" << endl;
                cout << "opcion: ";
                cin >> cin_batalla_deseas_continuar;
                eval_deseas_continuar = cin_batalla_deseas_continuar == 2 ? false : true;

                if (!eval_deseas_continuar)
                {
                    system("cls");
                    break;
                }

                system("cls");

                batalla_actual++;

                if (batalla_actual == batalla_duracion_guerra)
                {
                    cout << "~~ ULTIMA BATALLA ~~" << endl;
                    cout << endl;
                }

                /// INICIO
                cout << "Batalla NRO " << batalla_actual << endl << endl;

                /// PASIVA
                batalla_pasiva_nro_random = (float)(rand() % 100) / 100;

                cout << "NRO RANDOM: " << batalla_pasiva_nro_random << endl;

                /*

                 e.g: 0.2 < 0.5
                 => la pasiva se activa ya que tenia un 50% de probabilidades de que el numero se encuentre debajo del 0.5

                 e.g: 0.6 < 0.5
                 => la pasiva no se activa ya que perdio contra el 50%

                */


                if (batalla_pasiva_nro_random < batalla_pasiva_probabilidad)
                {
                    cout << "PASIVA ACTIVADA!" << endl;
                    est_batalla_pasiva_uso_cantidad++;
                    batalla_pasiva_eval_activada = true;

                    cout << "Si ganas, obtendras un " << batalla_pasiva_beneficio * 100 << "% de oro extra!" << endl;
                }
                else
                {
                    batalla_pasiva_eval_activada = false;
                }

                // LOGICA DE BATALLA
                batalla_victoria_eval = (rand() % 100 / 100) < 0.5 ? true : false;


                if (batalla_victoria_eval)
                {
                    cout << "HAZ GANADO LA BATALLA !!!!!" << endl;

                    batalla_aux_oro_ganado_parcial = rand() % 10000;
                    batalla_aux_comida_gastado     = rand() % 500;
                    batalla_aux_soldados_gastado   = rand() % 100;

                    if (batalla_pasiva_eval_activada)
                    {
                        batalla_aux_oro_ganado_pasiva = batalla_pasiva_beneficio * batalla_aux_oro_ganado_parcial;
                    }
                    else
                    {
                        batalla_aux_oro_ganado_pasiva = 0;
                    }

                    batalla_aux_oro_ganado_total = batalla_aux_oro_ganado_parcial + batalla_aux_oro_ganado_pasiva;

                    est_batalla_pasiva_oro_ganado  += batalla_aux_oro_ganado_pasiva;
                    est_batalla_comida_perdido     += batalla_aux_comida_gastado;
                    est_batalla_soldados_perdido   += batalla_aux_soldados_gastado;
                    est_batalla_oro_ganado         += batalla_aux_oro_ganado_total;
                    est_batalla_victorias_cantidad ++;

                }
                else
                {
                    cout << "HAZ PERDIDO LA BATALLA !!!!!" << endl;

                    batalla_aux_comida_gastado   = rand() % 1000;
                    batalla_aux_soldados_gastado = rand() % 500;

                    est_batalla_comida_perdido    += batalla_aux_comida_gastado;
                    est_batalla_soldados_perdido  += batalla_aux_soldados_gastado;
                    est_batalla_derrotas_cantidad ++;

                }

                /// Resumen

                cout << "Oro ganado en batalla: " << batalla_aux_oro_ganado_parcial << endl;
                cout << "Oro ganado x pasiva: "   << batalla_aux_oro_ganado_pasiva  << endl;
                cout << "Oro ganado total: "      << batalla_aux_oro_ganado_total   << endl;
                cout << "Comida perdida: "        << batalla_aux_comida_gastado     << endl;
                cout << "Soldados gastados: "     << batalla_aux_soldados_gastado   << endl;

                batalla_aux_oro_ganado_parcial = 0;
                batalla_aux_oro_ganado_pasiva  = 0;
                batalla_aux_oro_ganado_total   = 0;
                batalla_aux_comida_gastado     = 0;
                batalla_aux_soldados_gastado   = 0;

                /// CIERRE
                cout << endl;

                cout << "Presiona cualquier tecla para volver al menu.";
                system("pause");
                system("cls");

                break;
            }

            case 1: /// MENU TIENDA
            {

                opcion_tienda=0;
                while(opcion_tienda!=4)
                {

                    ///LANNISTER ESLOGAN
                    rlutil::locate(44,25);
                    rlutil::setBackgroundColor(rlutil::RED);
                    cout<<"                                             "<<endl;
                    rlutil::locate(44,29);
                    rlutil::setBackgroundColor(rlutil::RED);
                    cout<<"                                             "<<endl;
                    for (int i=25; i<29; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::RED);
                        rlutil::locate(44,i);
                        cout<<"  "<<endl;
                    }
                    for (int i=25; i<29; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::RED);
                        rlutil::locate(87,i);
                        cout<<"  "<<endl;
                    }
                    rlutil::setBackgroundColor(rlutil::BLACK);
                    rlutil::setColor(rlutil::YELLOW);

                    rlutil::locate(48, 27), cout << "Un Lannister siempre paga sus deudas!";

                    /// Se hace esto para pintar nuevamente de negro el fondo y blanco las letras por debajo
                    rlutil::setBackgroundColor(rlutil::BLACK);
                    rlutil::setColor(rlutil::WHITE);

                    rlutil::locate(19,32);
                    cout << "  _____          __  __ ______    ____  ______   _______ _    _ _____   ____  _   _ ______  _____ " << endl;
                    rlutil::locate(19,33);
                    cout << " / ____|   /\\   |  \\/  |  ____|  / __ \\|  ____| |__   __| |  | |  __ \\ / __ \\| \\ | |  ____|/ ____|" << endl;
                    rlutil::locate(19,34);
                    cout << "| |  __   /  \\  | \\  / | |__    | |  | | |__       | |  | |__| | |__) | |  | |  \\| | |__  | (___  " << endl;
                    rlutil::locate(19,35);
                    cout << "| | |_ | / /\\ \\ | |\\/| |  __|   | |  | |  __|      | |  |  __  |  _  /| |  | | . ` |  __|  \\___ \\ " << endl;
                    rlutil::locate(19,36);
                    cout << "| |__| |/ ____ \\| |  | | |____  | |__| | |         | |  | |  | | | \\ \\| |__| | |\\  | |____ ____) |" << endl;
                    rlutil::locate(19,37);
                    cout << " \\_____/_/    \\_\\_|  |_|______|  \\____/|_|         |_|  |_|  |_|_|  \\_\\\\____/|_| \\_|______|_____/ " << endl;


                    ///LOGO
                    for (int i = 0; i < logo_dimensionY; i++)
                    {
                        for (int j = 0; j < logo_dimensionX; j++)
                        {
                            rlutil::locate(10 + j, 1 + i);
                            switch (lannister_logo[i][j])
                            {
                            case 1:
                                rlutil::setBackgroundColor(rlutil::RED);
                                cout << " ";
                                break;
                            case 2:
                                rlutil::setBackgroundColor(rlutil::YELLOW);
                                cout << " ";
                                break;
                            case 3:
                                rlutil::setBackgroundColor(rlutil::BROWN);
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
                            rlutil::locate(95 + j, 1 + i);
                            switch (lannister_logo[i][j])
                            {
                            case 1:
                                rlutil::setBackgroundColor(rlutil::RED);
                                cout << " ";
                                break;
                            case 2:
                                rlutil::setBackgroundColor(rlutil::YELLOW);
                                cout << " ";
                                break;
                            case 3:
                                rlutil::setBackgroundColor(rlutil::BROWN);
                                cout << " ";
                                break;
                            default:
                                rlutil::setBackgroundColor(rlutil::BLACK);
                                cout << " ";
                            }
                        }
                    }


                    ///RECUADRO EXTERIOR TIENDA ----------------------------------------------------------------------------

                    rlutil::setBackgroundColor(rlutil::RED);
                    rlutil::locate(43,2);
                    cout<<"                                             "<<endl;


                    for (int i=3; i<23; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::RED);
                        rlutil::locate(43,i);
                        cout<<"  "<<endl;
                    }
                    rlutil::setBackgroundColor(rlutil::RED);
                    rlutil::locate(45,22);
                    cout<<"                                           "<<endl;
                    for (int i=2; i<23; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::RED);
                        rlutil::locate(88,i);
                        cout<<"  "<<endl;
                    }

                    ///RECUADRO "JUEGO DE TRONOS" ----------------------------------------------------------------------------

                    rlutil::setBackgroundColor(rlutil::RED);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(47,4);
                    cout<<"---------------------------------------"<<endl;


                    rlutil::setBackgroundColor(rlutil::BROWN);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(47,5);
                    cout<<"                                       "<<endl;


                    rlutil::setBackgroundColor(rlutil::YELLOW);
                    rlutil::setColor(rlutil::BLACK);
                    rlutil::locate(51,5);
                    cout<<"        JUEGO DE TRONOS        "<<endl;


                    rlutil::setBackgroundColor(rlutil::RED);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(47,6);
                    cout<<"---------------------------------------"<<endl;

                    rlutil::hidecursor();
                    rlutil::setBackgroundColor(rlutil::BLACK);

                    ///RECUADRO "TIENDA" ----------------------------------------------------------------------------

                    rlutil::setBackgroundColor(rlutil::YELLOW);
                    rlutil::setColor(rlutil::BLACK);
                    rlutil::locate(47,9);
                    cout<<"-------------    TIENDA    ------------"<<endl;
                    rlutil::setBackgroundColor(rlutil::BLACK);

                    /// DATOS DEL CLAN ----------------------------------------------------------------------------

                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(47,11);
                    cout << "     |Oro                 : " << juego_oro                  << endl;
                    rlutil::locate(47,12);
                    cout << "     |Comida              : " << juego_comida               << endl;
                    rlutil::locate(47,13);
                    cout << "     |Soldados            : " << juego_soldados             << endl;
                    rlutil::locate(47,14);
                    cout << "     |Pasiva              : " << batalla_pasiva_probabilidad<< endl;
                    rlutil::setColor(rlutil::WHITE);

                    /// PINTAR "SOLDADOS" ----------------------------------------------------------------------------

                    if(opcion_tienda==0)
                    {
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        rlutil::locate(47,17);
                        cout<<" "<<(char)175<<"  SOLDADOS  $"<<tienda_valor_x_soldado<< " x "  <<tienda_articulo_soldados<< " UNIDADES  "<<(char)174<<" ";
                    }
                    else
                    {
                        rlutil::setBackgroundColor(rlutil::BLACK);
                        rlutil::locate(47,17);
                        cout<<"    SOLDADOS  $"<<tienda_valor_x_soldado<< " x "  <<tienda_articulo_soldados<< " UNIDADES    "<<endl;
                    }

                    /// PINTAR "COMIDA" ----------------------------------------------------------------------------

                    if(opcion_tienda==1)
                    {
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        rlutil::locate(47,18);
                        cout<<" "<<(char)175<<"    COMIDA  $" << tienda_valor_x_comida         << " x "  << tienda_articulo_comida          << " UNIDADES    "<<(char)174<<" ";
                    }
                    else
                    {
                        rlutil::setBackgroundColor(rlutil::BLACK);
                        rlutil::locate(47,18);
                        cout<<"      COMIDA  $" << tienda_valor_x_comida         << " x "  << tienda_articulo_comida          << " UNIDADES      "<<endl;
                    }

                    /// PINTAR "MEJORA PASIVA" ----------------------------------------------------------------------------

                    if(opcion_tienda==2)
                    {
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        rlutil::locate(47,19);
                        cout<<" "<<(char)175<<"   MEJORAR PASIVA  $" << tienda_valor_x_mejora_pasiva  << " x +" << tienda_articulo_mejora_pasiva   << "   "<<(char)174<<" ";
                    }
                    else
                    {
                        rlutil::setBackgroundColor(rlutil::BLACK);
                        rlutil::locate(47,19);
                        cout<<"     MEJORAR PASIVA  $" << tienda_valor_x_mejora_pasiva  << " x +" << tienda_articulo_mejora_pasiva   << "     "<<endl;
                    }

                    /// PINTAR "VOLVER MENU PRINCIPAL" ----------------------------------------------------------------------------

                    if(opcion_tienda==3)
                    {
                        rlutil::setBackgroundColor(rlutil::BROWN);
                        rlutil::locate(47,20);
                        cout<<" "<<(char)175<<"     VOLVER AL MENU PRINCIPAL      "<<(char)174<<" ";
                    }
                    else
                    {
                        rlutil::setBackgroundColor(rlutil::BLACK);
                        rlutil::locate(47,20);
                        ///   "-------------    TIENDA    ------------"
                        cout<<"       VOLVER AL MENU PRINCIPAL        "<<endl;
                    }
                    rlutil::setBackgroundColor(rlutil::BLACK);

                    /// LOGICA DE LA TIENDA ----------------------------------------------------------------------------

                    tecla_tienda= rlutil::getkey();
                    switch(tecla_tienda)
                    {
                    case 14: //ARRIBA
                        rlutil::locate(50,10+opcion_tienda);
                        cout<<" "<<endl;
                        opcion_tienda--;
                        if(opcion_tienda<0)
                        {
                            opcion_tienda=3;
                        }
                        break;
                    case 15: //ABAJO
                        rlutil::locate(50,10+opcion_tienda);
                        cout<<" "<<endl;
                        opcion_tienda++;
                        if(opcion_tienda>3)
                        {
                            opcion_tienda=0;
                        }
                        break;
                    case 1: //ENTER
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

                                cout << "Presiona cualquier tecla para volver al menu.";
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
                            opcion_tienda=4;
                            system("cls");
                        }
                    }

                } ///TERMINA WHILE DE TIENDA
                break;
            }///TERMINA SWITHC DE TIENDA
            case 2: /// SALIR DEL JUEGO
            {
                return 0;
            }
            } /// TERMINA SWITH DEL MENU INTERNO
        } /// TERMINA SWITCH DE LOGICA DEL MENU
    } /// TERMINA WHILE DE MENU PRINCIPAL
}
