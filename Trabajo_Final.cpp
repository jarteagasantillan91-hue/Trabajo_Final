#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
using namespace std;

class Producto {
private:
    int codigo;
    string nombre;
    float precio;
    int stock;
public:
    Producto(int c=0, string n="", float p=0.0, int s=0) {
        codigo = c; nombre = n; precio = p; stock = s;
    }
    int getCodigo() const { return codigo; }
    string getNombre() const { return nombre; }
    float getPrecio() const { return precio; }
    int getStock() const { return stock; }
    void setStock(int s) { stock = s; }

    void mostrarInfo() const {
        cout << "Codigo: " << codigo 
             << " | Nombre: " << nombre 
             << " | Precio: " << precio 
             << " | Stock: " << stock << endl;
    }

    string formatoArchivo() const {
        return to_string(codigo) + "," + nombre + "," + 
               to_string(precio) + "," + to_string(stock);
    }
};

class Bodega {
private:
    vector<Producto> lista;
public:
    void agregar(const Producto &p) {
        
        for (auto &prod : lista) {
            if (prod.getCodigo() == p.getCodigo()) {
                cout << "Error: El codigo ya existe.\n";
                return;
            }
        }
        lista.push_back(p);
        cout << "Producto agregado correctamente.\n";
    }

    void mostrarTodos() const {
        if(lista.empty()) {
            cout << "No hay productos registrados.\n";
            return;
        }
        for(auto &p : lista) p.mostrarInfo();
    }

    Producto* buscar(int codigo) {
        for(auto &p : lista) {
            if(p.getCodigo() == codigo) return &p;
        }
        return nullptr;
    }

    void actualizarStock(int codigo, int nuevoStock) {
        Producto* p = buscar(codigo);
        if(p) {
            if(nuevoStock >= 0) {
                p->setStock(nuevoStock);
                cout << "Stock actualizado.\n";
            } else {
                cout << "Error: El stock no puede ser negativo.\n";
            }
        } else cout << "Producto no encontrado.\n";
    }

    void eliminarProducto(int codigo) {
        for(auto it = lista.begin(); it != lista.end(); ++it) {
            if(it->getCodigo() == codigo) {
                lista.erase(it);
                cout << "Producto eliminado.\n";
                return;
            }
        }
        cout << "Producto no encontrado.\n";
    }

    void guardarArchivo(string nombreArchivo) const {
        ofstream archivo(nombreArchivo);
        if(!archivo) {
            cout << "Error al abrir archivo para guardar.\n";
            return;
        }
        for(auto &p : lista) archivo << p.formatoArchivo() << endl;
        archivo.close();
        cout << "Datos guardados en " << nombreArchivo << endl;
    }

    void leerArchivo(string nombreArchivo) {
        ifstream archivo(nombreArchivo);
        if(!archivo) {
            cout << "Archivo no encontrado, se creara al guardar.\n";
            return;
        }
        lista.clear();
        int c, s; string n; float p;
        while(archivo >> c) {
            archivo.ignore(); 
            getline(archivo, n, ',');
            archivo >> p; archivo.ignore();
            archivo >> s;
            lista.push_back(Producto(c,n,p,s));
        }
        archivo.close();
        cout << "Datos cargados desde " << nombreArchivo << endl;
    }
};


void limpiarEntrada() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    Bodega bodega;
    int opcion;
    do {
        cout << "\n--- MENU BODEGA ---\n";
        cout << "1. Agregar producto\n";
        cout << "2. Mostrar productos\n";
        cout << "3. Buscar producto\n";
        cout << "4. Actualizar stock\n";
        cout << "5. Eliminar producto\n";
        cout << "6. Guardar en archivo\n";
        cout << "7. Leer desde archivo\n";
        cout << "0. Salir\n";
        cout << "Opcion: "; 
        cin >> opcion;

        if(cin.fail()) {
            limpiarEntrada();
            cout << "Entrada invalida. Intente nuevamente.\n";
            continue;
        }

        if(opcion == 1) {
            int c,s; string n; float p;
            cout << "Codigo: "; cin >> c;
            cout << "Nombre: "; cin >> n;
            cout << "Precio: "; cin >> p;
            cout << "Stock: "; cin >> s;
            if(cin.fail() || c <= 0 || p < 0 || s < 0) {
                limpiarEntrada();
                cout << "Error: Datos invalidos.\n";
            } else {
                bodega.agregar(Producto(c,n,p,s));
            }
        }
        else if(opcion == 2) bodega.mostrarTodos();
        else if(opcion == 3) {
            int c; cout << "Codigo: "; cin >> c;
            Producto* p = bodega.buscar(c);
            if(p) p->mostrarInfo();
            else cout << "No encontrado.\n";
        }
        else if(opcion == 4) {
            int c,s; cout << "Codigo: "; cin >> c;
            cout << "Nuevo stock: "; cin >> s;
            bodega.actualizarStock(c,s);
        }
        else if(opcion == 5) {
            int c; cout << "Codigo: "; cin >> c;
            bodega.eliminarProducto(c);
        }
        else if(opcion == 6) bodega.guardarArchivo("bodega.txt");
        else if(opcion == 7) bodega.leerArchivo("bodega.txt");
        else if(opcion == 0) cout << "Saliendo del sistema...\n";
        else cout << "Opcion invalida.\n";
    } while(opcion != 0);

    return 0;
}
