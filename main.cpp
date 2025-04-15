#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

using namespace std;
#include "rlutil.h"

int main()
{
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

    // pasiva STARK
    const float batalla_pasiva_base = 0.5;
    float batalla_pasiva_probabilidad = batalla_pasiva_base;

    const float batalla_pasiva_beneficio = 0.2; // 20% extra en oro ganado
    float batalla_pasiva_nro_random;

    bool batalla_pasiva_eval_activada;

    // combate
    int batalla_aux_oro_ganado_parcial = 0;
    int batalla_aux_oro_ganado_pasiva  = 0;
    int batalla_aux_oro_ganado_total   = 0;
    int batalla_aux_comida_gastado;
    int batalla_aux_soldados_gastado;

    bool batalla_victoria_eval;

    const int max_perdida_comida = 1000;
    const int max_perdida_soldados = 500;


    // ESTADISTICAS
    int est_batalla_victorias_cantidad   = 0;
    int est_batalla_derrotas_cantidad    = 0;

    int est_batalla_pasiva_uso_cantidad  = 0;
    int est_batalla_pasiva_oro_ganado    = 0;

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

    // Menu batalla
    int while_menu_batalla = 1;
    int opcion_menu_batalla = 0;

    int aux_tienda_gasto;

    // Generador de numeros aleatorios
    srand(time(0));

    //Logo dimensiones
    const int logo_dimensionY = 30;
    const int logo_dimensionX = 44;

    const int stark_logo[logo_dimensionY][logo_dimensionX] =
{
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,3,3,4,4,3,3,3,3,3,3,3,3,3,2,2,4,3,3,3,3,3,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,4,4,4,3,3,3,3,4,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4},
    {4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4},
    {4,4,4,4,4,4,4,4,4,4,3,4,3,3,3,3,4,4,3,3,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4},
    {4,4,4,4,4,4,4,4,4,3,3,3,4,3,3,3,3,3,4,4,3,3,3,3,3,3,3,3,4,3,3,3,3,3,3,3,3,3,4,4,4},
    {4,4,4,4,4,4,4,3,3,3,3,3,3,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,2,4,2,4,2,4,2,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,3,3,3,4,3,4,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4},
    {4,4,4,3,3,3,3,3,3,4,3,4,3,3,3,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,2,4,2,4,4,4,4,4,4},
    {4,3,3,4,4,3,3,3,3,3,4,3,3,3,3,3,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4},
    {4,4,4,3,3,3,3,3,3,4,3,4,3,3,3,4,3,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,3,3,4,3,3,3,4,3,4,3,3,3,4,3,3,3,3,3,3,3,4,4,4,4,4,3,3,3,4,4,4,4,4,4,4},
    {4,4,4,4,4,3,3,4,3,3,3,3,3,4,3,3,3,3,3,4,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4,3,3,3,3,4,3,4,3,3,3,3,3,4,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,4,3,4,3,3,3,4,3,3,3,4,3,4,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,4,4,3,3,3,3,3,4,3,4,3,3,3,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,3,4,3,3,3,3,3,3,4,3,3,3,3,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,4,3,4,3,3,3,3,4,3,4,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,4,3,3,4,3,3,4,3,3,3,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,3,3,3,3,4,4,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,4,4,3,4,3,3,4,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,4,4,4,3,3,3,3,4,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,3,3,4,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4}
};


    while(true)
    {
        ///RECUADRO FRASE ----------------------------------------------------------------------------
        rlutil::locate(53,26);  // +9
        rlutil::setBackgroundColor(rlutil::DARKGREY);
        cout<<"                                             "<<endl;
        rlutil::locate(53,30);  // +9
        rlutil::setBackgroundColor(rlutil::DARKGREY);
        cout<<"                                             "<<endl;
        for (int i=26; i<30; i++)
        {
            rlutil::setBackgroundColor(rlutil::DARKGREY);
            rlutil::locate(53,i);  // +9
            cout<<"  "<<endl;
        }
        for (int i=26; i<30; i++)
        {
            rlutil::setBackgroundColor(rlutil::DARKGREY);
            rlutil::locate(96,i);  // +9
            cout<<"  "<<endl;
        }

        rlutil::setBackgroundColor(rlutil::DARKGREY);
        rlutil::setColor(rlutil::BLUE);
        rlutil::locate(65, 28);
        cout << "  Winter is coming!  ";  // +9

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

        ///LOGOS ESCUDO STARK ----------------------------------------------------------------------------
        for (int i = 0; i < logo_dimensionY; i++)
        {
            for (int j = 0; j < logo_dimensionX; j++)
            {
                rlutil::locate(4 + j, 2 + i);
                switch (stark_logo[i][j])
                {
                    case 2:
                        rlutil::setBackgroundColor(7);   // claro
                        break;
                    case 3:
                        rlutil::setBackgroundColor(8);   // Gris medio
                        break;
                    case 4:
                        rlutil::setBackgroundColor(0);  // gris oscuro
                        break;
//                    case 3:
//                        rlutil::setBackgroundColor(7);   // Gris claro
//                        break;
                }
                cout << " ";
            }
        }

        for (int i = 0; i < logo_dimensionY; i++)
        {
            for (int j = 0; j < logo_dimensionX; j++)
            {
                rlutil::locate(103 + j, 2 + i);
                switch (stark_logo[i][j])
                {
                    case 2:
                        rlutil::setBackgroundColor(7);   // claro
                        break;
                    case 3:
                        rlutil::setBackgroundColor(8);   // Gris medio
                        break;
                    case 4:
                       rlutil::setBackgroundColor(0);  // gris oscuro
                        break;
//                    case 3:
//                        rlutil::setBackgroundColor(7);   // Gris claro
//                        break;
                }
                cout << " ";
            }
        }
        ///RECUADRO EXTERIOR ----------------------------------------------------------------------------

        rlutil::setBackgroundColor(rlutil::DARKGREY);
        rlutil::locate(52,3);
        cout<<"                                             "<<endl;

        for (int i=3; i<23; i++)
        {
            rlutil::setBackgroundColor(rlutil::DARKGREY);
            rlutil::locate(52,i+1);
            cout<<"  "<<endl;
        }
        rlutil::setBackgroundColor(rlutil::DARKGREY);
        rlutil::locate(54,23);
        cout<<"                                           "<<endl;

        for (int i=2; i<23; i++)
        {
            rlutil::setBackgroundColor(rlutil::DARKGREY);
            rlutil::locate(97,i+1);
            cout<<"  "<<endl;
        }

        /// RECUADRO "JUEGO DE TRONOS"
        rlutil::setBackgroundColor(rlutil::DARKGREY);
        rlutil::setColor(rlutil::WHITE);
        rlutil::locate(56,5);
        cout<<"---------------------------------------"<<endl;

        rlutil::setBackgroundColor(rlutil::LIGHTBLUE);
        rlutil::setColor(rlutil::WHITE);
        rlutil::locate(56,6);
        cout<<"                                       "<<endl;

        rlutil::setBackgroundColor(rlutil::BLUE);
        rlutil::setColor(rlutil::BLACK);
        rlutil::locate(60,6);
        cout<<"        JUEGO DE TRONOS        "<<endl;

        rlutil::setBackgroundColor(rlutil::DARKGREY);
        rlutil::setColor(rlutil::WHITE);
        rlutil::locate(56,7);
        cout<<"---------------------------------------"<<endl;
        rlutil::setBackgroundColor(rlutil::BLACK);

        rlutil::hidecursor();

        /// RECUADRO "STARK"
        rlutil::setBackgroundColor(rlutil::BLUE);
        rlutil::setColor(rlutil::BLACK);
        rlutil::locate(58,10);
        cout<<"-------------  STARK  -------------"<<endl;
        rlutil::setBackgroundColor(rlutil::BLACK);

        /// DATOS DEL CLAN
        rlutil::locate(56,12);
        rlutil::setColor(rlutil::WHITE);
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

        /// PINTAR "IR A LA BATALLA"
        if(opcion_menu==0)
        {
            rlutil::setBackgroundColor(rlutil::LIGHTBLUE);
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
            rlutil::setBackgroundColor(rlutil::LIGHTBLUE);
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
            rlutil::setBackgroundColor(rlutil::LIGHTBLUE);
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

        tecla_menu= rlutil::getkey();
        switch(tecla_menu)
        {
        case 14: //ARRIBA
//            rlutil::locate(59,10+opcion_menu);  // +9
//            cout<<" "<<endl;
            opcion_menu--;
            if(opcion_menu<0)
            {
                opcion_menu=2;
            }
            break;
        case 15: //ABAJO
//            rlutil::locate(59,10+opcion_menu);  // +9
//            cout<<" "<<endl;
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
                while(true){

                    ///VALIDO APTITUD PARA LA GUERRA
                    if (batalla_actual >= batalla_duracion_guerra)
                    {
                        cout << "No hay mas batallas por luchar, puedes descansar!" << endl;

                        cout << endl;

                        system("pause");
                        system("cls");
                        break;
                    }
                    if(juego_comida < max_perdida_comida)
                    {

                        rlutil::locate(43,15);cout << "No tienes la cantidad de comida suficiente para ir a la batalla!" << endl;
                        rlutil::locate(49,16);cout << "Compra mas comida en la tienda y vuelve mas tarde..." << endl;
                        rlutil::locate(55,20);system("pause");
                        system("cls");
                        break;
                    }

                    if(juego_soldados < max_perdida_soldados)
                    {
                        rlutil::locate(43,15);cout << "No tienes la cantidad de soldados suficientes para ir a la batalla!" << endl;
                        rlutil::locate(49,16);cout << "Compra mas soldados en la tienda y vuelve mas tarde..." << endl;
                        rlutil::locate(55,20);system("pause");
                        system("cls");
                        break;
                    }

                    ///UI "DESEA IR A LA GUERRA?"
                    {

                    ///RECUADRO EXTERIOR BATALLA -------------------------------------------
                    rlutil::setBackgroundColor(rlutil::DARKGREY);
                    rlutil::locate(52,3);
                    cout<<"                                             "<<endl;

                    for (int i=3; i<23; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::DARKGREY);
                        rlutil::locate(52,i+1);
                        cout<<"  "<<endl;
                    }
                    rlutil::setBackgroundColor(rlutil::DARKGREY);
                    rlutil::locate(54,23);
                    cout<<"                                           "<<endl;

                    for (int i=2; i<23; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::DARKGREY);
                        rlutil::locate(97,i+1);
                        cout<<"  "<<endl;
                    }

                    /// RECUADRO "JUEGO DE TRONOS"
                    rlutil::setBackgroundColor(rlutil::DARKGREY);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(56,5);
                    cout<<"---------------------------------------"<<endl;

                    rlutil::setBackgroundColor(rlutil::LIGHTBLUE);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(56,6);
                    cout<<"                                       "<<endl;

                    rlutil::setBackgroundColor(rlutil::BLUE);
                    rlutil::setColor(rlutil::BLACK);
                    rlutil::locate(60,6);
                    cout<<"        JUEGO DE TRONOS        "<<endl;

                    rlutil::setBackgroundColor(rlutil::DARKGREY);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(56,7);
                    cout<<"---------------------------------------"<<endl;
                    rlutil::setBackgroundColor(rlutil::BLACK);

                    rlutil::hidecursor();

                    /// RECUADRO "DESEAS IR A LA GUERRA"
                    rlutil::setBackgroundColor(rlutil::BLUE);
                    rlutil::setColor(rlutil::BLACK);
                    rlutil::locate(56,10);
                    cout<<"------   DESEAS IR A LA GUERRA?  ------"<<endl;
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::setBackgroundColor(rlutil::BLACK);

                    /// DATOS RECURSOS
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

                    ///LOGOS ESCUDO LANNISTER ----------------------------------------------------------------------------
                    for (int i = 0; i < logo_dimensionY; i++)
                    {
                        for (int j = 0; j < logo_dimensionX; j++)
                        {
                            rlutil::locate(4 + j, 2 + i);  // +9
                            switch (stark_logo[i][j])
                            {
                            case 2:
                                rlutil::setBackgroundColor(7);
                                cout << " ";
                                break;
                            case 3:
                                rlutil::setBackgroundColor(8);
                                cout << " ";
                                break;
                            case 4:
                                rlutil::setBackgroundColor(0);
                                cout << " ";
                                break;
                            }
                        }
                    }

                    for (int i = 0; i < logo_dimensionY; i++)
                    {
                        for (int j = 0; j < logo_dimensionX; j++)
                        {
                            rlutil::locate(103 + j, 2 + i);  // +9
                            switch (stark_logo[i][j])
                            {
                            case 2:
                                rlutil::setBackgroundColor(7);
                                cout << " ";
                                break;
                            case 3:
                                rlutil::setBackgroundColor(8);
                                cout << " ";
                                break;
                            case 4:
                                rlutil::setBackgroundColor(0);
                                cout << " ";
                                break;
                            }
                        }
                    }

                    }//CIERRE UI

                    while_menu_batalla = 1;
                    opcion_menu_batalla = 0;
                    while(while_menu_batalla)
                    {
                        ///PINTA SI
                        if(opcion_menu_batalla==0)
                        {
                            rlutil::setBackgroundColor(rlutil::LIGHTBLUE);
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
                            rlutil::setBackgroundColor(rlutil::LIGHTBLUE);
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

                        //cout << "Si ganas, obtendras un " << batalla_pasiva_beneficio * 100 << "% de oro extra!" << endl;
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
                        rlutil::setBackgroundColor(rlutil::BLUE);
                        rlutil::setColor(rlutil::BLACK);
                        rlutil::locate(56,10);
                        cout<<"----    HAS GANADO LA BATALLA!!!   ----"<<endl;
                        rlutil::setColor(rlutil::WHITE);
                        rlutil::setBackgroundColor(rlutil::BLACK);

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
                        juego_oro += batalla_aux_oro_ganado_total;

                        est_batalla_pasiva_oro_ganado  += batalla_aux_oro_ganado_pasiva;
                        est_batalla_comida_perdido     += batalla_aux_comida_gastado;
                        est_batalla_soldados_perdido   += batalla_aux_soldados_gastado;
                        est_batalla_oro_ganado         += batalla_aux_oro_ganado_total;
                        est_batalla_victorias_cantidad ++;

                    }
                    else
                    {
                         rlutil::setBackgroundColor(rlutil::BLUE);
                        rlutil::setColor(rlutil::BLACK);
                        rlutil::locate(56,10);
                        cout<<"----   HAS PERDIDO LA BATALLA!!!   ----"<<endl;
                        rlutil::setColor(rlutil::WHITE);
                        rlutil::setBackgroundColor(rlutil::BLACK);

                        batalla_aux_comida_gastado   = rand() % max_perdida_comida;
                        batalla_aux_soldados_gastado = rand() % max_perdida_soldados;

                        est_batalla_comida_perdido    += batalla_aux_comida_gastado;
                        est_batalla_soldados_perdido  += batalla_aux_soldados_gastado;
                        est_batalla_derrotas_cantidad ++;

                    }

                    ///UI DATOS DE GUERRA
                    {
                    ///RECUADRO EXTERIOR RESUMEN
                    rlutil::setBackgroundColor(rlutil::DARKGREY);
                    rlutil::locate(52,3);
                    cout<<"                                             "<<endl;

                    for (int i=3; i<23; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::DARKGREY);
                        rlutil::locate(52,i+1);
                        cout<<"  "<<endl;
                    }
                    rlutil::setBackgroundColor(rlutil::DARKGREY);
                    rlutil::locate(54,23);
                    cout<<"                                           " <<endl;

                    for (int i=2; i<23; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::DARKGREY);
                        rlutil::locate(97,i+1);
                        cout<<"  "<<endl;
                    }

                    /// RECUADRO "JUEGO DE TRONOS"
                    rlutil::setBackgroundColor(rlutil::DARKGREY);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(56,5);
                    cout<<"---------------------------------------"<<endl;

                    rlutil::setBackgroundColor(rlutil::LIGHTBLUE);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(56,6);
                    cout<<"                                       "<<endl;

                    rlutil::setBackgroundColor(rlutil::BLUE);
                    rlutil::setColor(rlutil::BLACK);
                    rlutil::locate(60,6);
                    cout<<"        JUEGO DE TRONOS        "<<endl;

                    rlutil::setBackgroundColor(rlutil::DARKGREY);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(56,7);
                    cout<<"---------------------------------------"<<endl;
                    rlutil::setBackgroundColor(rlutil::BLACK);

                    rlutil::hidecursor();

                    /// DATOS GUERRA
                    rlutil::locate(56,14);
                    cout << "    |Oro ganado en batalla : " << batalla_aux_oro_ganado_parcial << endl;
                    rlutil::locate(56,15);
                    cout << "    |Oro ganado por pasiva : " << batalla_aux_oro_ganado_pasiva  << endl;
                    rlutil::locate(56,16);
                    cout << "    |Oro ganado en total   : " << batalla_aux_oro_ganado_total   << endl;
                    rlutil::locate(56,17);
                    cout << "    |Comida perdida        : " << batalla_aux_comida_gastado     << endl;
                    rlutil::locate(56,18);
                    cout << "    |Soldados perdidos     : " << batalla_aux_soldados_gastado   << endl;

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
                    ///LOGOS ESCUDO LANNISTER ----------------------------------------------------------------------------
                    for (int i = 0; i < logo_dimensionY; i++)
                    {
                        for (int j = 0; j < logo_dimensionX; j++)
                        {
                            rlutil::locate(8 + j, 2 + i);  // +9
                            switch (stark_logo[i][j])
                            {
                            case 2:
                                rlutil::setBackgroundColor(7);
                                cout << " ";
                                break;
                            case 3:
                                rlutil::setBackgroundColor(8);
                                cout << " ";
                                break;
                            case 4:
                                rlutil::setBackgroundColor(0);
                                cout << " ";
                                break;
                            }
                        }
                    }

                    for (int i = 0; i < logo_dimensionY; i++)
                    {
                        for (int j = 0; j < logo_dimensionX; j++)
                        {
                            rlutil::locate(108 + j, 2 + i);  // +9
                            switch (stark_logo[i][j])
                            {
                            case 2:
                                rlutil::setBackgroundColor(7);
                                cout << " ";
                                break;
                            case 3:
                                rlutil::setBackgroundColor(8);
                                cout << " ";
                                break;
                            case 4:
                                rlutil::setBackgroundColor(0);
                                cout << " ";
                                break;
                            }
                        }
                    }

                    }//CIERRE UI


                    //Descuento los recursos perdidos
                    juego_soldados -= batalla_aux_soldados_gastado;
                    juego_comida -= batalla_aux_comida_gastado;

                    batalla_aux_oro_ganado_parcial = 0;
                    batalla_aux_oro_ganado_pasiva  = 0;
                    batalla_aux_oro_ganado_total   = 0;
                    batalla_aux_comida_gastado     = 0;
                    batalla_aux_soldados_gastado   = 0;

                    /// CIERRE

                    rlutil::locate(56,21);
                    rlutil::setBackgroundColor(rlutil::LIGHTBLUE);
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

                opcion_tienda=0;
                while(opcion_tienda!=4)
                {

                    ///RECUADRO FRASE ----------------------------------------------------------------------------
                    rlutil::locate(53,26);  // +9
                    rlutil::setBackgroundColor(rlutil::DARKGREY);
                    cout<<"                                             "<<endl;
                    rlutil::locate(53,30);  // +9
                    rlutil::setBackgroundColor(rlutil::DARKGREY);
                    cout<<"                                             "<<endl;
                    for (int i=26; i<30; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::DARKGREY);
                        rlutil::locate(53,i);  // +9
                        cout<<"  "<<endl;
                    }
                    for (int i=26; i<30; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::DARKGREY);
                        rlutil::locate(96,i);  // +9
                        cout<<"  "<<endl;
                    }

                    rlutil::setBackgroundColor(rlutil::DARKGREY);
                    rlutil::setColor(rlutil::BLUE);
                    rlutil::locate(65, 28);
                    cout << "  Winter is coming!  ";

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


                    ///LOGOS ESCUDO STARK ----------------------------------------------------------------------------
                    for (int i = 0; i < logo_dimensionY; i++)
                    {
                        for (int j = 0; j < logo_dimensionX; j++)
                        {
                            rlutil::locate(4 + j, 2 + i);
                            switch (stark_logo[i][j])
                            {
                                case 2:
                                    rlutil::setBackgroundColor(15);   // claro
                                    break;
                                case 3:
                                    rlutil::setBackgroundColor(8);   // Gris medio
                                    break;
                                case 4:
                                    rlutil::setBackgroundColor(0);  // gris oscuro
                                    break;
//                                case 3:
//                                    rlutil::setBackgroundColor(7);   // Gris claro
//                                    break;
                            }
                            cout << " ";
                        }
                    }

                    for (int i = 0; i < logo_dimensionY; i++)
                    {
                        for (int j = 0; j < logo_dimensionX; j++)
                        {
                            rlutil::locate(103 + j, 2 + i);
                            switch (stark_logo[i][j])
                            {
                                case 2:
                                    rlutil::setBackgroundColor(15);   // claro
                                    break;
                                case 3:
                                    rlutil::setBackgroundColor(8);   // Gris medio
                                    break;
                                case 4:
                                    rlutil::setBackgroundColor(0);  // gris oscuro
                                    break;
//                                case 3:
//                                    rlutil::setBackgroundColor(7);   // Gris claro
//                                    break;
                            }
                            cout << " ";
                        }
                    }

                    ///RECUADRO EXTERIOR TIENDA ----------------------------------------------------------------------------

                    rlutil::setBackgroundColor(rlutil::DARKGREY);
                    rlutil::locate(52,3);
                    cout<<"                                             "<<endl;

                    for (int i=3; i<23; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::DARKGREY);
                        rlutil::locate(52,i+1);
                        cout<<"  "<<endl;
                    }
                    rlutil::setBackgroundColor(rlutil::DARKGREY);
                    rlutil::locate(54,23);
                    cout<<"                                           "<<endl;

                    for (int i=2; i<23; i++)
                    {
                        rlutil::setBackgroundColor(rlutil::DARKGREY);
                        rlutil::locate(97,i+1);
                        cout<<"  "<<endl;
                    }

                    /// RECUADRO "JUEGO DE TRONOS"
                    rlutil::setBackgroundColor(rlutil::DARKGREY);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(56,5);
                    cout<<"---------------------------------------"<<endl;

                    rlutil::setBackgroundColor(rlutil::LIGHTBLUE);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(56,6);
                    cout<<"                                       "<<endl;

                    rlutil::setBackgroundColor(rlutil::BLUE);
                    rlutil::setColor(rlutil::BLACK);
                    rlutil::locate(60,6);
                    cout<<"        JUEGO DE TRONOS        "<<endl;

                    rlutil::setBackgroundColor(rlutil::DARKGREY);
                    rlutil::setColor(rlutil::WHITE);
                    rlutil::locate(56,7);
                    cout<<"---------------------------------------"<<endl;
                    rlutil::setBackgroundColor(rlutil::BLACK);

                    rlutil::hidecursor();

                    /// RECUADRO "STARK"
                    rlutil::setBackgroundColor(rlutil::BLUE);
                    rlutil::setColor(rlutil::BLACK);
                    rlutil::locate(56,10);
                    cout<<"-------------    TIENDA    ------------"<<endl;
                    rlutil::setBackgroundColor(rlutil::BLACK);


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

                    if(opcion_tienda==0)
                    {
                        rlutil::setBackgroundColor(rlutil::LIGHTBLUE);
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
                        rlutil::setBackgroundColor(rlutil::LIGHTBLUE);
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
                        rlutil::setBackgroundColor(rlutil::LIGHTBLUE);
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
                        rlutil::setBackgroundColor(rlutil::LIGHTBLUE);
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

                    tecla_tienda= rlutil::getkey();
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
