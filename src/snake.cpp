#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

// размеры окна
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// размеры ячейки
const int CELL_SIZE = 20;

// количество ячеек по горизонтали и вертикали
const int GRID_WIDTH = WINDOW_WIDTH / CELL_SIZE;
const int GRID_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

// цвета
const SDL_Color BLACK = {0, 0, 0};
const SDL_Color WHITE = {255, 255, 255};
const SDL_Color GREEN = {0, 255, 0};
const SDL_Color RED = {255, 0, 0};

// направления движения змейки
enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// структура для хранения координат ячейки
struct Cell {
    int x;
    int y;
};





