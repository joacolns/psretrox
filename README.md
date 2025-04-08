# psretrox - Playstation 2 Decompilation & Recompilation Tool

[Project Status]
Due to recent attention and viral spread, I want to clarify that this project has been paused for several months. It was started as a personal and experimental exploration into low-level C++, and it is not currently under active development. Feel free to explore and use the code freely, with no restrictions.

PSRetrox is a C++ application designed to reverse engineer PlayStation 2 games. The primary goal of this project is to extract and process game files, enabling native PC ports of PS2 games. PSRetrox includes built-in tools for decompiling and decoding game assets, such as audio, 3D models, source code, and other game data.

---

## **Progress**

<img src="./progress/psretrox_progressbar.svg">

<img src="./progress/gui_progressbar.svg">

<img src="./progress/mips_progressbar.svg">

<img src="./progress/recompilation_progressbar.svg">

<img src="./progress/retroxengine_progressbar.svg">

## **Features**
- **Reverse Engineering**: Decompiles PS2 game files from extracted .ISO files.
- **Recompile**: Recompile assembly files to C code.
- **Audio Extraction**: Extracts and decodes audio tracks (e.g., `.vag` files) into standard formats like `.mp3` or `.wav`.
- **3D Model Processing**: Extracts and processes 3D model data from game files for use in modern 3D tools.
- **File Decoding**: Handles PS2-specific formats like `.MB`, `.MH`, `.BD`, `.BH`, `.PSS`, `SLUS_XXX`, and more.
- **Built-in Decompiler**: Uses Capstone for binary disassembly.
- **High Performance**: Designed for fast and efficient extraction.
- **Retrox Engine**: Tool to port PS2 recompiled .C files to PC.

## **Contributing**
We welcome contributions to enhance PSRetrox. Here's how you can contribute:

### Code Contribution

1. Fork the repository and create your feature branch:
`git checkout -b feature/YourFeature`

2. Make your changes and commit them:
`git commit -m "Add YourFeature"`

3. Push your changes to your fork:
`git push origin feature/YourFeature`

4. Open a pull request on the main repository.

### **Code Guidelines**
Use consistent formatting and follow modern C++ practices.

- Provide clear comments for complex logic.

- Ensure your changes do not break existing functionality.

- Reporting Issues

### **Reporting Issues**

If you find a bug or have a feature request, please open an issue with a detailed description. 
