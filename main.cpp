#include <iostream>
#include <string>
#include <fstream> 
#include "src/DoubleCircularList.h"
#include "src/Avion.h"
#include "src/Piloto.h"
#include "src/arbolBB.h"
#include "src/json.h"
#include "src/json.cpp"
#include "src/ArbolB.h"
#include "src/Grafo.h"
#include "src/Hash.h"
#include "src/Matriz.h"
using namespace std;
void menu();
void cargar_aviones();
void cargarPilotos();
void cargarRutas();
void cargarMovimientos();
void reporte_aviones_mantenimiento();
void reportes();
void Recorridos();
//void ConsultarHorasVuelo();
void recomendacionRuta();
Hash tablaPilotos(18);
MatrizDispersa matrizVuelosCiudades;
ArbolB arbolDisponible;
ArbolBB arbolPilotos; 
AdjacencyList listaRutas;
DoubleCircularList<Avion> listaAvionesMantenimiento;




void menu(){
  cout << "" << endl;
  cout << "╔═════════════════════════════════════════╗" << endl;
  cout << "╠═════════════ PROYECTO EDD ══════════════╣" << endl;
  cout << "╠═════════════════════════════════════════╣" << endl;
  cout << "║             Elija una opcion            ║" << endl;
  cout << "╠═════════════════════════════════════════╣" << endl;
  cout << "║ 1. Carga de aviones                     ║" << endl;
  cout << "║ 2. Carga de pilotos                     ║" << endl;
  cout << "║ 3. Carga de rutas                       ║" << endl;
  cout << "║ 4. Carga de movimientos                 ║" << endl;
  cout << "║ 5. Consultar de horas de vuelo (pilotos)║" << endl;
  cout << "║ 6. Recomendar Ruta                      ║" << endl;
  cout << "║ 7. Visualizar reportes                  ║" << endl;
  cout << "║ 8. Salir                                ║" << endl;
  cout << "╚═════════════════════════════════════════╝" << endl;
  cout << "" << endl;
  cout << "Ingrese la opcion deseada:" << endl;
  
}

void cargar_aviones() {
  string filename = "ArchivosDeEntrada/aviones.json";
  ifstream ifs(filename, ifstream::binary);
  if (!ifs.is_open()) {
    cerr << "No se pudo abrir el archivo: " << filename << endl;
  }
  Json::Value root;
  Json::CharReaderBuilder builder;
  string errs;
  // Parsear JSON
  bool parsingSuccessful = Json::parseFromStream(builder, ifs, &root, &errs);
  if (!parsingSuccessful) {
    cerr << "Error al parsear el archivo JSON: " << errs << endl;
  }
  ifs.close();
  for (const auto& avion : root) {
    Avion avionNuevo(avion["vuelo"].asString(), avion["numero_de_registro"].asString(), avion["modelo"].asString(),
        avion["capacidad"].asInt(),avion["aerolinea"].asString(),avion["ciudad_destino"].asString(), avion["estado"].asString());
    // Agregar el avión por su estado
    if (avionNuevo.getEstado() == "Disponible") {
      Avion* avionNuevoDisponible = new Avion(avion["vuelo"].asString(), avion["numero_de_registro"].asString(), avion["modelo"].asString(), avion["capacidad"].asInt(), avion["aerolinea"].asString(), avion["ciudad_destino"].asString(), avion["estado"].asString());
      arbolDisponible.insert(avionNuevoDisponible);
      matrizVuelosCiudades.insertarAvion(avionNuevo);
      
    } else if (avionNuevo.getEstado() == "Mantenimiento") {
      Avion* avionNuevoMantenimiento = new Avion(avion["vuelo"].asString(), avion["numero_de_registro"].asString(), avion["modelo"].asString(), avion["capacidad"].asInt(), avion["aerolinea"].asString(), avion["ciudad_destino"].asString(), avion["estado"].asString());
      listaAvionesMantenimiento.insert(avionNuevo);
      matrizVuelosCiudades.insertarAvion(avionNuevo);
    }
  }
  cout << "║   Aviones cargados exitosamente.    ║" << endl;
  cout << "╚═════════════════════════════════════╝" << endl;
}


void cargarPilotos() {
    string filename = "ArchivosDeEntrada/pilotos.json";
    ifstream ifs(filename, ifstream::binary);
    if (!ifs.is_open()) {
        cerr << "No se pudo abrir el archivo: " << filename << endl;
    }
    Json::Value root;
    Json::CharReaderBuilder builder;
    string errs;
    // Parsear el archivo JSON
    bool parsingSuccessful = Json::parseFromStream(builder, ifs, &root, &errs);
    if (!parsingSuccessful) {
        cerr << "Error al parsear el archivo JSON: " << errs << endl;
    }
    ifs.close();
    for (const auto& piloto : root) {
        string nombre = piloto["nombre"].asString();
        string nacionalidad = piloto["nacionalidad"].asString();
        string numeroId = piloto["numero_de_id"].asString();
        string vuelo = piloto["vuelo"].asString();
        int horasVuelo = piloto["horas_de_vuelo"].asInt();
        string tipoLicencia = piloto["tipo_de_licencia"].asString();
        Piloto nuevoPiloto(nombre, nacionalidad, numeroId, vuelo, horasVuelo, tipoLicencia);
        Piloto* pilotoNuevoPtr = new Piloto(nombre, nacionalidad, numeroId, vuelo, horasVuelo, tipoLicencia);
        arbolPilotos.insertar(nuevoPiloto);
        tablaPilotos.insert(pilotoNuevoPtr);
        matrizVuelosCiudades.insertarPiloto(nuevoPiloto);
    }
  cout << "║   Pilotos cargados exitosamente.    ║" << endl;
  cout << "╚═════════════════════════════════════╝" << endl;
}


void cargarRutas() {
    ifstream file("ArchivosDeEntrada/rutas.txt");
    string linea;
    if (file.is_open())
    {
        while (getline(file, linea))
        {
            string origen = linea.substr(0, linea.find("/"));
            string destino = linea.substr(linea.find("/") + 1, linea.find_last_of("/") - linea.find("/") - 1);
            int distancia = stoi(linea.substr(linea.find_last_of("/") + 1));
            cout << "Origen: " << origen << ", Destino: " << destino << ", Distancia: " << distancia << endl;
            // Agregar la arista a AdjacencyList
            listaRutas.addEdge(origen, destino, distancia);
        }
    }
    file.close();
    cout << "\nRutas cargadas exitosamente.\n"
        << endl;
}

void cargarMovimientos() {
    ifstream file("ArchivosDeEntrada/movimientos.txt");
    string linea;
    if (file.is_open()) {
        cout << "\nMovimientos realizados:" << endl;
        while (getline(file, linea)) {
            //"MantenimientoAviones,Ingreso,"
            if (linea.find("MantenimientoAviones,Ingreso,") != string::npos) {
                string ingreso = linea.substr(linea.find(",") + 8 + 1, linea.find(";") - linea.find(",") - 9);
                Avion* avion = arbolDisponible.buscarAvion(ingreso);
                if (avion != nullptr) {
                    Avion* avionNuevo = new Avion(avion->getVuelo(), avion->getNumeroRegistro(), avion->getModelo(), avion->getCapacidad(), avion->getAerolinea(), avion->getCiudadDestino(), "Mantenimiento");
                    listaAvionesMantenimiento.insert(*avionNuevo);
                    arbolDisponible.Deletear(ingreso);
                }
                cout << "El avion con registro " << ingreso << " ingreso a mantenimiento" << endl;
            } else if (linea.find("MantenimientoAviones,Salida,") != string::npos) {
                // número de registro del avión
                string salida = linea.substr(linea.find(",") + 8, linea.find(";") - linea.find(",") - 9 + 1);
                Avion* avion = listaAvionesMantenimiento.buscarPorNumeroDeRegistro(salida);
                if (avion != nullptr) {
                    Avion* avionSale = new Avion(avion->getVuelo(), avion->getNumeroRegistro(), avion->getModelo(), avion->getCapacidad(), avion->getAerolinea(), avion->getCiudadDestino(), "Disponible");
                    arbolDisponible.insert(avionSale);
                    listaAvionesMantenimiento.eliminarPorNumeroDeRegistro(salida);
                }
                cout << "El avion con registro  " << salida << " salio de mantenimiento" << endl;
            } // DarDeBaja();
            else if (linea.find("DarDeBaja(") != string::npos) {
                // No. de id del piloto
                string numeroDeId = linea.substr(linea.find("(") + 1, linea.find(")") - linea.find("(") - 1);
                // eliminar en la tabla hash, arbol bb y matriz
                tablaPilotos.deletear(numeroDeId);
                arbolPilotos.eliminarPiloto(numeroDeId);
                matrizVuelosCiudades.eliminarPorNumeroDeID(numeroDeId);
                cout << "El pasajero con numero de id " << numeroDeId << " se dio de baja" << endl;
            }
        }
    }
    cout << "\nMovimientos cargados exitosamente.\n" << endl;
}

void reporte_aviones_mantenimiento() {
  std::ofstream archivo("Reportes/reporte_aviones_mantenimiento.dot");
  if (!archivo) {
    std::cerr << "Error al abrir el archivo" << std::endl;
    return;
  }

  archivo << "digraph Aviones {\n";
  archivo << "layout=circo;\n";
  archivo << "rankdir=LR;\n"; // Para que las flechas vayan de izquierda a derecha
  archivo << "node [fontname=\"Courier New\"];\n";
  archivo << "node [shape=component, style=filled, color=blue, fillcolor=\"#65babf\"];\n";
  archivo << "Titulo [fontname=\"Courier New\", color=red shape=box3d label=\"Lista circular doble de aviones en mantenimiento\"];\n";

  // Recorre la lista de aviones en mantenimiento y los imprime en el archivo .dot
  if (listaAvionesMantenimiento.getSize() > 0) {
    for (int i = 0; i < listaAvionesMantenimiento.getSize(); i++) {
      try {
        Avion avion = listaAvionesMantenimiento.getElement(i);
        archivo << "  \"M" << avion.getNumeroRegistro() << "\" [label=\"" 
            << avion.getNumeroRegistro() << "\", shape=box, color=red, fillcolor=\"#ffcccb\"];\n";

        int siguienteIndex = (i + 1) % listaAvionesMantenimiento.getSize();
        int anteriorIndex = (i - 1 + listaAvionesMantenimiento.getSize()) % listaAvionesMantenimiento.getSize();

        Avion avionSiguiente = listaAvionesMantenimiento.getElement(siguienteIndex);
        Avion avionAnterior = listaAvionesMantenimiento.getElement(anteriorIndex);

        archivo << "  \"M" << avion.getNumeroRegistro() << "\" -> \"M" << avionSiguiente.getNumeroRegistro() << "\";\n";
        archivo << "  \"M" << avion.getNumeroRegistro() << "\" -> \"M" << avionAnterior.getNumeroRegistro() << "\";\n";
      } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
      }
    }
  }

  archivo << "}\n";
  archivo.close();

  // Genera la imagen utilizando Graphviz
  int result = system("dot -Tpng Reportes/reporte_aviones_mantenimiento.dot -o Reportes/reporte_aviones_mantenimiento.png");
  if (result == -1) {
    std::cerr << "Error ejecutando el comando dot" << std::endl;
    std::exit(EXIT_FAILURE);
    return;
  }
  std::cout << "Grafica de aviones generada exitosamente." << std::endl;
  system("start Reportes/reporte_aviones_mantenimiento.png");
}

void recomendacionRuta() {
    string origen, destino;
    cout << "Ingrese la ciudad de origen: ";
    cin >> origen;
    cout << "Ingrese la ciudad de destino: ";
    cin >> destino;

    auto resultado = listaRutas.shortestPath(origen, destino);
    if (resultado.second == -1) {
        cout << "No se encontró una ruta entre " << origen << " y " << destino << endl;
    } else {
        cout << "La ruta más corta entre " << origen << " y " << destino << " es: ";
        for (const auto& ciudad : resultado.first) {
            cout << ciudad << " , ";
        }
        cout << "Distancia total de la ruta: " << resultado.second  << endl;
    }
}

void reportes() {
  int opcion_reporte;

  cout << endl;
  cout << "╔═════════════════════════════════════════╗" << endl;
  cout << "╠═══════════════ REPORTES ════════════════╣" << endl;
  cout << "╠═════════════════════════════════════════╣" << endl;
  cout << "║ 1. Arbol B con aviones disponibles      ║" << endl;
  cout << "║ 2. Lista de aviones en mantenimiento    ║" << endl;
  cout << "║ 3. Arbol BB horas de cada piloto        ║" << endl;
  cout << "║ 4. Tabla hash de pilotos                ║" << endl;
  cout << "║ 5. Grafo dirigido con las rutas         ║" << endl;
  cout << "║ 6. Matriz dispersa de vuelos y ciudades ║" << endl;
  cout << "║ 7. Regresar                             ║" << endl;
  cout << "╚═════════════════════════════════════════╝" << endl;
  cout << endl;


  do {
    cout << "Selecciona una opción: " << endl;
    cin >> opcion_reporte;

    switch (opcion_reporte) {
      case 1:
        //reporte Arbol B con aviones disponibles
        arbolDisponible.generarDot();
        break;
      case 2:
        //reporte Lista de aviones en mantenimiento
        reporte_aviones_mantenimiento();
        break;
      case 3:
        //reporte Arbol BB horas de cada piloto
        arbolPilotos.generarReporte();
        break;
      case 4:
        //reporte Tabla hash de pilotos
        tablaPilotos.GenerarGraficoTablaHash();
        break;
      case 5:
        //reporte Grafo dirigido con las rutas
        listaRutas.generarGrafoGraphviz();
        //listaRutas.displayList();
        break;
      case 6:
        //reporte Matriz dispersa de vuelos y ciudades
        matrizVuelosCiudades.generarGraficoGraphviz();
        //matrizVuelosCiudades.imprimirMatriz();
        break;
      case 7:
        cout << "Regresando al menu principal..." << endl;
        return; // Regresar al menú principal
      default:
        cout << "Opción no válida." << endl;
        break;
    }
  } while (opcion_reporte != 7);
}
/*
void ConsultarHorasVuelo() {
  cout << "Ingrese el No. de Identificacion del piloto: ";
  string id;
  cin >> id;

  if (id.empty()) {
        cerr << "El número de pasaporte no puede estar vacío." << endl;
        return;
  }

  NodoBB* nodo = arbolPilotos.buscarPorId(id); // Buscar el piloto por su número de identificación

  // informacion del piloto
  if (nodo != nullptr) {
    Piloto piloto = nodo->getPiloto();
    cout << "Nombre: " << piloto.getNombre() << endl;
    cout << "Nacionalidad: " << piloto.getNacionalidad() << endl;
    cout << "Número de Identificación: " << piloto.getNumeroDeId() << endl;
    cout << "Vuelo: " << piloto.getVuelo() << endl;
    cout << "Horas de vuelo: " << piloto.getHorasDeVuelo() << endl;
    cout << "Tipo de licencia: " << piloto.getTipoDeLicencia() << endl;
  } else {
    cout << "No se encontró un piloto con el número de identificación proporcionado." << endl;
  }
}
*/
void Recorridos() {
    int opcion;
    do {
        cout << "╔═════════════════════════════════════╗" << endl;
        cout << "║             RECORRIDOS              ║" << endl;
        cout << "╠═════════════════════════════════════╣" << endl;
        cout << "║ 1. Pre-Orden                        ║" << endl;
        cout << "║ 2. In-Orden                         ║" << endl;
        cout << "║ 3. Post-Orden                       ║" << endl;
        cout << "║ 4. Regresar al Menu Principal       ║" << endl;
        cout << "╚═════════════════════════════════════╝" << endl;
        cout << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "Recorrido Pre-Orden:" << endl;
                cout << "" << endl;
                arbolPilotos.RecorridoPreOrden();
                cout << endl;
                break;
            case 2:
                cout << "Recorrido In-Orden:" << endl;
                cout << "" << endl;
                arbolPilotos.RecorridoInOrden();
                cout << endl;
                break;
            case 3:
                cout << "Recorrido Post-Orden:" << endl;
                cout << "" << endl;
                arbolPilotos.RecorridoPostOrden();
                cout << endl;
                break;
            case 4:
                break;
            default:
                cout << "Opcion no valida" << endl;
                cout << endl;
                break;
        }
    } while (opcion != 4);
}

int main(){
  int opcion;
  do{
    menu();
    cin >> opcion;
    switch(opcion){
      case 1:
        cout << "╔═════════════════════════════════════╗" << endl;
        cout << "║         Carga de aviones            ║" << endl;
        cout << "╠═════════════════════════════════════╣" << endl;
        cargar_aviones();
        break;
      case 2:
        cout << "╔═════════════════════════════════════╗" << endl;
        cout << "║         Carga de pilotos            ║" << endl;
        cout << "╠═════════════════════════════════════╣" << endl;
        cargarPilotos();
        break;
      case 3:
        cout << "╔═════════════════════════════════════╗" << endl;
        cout << "║           Carga de rutas            ║" << endl;
        cout << "╚═════════════════════════════════════╝" << endl;
        cargarRutas();
        break;
      case 4:
        cout << "╔═════════════════════════════════════╗" << endl;
        cout << "║        Carga de movimientos         ║" << endl;
        cout << "╚═════════════════════════════════════╝" << endl;
        cout << "" << endl;
        cargarMovimientos();
        break;
      case 5:
        cout << "╔═════════════════════════════════════╗" << endl;
        cout << "║     Consultar de horas de vuelo     ║" << endl;
        cout << "╚═════════════════════════════════════╝" << endl;
        cout << "" << endl;
        Recorridos();
        break;
      case 6:
        cout << "╔═════════════════════════════════════╗" << endl;
        cout << "║           Recomendar Ruta           ║" << endl;
        cout << "╚═════════════════════════════════════╝" << endl;
        cout << "" << endl;
        recomendacionRuta();
        break;
      case 7:
        cout << "╔═════════════════════════════════════╗" << endl;
        cout << "║         Visualizar reportes         ║" << endl;
        cout << "╚═════════════════════════════════════╝" << endl;
        cout << "" << endl;
        reportes();
        break;
      case 8:
        cout << "Saliendo..." << endl;
        break;
      default:
        cout << "Opcion no valida" << endl;
        break;
    }
  }while(opcion != 8);
  return 0;
}