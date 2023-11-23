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

    Contacto() : frecuencia(0) {} 
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


#include <iostream>

int main() {

    // Crear un objeto de la clase ArbolBTS
    ArbolBTS arbol;

    Contacto nuevoContacto;
    nuevoContacto.nombre = "Juan";
    nuevoContacto.telefono = "123456789";
    nuevoContacto.redesSociales = "@juan";
    nuevoContacto.icono = "😊";
    nuevoContacto.direccion = "Calle 123";

    Contacto nuevoContacto2;
    nuevoContacto2.nombre = "Maria";
    nuevoContacto2.telefono = "123456789";
    nuevoContacto2.redesSociales = "@maria";
    nuevoContacto2.icono = "👾";
    nuevoContacto2.direccion = "Calle 123";

    Contacto nuevoContacto3;
    nuevoContacto3.nombre = "Carlos";
    nuevoContacto3.telefono = "123456789";
    nuevoContacto3.redesSociales = "@carlos";
    nuevoContacto3.icono = "🎃";
    nuevoContacto3.direccion = "Calle 123";

    Contacto nuevoContacto4;
    nuevoContacto4.nombre = "Javier";
    nuevoContacto4.telefono = "123456789";
    nuevoContacto4.redesSociales = "@javier";
    nuevoContacto4.icono = "🚀";
    nuevoContacto4.direccion = "Calle 123";

    arbol.agregarContacto(nuevoContacto);
    arbol.agregarContacto(nuevoContacto2);
    arbol.agregarContacto(nuevoContacto3);
    arbol.agregarContacto(nuevoContacto4);


    std::cout << "Lista de contactos ordenada: " << std::endl;
    arbol.mostrarListaOrdenada();

    std::cout<<std::endl;

    arbol.eliminarContacto("Maria");

    std::cout<<std::endl;

    std::cout << "Lista de contactos después de eliminar 'Maria': "<<std::endl;
    arbol.mostrarListaOrdenada();

    std::cout<<std::endl;

    std::cout << "Buscar y mostrar información de 'Carlos': "<< std::endl;
    arbol.buscarYmostrar("Carlos");
    arbol.buscarYmostrar("Carlos");
    arbol.buscarYmostrar("Carlos");
    arbol.buscarYmostrar("Juan");
    arbol.buscarYmostrar("Juan");
    arbol.buscarYmostrar("Juan");
    arbol.buscarYmostrar("Juan");
    arbol.buscarYmostrar("Javier");

    std::cout<<std::endl;

    std::cout << "Cantidad total de contactos: " << arbol.obtenerCantidadTotalContactos() << std::endl;

    std::cout<<std::endl;

    std::cout << "Contactos cuyos nombres comienzan con la letra 'J': " << std::endl;
    arbol.mostrarContactosPorLetra('J');

    std::map<int, Contacto> mapaContactos = arbol.obtenerMapaContactosOrdenados();

    std::cout << "Mapa de contactos ordenados:" << std::endl;
    for (const auto& par : mapaContactos) {
        std::cout << "Índice: " << par.first;
        std::cout << " Información del contacto:" << std::endl;
        arbol.mostrarInformacionContacto(par.second);
        std::cout << std::endl;
    }

    std::cout<<std::endl;

    std::vector<Contacto> vectorIndiceRapido = arbol.obtenerVectorIndiceRapido();

    
    std::cout << "Vector de índice rápido:" << std::endl;
    for (const auto& contacto : vectorIndiceRapido) {
        std::cout << "Nombre: " << contacto.nombre << std::endl;
        // Mostrar otros campos del contacto si es necesario
        std::cout << "Información del contacto:" << std::endl;
        arbol.mostrarInformacionContacto(contacto);
        std::cout << std::endl;
    }

    std::cout<<std::endl;

    arbol.guardarEnArchivo("libreta_backup.txt");

    std::cout<<std::endl;

    arbol.realizarCopiaSeguridad();

    std::cout << "Última copia de seguridad: " << arbol.obtenerUltimaCopiaSeguridad() << std::endl;

    std::cout<<std::endl;
    arbol.mostrarContactosFrecuentes(3);


    return 0;
}
