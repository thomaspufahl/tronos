#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{
    /// JUGABILIDAD
    const int juego_presupuesto_base = 50000;
    int juego_oro = juego_presupuesto_base;

    const int juego_soldados_base = 0;
    int juego_soldados = juego_soldados_base;

    const int juego_comida_base = 0;
    int juego_comida = juego_comida_base;

    const float juego_pasiva_base = 0.5;
    float juego_pasiva= juego_pasiva_base;

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
    float batalla_pasiva;

    /// MENU
    while(true)
    {
        system("cls");

        cout << "---------------------------------------"               << endl;
        cout << "            Juego de Tronos            "               << endl;
        cout << "---------------------------------------"               << endl;
        cout << "~LANNISTER "                                           << endl;
        cout << "|batallas realizadas : " << batalla_actual             << endl;
        cout << "|presupuesto         : " << juego_presupuesto_base     << endl;
        cout << "|oro                 : " << juego_oro                  << endl;
        cout << "|comida              : " << juego_comida               << endl;
        cout << "|soldados            : " << juego_soldados             << endl;
        cout << "|pasiva              : " << juego_pasiva               << endl;
        cout << "---------------------------------------"               << endl;
        cout << idx_opcion_menu_batalla << ". IR A LA BATALLA"          << endl;
        cout << idx_opcion_menu_tienda  << ". TIENDA"                   << endl;
        cout << idx_opcion_menu_volver  << ". VOLVER AL MENU PRINCIPAL" << endl;
        cout << "---------------------------------------"               << endl;
        cout << "OPCION: ";
        cin >> cin_opcion_menu;

        system("cls");

        switch(cin_opcion_menu)
        {
            case idx_opcion_menu_batalla:
            {
                if (batalla_actual > batalla_duracion_guerra)
                {
                    cout << "No hay mas batallas por luchar, puedes descansar!" << endl;

                    cout << endl;

                    cout << "Presiona cualquier tecla para volver al menu.";
                    system("pause");
                    break;
                }

                cout << "Soldados: "                << juego_soldados            << endl;
                cout << "Comida: "                  << juego_comida              << endl;
                cout << "Probabilidad de pasiva: "  << juego_pasiva              << endl;

                cout << endl;

                //? considerar usar 1 y 0 para no usar el ternario
                cout << "Deseas continuar? 1(SI) / 2(NO)" << endl;
                cout << "opcion: ";
                cin >> cin_batalla_deseas_continuar;
                eval_deseas_continuar = cin_batalla_deseas_continuar == 2 ? false : true;

                if (!eval_deseas_continuar)
                {
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

                // pasiva Lannister
                batalla_pasiva = (float)(rand() % 100) / 100;

                cout << "NRO RANDOM: " << batalla_pasiva << endl;

                if (batalla_pasiva < juego_pasiva)
                {
                    cout << "PASIVA ACTIVADA!" << endl;
                    est_batalla_pasiva_uso_cantidad++;
                }

                /// CIERRE
                cout << endl;

                cout << "Presiona cualquier tecla para volver al menu.";
                system("pause");

                break;
            }
            case idx_opcion_menu_tienda:
            {
                while(true)
                {
                    if (cin_opcion_tienda == idx_opcion_tienda_volver)
                    {
                        break;
                    }

                    system("cls");

                    cout << "---------------------------------------"  << endl;
                    cout << "~LANNISTER"                               << endl;
                    cout << "|presupuesto: " << juego_presupuesto_base << endl;
                    cout << "|oro        : " << juego_oro              << endl;
                    cout << "|comida     : " << juego_comida           << endl;
                    cout << "|soldados   : " << juego_soldados         << endl;
                    cout << "|pasiva     : " << juego_pasiva           << endl;
                    cout << "---------------------------------------"  << endl;
                    cout << "TIENDA" << endl;
                    cout << "---------------------------------------"  << endl;
                    cout << "1. Soldados          $" << tienda_valor_x_soldado        << " x "  << tienda_articulo_soldados        << " unidades." << endl;
                    cout << "2. Comida            $" << tienda_valor_x_comida         << " x "  << tienda_articulo_comida          << " unidades." << endl;
                    cout << "3. Mejorar pasiva    $" << tienda_valor_x_mejora_pasiva  << " x +" << tienda_articulo_mejora_pasiva   << "." << endl;
                    cout << "4. Volver al menu principal" << endl;
                    cout << "---------------------------------------" << endl;
                    cout << "Opcion: ";
                    cin >> cin_opcion_tienda;

                    system("cls");

                    switch(cin_opcion_tienda)
                    {
                        case idx_opcion_tienda_soldados:
                        {
                            if (juego_oro >= tienda_valor_x_soldado)
                            {
                                aux_tienda_gasto = tienda_valor_x_soldado;

                                juego_soldados += tienda_articulo_soldados;
                                juego_oro -= aux_tienda_gasto;

                                est_tienda_total_gastado_oro += aux_tienda_gasto;
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
                        case idx_opcion_tienda_comida:
                        {
                            if (juego_oro >= tienda_valor_x_comida)
                            {
                                aux_tienda_gasto = tienda_valor_x_comida;

                                juego_comida += tienda_articulo_comida;
                                juego_oro -= aux_tienda_gasto;

                                est_tienda_total_gastado_oro += aux_tienda_gasto;
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
                        case idx_opcion_tienda_mejora_pasiva:
                        {
                            if (juego_oro >= tienda_valor_x_mejora_pasiva)
                            {
                                juego_pasiva += tienda_articulo_mejora_pasiva;
                                juego_oro -= tienda_valor_x_mejora_pasiva;
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
                        case idx_opcion_tienda_volver:
                        default:
                        {
                            break;
                        }
                    }
                }

                break;
            }
            case idx_opcion_menu_volver:
            {
                return 0;
            }
        }
    }
}
