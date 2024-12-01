#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <vector>
#include <string>

#include "iso_reader.h"
#include "capstone_wrapper.h"
#include "mips_mapping.h"
#include "audio_extractor.h"
#include "3d_proccesor.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <sstream>

namespace fs = std::filesystem;

void displayLogo() {
    // Códigos de escape ANSI para texto rojo
    const std::string red = "\033[31m";
    const std::string reset = "\033[0m";

    std::cout << red;
    std::cout << R"(
                         _                       ___    __ 
                        | |                     / _ \  /_ |
  _ __   ___  _ __  ___ | |_  _ __  ___ __  __ | | | |  | |
 | '_ \ / __|| '__|/ _ \| __|| '__|/ _ \\ \/ / | | | |  | |
 | |_) |\__ \| |  |  __/| |_ | |  | (_) |>  <  | |_| |_ | |
 | .__/ |___/|_|   \___| \__||_|   \___//_/\_\  \___/(_)|_|   
 | |                                                       
 |_|                       
    
    )" << std::endl;
    std::cout << reset; // Restablecer el color de la terminal
}

void menu() {
        std::cout << "1 - Decompile files and recompile to C" << std::endl;
        std::cout << "2 - Decompile to MIPS assembly" << std::endl;
        std::cout << "3 - Recompile to C" << std::endl;
        std::cout << "4 - Decompile 3D models" << std::endl;
        std::cout << "5 - Decompile cutscenes" << std::endl;
        std::cout << "6 - Decompile audios" << std::endl;
        std::cout << "q - Quit" << std::endl;
        std::cout << "\n";
}

int convertPSSToMP4(const std::string& inputFilePath, const std::string& outputFilePath) {
    // Construir el comando de FFmpeg para convertir el archivo .PSS a .MP4
    std::string command = "ffmpeg -i " + inputFilePath + " -vcodec libx264 -acodec aac " + outputFilePath;

    // Ejecutar el comando de FFmpeg
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Error al convertir el archivo." << std::endl;
        return -1;
    }

    std::cout << "Conversión completada: " << outputFilePath << std::endl;

    return 0;
}

void recomp_C() {
    std::cout << "Recompilando..." << std::endl;

    std::string OUTFolder = "out/CrashTwinsanity";
    std::string RECOMPFolder = "out/CrashTwinsanity/C_recomp/";

    // Asegúrate de que la carpeta existe
    if (!fs::exists(OUTFolder) || !fs::is_directory(OUTFolder)) {
        std::cerr << "La carpeta " << OUTFolder << " no existe o no es un directorio válido." << std::endl;
        return;
    }

    if (!fs::exists(RECOMPFolder)) {
        std::cout << "La carpeta " << RECOMPFolder << " no existe. Creándola..." << std::endl;
        if (!fs::create_directory(RECOMPFolder)) {
            std::cerr << "Error al crear la carpeta " << RECOMPFolder << std::endl;
            return;
        }
    }

    for (const auto& entry : fs::directory_iterator(OUTFolder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".asm") {
            std::string filePath = entry.path().string();
            std::string fileName = entry.path().stem().string(); // Nombre del archivo sin extensión

            std::cout << "Procesando archivo: " << fileName << ".asm" << std::endl;

            // Generar un archivo de salida único con el mismo nombre del archivo .log
            std::string archivo_salida = RECOMPFolder + fileName + ".c";

            // Leer y desensamblar el archivo .log
            std::vector<uint8_t> assemblyfile = readFile(filePath);
            if (!assemblyfile.empty()) {
                std::cout << "Recompilando " << fileName << " a C..." << std::endl;
                convertir_a_C(filePath, archivo_salida);  // Usar el nombre dinámico
                std::cout << "Recompilado Completado para " << fileName << std::endl;
            }
            else {
                std::cerr << "No se pudo leer el archivo " << fileName << ".log" << std::endl;
            }
        }
    }
}

int DecompileMIPS() {

    std::cout << "Desensamblando Crash Twinsanity" << std::endl;

    std::string filePathSYS = "iso/SYSTEM.CNF";

    std::string filePathBD = "iso/CRASH6/CRASH.BD";
    std::string filePathBH = "iso/CRASH6/CRASH.MH";
    std::string filePathSLUS = "iso/SLUS_209.09";
    std::string filePathMUSIC_BD = "iso/CRASH6/MUSIC.MB";
    std::string filePathMUSIC_MH = "iso/CRASH6/MUSIC.MH";
    std::string filePathAMERICAN_MB = "iso/CRASH6/AMERICAN.MB";
    std::string filePathAMERICAN_MH = "iso/CRASH6/AMERICAN.MH";

    std::string IRXFolder = "iso/CRASH6/SYS";

    std::string outputDir = "out/CrashTwinsanity/videos";

    if (!fs::exists(outputDir)) {
        fs::create_directory(outputDir);
    }

    // Leer los archivos binarios
    std::vector<uint8_t> binaryCodeBD = readFile(filePathBD);
    std::vector<uint8_t> binaryCodeSLUS = readFile(filePathSLUS);
    std::vector<uint8_t> binaryCodeMUSIC_BD = readFile(filePathMUSIC_BD);
    std::vector<uint8_t> binaryCodeMUSIC_MH = readFile(filePathMUSIC_MH);
    std::vector<uint8_t> binaryCodeAMERICAN_MB = readFile(filePathAMERICAN_MB);
    std::vector<uint8_t> binaryCodeAMERICAN_MH = readFile(filePathAMERICAN_MH);
    std::vector<uint8_t> binaryCodeSYS = readFile(filePathSYS);

    if (binaryCodeBD.empty() && binaryCodeSLUS.empty() && binaryCodeMUSIC_BD.empty() && binaryCodeMUSIC_MH.empty() && binaryCodeAMERICAN_MB.empty() && binaryCodeAMERICAN_MH.empty() && binaryCodeSYS.empty()) {
        std::cerr << "No se pudo leer ninguno de los archivos." << std::endl;
        return 1;
    }

    // Desensamblar los archivos
    if (!binaryCodeBD.empty()) {
        std::cout << "Desensamblando CRASH.BD..." << std::endl;
        disassembleCode(binaryCodeBD, "CRASH.BD");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    if (!binaryCodeSLUS.empty()) {
        std::cout << "Desensamblando SLUS_209.09..." << std::endl;
        disassembleCode(binaryCodeSLUS, "SLUS_209.09");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    if (!binaryCodeMUSIC_BD.empty()) {
        std::cout << "Desensamblando MUSIC.MB..." << std::endl;
        disassembleCode(binaryCodeMUSIC_BD, "MUSIC.MB");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    if (!binaryCodeMUSIC_MH.empty()) {
        std::cout << "Desensamblando MUSIC.MH..." << std::endl;
        disassembleCode(binaryCodeMUSIC_MH, "MUSIC.MH");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    if (!binaryCodeAMERICAN_MB.empty()) {
        std::cout << "Desensamblando AMERICAN.MB..." << std::endl;
        disassembleCode(binaryCodeAMERICAN_MB, "AMERICAN.MB");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    if (!binaryCodeAMERICAN_MH.empty()) {
        std::cout << "Desensamblando AMERICAN.MH..." << std::endl;
        disassembleCode(binaryCodeAMERICAN_MH, "AMERICAN.MH");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    if (!binaryCodeSYS.empty()) {
        std::cout << "Desensamblando SYSTEM.CNF..." << std::endl;
        disassembleCode(binaryCodeSYS, "SYSTEM.CNF");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    std::cout << "Procesando archivos en la carpeta SYS (IRX)..." << std::endl;
    for (const auto& entry : fs::directory_iterator(IRXFolder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".IRX") {
            std::string filePath = entry.path().string();
            std::string fileName = entry.path().stem().string(); // Nombre del archivo sin extensión

            // Leer y desensamblar el archivo .PSS
            std::vector<uint8_t> binaryCodeIRX = readFile(filePath);
            if (!binaryCodeIRX.empty()) {
                std::cout << "Desensamblando " << fileName << ".IRX..." << std::endl;
                disassembleCode(binaryCodeIRX, fileName + ".IRX");
                std::cout << "Desensamblado Completado" << std::endl;
            }
            else {
                std::cerr << "No se pudo leer " << fileName << ".IRX" << std::endl;
            }

        }
    }


    return 0;
}

int PSS_Processor() {

    std::string fmvFolder = "iso/FMV"; // Carpeta donde están los archivos .PSS
    std::string fmvFolderBONS = "iso/FMV/BONUS"; // <-------- ONLY FOR CRASH BANDICOOT TWINSANITY

    std::string outputDir = "out/CrashTwinsanity/videos";

    if (!fs::exists(outputDir)) {
        fs::create_directory(outputDir);
    }

    // Procesar archivos .PSS en la carpeta FMV
    std::cout << "Procesando archivos en la carpeta FMV..." << std::endl;
    for (const auto& entry : fs::directory_iterator(fmvFolder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".PSS") {
            std::string filePath = entry.path().string();
            std::string fileName = entry.path().stem().string(); // Nombre del archivo sin extensión

            // Leer y desensamblar el archivo .PSS
            std::vector<uint8_t> binaryCodePSS = readFile(filePath);
            if (!binaryCodePSS.empty()) {
                std::cout << "Desensamblando " << fileName << ".PSS..." << std::endl;
                disassembleCode(binaryCodePSS, fileName + ".PSS");
                std::cout << "Desensamblado Completado" << std::endl;
            }
            else {
                std::cerr << "No se pudo leer " << fileName << ".PSS" << std::endl;
            }

            std::string inputFilePath = fmvFolder + "/" + fileName + ".PSS";  // Ruta completa al archivo .PSS
            std::string outputFilePathMP4 = outputDir + "/" + fileName + ".mp4";  // Ruta completa al archivo .PSS

            convertPSSToMP4(inputFilePath, outputFilePathMP4);

        }
    }

    std::cout << "Procesando archivos en la carpeta BONUS..." << std::endl;
    for (const auto& entry : fs::directory_iterator(fmvFolderBONS)) {
        if (entry.is_regular_file() && entry.path().extension() == ".PSS") {
            std::string filePath = entry.path().string();
            std::string fileName = entry.path().stem().string(); // Nombre del archivo sin extensión

            // Leer y desensamblar el archivo .PSS
            std::vector<uint8_t> binaryCodePSS = readFile(filePath);
            if (!binaryCodePSS.empty()) {
                std::cout << "Desensamblando " << fileName << ".PSS..." << std::endl;
                disassembleCode(binaryCodePSS, fileName + ".PSS");
                std::cout << "Desensamblado Completado" << std::endl;
            }
            else {
                std::cerr << "No se pudo leer " << fileName << ".PSS" << std::endl;
            }

            std::string inputFilePath = fmvFolderBONS + "/" + fileName + ".PSS";  // Ruta completa al archivo .PSS
            std::string outputFilePathMP4 = outputDir + "/" + fileName + ".mp4";  // Ruta completa al archivo .PSS

            convertPSSToMP4(inputFilePath, outputFilePathMP4);

        }
    }

    return 0;
}

int audio_extraction_test() {
    try {
        // Ruta de los archivos de entrada
        std::string musicMBPath = "iso/CRASH6/MUSIC.MB";
        std::string musicMHPath = "iso/CRASH6/MUSIC.MH";

        // Directorio de salida
        std::string outputDir = "out/CrashTwinsanity/audios";

        if (!std::filesystem::exists(outputDir)) {
            std::filesystem::create_directory(outputDir);
        }

        // Extraer las pistas
        extractAudioTracks(musicMBPath, musicMHPath, outputDir);
        std::cout << "Extracción completada. Los archivos están en: " << outputDir << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

void convertVagToAudio(const std::string& inputFilePath, const std::string& outputFilePath) {
    // Ruta completa a vgmstream-cli
    std::string vgmstreamPath = "C:/VisualStudioLibraries/vgmstream/vgmstream-cli.exe";
    std::string command = vgmstreamPath + " -o " + outputFilePath + " " + inputFilePath;

    // Ejecutar el comando en la terminal
    int result = std::system(command.c_str());

    // Verificar si la conversión fue exitosa
    if (result == 0) {
        std::cout << "Conversión exitosa: " << inputFilePath << " a " << outputFilePath << std::endl;
    }
    else {
        std::cerr << "Hubo un error al convertir el archivo: " << inputFilePath << std::endl;
    }
}

int audioOut() {
    // Directorio donde están los archivos .vag extraídos
    std::string inputDirectory = "out/CrashTwinsanity/audios";

    // Directorio donde guardar los archivos convertidos
    std::string outputDirectory = "out/CrashTwinsanity/audios";

    // Verificar que el directorio de salida existe, si no, crearlo
    if (!std::filesystem::exists(outputDirectory)) {
        std::filesystem::create_directory(outputDirectory);
    }

    // Iterar sobre los archivos .vag en el directorio de entrada
    for (const auto& entry : std::filesystem::directory_iterator(inputDirectory)) {
        if (entry.path().extension() == ".vag") {
            // Obtener la ruta completa del archivo .vag
            std::string inputFilePath = entry.path().string();

            // Crear la ruta de salida para el archivo convertido (en .mp3)
            std::string outputFilePath = outputDirectory + entry.path().stem().string() + ".mp3";

            // Llamar a la función para convertir el archivo
            convertVagToAudio(inputFilePath, outputFilePath);
        }
    }

    return 0;
}

int tdproccesor() {
    std::string crashBHPath = "iso/CRASH6/CRASH.BH";
    std::string crashBDPath = "iso/CRASH6/CRASH.BD";
    std::string outputDir = "out/CrashTwinsanity/3D";

    if (!std::filesystem::exists(outputDir)) {
        std::filesystem::create_directory(outputDir);
    }

    try {
        extractModels(crashBHPath, crashBDPath, outputDir);
        std::cout << "Extracción y conversión de modelos completada." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

class Logger {
public:
    Logger() {
        old_buf = std::cout.rdbuf();
        std::cout.rdbuf(buffer.rdbuf());
    }

    ~Logger() {
        std::cout.rdbuf(old_buf);
    }

    void clear() {
        buffer.str("");
        buffer.clear();
    }

    std::string getBuffer() const {
        return buffer.str();
    }

private:
    std::stringstream buffer;
    std::streambuf* old_buf;
};

int main() {
    // Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW" << std::endl;
        return -1;
    }

    // Configurar GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Crear una ventana GLFW
    GLFWwindow* window = glfwCreateWindow(1280, 720, "psretrox v0.1", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Habilitar V-Sync

    // Inicializar ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Configurar ImGui
    ImGui::StyleColorsDark();

    // Inicializar ImGui para GLFW y OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // DEFAULT PATH(S) FOR TESTING
    char SLUSPath[256] = "iso/SLUS_209.09";
    char SYSTEMPath[256] = "iso/SYSTEM.CNF";
    char crashBHPath[256] = "iso/CRASH6/CRASH.BH";
    char crashBDPath[256] = "iso/CRASH6/CRASH.BD";
    char lFile[256] = "iso/CRASH6/AMERICAN.MB";
    char lFileHeader[256] = "iso/CRASH6/AMERICAN.MH";
    char MusicFile[256] = "iso/CRASH6/MUSIC.MB";
    char MusicFileHeader[256] = "iso/CRASH6/MUSIC.MH";
    char PSS[256] = "iso/CRASH6/FMV/";
    char SYS[256] = "iso/CRASH6/SYS/";
    char outputDir[256] = "out/CrashTwinsanity/";

    // Crear una instancia de Logger
    Logger logger;

    // Bucle principal
    while (!glfwWindowShouldClose(window)) {
        // Manejar eventos de GLFW
        glfwPollEvents();

        // Iniciar un nuevo frame ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Crear la ventana ImGui
        ImGui::Begin("Decompiler/Recompiler");

        // Cuadros de texto para importar archivos
        ImGui::InputText(".SLUS Path", SLUSPath, IM_ARRAYSIZE(SLUSPath));
        ImGui::InputText(".BH Path", crashBHPath, IM_ARRAYSIZE(crashBHPath));
        ImGui::InputText(".BD Path", crashBDPath, IM_ARRAYSIZE(crashBDPath));
        ImGui::InputText("Music File", MusicFile, IM_ARRAYSIZE(MusicFile));
        ImGui::InputText("Music File (Header)", MusicFileHeader, IM_ARRAYSIZE(MusicFileHeader));
        ImGui::InputText("Language File", lFile, IM_ARRAYSIZE(lFile));
        ImGui::InputText("Language File (Header)", lFileHeader, IM_ARRAYSIZE(lFileHeader));
        ImGui::InputText("PSS/FMV Directory", PSS, IM_ARRAYSIZE(PSS));
        ImGui::InputText("SYS/IRX Directory", SYS, IM_ARRAYSIZE(SYS));

        ImGui::InputText("Output Directory", outputDir, IM_ARRAYSIZE(outputDir));

        // Botones para ejecutar las funciones
        if (ImGui::Button("Decompile & Recompile files")) {
            DecompileMIPS();
            recomp_C();
            PSS_Processor();
            audio_extraction_test();
            extractModels(crashBHPath, crashBDPath, outputDir);
        }
        if (ImGui::Button("Decompile MIPS")) {
            DecompileMIPS();
        }
        if (ImGui::Button("Recompile to C")) {
            recomp_C();
        }
        if (ImGui::Button("Decompile 3D Models")) {
            extractModels(crashBHPath, crashBDPath, outputDir);
        }
        if (ImGui::Button("Decompile PSS")) {
            PSS_Processor();
        }
        if (ImGui::Button("Decompile Audios")) {
            audio_extraction_test();
        }
        if (ImGui::Button("Open Retrox Engine")) {
            return 0;
        }
        /*
        if (ImGui::Button("Decompile Textures")) {
            extractTextures(crashBHPath, crashBDPath, outputDir);
        }
        */

        ImGui::End();

        // Crear la ventana de registro
        ImGui::Begin("Log");
        ImGui::TextUnformatted(logger.getBuffer().c_str());
        ImGui::End();

        ImGui::Begin("MIPS Files");
        ImGui::End();

        ImGui::Begin("Recompiled Files");
        ImGui::End();

        ImGui::Begin("3D Viewer");
        ImGui::End();

        // Renderizar ImGui
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Intercambiar buffers
        glfwSwapBuffers(window);
    }

    // Limpiar y finalizar ImGui y GLFW
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

/*
int main() {

     char selection;

    displayLogo();

    menu();

    std::cin >> selection;

    switch (selection) {
    case '1':
        DecompileMIPS();
        PSS_Processor();
        recomp_C();
        audio_extraction_test();
        audioOut();
		tdproccesor();
        break;
    case '2':
        DecompileMIPS();
        break;
    case '3':
        recomp_C();
        break;
    case '4':
        tdproccesor();
        break;
    case '5':
        PSS_Processor();
        break;
    case '6':
        audio_extraction_test();
        audioOut();
        break;
    case 'q':
        break;
    default:
        std::cout << "Invalid option. Please try again.\n";
        break;
    }


    std::cout << "Proceso finalizado." << std::endl;

    return 0;

}
*/
