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

    else if(operacion == "jge") {
        string dest;
        ss >> dest;
        salida << "if (SF == OF && ZF == 0) goto " << dest << "; // Jump if Greater or Equal" << endl;
    }
    else if (operacion == "pavgb") {
        string reg1, reg2;
        ss >> reg1 >> reg2;
        salida << reg1 << " = ( " << reg1 << " + " << reg2 << " ) / 2; // Packed Average Bytes" << endl;
    }
    else if (operacion == "ret") {
        salida << "return;" << endl;
    }
    else if (operacion == "je") {
        string dest;
        ss >> dest;
        salida << "if (ZF == 1) goto " << dest << "; // Jump if Equal" << endl;
    }
    else if (operacion == "nop") {
        salida << "// No Operation" << endl;
    }
    else if (operacion == "pushal") {
        salida << "// Simular 'pushal', guardar todos los registros" << endl;
        salida << "stack.push(eax);" << endl;
        salida << "stack.push(ebx);" << endl;
        salida << "stack.push(ecx);" << endl;
        salida << "stack.push(edx);" << endl;
        salida << "stack.push(esi);" << endl;
        salida << "stack.push(edi);" << endl;
        salida << "stack.push(ebp);" << endl;
    }
    else if (operacion == "sldt") {
        string ptr;
        ss >> ptr;
        salida << "ldt = " << ptr << "; // Cargar Local Descriptor Table" << endl;
    }
    else if (operacion == "cwde") {
        salida << "eax = (int)eax; // Convertir de palabra a doble palabra (extend)" << endl;
    }
    else if (operacion == "jecxz") {
        string dest;
        ss >> dest;
        salida << "if (ecx == 0) goto " << dest << "; // Jump if ECX is zero" << endl;
    }
    else if (operacion == "insd") {
        string ptr;
        ss >> ptr;
        salida << "port_in(" << ptr << "); // Input String or Data to es:[edi]" << endl;
    }
    else if (operacion == "das") {
        salida << "// Decimal Adjust after Subtraction (no equivalente en C)" << endl;
    }
    else if (operacion == "js") {
        string dest;
        ss >> dest;
        salida << "if (SF == 1) goto " << dest << "; // Jump if Signed" << endl;
    }
    else if (operacion == "loopne") {
        string dest;
        ss >> dest;
        salida << "if (ecx != 0 && ZF == 0) goto " << dest << "; // Loop while not Zero and Not Equal" << endl;
    }
    else if (operacion == "rcl") {
        string reg;
        ss >> reg;
        salida << "rotate_carry_left(" << reg << "); // Rotate Carry Left" << endl;
    }
    else if (operacion == "imul") {
        string reg1, reg2;
        ss >> reg1 >> reg2;
        salida << reg1 << " *= " << reg2 << "; // Integer Multiply" << endl;
    }
    else if (operacion == "leave") {
        salida << "ebp = stack.pop(); // Leave function (restore stack)" << endl;
    }
    else if (operacion == "out") {
        string port, reg;
        ss >> port >> reg;
        salida << "port_out(" << port << ", " << reg << "); // Output to port" << endl;
    }
    else if (operacion == "psubd") {
        string reg1, reg2;
        ss >> reg1 >> reg2;
        salida << reg1 << " -= " << reg2 << "; // Subtract Packed Data" << endl;
    }
    else if (operacion == "cmc") {
        salida << "CF = !CF; // Complement Carry Flag" << endl;
    }
    else if (operacion == "jno") {
        string dest;
        ss >> dest;
        salida << "if (!OF) goto " << dest << "; // Jump if No Overflow" << endl;
    }
    else if (operacion == "outsd") {
        string port, src;
        ss >> port >> src;
        salida << "port_out(" << port << ", " << src << "); // Output String Data" << endl;
    }
    else if (operacion == "call") {
        string addr;
        ss >> addr;
        salida << "call_function(" << addr << "); // Llamar a dirección específica" << endl;
    }
    else if (operacion == "fistp") {
        string ptr;
        ss >> ptr;
        salida << "*(" << ptr << ") = (int)FPU_TOP; // Store Integer and Pop FPU Stack" << endl;
    }
    else if (operacion == "fsubr") {
        string src;
        ss >> src;
        salida << "FPU_TOP = " << src << " - FPU_TOP; // Reverse Subtraction on FPU" << endl;
    }
    else if (operacion == "lea") {
        string dest, addr;
        ss >> dest >> addr;
        salida << dest << " = &(" << addr << "); // Load Effective Address" << endl;
    }
    else if (operacion == "int3") {
        salida << "debug_breakpoint(); // Interrupt 3" << endl;
    }
    else if (operacion == "not") {
        string dest;
        ss >> dest;
        salida << dest << " = ~" << dest << "; // Logical NOT" << endl;
    }
    else if (operacion == "popal") {
        salida << "// Simular 'popal', restaurar todos los registros" << endl;
        salida << "edi = stack.pop();" << endl;
        salida << "esi = stack.pop();" << endl;
        salida << "ebp = stack.pop();" << endl;
        salida << "// Ignorar 'esp', mantenerlo intacto" << endl;
        salida << "ebx = stack.pop();" << endl;
        salida << "edx = stack.pop();" << endl;
        salida << "ecx = stack.pop();" << endl;
        salida << "eax = stack.pop();" << endl;
    }
    else if (operacion == "outsb") {
        string port, src;
        ss >> port >> src;
        salida << "port_out(" << port << ", " << src << "); // Output String Byte" << endl;
    }
    else if (operacion == "les") {
        string dest, src;
        ss >> dest >> src;
        salida << dest << " = load_pointer(" << src << "); // Load ES Segment" << endl;
    }
    else if (operacion == "lodsb") {
        string reg, src;
        ss >> reg >> src;
        salida << reg << " = *(" << src << "); // Load String Byte" << endl;
    }
    else if (operacion == "aad") {
        string imm;
        ss >> imm;
        salida << "AL = (AL + AH * " << imm << ") & 0xFF; // ASCII Adjust AX after Division" << endl;
    }
    else if (operacion == "pushfd") {
        salida << "stack.push(EFLAGS); // Push Flags" << endl;
    }
    else if (operacion == "loope") {
        string dest;
        ss >> dest;
        salida << "if (--ECX && ZF) goto " << dest << "; // Loop while Equal" << endl;
    }
    else if (operacion == "aaa") {
        salida << "if ((AL & 0xF) > 9 || AF) { AL += 6; AH += 1; AF = 1; } else { AF = 0; } AL &= 0xF; // ASCII Adjust AL after Addition" << endl;
    }
    else if (operacion == "sar") {
        string reg, imm;
        ss >> reg >> imm;
        salida << reg << " >>= " << imm << "; // Arithmetic Shift Right" << endl;
    }
    else if (operacion == "daa") {
        salida << "// Decimal Adjust AL (no equivalente en alto nivel)" << endl;
    }
    else if (operacion == "scasd") {
        string reg, ptr;
        ss >> reg >> ptr;
        salida << "if (" << reg << " != *(" << ptr << ")) ZF = 0; else ZF = 1; // Compare String Data" << endl;
    }
    else if (operacion == "idiv") {
        string src;
        ss >> src;
        salida << "EDX:EAX /= " << src << "; // Integer Division" << endl;
    }
    else if (operacion == "movsb") {
        string dest, src;
        ss >> dest >> src;
        salida << "*(" << dest << ") = *(" << src << "); // Move Byte String" << endl;
    }
    else if (operacion == "aam") {
        string imm;
        ss >> imm;
        salida << "AH = AL / " << imm << "; AL %= " << imm << "; // ASCII Adjust AX after Multiplication" << endl;
    }
    else if (operacion == "clc") {
        salida << "CF = 0; // Clear Carry Flag" << endl;
    }
    else if (operacion == "int1") {
        salida << "debug_trap(); // Interrupt 1 (Debug)" << endl;
    }
    else if (operacion == "shl") {
        string reg, imm;
        ss >> reg >> imm;
        salida << reg << " <<= " << imm << "; // Logical Shift Left" << endl;
    }
    else if (operacion == "cli") {
        salida << "IF = 0; // Clear Interrupt Flag" << endl;
    }
    else if (operacion == "jle") {
        string dest;
        ss >> dest;
        salida << "if (ZF || SF != OF) goto " << dest << "; // Jump if Less or Equal" << endl;
    }
    else if (operacion == "jae") {
    string dest;
    ss >> dest;
    salida << "if (CF == 0) goto " << dest << "; // Jump if Above or Equal" << endl;
}
else if (operacion == "jb") {
    string dest;
    ss >> dest;
    salida << "if (CF == 1) goto " << dest << "; // Jump if Below" << endl;
}
else if (operacion == "jbe") {
    string dest;
    ss >> dest;
    salida << "if (CF == 1 || ZF == 1) goto " << dest << "; // Jump if Below or Equal" << endl;
}
else if (operacion == "ja") {
    string dest;
    ss >> dest;
    salida << "if (CF == 0 && ZF == 0) goto " << dest << "; // Jump if Above" << endl;
}
else if (operacion == "fimul") {
    salida << "// Integer Multiply (sin equivalencia directa en C++)" << endl;
}
else if (operacion == "jp") {
    string dest;
    ss >> dest;
    salida << "if (PF == 1) goto " << dest << "; // Jump if Parity" << endl;
}
else if (operacion == "int") {
    int numero;
    ss >> numero;
    salida << "interrupt(" << numero << "); // Interrupción del sistema" << endl;
}
else if (operacion == "lodsd") {
    salida << "eax = *(uint32_t*)esi; esi += 4; // Load String DWORD" << endl;
}
else if (operacion == "loop") {
    string dest;
    ss >> dest;
    salida << "if (--ecx != 0) goto " << dest << "; // Loop" << endl;
}
else if (operacion == "insb") {
    string ptr, reg;
    ss >> ptr >> reg;
    salida << ptr << " = " << reg << "; // Input Byte to ES:[EDI]" << endl;
}
else if (operacion == "fld") {
    string mem;
    ss >> mem;
    salida << "st0 = *(float*)" << mem << "; // Load floating-point value" << endl;
}
else if (operacion == "rcr") {
    string ptr, count;
    ss >> ptr >> count;
    salida << "rotate_carry_right(" << ptr << ", " << count << "); // Rotate Carry Right" << endl;
}
else if (operacion == "fadd") {
    string mem;
    ss >> mem;
    salida << "st0 += *(float*)" << mem << "; // Floating-point Add" << endl;
}
else if (operacion == "lahf") {
    salida << "ah = (SF << 7) | (ZF << 6) | (0 << 5) | (AF << 4) | (0 << 3) | (PF << 2) | (1 << 1) | CF; // Load flags into AH" << endl;
}
else if (operacion == "jae") {
    string dest;
    ss >> dest;
    salida << "if (CF == 0) goto " << dest << "; // Jump if Above or Equal" << endl;
}
else if (operacion == "jb") {
    string dest;
    ss >> dest;
    salida << "if (CF == 1) goto " << dest << "; // Jump if Below" << endl;
}
else if (operacion == "jbe") {
    string dest;
    ss >> dest;
    salida << "if (CF == 1 || ZF == 1) goto " << dest << "; // Jump if Below or Equal" << endl;
}
else if (operacion == "ja") {
    string dest;
    ss >> dest;
    salida << "if (CF == 0 && ZF == 0) goto " << dest << "; // Jump if Above" << endl;
}
else if (operacion == "fimul") {
    salida << "// Integer Multiply (sin equivalencia directa en C++)" << endl;
}
else if (operacion == "jp") {
    string dest;
    ss >> dest;
    salida << "if (PF == 1) goto " << dest << "; // Jump if Parity" << endl;
}
else if (operacion == "int") {
    int numero;
    ss >> numero;
    salida << "interrupt(" << numero << "); // Interrupción del sistema" << endl;
}
else if (operacion == "lodsd") {
    salida << "eax = *(uint32_t*)esi; esi += 4; // Load String DWORD" << endl;
}
else if (operacion == "loop") {
    string dest;
    ss >> dest;
    salida << "if (--ecx != 0) goto " << dest << "; // Loop" << endl;
}
else if (operacion == "insb") {
    string ptr, reg;
    ss >> ptr >> reg;
    salida << ptr << " = " << reg << "; // Input Byte to ES:[EDI]" << endl;
}
else if (operacion == "fld") {
    string mem;
    ss >> mem;
    salida << "st0 = *(float*)" << mem << "; // Load floating-point value" << endl;
}
else if (operacion == "rcr") {
    string ptr, count;
    ss >> ptr >> count;
    salida << "rotate_carry_right(" << ptr << ", " << count << "); // Rotate Carry Right" << endl;
}
else if (operacion == "fadd") {
    string mem;
    ss >> mem;
    salida << "st0 += *(float*)" << mem << "; // Floating-point Add" << endl;
}
else if (operacion == "lahf") {
    salida << "ah = (SF << 7) | (ZF << 6) | (0 << 5) | (AF << 4) | (0 << 3) | (PF << 2) | (1 << 1) | CF; // Load flags into AH" << endl;
}

else if (operacion == "jae") {
    string dest;
    ss >> dest;
    salida << "if (CF == 0) goto " << dest << "; // Jump if Above or Equal" << endl;
}
else if (operacion == "jb") {
    string dest;
    ss >> dest;
    salida << "if (CF == 1) goto " << dest << "; // Jump if Below" << endl;
}
else if (operacion == "int") {
    int numero;
    ss >> numero;
    salida << "interrupt(" << numero << "); // Interrupción del sistema" << endl;
}
else if (operacion == "lodsd") {
    salida << "eax = *(uint32_t*)esi; esi += 4; // Load String DWORD" << endl;
}
else if (operacion == "loop") {
    string dest;
    ss >> dest;
    salida << "if (--ecx != 0) goto " << dest << "; // Loop" << endl;
}
else if (operacion == "insb") {
    string ptr, reg;
    ss >> ptr >> reg;
    salida << ptr << " = " << reg << "; // Input Byte to ES:[EDI]" << endl;
}
else if (operacion == "fld") {
        string mem;
        ss >> mem;
        salida << "st0 = *(float*)" << mem << "; // Load floating-point value" << endl;
}
else if (operacion == "rcr") {
    string ptr, count;
    ss >> ptr >> count;
    salida << "rotate_carry_right(" << ptr << ", " << count << "); // Rotate Carry Right" << endl;
}
else if (operacion == "fadd") {
        string mem;
        ss >> mem;
        salida << "st0 += *(float*)" << mem << "; // Floating-point Add" << endl;
}
else if (operacion == "lahf") {
            salida << "ah = (SF << 7) | (ZF << 6) | (0 << 5) | (AF << 4) | (0 << 3) | (PF << 2) | (1 << 1) | CF; // Load flags into AH" << endl;
}




else {
      salida << "// No mapped instruction: " << instruccion << endl;
    }
}



void convertir_a_C(const string& archivo_log, const string& archivo_salida) {
    std::ifstream log_file(archivo_log);
    if (!log_file.is_open()) {
        std::cerr << "Error opening assembly file: " << archivo_log << std::endl;
        return;
    }

    std::ofstream salida(archivo_salida);
    if (!salida.is_open()) {
        std::cerr << "Error creating .C file: " << archivo_salida << std::endl;
        perror("Detalles del error");
        return;
    }

    agregar_bibliotecas(salida);

    std::string linea;
    while (getline(log_file, linea)) {
        procesar_instruccion(linea, salida);
    }

    log_file.close();
    salida.close();
    std::cout << "Successfully generated output file: " << archivo_salida << std::endl;
}