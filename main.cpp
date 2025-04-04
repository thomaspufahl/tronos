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
        cout << "|pasiva              : " << batalla_pasiva_probabilidad<< endl;
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

                    cout << "---------------------------------------"       << endl;
                    cout << "~LANNISTER"                                    << endl;
                    cout << "|presupuesto: " << juego_presupuesto_base      << endl;
                    cout << "|oro        : " << juego_oro                   << endl;
                    cout << "|comida     : " << juego_comida                << endl;
                    cout << "|soldados   : " << juego_soldados              << endl;
                    cout << "|pasiva     : " << batalla_pasiva_probabilidad << endl;
                    cout << "---------------------------------------"       << endl;
                    cout << "TIENDA"                                        << endl;
                    cout << "---------------------------------------"       << endl;
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
                        case idx_opcion_tienda_comida:
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
                        case idx_opcion_tienda_mejora_pasiva:
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
