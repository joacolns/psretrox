#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void agregar_bibliotecas(ofstream& salida) {
    salida << "#include <iostream>" << endl;
    salida << "#include <fstream>" << endl;
    salida << "#include <sstream>" << endl;
    salida << "#include <string>" << endl;
    salida << "#include <vector>" << endl;
    salida << "#include <stack>" << endl;
    salida << endl;
}

void procesar_instruccion(const string& instruccion, ofstream& salida) {
    stringstream ss(instruccion);
    string operacion;
    ss >> operacion;

    if (operacion == "add") {
        string dest, src;
        ss >> dest >> src;
        salida << dest << " += " << src << ";" << endl;
    }
    else if (operacion == "sub") {
        string dest, src;
        ss >> dest >> src;
        salida << dest << " -= " << src << ";" << endl;
    }
    else if (operacion == "and") {
        string dest, src;
        ss >> dest >> src;
        salida << dest << " &= " << src << ";" << endl;
    }
    else if (operacion == "or") {
        string dest, src;
        ss >> dest >> src;
        salida << dest << " |= " << src << ";" << endl;
    }
    else if (operacion == "xor") {
        string dest, src;
        ss >> dest >> src;
        salida << dest << " ^= " << src << ";" << endl;
    }
    else if (operacion == "adc") {
        string dest, src;
        ss >> dest >> src;
        salida << dest << " += " << src << " + CF; // Suma con Carry" << endl;
    }
    else if (operacion == "cmp") {
        string op1, op2;
        ss >> op1 >> op2;
        salida << "if (" << op1 << " != " << op2 << ") { /* Acción */ };" << endl;
    }
    else if (operacion == "test") {
        string op1, op2;
        ss >> op1 >> op2;
        salida << "if ((" << op1 << " & " << op2 << ") == 0) { /* Acción */ };" << endl;
    }
    else if (operacion == "jmp") {
        string dest;
        ss >> dest;
        salida << "goto " << dest << ";" << endl;
    }
    else if (operacion == "jo") {
        string dest;
        ss >> dest;
        salida << "if (OF) goto " << dest << "; // Overflow Flag" << endl;
    }
    else if (operacion == "jg") {
        string dest;
        ss >> dest;
        salida << "if (SF == OF && ZF == 0) goto " << dest << "; // Jump if Greater" << endl;
    }
    else if (operacion == "scasb") {
        string reg, ptr;
        ss >> reg >> ptr;
        salida << "if (" << reg << " == " << ptr << ") { /* Acción */ };" << endl;
    }
    else if (operacion == "arpl") {
        string dest, src;
        ss >> dest >> src;
        salida << "// Ajuste de privilegios de segmento no soportado en alto nivel" << endl;
    }
    else if (operacion == "cld") {
        salida << "DF = 0; // Clear Direction Flag" << endl;
    }
    else if (operacion == "sbb") {
        string dest, src;
        ss >> dest >> src;
        salida << dest << " = " << dest << " - (" << src << " + CF); // Subtract with Borrow" << endl;
    }
    else if (operacion == "push") {
        string src;
        ss >> src;
        salida << "stack.push(" << src << ");" << endl;
    }
    else if (operacion == "pop") {
        string dest;
        ss >> dest;
        salida << dest << " = stack.pop();" << endl;
    }
    else if (operacion == "in") {
        string dest, port;
        ss >> dest >> port;
        salida << dest << " = port_in(" << port << ");" << endl;
    }
    else if (operacion == "invd") {
        salida << "// Invalidate internal caches (no equivalente en alto nivel)" << endl;
    }
    else if (operacion == "lds") {
        string dest, src;
        ss >> dest >> src;
        salida << dest << " = " << src << "; // Load pointer and DS segment" << endl;
    }
    else if (operacion == "movsd") {
        string dest, src;
        ss >> dest >> src;
        salida << "*(" << dest << ") = *(" << src << "); // Move string data" << endl;
    }
    else if (operacion == "dec") {
        string dest;
        ss >> dest;
        salida << dest << " -= 1;" << endl;
    }
    else if (operacion == "mov") {
        string reg, value;
        ss >> reg >> value;
        if (reg == "ch" && value == "0xff") {
            salida << "ch = 0xff;" << endl;
        }
    }
    else if (operacion == "inc") {
        string reg;
        ss >> reg;
        salida << reg << " += 1;" << endl;
    }
    else if (operacion == "rol") {
        string reg, val;
        ss >> reg >> val;
        salida << "rotate_left(" << reg << ", " << val << ");" << endl;
    }
    else if (operacion == "xchg") {
        string reg1, reg2;
        ss >> reg1 >> reg2;
        salida << "swap(" << reg1 << ", " << reg2 << ");" << endl;
    }
    else {
        salida << "// Instrucción no mapeada: " << instruccion << endl;
    }
}



void convertir_a_C(const string& archivo_log, const string& archivo_salida) {
    std::ifstream log_file(archivo_log);
    if (!log_file.is_open()) {
        std::cerr << "Error al abrir el archivo de log: " << archivo_log << std::endl;
        return;
    }

    std::ofstream salida(archivo_salida);
    if (!salida.is_open()) {
        std::cerr << "Error al crear el archivo de salida: " << archivo_salida << std::endl;
        perror("Detalles del error"); // Muestra más detalles sobre el error
        return;
    }

    // Agregar bibliotecas al principio del archivo
    agregar_bibliotecas(salida);

    std::string linea;
    while (getline(log_file, linea)) {
        procesar_instruccion(linea, salida);
    }

    log_file.close();
    salida.close();
    std::cout << "Archivo de salida generado correctamente: " << archivo_salida << std::endl;
}