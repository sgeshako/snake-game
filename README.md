# Classic Snake 🐍: A simple console game in C that is also portable!  

This is a simple Console game developed in C for fun and as a learning experience.  
It is cross-platform and can be compiled for both Linux and Windows.

## Features  
- Classic Snake gameplay in the terminal: **Arrow Keys** → Move the snake. **Q** → Quit the game.
- No use of threads, just a simple main loop  
- Cross-platform support (uses ncurses / Win32 Console API)  
- Binary size: ~25KB  
- No dynamic memory allocation (All memory is statically allocated at compile time)

## Project Structure
```plaintext
├── main.c                               # Main application
├── snake_controls.h/snake_controls.c    # Calculate Snake movement and direction
├── snake_gui.h/snake_gui.c              # Draw Snake, game border and food
├── snake_io.h                           # Abstract OS-specific functionality
├── snake_io_linux.c                     # Implement Linux-specific I/O functions
├── snake_io_windows.c                   # Implement Windows-specific I/O functions
└── utils.h/utils.c                      # Utility functions
```

## Demo
https://github.com/user-attachments/assets/8653d31c-0139-4ac1-b678-e56f050a79bc

## How to Build  

### **Linux** 
Checkout code and run Make. You need to have `gcc` and `ncurses` installed:  
```sh
git clone https://github.com/sgeshako/snake-game.git
cd snake-game
make linux
```
Run the game:
```sh
./build/snake.bin
```
### **Windows**
You need MinGW-w64 to compile the game:
```sh
CC=x86_64-w64-mingw32-gcc make windows
# or
make windows
```
Run the game:
```sh
./build/snake.exe
```
Note: Has noticeable flashing on Windows, probably due to use of `system("cls");`  
