# SDL2 A*

<div align="center"><img src=gifs/astar_maze.gif /></div>


## How to run
In order to compile the code, you need the [SDL2 library](https://wiki.libsdl.org/SDL2/Installation) installed.

Pre-built binaries are available for most operating systems, and chances are the library is already installed as a dependency of other software you already have.

Compile the code with:
```bash
$ cmake .
$ cmake --build .
```

Then, run the A_star executable with or without arguments:
```bash
$ A_star               # Shows a blank screen
$ A_star 1             # Shows a full screen of walls (edit by removing rathen then adding)
$ A_star filepath      # Loads a grid from a file
```

To play around with the maze shown in the video:
```bash
$ A_star saved_grid.txt
```


## Controls
You can interact with the grid using your mouse.

- Left-button adds new items
- Right-button removes items

You can also press a mouse button and move around for faster editing.

Depending on the "mode" you are in, you can draw one of three things:
- A new Starting point
- A new Destination point
- The walls of the maze

You can set a mode and perform other operations through the keyboard.

| Key | Description |
|-----------|-------------|
|S/B| Set Start (or Begin) mode|
|D/E| Set Destination (or End) mode|
|W| Set Wall Drawing mode |
|Enter|  Run A* / Reset grid |
|P| Print the current grid to a file (saved in current directory) |
|L| Loads a grid from a file named _saved_grid.txt_  in current directory |
|Q| Close Window |

