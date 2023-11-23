#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <queue>
#include <algorithm>


// estrcutura contacto
struct Contacto {
    std::string nombre;
    std::string telefono;
    std::string redesSociales;
    std::string icono;
    std::string direccion;

    int frecuencia;

    Contacto(){
        frecuencia = 0;
    }
};

// Nodo para manejar los contactos que tienen el mismo nombre
struct NodoLista {
    Contacto contacto;
    NodoLista* siguiente;

    NodoLista(const Contacto& c){
      contacto = c;
      siguiente = nullptr;
    }

};

struct NodoBST {
    Contacto contacto;
    NodoLista* listaEnlazada; // Para manejar contactos con el mismo nombre
    NodoBST* izquierda;
    NodoBST* derecha;

    NodoBST(const Contacto& c){
      contacto = c;
      listaEnlazada = nullptr;
      izquierda = nullptr;
      derecha = nullptr;
    }

};

class ArbolBTS {
private:
    NodoBST* raiz;

    //Insertar un contacto en el árbol de manera recursiva
    NodoBST* insertar(NodoBST* nodo, const Contacto& nuevoContacto) {
        if (nodo == nullptr) {
            // Crear un nuevo nodo si el nodo actual es nulo
            return new NodoBST(nuevoContacto);
        }

        //Clasificará los contactos alfabéticamente en función de sus nombres.
        //relación lexicográfica entre las dos cadenas. Si el valor devuelto es 0, 
        //las cadenas son iguales. Si es negativo, la primera cadena es menor que la segunda, y 
        //si es positivo, la primera cadena es mayor que la segunda.
        int comparacion = nuevoContacto.nombre.compare(nodo->contacto.nombre);

        // Insertar en el subárbol izquierdo o derecho según la comparación
        if (comparacion < 0) {
            nodo->izquierda = insertar(nodo->izquierda, nuevoContacto);
        } else if (comparacion > 0) {
            nodo->derecha = insertar(nodo->derecha, nuevoContacto);
        } else {
            // En caso de que los nombres sean iguales, añadir a la lista enlazada
            NodoLista* nuevoNodoLista = new NodoLista(nuevoContacto);
            nuevoNodoLista->siguiente = nodo->listaEnlazada;
            nodo->listaEnlazada = nuevoNodoLista;
        }

        return nodo;
    }

    //Eliminar un Node del arbol de manera recursiva
    NodoBST* eliminarPorNombre(NodoBST* nodo, const std::string& nombreContacto) {
        if (nodo == nullptr) {
            // El contacto no se encontró, no se realiza ninguna acción
            return nullptr;
        }

        //Clasificará los contactos alfabéticamente en función de sus nombres.
        //relación lexicográfica entre las dos cadenas. Si el valor devuelto es 0, 
        //las cadenas son iguales. Si es negativo, la primera cadena es menor que la segunda, y 
        //si es positivo, la primera cadena es mayor que la segunda.
        int comparacion = nombreContacto.compare(nodo->contacto.nombre);

        // Buscar en el subárbol izquierdo o derecho según la comparación
        if (comparacion < 0) {
            nodo->izquierda = eliminarPorNombre(nodo->izquierda, nombreContacto);
        } else if (comparacion > 0) {
            nodo->derecha = eliminarPorNombre(nodo->derecha, nombreContacto);
        } else {
            // Se encontró el contacto, realizar la eliminación
            if (nodo->listaEnlazada != nullptr) {
                // Eliminar la lista enlazada si existe
                borrarListaEnlazada(nodo->listaEnlazada);
            }

            // Caso 1: Nodo sin hijos o con un solo hijo
            if (nodo->izquierda == nullptr) {
                NodoBST* temp = nodo->derecha;
                delete nodo;
                return temp;
            } else if (nodo->derecha == nullptr) {
                NodoBST* temp = nodo->izquierda;
                delete nodo;
                return temp;
            }

            // Caso 2: Nodo con dos hijos, encontrar el sucesor inmediato (mínimo en el subárbol derecho)
            NodoBST* sucesor = encontrarMinimo(nodo->derecha);

            // Copiar el contenido del sucesor al nodo actual
            nodo->contacto = sucesor->contacto;

            // Eliminar el sucesor
            nodo->derecha = eliminarPorNombre(nodo->derecha, sucesor->contacto.nombre);
        }

        return nodo;
    }


    NodoBST* encontrarMinimo(NodoBST* nodo) {
        while (nodo->izquierda != nullptr) {
            nodo = nodo->izquierda;
        }
        return nodo;
    }


    void borrarListaEnlazada(NodoLista* lista) {
        while (lista != nullptr) {
            NodoLista* temporal = lista;
            lista = lista->siguiente;
            delete temporal;
        }
    }

    NodoBST* buscarPorNombre(NodoBST* nodo, const std::string& nombreContacto) {
        if (nodo == nullptr) {
            // El contacto no se encontró
            return nullptr;
        }

        //Clasificará los contactos alfabéticamente en función de sus nombres.
        //relación lexicográfica entre las dos cadenas. Si el valor devuelto es 0, 
        //las cadenas son iguales. Si es negativo, la primera cadena es menor que la segunda, y 
        //si es positivo, la primera cadena es mayor que la segunda.
        int comparacion = nombreContacto.compare(nodo->contacto.nombre);

        // Buscar en el subárbol izquierdo o derecho según la comparación
        if (comparacion < 0) {
            return buscarPorNombre(nodo->izquierda, nombreContacto);
        } else if (comparacion > 0) {
            return buscarPorNombre(nodo->derecha, nombreContacto);
        } else {
            // Se encontró el contacto
            return nodo;
        }
    }

    void mostrarListaOrdenada(NodoBST* nodo) {
        if (nodo != nullptr) {
            mostrarListaOrdenada(nodo->izquierda);
            mostrarInformacionContacto(nodo->contacto);
            if (nodo->listaEnlazada != nullptr) {
                mostrarListaEnlazada(nodo->listaEnlazada);
            }
            mostrarListaOrdenada(nodo->derecha);
        }
    }

    void mostrarListaEnlazada(NodoLista* lista) {
        NodoLista* actual = lista;
        while (actual != nullptr) {
            mostrarInformacionContacto(actual->contacto);
            actual = actual->siguiente;
        }
    }

  int contarNodos(NodoBST* nodo) {
        if (nodo == nullptr) {
            return 0;
        }

        // Contar el nodo actual y los nodos en los subárboles izquierdo y derecho
        int contIzquierdo = contarNodos(nodo->izquierda);
        int contDerecho = contarNodos(nodo->derecha);

        // Sumar los resultados y agregar 1 para contar el nodo actual
        return contIzquierdo + contDerecho + 1;
    }

    void mostrarContactosPorLetraLista(NodoLista* lista, char letra) {
        NodoLista* actual = lista;
        while (actual != nullptr) {
            // Verificar si el nombre comienza con la letra específica
            if (actual->contacto.nombre[0] == letra) {
                mostrarInformacionContacto(actual->contacto);
            }
            actual = actual->siguiente;
        }
    }

    void mostrarContactosPorLetra(NodoBST* nodo, char letra) {
        if (nodo != nullptr) {
            mostrarContactosPorLetra(nodo->izquierda, letra);

            // Verificar si el nombre comienza con la letra específica
            if (nodo->contacto.nombre[0] == letra) {
                mostrarInformacionContacto(nodo->contacto);
            }

            if (nodo->listaEnlazada != nullptr) {
                // Mostrar contactos de la lista enlazada con la misma letra
                mostrarContactosPorLetraLista(nodo->listaEnlazada, letra);
            }

            mostrarContactosPorLetra(nodo->derecha, letra);
        }
    }

    void obtenerContactosOrdenados(std::map<int, Contacto>& mapa, NodoBST* nodo, int& indice) {
        if (nodo != nullptr) {
            obtenerContactosOrdenados(mapa, nodo->izquierda, indice);

            // Agregar el contacto al mapa con el índice actual
            mapa[indice++] = nodo->contacto;

            // Agregar los contactos de la lista enlazada al mapa
            NodoLista* actual = nodo->listaEnlazada;
            while (actual != nullptr) {
                mapa[indice++] = actual->contacto;
                actual = actual->siguiente;
            }

            obtenerContactosOrdenados(mapa, nodo->derecha, indice);
        }
    }

    void llenarVectorIndiceRapido(const std::map<int, Contacto>& mapa, std::vector<Contacto>& vectorIndiceRapido) {
        for (const auto& par : mapa) {
            vectorIndiceRapido.push_back(par.second);
        }
    }

public:

    // Constructor
    ArbolBTS() : raiz(nullptr) {}

    void agregarContacto(const Contacto& nuevoContacto) {
        raiz = insertar(raiz, nuevoContacto);
    }

    void eliminarContacto(const std::string& nombreContacto) {
        raiz = eliminarPorNombre(raiz, nombreContacto);
    }

   void buscarYmostrar(const std::string& nombreContacto) {
        NodoBST* resultado = buscarPorNombre(raiz, nombreContacto);

        if (resultado != nullptr) {
            mostrarInformacionContacto(resultado->contacto);
            resultado->contacto.frecuencia += 1;
        } else {
            std::cout << "Contacto no encontrado. "<<std::endl;
        }

    }

    void mostrarInformacionContacto(const Contacto& contacto) {
        std::cout << "Nombre: " << contacto.nombre << std::endl;
        std::cout << "Teléfono: " << contacto.telefono << std::endl;
        std::cout << "Dirección: " << contacto.direccion << std::endl;
        std::cout << "Icono: " << contacto.icono << std::endl;
    }

   void mostrarListaOrdenada() {
        if (raiz != nullptr) {
            mostrarListaOrdenada(raiz);
        } else {
            std::cout << "La lista de contactos está vacía. "<<std::endl;
        }
    }

  int obtenerCantidadTotalContactos() {
        return contarNodos(raiz);
    }

  void mostrarContactosPorLetra(char letra) {
        mostrarContactosPorLetra(raiz, letra);
    }

    std::map<int, Contacto> obtenerMapaContactosOrdenados() {
        std::map<int, Contacto> mapa;
        int indice = 1; // Inicializar el índice en 1
        obtenerContactosOrdenados(mapa, raiz, indice);
        return mapa;
    }

    std::vector<Contacto> obtenerVectorIndiceRapido() {
        std::map<int, Contacto> mapa = obtenerMapaContactosOrdenados();
        std::vector<Contacto> vectorIndiceRapido;
        llenarVectorIndiceRapido(mapa, vectorIndiceRapido);
        return vectorIndiceRapido;
    }

    void guardarEnArchivo(const std::string& nombreArchivo) const {
        std::ofstream archivo(nombreArchivo);

        if (!archivo.is_open()) {
            std::cerr << "No se pudo abrir el archivo para escribir." << std::endl;
            std::exit(EXIT_FAILURE);
        }

        guardarEnArchivoAux(raiz, archivo);
        archivo.close();
        std::cout << "Copia de seguridad realizada con éxito en '" << nombreArchivo << "'." << std::endl;
    }

    void guardarEnArchivoAux(NodoBST* nodo, std::ofstream& archivo) const {
        if (nodo != nullptr) {
            guardarEnArchivoAux(nodo->izquierda, archivo);
            archivo << nodo->contacto.nombre << "," << nodo->contacto.telefono << ","
                    << nodo->contacto.redesSociales << "," << nodo->contacto.icono << ","
                    << nodo->contacto.direccion << "\n";

            // Guardar contactos de la lista enlazada
            NodoLista* actual = nodo->listaEnlazada;
            while (actual != nullptr) {
                archivo << actual->contacto.nombre << "," << actual->contacto.telefono << ","
                        << actual->contacto.redesSociales << "," << actual->contacto.icono << ","
                        << actual->contacto.direccion << "\n";
                actual = actual->siguiente;
            }

            guardarEnArchivoAux(nodo->derecha, archivo);
        }
    }

    std::queue<std::string> colaCopiasSeguridad;

    void realizarCopiaSeguridad() {

        auto now = std::chrono::system_clock::now();
        std::time_t timestamp = std::chrono::system_clock::to_time_t(now);
        std::string nombreArchivo = "libreta_copia_seguridad_" + std::to_string(timestamp) + ".txt";

        
        guardarEnArchivo(nombreArchivo);
        colaCopiasSeguridad.push(nombreArchivo);

        std::cout << "Copia de seguridad realizada con éxito en '" << nombreArchivo << "'." << std::endl;
    }

    std::string obtenerUltimaCopiaSeguridad() const {
        if (!colaCopiasSeguridad.empty()) {
            return colaCopiasSeguridad.back();
        } else {
            return "No hay copias de seguridad disponibles.";
        }
    }

    static bool compararPorFrecuencia(const Contacto& a, const Contacto& b) {
        return a.frecuencia > b.frecuencia;
    }

    void mostrarContactosFrecuentes(int n) {
        std::map<int, Contacto> mapaContactos = obtenerMapaContactosOrdenados();
        std::vector<Contacto> contactosOrdenados;
        llenarVectorIndiceRapido(mapaContactos, contactosOrdenados);

        // Ordenar el vector por frecuencia en orden descendente
        std::sort(contactosOrdenados.begin(), contactosOrdenados.end(), compararPorFrecuencia);

        // Mostrar los primeros N contactos
        std::cout << "Contactos más frecuentes: " << std::endl;
        for (int i = 0; i < n && i < contactosOrdenados.size(); ++i) {
            mostrarInformacionContacto(contactosOrdenados[i]);
            std::cout << "Frecuencia: " << contactosOrdenados[i].frecuencia << std::endl;
            std::cout << std::endl;
        }
    }


};

int main() {

    ArbolBTS arbol;

    while (true) {
        std::cout << "Menú de opciones:" << std::endl;
        std::cout << "1. Agregar Contacto" << std::endl;
        std::cout << "2. Eliminar Contacto" << std::endl;
        std::cout << "3. Buscar y Mostrar Contacto" << std::endl;
        std::cout << "4. Mostrar Todos los Contactos" << std::endl;
        std::cout << "5. Mostrar Contactos Frecuentes" << std::endl;
        std::cout << "6. Realizar Copia de Seguridad" << std::endl;
        std::cout << "7. Mostrar Última Copia de Seguridad" << std::endl;
        std::cout << "8. Salir" << std::endl;
        std::cout << "Ingrese el número de la opción: ";

        int opcion;
        std::cin >> opcion;

        switch (opcion) {
            case 1: {
                // Lógica para agregar un contacto
                Contacto nuevoContacto;
                std::string nombre;
                std::cout << "Ingrese el nombre: ";
                std::cin >> nombre;
                nuevoContacto.nombre = nombre;
                std::string telefono;
                std::cout << "Ingrese el telefono: ";
                std::cin >> telefono;
                nuevoContacto.telefono = telefono;
                std::string red;
                std::cout << "Ingrese una nombre de usuario de red_social: ";
                std::cin >> red;
                nuevoContacto.redesSociales = red;
                std::string icono;
                std::cout << "Ingrese el icono: ";
                std::cin >> icono;
                nuevoContacto.icono = icono;
                std::string direccion;
                std::cout << "Ingrese la direccion: ";
                std::cin >> direccion;
                nuevoContacto.direccion = direccion;
                arbol.agregarContacto(nuevoContacto);
                std::cout<< "El contacto ha sido agregado" << std::endl;
                break;
            }
            case 2: {
                std::string nombreEliminar;
                std::cout << "Ingrese el nombre del contacto a eliminar: ";
                std::cin >> nombreEliminar;
                arbol.eliminarContacto(nombreEliminar);
                break;
            }
            case 3: {
                std::string nombreBuscar;
                std::cout << "Ingrese el nombre del contacto a buscar: ";
                std::cin >> nombreBuscar;
                arbol.buscarYmostrar(nombreBuscar);
                break;
            }
            case 4: {
                arbol.mostrarListaOrdenada();
                break;
            }
            case 5: {
                int n;
                std::cout << "Ingrese la cantidad de contactos frecuentes a mostrar: ";
                std::cin >> n;
                arbol.mostrarContactosFrecuentes(n);
                break;
            }
            case 6: {
                arbol.realizarCopiaSeguridad();
                break;
            }
            case 7: {
                std::cout << "Última copia de seguridad: " << arbol.obtenerUltimaCopiaSeguridad() << std::endl;
                break;
            }
            case 8: {
                std::cout << "Cerrando Libreta" << std::endl;
                return 0;
            }
            default: {
                std::cout << "Opción no válida. Por favor, ingrese un número válido." << std::endl;
                break;
            }
        }
    }

    return 0;
}
