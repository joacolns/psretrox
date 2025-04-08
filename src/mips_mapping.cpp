#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void addLibraries(ofstream& output) {
    output << "#include <iostream>" << endl;
    output << "#include <fstream>" << endl;
    output << "#include <sstream>" << endl;
    output << "#include <string>" << endl;
    output << "#include <vector>" << endl;
    output << "#include <stack>" << endl;
    output << endl;
}

void processInstruction(const string& instruction, ofstream& output) {
    stringstream ss(instruction);
    string operation;
    ss >> operation;

    if (operation == "add") {
        string dest, src;
        ss >> dest >> src;
        output << dest << " += " << src << ";" << endl;
    }
    else if (operation == "sub") {
        string dest, src;
        ss >> dest >> src;
        output << dest << " -= " << src << ";" << endl;
    }
    else if (operation == "and") {
        string dest, src;
        ss >> dest >> src;
        output << dest << " &= " << src << ";" << endl;
    }
    else if (operation == "or") {
        string dest, src;
        ss >> dest >> src;
        output << dest << " |= " << src << ";" << endl;
    }
    else if (operation == "xor") {
        string dest, src;
        ss >> dest >> src;
        output << dest << " ^= " << src << ";" << endl;
    }
    else if (operation == "adc") {
        string dest, src;
        ss >> dest >> src;
        output << dest << " += " << src << " + CF; // Suma con Carry" << endl;
    }
    else if (operation == "cmp") {
        string op1, op2;
        ss >> op1 >> op2;
        output << "if (" << op1 << " != " << op2 << ") { /* Acci�n */ };" << endl;
    }
    else if (operation == "test") {
        string op1, op2;
        ss >> op1 >> op2;
        output << "if ((" << op1 << " & " << op2 << ") == 0) { /* Acci�n */ };" << endl;
    }
    else if (operation == "jmp") {
        string dest;
        ss >> dest;
        output << "goto " << dest << ";" << endl;
    }
    else if (operation == "jo") {
        string dest;
        ss >> dest;
        output << "if (OF) goto " << dest << "; // Overflow Flag" << endl;
    }
    else if (operation == "jg") {
        string dest;
        ss >> dest;
        output << "if (SF == OF && ZF == 0) goto " << dest << "; // Jump if Greater" << endl;
    }
    else if (operation == "scasb") {
        string reg, ptr;
        ss >> reg >> ptr;
        output << "if (" << reg << " == " << ptr << ") { /* Acci�n */ };" << endl;
    }
    else if (operation == "arpl") {
        string dest, src;
        ss >> dest >> src;
        output << "//Segment privilege not supported on high levels" << endl;
    }
    else if (operation == "cld") {
        output << "DF = 0; // Clear Direction Flag" << endl;
    }
    else if (operation == "sbb") {
        string dest, src;
        ss >> dest >> src;
        output << dest << " = " << dest << " - (" << src << " + CF); // Subtract with Borrow" << endl;
    }
    else if (operation == "push") {
        string src;
        ss >> src;
        output << "stack.push(" << src << ");" << endl;
    }
    else if (operation == "pop") {
        string dest;
        ss >> dest;
        output << dest << " = stack.pop();" << endl;
    }
    else if (operation == "in") {
        string dest, port;
        ss >> dest >> port;
        output << dest << " = port_in(" << port << ");" << endl;
    }
    else if (operation == "invd") {
        output << "// Invalidate internal caches (no equivalent in high-level)" << endl;
    }
    else if (operation == "lds") {
        string dest, src;
        ss >> dest >> src;
        output << dest << " = " << src << "; // Load pointer and DS segment" << endl;
    }
    else if (operation == "movsd") {
        string dest, src;
        ss >> dest >> src;
        output << "*(" << dest << ") = *(" << src << "); // Move string data" << endl;
    }
    else if (operation == "dec") {
        string dest;
        ss >> dest;
        output << dest << " -= 1;" << endl;
    }
    else if (operation == "mov") {
        string reg, value;
        ss >> reg >> value;
        if (reg == "ch" && value == "0xff") {
            output << "ch = 0xff;" << endl;
        }
    }
    else if (operation == "inc") {
        string reg;
        ss >> reg;
        output << reg << " += 1;" << endl;
    }
    else if (operation == "rol") {
        string reg, val;
        ss >> reg >> val;
        output << "rotate_left(" << reg << ", " << val << ");" << endl;
    }
    else if (operation == "xchg") {
        string reg1, reg2;
        ss >> reg1 >> reg2;
        output << "swap(" << reg1 << ", " << reg2 << ");" << endl;
    }

    else if(operation == "jge") {
        string dest;
        ss >> dest;
        output << "if (SF == OF && ZF == 0) goto " << dest << "; // Jump if Greater or Equal" << endl;
    }
    else if (operation == "pavgb") {
        string reg1, reg2;
        ss >> reg1 >> reg2;
        output << reg1 << " = ( " << reg1 << " + " << reg2 << " ) / 2; // Packed Average Bytes" << endl;
    }
    else if (operation == "ret") {
        output << "return;" << endl;
    }
    else if (operation == "je") {
        string dest;
        ss >> dest;
        output << "if (ZF == 1) goto " << dest << "; // Jump if Equal" << endl;
    }
    else if (operation == "nop") {
        output << "// No Operation" << endl;
    }
    else if (operation == "pushal") {
        output << "//Akin to 'pushal', save all entries" << endl;
        output << "stack.push(eax);" << endl;
        output << "stack.push(ebx);" << endl;
        output << "stack.push(ecx);" << endl;
        output << "stack.push(edx);" << endl;
        output << "stack.push(esi);" << endl;
        output << "stack.push(edi);" << endl;
        output << "stack.push(ebp);" << endl;
    }
    else if (operation == "sldt") {
        string ptr;
        ss >> ptr;
        output << "ldt = " << ptr << "; // Load Local Descriptor Table" << endl;
    }
    else if (operation == "cwde") {
        output << "eax = (int)eax; // Convert from word to double word (extend)" << endl;
    }
    else if (operation == "jecxz") {
        string dest;
        ss >> dest;
        output << "if (ecx == 0) goto " << dest << "; // Jump if ECX is zero" << endl;
    }
    else if (operation == "insd") {
        string ptr;
        ss >> ptr;
        output << "port_in(" << ptr << "); // Input String or Data to es:[edi]" << endl;
    }
    else if (operation == "das") {
        output << "//Decimal Adjust after Subtraction (no equivalent in C)" << endl;
    }
    else if (operation == "js") {
        string dest;
        ss >> dest;
        output << "if (SF == 1) goto " << dest << "; // Jump if Signed" << endl;
    }
    else if (operation == "loopne") {
        string dest;
        ss >> dest;
        output << "if (ecx != 0 && ZF == 0) goto " << dest << "; // Loop while not Zero and Not Equal" << endl;
    }
    else if (operation == "rcl") {
        string reg;
        ss >> reg;
        output << "rotate_carry_left(" << reg << "); // Rotate Carry Left" << endl;
    }
    else if (operation == "imul") {
        string reg1, reg2;
        ss >> reg1 >> reg2;
        output << reg1 << " *= " << reg2 << "; // Integer Multiply" << endl;
    }
    else if (operation == "leave") {
        output << "ebp = stack.pop(); // Leave function (restore stack)" << endl;
    }
    else if (operation == "out") {
        string port, reg;
        ss >> port >> reg;
        output << "port_out(" << port << ", " << reg << "); // Output to port" << endl;
    }
    else if (operation == "psubd") {
        string reg1, reg2;
        ss >> reg1 >> reg2;
        output << reg1 << " -= " << reg2 << "; // Subtract Packed Data" << endl;
    }
    else if (operation == "cmc") {
        output << "CF = !CF; // Complement Carry Flag" << endl;
    }
    else if (operation == "jno") {
        string dest;
        ss >> dest;
        output << "if (!OF) goto " << dest << "; // Jump if No Overflow" << endl;
    }
    else if (operation == "outsd") {
        string port, src;
        ss >> port >> src;
        output << "port_out(" << port << ", " << src << "); // Output String Data" << endl;
    }
    else if (operation == "call") {
        string addr;
        ss >> addr;
        output << "call_function(" << addr << "); //Call to specific address" << endl;
    }
    else if (operation == "fistp") {
        string ptr;
        ss >> ptr;
        output << "*(" << ptr << ") = (int)FPU_TOP; // Store Integer and Pop FPU Stack" << endl;
    }
    else if (operation == "fsubr") {
        string src;
        ss >> src;
        output << "FPU_TOP = " << src << " - FPU_TOP; // Reverse Subtraction on FPU" << endl;
    }
    else if (operation == "lea") {
        string dest, addr;
        ss >> dest >> addr;
        output << dest << " = &(" << addr << "); // Load Effective Address" << endl;
    }
    else if (operation == "int3") {
        output << "debug_breakpoint(); // Interrupt 3" << endl;
    }
    else if (operation == "not") {
        string dest;
        ss >> dest;
        output << dest << " = ~" << dest << "; // Logical NOT" << endl;
    }
    else if (operation == "popal") {
        output << "//Simulate 'popal', restore all registers" << endl;
        output << "edi = stack.pop();" << endl;
        output << "esi = stack.pop();" << endl;
        output << "ebp = stack.pop();" << endl;
        output << "//Ignore 'esp', keep it intact" << endl;
        output << "ebx = stack.pop();" << endl;
        output << "edx = stack.pop();" << endl;
        output << "ecx = stack.pop();" << endl;
        output << "eax = stack.pop();" << endl;
    }
    else if (operation == "outsb") {
        string port, src;
        ss >> port >> src;
        output << "port_out(" << port << ", " << src << "); // Output String Byte" << endl;
    }
    else if (operation == "les") {
        string dest, src;
        ss >> dest >> src;
        output << dest << " = load_pointer(" << src << "); // Load ES Segment" << endl;
    }
    else if (operation == "lodsb") {
        string reg, src;
        ss >> reg >> src;
        output << reg << " = *(" << src << "); // Load String Byte" << endl;
    }
    else if (operation == "aad") {
        string imm;
        ss >> imm;
        output << "AL = (AL + AH * " << imm << ") & 0xFF; // ASCII Adjust AX after Division" << endl;
    }
    else if (operation == "pushfd") {
        output << "stack.push(EFLAGS); // Push Flags" << endl;
    }
    else if (operation == "loope") {
        string dest;
        ss >> dest;
        output << "if (--ECX && ZF) goto " << dest << "; // Loop while Equal" << endl;
    }
    else if (operation == "aaa") {
        output << "if ((AL & 0xF) > 9 || AF) { AL += 6; AH += 1; AF = 1; } else { AF = 0; } AL &= 0xF; // ASCII Adjust AL after Addition" << endl;
    }
    else if (operation == "sar") {
        string reg, imm;
        ss >> reg >> imm;
        output << reg << " >>= " << imm << "; // Arithmetic Shift Right" << endl;
    }
    else if (operation == "daa") {
        output << "//Decimal Adjust AL (no equivalent in high-level)" << endl;
    }
    else if (operation == "scasd") {
        string reg, ptr;
        ss >> reg >> ptr;
        output << "if (" << reg << " != *(" << ptr << ")) ZF = 0; else ZF = 1; // Compare String Data" << endl;
    }
    else if (operation == "idiv") {
        string src;
        ss >> src;
        output << "EDX:EAX /= " << src << "; // Integer Division" << endl;
    }
    else if (operation == "movsb") {
        string dest, src;
        ss >> dest >> src;
        output << "*(" << dest << ") = *(" << src << "); // Move Byte String" << endl;
    }
    else if (operation == "aam") {
        string imm;
        ss >> imm;
        output << "AH = AL / " << imm << "; AL %= " << imm << "; // ASCII Adjust AX after Multiplication" << endl;
    }
    else if (operation == "clc") {
        output << "CF = 0; // Clear Carry Flag" << endl;
    }
    else if (operation == "int1") {
        output << "debug_trap(); // Interrupt 1 (Debug)" << endl;
    }
    else if (operation == "shl") {
        string reg, imm;
        ss >> reg >> imm;
        output << reg << " <<= " << imm << "; // Logical Shift Left" << endl;
    }
    else if (operation == "cli") {
        output << "IF = 0; // Clear Interrupt Flag" << endl;
    }
    else if (operation == "jle") {
        string dest;
        ss >> dest;
        output << "if (ZF || SF != OF) goto " << dest << "; // Jump if Less or Equal" << endl;
    }
    else if (operation == "jae") {
    string dest;
    ss >> dest;
    output << "if (CF == 0) goto " << dest << "; // Jump if Above or Equal" << endl;
}
else if (operation == "jb") {
    string dest;
    ss >> dest;
    output << "if (CF == 1) goto " << dest << "; // Jump if Below" << endl;
}
else if (operation == "jbe") {
    string dest;
    ss >> dest;
    output << "if (CF == 1 || ZF == 1) goto " << dest << "; // Jump if Below or Equal" << endl;
}
else if (operation == "ja") {
    string dest;
    ss >> dest;
    output << "if (CF == 0 && ZF == 0) goto " << dest << "; // Jump if Above" << endl;
}
else if (operation == "fimul") {
    output << "//Integer Multiply (no direct equivalent in C++)" << endl;
}
else if (operation == "jp") {
    string dest;
    ss >> dest;
    output << "if (PF == 1) goto " << dest << "; // Jump if Parity" << endl;
}
else if (operation == "int") {
    int numero;
    ss >> numero;
    output << "interrupt(" << numero << "); //System interrupt" << endl;
}
else if (operation == "lodsd") {
    output << "eax = *(uint32_t*)esi; esi += 4; // Load String DWORD" << endl;
}
else if (operation == "loop") {
    string dest;
    ss >> dest;
    output << "if (--ecx != 0) goto " << dest << "; // Loop" << endl;
}
else if (operation == "insb") {
    string ptr, reg;
    ss >> ptr >> reg;
    output << ptr << " = " << reg << "; // Input Byte to ES:[EDI]" << endl;
}
else if (operation == "fld") {
    string mem;
    ss >> mem;
    output << "st0 = *(float*)" << mem << "; // Load floating-point value" << endl;
}
else if (operation == "rcr") {
    string ptr, count;
    ss >> ptr >> count;
    output << "rotate_carry_right(" << ptr << ", " << count << "); // Rotate Carry Right" << endl;
}
else if (operation == "fadd") {
    string mem;
    ss >> mem;
    output << "st0 += *(float*)" << mem << "; // Floating-point Add" << endl;
}
else if (operation == "lahf") {
    output << "ah = (SF << 7) | (ZF << 6) | (0 << 5) | (AF << 4) | (0 << 3) | (PF << 2) | (1 << 1) | CF; // Load flags into AH" << endl;
}
else if (operation == "jae") {
    string dest;
    ss >> dest;
    output << "if (CF == 0) goto " << dest << "; // Jump if Above or Equal" << endl;
}
else if (operation == "jb") {
    string dest;
    ss >> dest;
    output << "if (CF == 1) goto " << dest << "; // Jump if Below" << endl;
}
else if (operation == "jbe") {
    string dest;
    ss >> dest;
    output << "if (CF == 1 || ZF == 1) goto " << dest << "; // Jump if Below or Equal" << endl;
}
else if (operation == "ja") {
    string dest;
    ss >> dest;
    output << "if (CF == 0 && ZF == 0) goto " << dest << "; // Jump if Above" << endl;
}
else if (operation == "fimul") {
    output << "// Integer Multiply (no direct equivalent in C++)" << endl;
}
else if (operation == "jp") {
    string dest;
    ss >> dest;
    output << "if (PF == 1) goto " << dest << "; // Jump if Parity" << endl;
}
else if (operation == "int") {
    int numero;
    ss >> numero;
    output << "interrupt(" << numero << "); //System interrupt" << endl;
}
else if (operation == "lodsd") {
    output << "eax = *(uint32_t*)esi; esi += 4; // Load String DWORD" << endl;
}
else if (operation == "loop") {
    string dest;
    ss >> dest;
    output << "if (--ecx != 0) goto " << dest << "; // Loop" << endl;
}
else if (operation == "insb") {
    string ptr, reg;
    ss >> ptr >> reg;
    output << ptr << " = " << reg << "; // Input Byte to ES:[EDI]" << endl;
}
else if (operation == "fld") {
    string mem;
    ss >> mem;
    output << "st0 = *(float*)" << mem << "; // Load floating-point value" << endl;
}
else if (operation == "rcr") {
    string ptr, count;
    ss >> ptr >> count;
    output << "rotate_carry_right(" << ptr << ", " << count << "); // Rotate Carry Right" << endl;
}
else if (operation == "fadd") {
    string mem;
    ss >> mem;
    output << "st0 += *(float*)" << mem << "; // Floating-point Add" << endl;
}
else if (operation == "lahf") {
    output << "ah = (SF << 7) | (ZF << 6) | (0 << 5) | (AF << 4) | (0 << 3) | (PF << 2) | (1 << 1) | CF; // Load flags into AH" << endl;
}

else if (operation == "jae") {
    string dest;
    ss >> dest;
    output << "if (CF == 0) goto " << dest << "; // Jump if Above or Equal" << endl;
}
else if (operation == "jb") {
    string dest;
    ss >> dest;
    output << "if (CF == 1) goto " << dest << "; // Jump if Below" << endl;
}
else if (operation == "int") {
    int numero;
    ss >> numero;
    output << "interrupt(" << numero << "); //System interrupt" << endl;
}
else if (operation == "lodsd") {
    output << "eax = *(uint32_t*)esi; esi += 4; // Load String DWORD" << endl;
}
else if (operation == "loop") {
    string dest;
    ss >> dest;
    output << "if (--ecx != 0) goto " << dest << "; // Loop" << endl;
}
else if (operation == "insb") {
    string ptr, reg;
    ss >> ptr >> reg;
    output << ptr << " = " << reg << "; // Input Byte to ES:[EDI]" << endl;
}
else if (operation == "fld") {
        string mem;
        ss >> mem;
        output << "st0 = *(float*)" << mem << "; // Load floating-point value" << endl;
}
else if (operation == "rcr") {
    string ptr, count;
    ss >> ptr >> count;
    output << "rotate_carry_right(" << ptr << ", " << count << "); // Rotate Carry Right" << endl;
}
else if (operation == "fadd") {
        string mem;
        ss >> mem;
        output << "st0 += *(float*)" << mem << "; // Floating-point Add" << endl;
}
else if (operation == "lahf") {
            output << "ah = (SF << 7) | (ZF << 6) | (0 << 5) | (AF << 4) | (0 << 3) | (PF << 2) | (1 << 1) | CF; // Load flags into AH" << endl;
}




else {
      output << "//Unmapped instruction: " << instruction << endl;
    }
}



void convertToC(const string& logFile, const string& outputFile) {
    std::ifstream logFile(logFile);
    if (!logFile.is_open()) {
        std::cerr << "Error opening log: " << logFile << std::endl;
        return;
    }

    std::ofstream output(outputFile);
    if (!outputFile.is_open()) {
        std::cerr << "Error when creating file: " << outputFile << std::endl;
        perror("Error details"); //Show error details
        return;
    }

    //Add libraries
    addLibraries(output);

    std::string line;
    while (getline(logFile, line)) {
        processInstructions(line, output);
    }

    logFile.close();
    output.close();
    std::cout << "File was correctly processed: " << outputFile << std::endl;
}