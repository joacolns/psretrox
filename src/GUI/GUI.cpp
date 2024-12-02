#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <string>
#include <cstdlib>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "iso_reader.h"
#include "capstone_wrapper.h"
#include "mips_mapping.h"
#include "audio_extractor.h"
#include "3d_proccesor.h"
#include <PSS_Proccesor.h>

namespace fs = std::filesystem;

std::vector<std::string> getFilesInDirectory(const std::string& directory, const std::string& extension) {
    std::vector<std::string> files;
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        return files;
    }
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            files.push_back(entry.path().filename().string());
        }
    }
    return files;
}

std::string readFileContent(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return "Error opening file.";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void saveFileContent(const std::string& filePath, const std::string& content) {
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << content;
        file.close();
    }
    else {
        std::cerr << "Error opening file: " << filePath << std::endl;
    }
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

int renderGUI() {
    if (!glfwInit()) {
        std::cerr << "GLFW Error" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "psretrox v0.1", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW Window Error" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // V-Sync ON

    // Cargar el ícono
    int iconWidth, iconHeight, iconChannels;
    unsigned char* iconPixels = stbi_load("include/ico.png", &iconWidth, &iconHeight, &iconChannels, 4);
    if (iconPixels) {
        GLFWimage images[1];
        images[0].width = iconWidth;
        images[0].height = iconHeight;
        images[0].pixels = iconPixels;
        glfwSetWindowIcon(window, 1, images);
        stbi_image_free(iconPixels);
    }
    else {
        std::cerr << "Icon Error" << std::endl;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

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

    Logger logger;

    std::string selectedFileContent;
    std::string selectedFilePath;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Decompiler/Recompiler");

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

        if (ImGui::Button("Decompile & Recompile files")) {
            DecompileMIPS();
            recomp_C();
            PSS_Processor();
            audio_extraction_test();
            proccesor();
        }
        if (ImGui::Button("Decompile MIPS")) {
            DecompileMIPS();
        }
        if (ImGui::Button("Recompile to C")) {
            recomp_C();
        }
        if (ImGui::Button("Decompile 3D Models")) {
            proccesor();
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

        ImGui::End();

        ImGui::Begin("Log");
        ImGui::TextUnformatted(logger.getBuffer().c_str());
        ImGui::End();

        ImGui::Begin("MIPS Files");
        if (fs::exists("out/CrashTwinsanity") && fs::is_directory("out/CrashTwinsanity")) {
            std::vector<std::string> mipsFiles = getFilesInDirectory("out/CrashTwinsanity", ".asm");
            for (const auto& file : mipsFiles) {
                if (ImGui::Selectable(file.c_str(), selectedFilePath == "out/CrashTwinsanity/" + file)) {
                    selectedFilePath = "out/CrashTwinsanity/" + file;
                    selectedFileContent = readFileContent(selectedFilePath);
                }
            }
        }
        ImGui::End();

        ImGui::Begin("Recompiled Files");
        if (fs::exists("out/CrashTwinsanity/C_recomp") && fs::is_directory("out/CrashTwinsanity/C_recomp")) {
            std::vector<std::string> recompiledFiles = getFilesInDirectory("out/CrashTwinsanity/C_recomp", ".c");
            std::vector<std::string> recompiledCppFiles = getFilesInDirectory("out/CrashTwinsanity/C_recomp", ".cpp");
            recompiledFiles.insert(recompiledFiles.end(), recompiledCppFiles.begin(), recompiledCppFiles.end());
            for (const auto& file : recompiledFiles) {
                if (ImGui::Selectable(file.c_str(), selectedFilePath == "out/CrashTwinsanity/C_recomp/" + file)) {
                    selectedFilePath = "out/CrashTwinsanity/C_recomp/" + file;
                    selectedFileContent = readFileContent(selectedFilePath);
                }
            }
        }
        ImGui::End();

        ImGui::Begin("Code/3D Viewer");
        ImGui::InputTextMultiline("##source", &selectedFileContent[0], selectedFileContent.size() + 1, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 50));
        if (ImGui::Button("Save")) {
            saveFileContent(selectedFilePath, selectedFileContent);
        }
        ImGui::End();

        // ImGui Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Switch Buffers
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