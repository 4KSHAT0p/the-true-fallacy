# The-True-Fallacy

The True Fallacy is a simple 2D fighting game developed with C language ,SDL2 and it's sub-libraries. The game features two characters that players can control to engage in combat within a beautifully rendered environment. This project serves as an introduction to game development concepts such as animation, collision detection, and sprite handling.

The game's characters are based on
#### https://typemoon.fandom.com/wiki/Shiki_Tohno
#### https://typemoon.fandom.com/wiki/Shiki_Ryougi

## Features:

- **Two Playable Characters**: Each with unique animations and actions.
- **Collision Detection**: Ensuring characters interact correctly with the environment and each other.
- **Stunning Backgrounds**: Vibrant and immersive environments which change after every match.
- **Dynamic variation of colors**: The color of health of each character changes from green to red based on average health of the players.
- **Simple Controls**:

    Press **Enter** to start the game.

    For the male character named "**Shiki Tohno**" :
    | Key           | Action               |
    | ------------- | -------------------- |
    | A             | Move Left            |
    | D             | Move Right           |
    | Space         | Jump                 |
    | X             | Attack               |

    For the female character named "**Shiki Ryougi**" :
    | Key           | Action               |
    | ------------- | -------------------- |
    | 4 (numpad)    | Move Left            |
    | 6 (numpad)    | Move Right           |
    | 0 (numpad)    | Jump                 |
    | Enter (numpad)| Attack               |


## Screenshots:

![Screenshot 2023-06-05 002453](https://github.com/user-attachments/assets/0b6cd76a-c356-4f43-bdb4-6f9847d8fac3)

![Screenshot 2024-08-04 182725](https://github.com/user-attachments/assets/6a98b8fc-3800-4b13-a9b7-fbfcfbdd0742)

![Screenshot 2024-08-04 190532](https://github.com/user-attachments/assets/82714ce4-e85f-4838-97cb-5c238cf934e8)

![tt2](https://github.com/user-attachments/assets/1678b7c4-e801-44fa-8bd6-f56d84b0890f)

# About SDL

- **[SDL2](https://www.libsdl.org/)**  
  Simple DirectMedia Layer – core graphics, input, and window handling.

- **[SDL2_image](https://www.libsdl.org/projects/SDL_image/)**  
  Support for loading images like PNG, JPG, and others.

- **[SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)**  
  TrueType font rendering support for SDL2.

- **[SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/)**  
  Multi-channel audio mixer for sound effects and music.

  
## How to Run it?

1. Make sure that **gcc compiler (mingw32)** is installed in your system. It may already be installed if you have written some C/C++ code in past.

2. Clone the repository:
```bash
git clone https://github.com/4KSHAT0p/the-true-fallacy.git
```

3. Run the game via:
```bash
mingw32-make -f MakeFile
./main
```



# If you want to setup SDL2 from scratch

### Setting up SDL2 on Windows:

1. Download the "SDL2-devel-2.30.6-mingw.tar.gz" from [here](https://github.com/libsdl-org/SDL/releases/tag/release-2.30.6).
2. Extract the files and copy the all header files from "x86_64-w64-mingw32/include/SDL2" folder to a new folder named "sdl" and move that folder to the project's include directory which I have already provided in the repository.
3. Copy the lib folder from the "x86_64-w64-mingw32" folder to the project directory as it is.
4. Now for the DLL files, copy the dll files present in "x86_64-w64-mingw32/bin" and paste them in the project directory (dont create new folder for these).
5. Do the same for: [SDL_image](https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.8.2-mingw.tar.gz) ,
[SDL_ttf](https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.22.0-mingw.tar.gz) ,
[SDL_Mixer](https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.8.0-mingw.tar.gz).
    
6. Compile the project using the provided `MakeFile` or edit it according to your requirements.
7. Run the `Main.exe` using `./main`.

