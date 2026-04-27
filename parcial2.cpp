#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// --- Procedimientos y Funciones ---
void mostrarMenu() {
    cout << "\n--- SISTEMA DE VENTAS ---" << endl;
    cout << "1. Registrar una venta" << endl;
    cout << "2. Mostrar todas las ventas" << endl;
    cout << "3. Generar reporte general" << endl;
    cout << "4. Buscar ventas por monto" << endl;
    cout << "5. Salir" << endl;
    cout << "Seleccione una opcion: ";
}

double calcularDescuento(double subtotal, int tipoPago) {
    double desc = 0;
    if (subtotal >= 500) desc += subtotal * 0.05;
    if (tipoPago == 1 && subtotal >= 300) desc += subtotal * 0.02;
    return desc;
}

double calcularTotal(double subtotal, double descuento) {
    return subtotal - descuento;
}

void registrarVenta() {
    string cliente, producto;
    int cantidad, tipoPago;
    double precio, subtotal, descuento, total;

    cin.ignore();
    cout << "Nombre del cliente: "; getline(cin, cliente);
    cout << "Nombre del producto: "; getline(cin, producto);

    do {
        cout << "Cantidad (mayor a 0): "; cin >> cantidad;
    } while (cantidad <= 0);

    do {
        cout << "Precio unitario (mayor a 0): "; cin >> precio;
    } while (precio <= 0);

    do {
        cout << "Tipo de pago (1=Efectivo, 2=Tarjeta, 3=Transferencia): "; cin >> tipoPago;
        if (tipoPago < 1 || tipoPago > 3) cout << "Opcion invalida." << endl;
    } while (tipoPago < 1 || tipoPago > 3);

    subtotal = cantidad * precio;
    descuento = calcularDescuento(subtotal, tipoPago);
    total = calcularTotal(subtotal, descuento);

    ofstream archivo("ventas.txt", ios::app);
    if (archivo.is_open()) {
        // Guardamos los datos separados por comas
        archivo << cliente << "," << producto << "," << cantidad << "," << precio << "," 
                << tipoPago << "," << subtotal << "," << descuento << "," << total << endl;
        archivo.close();
        cout << "Venta registrada con exito." << endl;
    }
}

void mostrarVentas() {
    ifstream archivo("ventas.txt");
    string linea;
    cout << "\n--- LISTADO DE VENTAS ---" << endl;
    if (archivo.is_open()) {
        while (getline(archivo, linea)) {
            cout << linea << endl;
        }
        archivo.close();
    } else {
        cout << "No hay ventas registradas." << endl;
    }
}

// --- Programa Principal ---
int main() {
    int opcion;

    do {
        mostrarMenu();
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue; // Uso de continue para reintentar si meten letras
        }

        switch (opcion) {
            case 1:
                registrarVenta();
                break;
            case 2:
                mostrarVentas();
                break;
            case 3: { // Reporte General
                ifstream archivo("ventas.txt");
                string c, p, temp;
                double cant, pre, tPago, sub, desc, tot;
                double sumaTotal = 0, totalVentas = 0, descCont = 0;
                int efec = 0, tarj = 0, trans = 0;

                while (getline(archivo, c, ',')) {
                    getline(archivo, p, ',');
                    getline(archivo, temp, ','); cant = stod(temp);
                    getline(archivo, temp, ','); pre = stod(temp);
                    getline(archivo, temp, ','); tPago = stoi(temp);
                    getline(archivo, temp, ','); sub = stod(temp);
                    getline(archivo, temp, ','); desc = stod(temp);
                    getline(archivo, temp); tot = stod(temp);

                    totalVentas++;
                    sumaTotal += tot;
                    if (tPago == 1) efec++;
                    else if (tPago == 2) tarj++;
                    else trans++;
                    if (desc > 0) descCont++;
                }
                archivo.close();

                if (totalVentas > 0) {
                    cout << "\n--- REPORTE GENERAL ---" << endl;
                    cout << "Total de ventas: " << totalVentas << endl;
                    cout << "Suma total vendida: Q" << sumaTotal << endl;
                    cout << "Promedio de venta: Q" << (sumaTotal / totalVentas) << endl;
                    cout << "Ventas en Efectivo: " << efec << "\nTarjeta: " << tarj << "\nTransferencia: " << trans << endl;
                    cout << "Ventas con descuento: " << descCont << endl;
                } else cout << "No hay datos." << endl;
                break;
            }
            case 4: { // Buscar por monto
                double montoBusqueda;
                cout << "Mostrar ventas mayores o iguales a: Q"; cin >> montoBusqueda;
                
                ifstream archivo("ventas.txt");
                string c, p, temp;
                double tot;
                bool encontro = false;

                while (getline(archivo, c, ',')) {
                    getline(archivo, p, ',');
                    for(int i=0; i<5; i++) getline(archivo, temp, ','); // Saltamos datos intermedios
                    getline(archivo, temp); tot = stod(temp);

                    if (tot >= montoBusqueda) {
                        cout << "Cliente: " << c << " | Total: Q" << tot << endl;
                        encontro = true;
                    }
                }
                if (!encontro) cout << "No hay ventas mayores a ese monto." << endl;
                archivo.close();
                break;
            }
            case 5:
                cout << "Saliendo del sistema..." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
                break;
        }
    } while (opcion != 5);

    return 0;
}
