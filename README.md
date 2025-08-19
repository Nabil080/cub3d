# Cub3d - Raycasting Engine

Cub3d is a simple raycasting engine written in C, using the MiniLibX graphical library to render a 3D environment from 2D map files. It supports basic movement, camera adjustments, and a customizable minimap, serving as a minimal recreation of a raycasting-based environment without gameplay elements.

<img width="800" height="608" alt="demo" src="cub3d.gif" />

> Note: Check out this [demo video](https://youtu.be/TiSBKJ6pB04) to see Cub3d in action.

This project integrates submodules for MiniLibX and libft, which are automatically handled via the Makefile.

## Features

- Movement Mechanics : Move, sprint, jump, crouch
- Stamina mangement : Toggleable stamina consumption and recovery
- Camera control : Look around / up / down, change the FOV
- Minimap control : Toggle, zoom, resize
- Toggleable minimap visuals : casted rays, wall highlights, grid, ray frequency...
- Others:
  - Lightless mode
  - Loads any valid map (You can make your own or even load any texture you want)
  - Handles different colors for ceiling/floor and different textures for wall depending on their orientation.
  - Error handling for invalid map files.
  - Fps counter.

## Dependencies

- A C compiler (`gcc` or `clang`)
- `make` (for building the project)
- MiniLibX for Linux requires xorg, x11, and zlib. Install these dependencies on Ubuntu with:
  ```sh
  sudo apt-get update && sudo apt-get install xorg libxext-dev zlib1g-dev libbsd-dev
  ```

## Installation

Clone the repository and build the project:

```sh
git clone https://github.com/your-username/cub3d
cd cub3d
make
```

The Makefile automatically pulls and compiles the required submodules (MiniLibX and libft).

## Usage

Run the program with a valid map file:

```sh
./cub3d maps/map.cub
```

Preset maps are available in the `maps/` directory. You can create custom maps following the [file format](#file-format).

## File Format (.cub)

The `.cub` file defines the game map, including walls, player start position, and textures. It consists of configuration lines (e.g., textures, colors) followed by a 2D grid of characters representing the map layout.

Example:
```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm
F 220,100,0
C 225,30,0

11111
10001
10N01
10001
11111
```
- `NO`, `SO`, `WE`, `EA`: Texture paths for walls.
- `F`, `C`: Floor and ceiling colors (RGB).
- Map grid: `1` (wall), `0` (empty space), `N`/`S`/`E`/`W` (player start position and orientation).

## Controls

- **Escape**: Exit the game
- **WASD/Arrow Keys**: Move
- **Space**: Jump
- **Shift**: Sprint
- **Ctrl**: Crouch
- **Mouse**: Look around/up/down
- **F**: Change FOV
- **M**: Toggle minimap
- **Numpad +/-**: Adjust map size/zoom
- **R**: Change minimap ray rate
- **T**: Toggle minimap rays
- **Y**: Toggle minimap wall highlights
- **G**: Toggle minimap grid
- **L**: Toggle light mode
- **P**: Toggle stamina
- **B**: Toggle debug mode

## Cleaning Up

Remove compiled object files:

```sh
make clean
```

Remove all binaries and object files:

```sh
make fclean
```
