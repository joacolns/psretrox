cmake_minimum_required(VERSION 3.10)
project(psretrox)

set(CMAKE_CXX_STANDARD 17)

# Añadir las rutas de los archivos fuente
include_directories(include)

# Añadir Capstone como dependencia
find_package(Capstone REQUIRED)

# Añadir los archivos fuente
add_executable(psretrox src/main.cpp src/iso_reader.cpp src/capstone_wrapper.cpp)

# Vincular Capstone
target_link_libraries(psretrox Capstone::Capstone)