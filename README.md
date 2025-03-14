# cub3D - Raycasting Maze Explorer

## About

**cub3D** is a first-person 3D maze exploration game inspired by **Wolfenstein 3D**. It implements **raycasting** to create a pseudo-3D effect using **MiniLibX**. The project includes features like **collision handling**, a **minimap**, and **mouse control** for an immersive experience.

This project was made in collaboration with [@corentin-ltc](https://github.com/corentin-ltc/)

## Features

- **Raycasting-based 3D rendering**
- **Wall collision detection**
- **Minimap for navigation assistance**
- **Mouse control for smooth camera movement**
- **Textured walls and customizable colors**
- **Support for customizable `.cub` map files**

## Dependencies

- A C compiler (`gcc` or `clang`)
- `make` (for building the project)

## Installation

Clone the repository and build the project:

```sh
git clone https://github.com/Nabil080/cub3d
cd cub3D
make
```
The Makefile will automatically pull and compile the required submodules.

## Usage

Run the program with a `.cub` map file:

```sh
./cub3D maps/example.cub
```

## Map Format (.cub)

The `.cub` file defines the level layout with the following characters:

- `1` → Wall
- `0` → Walkable space
- `N`, `S`, `E`, `W` → Player start position and direction

Example map:
```
111111
100101
101001
1100N1
111111
```

## Controls

- **W / A / S / D** → Move forward, left, backward, right
- **Shift** → Sprint
- **Arrow keys / Mouse Movement** → Look around
- **ESC** → Exit the game

## Cleaning Up

To remove compiled files, run:

```sh
make clean
```

To completely remove all binaries and object files:

```sh
make fclean
```
