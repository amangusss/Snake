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
const SDL_Color BLACK = { 0, 0, 0 };
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

// класс для змейки
class Snake {
public:
    // конструктор
    Snake() {
        // устанавливаем начальное состояние змейки
        reset();

    }

    // метод для получения значения флага жизни змейки
    bool getIsAlive() {
        return isAlive;
    }

    // метод для обновления состояния змейки
    void update() {
        // если змейка жива
        if (isAlive) {
            // перемещаем голову змейки в соответствии с направлением движения
            switch (direction) {
            case UP:
                head.y--;
                break;
            case DOWN:
                head.y++;
                break;
            case LEFT:
                head.x--;
                break;
            case RIGHT:
                head.x++;
                break;
            }
            // проверяем, не вышла ли змейка за границы поля
            if (head.x < 0 || head.x >= GRID_WIDTH || head.y < 0 || head.y >= GRID_HEIGHT) {
                // если вышла, то перемещаем ее на противоположную сторону
                head.x = (head.x + GRID_WIDTH) % GRID_WIDTH;
                head.y = (head.y + GRID_HEIGHT) % GRID_HEIGHT;
            }
            // проверяем, не съела ли змейка сама себя
            for (int i = 0; i < length - 1; i++) {
                // если координаты головы совпадают с координатами какого-то элемента тела
                if (head.x == body[i].x && head.y == body[i].y) {
                    // то змейка умирает
                    isAlive = false;
                }
            }
            // если змейка не ест
            if (!isEating) {
                // то перемещаем хвост змейки на одну ячейку вперед по телу змейки
                tail.x = body[length - 2].x;
                tail.y = body[length - 2].y;
            }
            else {
                // иначе увеличиваем длину змейки на одну ячейку
                length++;
                // и сбрасываем флаг еды
                isEating = false;
            }
            // сдвигаем тело змейки на одну ячейку вперед по направлению к голове
            for (int i = length - 2; i > 0; i--) {
                body[i].x = body[i - 1].x;
                body[i].y = body[i - 1].y;
            }
            // устанавливаем первый элемент тела змейки равным предыдущему положению головы
            body[0].x = prevHead.x;
            body[0].y = prevHead.y;
            // запоминаем текущее положение головы змейки
            prevHead.x = head.x;
            prevHead.y = head.y;
        }
    }

    // метод для отрисовки змейки на экране
    void render(SDL_Renderer* renderer) {
        // если змейка жива
        if (isAlive) {
            // то рисуем ее голову и тело зеленым цветом
            SDL_SetRenderDrawColor(renderer, GREEN.r, GREEN.g, GREEN.b, GREEN.a);
            SDL_Rect rect;
            rect.w = CELL_SIZE;
            rect.h = CELL_SIZE;
            rect.x = head.x * CELL_SIZE;
            rect.y = head.y * CELL_SIZE;
            SDL_RenderFillRect(renderer, &rect);
            for (int i = 0; i < length - 1; i++) {
                rect.x = body[i].x * CELL_SIZE;
                rect.y = body[i].y * CELL_SIZE;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
        else {
            // иначе рисуем ее красным цветом
            SDL_SetRenderDrawColor(renderer, RED.r, RED.g, RED.b, RED.a);
            SDL_Rect rect;
            rect.w = CELL_SIZE;
            rect.h = CELL_SIZE;
            rect.x = head.x * CELL_SIZE;
            rect.y = head.y * CELL_SIZE;
            SDL_RenderFillRect(renderer, &rect);
            for (int i = 0; i < length - 1; i++) {
                rect.x = body[i].x * CELL_SIZE;
                rect.y = body[i].y * CELL_SIZE;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    // метод для изменения направления движения змейки по нажатию клавиши
    void changeDirection(SDL_Keycode key) {
        switch (key) {
        case SDLK_UP:
            if (direction != DOWN) direction = UP;
            break;
        case SDLK_DOWN:
            if (direction != UP) direction = DOWN;
            break;
        case SDLK_LEFT:
            if (direction != RIGHT) direction = LEFT;
            break;
        case SDLK_RIGHT:
            if (direction != LEFT) direction = RIGHT;
            break;
        }
    }

    // метод для проверки столкновения головы змейки с едой
    bool checkCollisionWithFood(Cell food) {
        if (head.x == food.x && head.y == food.y) {
            return true;
        }
        else {
            return false;
        }
    }

    // метод для установки флага еды в истину
    void eat() {
        isEating = true;
    }

    // метод для возвращения змейки в начальное состояние
    void reset() {
        // устанавливаем начальное направление движения
        direction = RIGHT;
        // устанавливаем начальную длину змейки
        length = 3;
        // устанавливаем начальные координаты головы и хвоста змейки
        head.x = GRID_WIDTH / 2;
        head.y = GRID_HEIGHT / 2;
        tail.x = head.x - length + 1;
        tail.y = head.y;
        // устанавливаем флаги для проверки столкновений
        isAlive = true;
        isEating = false;

    }
    bool isAlive; // флаг жизни змейки

private:
    Cell head; // голова змейки
    Cell prevHead; // предыдущее положение головы змейки
    Cell tail; // хвост змейки
    Cell body[GRID_WIDTH * GRID_HEIGHT]; // тело змейки (массив ячеек)
    Direction direction; // направление движения змейки
    int length; // длина змейки в ячейках
    bool isEating; // флаг еды змейки

};

// класс для еды
class Food {
public:
    // конструктор без параметров
    Food() {}

    // конструктор с параметром - указателем на объект класса Snake 
    Food(Snake* snakePtr) {
        snake = snakePtr; 
        generate(); 
    }

    // метод для генерации новой позиции еды на поле случайным образом 
    void generate() {
        bool validPosition; 
        do {
           validPosition = true; 
           x = rand() % GRID_WIDTH; 
           y = rand() % GRID_HEIGHT; 
           if (snake->checkCollisionWithFood({x,y})) { 
               validPosition=false; 
           }  
       } while (!validPosition);  
   }

   // метод для отрисовки еды на экране красным цветом 
   void render(SDL_Renderer* renderer) {  
       SDL_SetRenderDrawColor(renderer, RED.r, RED.g, RED.b,RED.a); 
       SDL_Rect rect; 
       rect.w = CELL_SIZE; 
       rect.h = CELL_SIZE; 
       rect.x = x * CELL_SIZE; 
       rect.y = y * CELL_SIZE; 
       SDL_RenderFillRect(renderer, &rect); 
   }

   // метод для получения координат еды в виде структуры Cell 
   Cell getCell() { 
       return {x,y}; 
   }

private:
    int x; // координата x еды
    int y; // координата y еды
    Snake* snake; // указатель на объект класса Snake
};

// главная функция
int main(int argc, char* argv[]) {
    // инициализируем генератор случайных чисел
    srand(time(NULL));
    // инициализируем библиотеку SDL2
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // создаем окно с заданными размерами и заголовком
    SDL_Window* window = SDL_CreateWindow("Snake Game", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    // создаем рендерер для отрисовки графики в окне
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    // создаем объект класса Snake
    Snake snake;
    // создаем объект класса Food и передаем ему указатель на объект класса Snake
    Food food(&snake);
    // создаем переменную для хранения состояния игрового цикла
    bool quit = false;
    // создаем переменную для хранения событий от клавиатуры и мыши
    SDL_Event event;
    // запускаем игровой цикл
    while (!quit) {
        // обрабатываем события от клавиатуры и мыши
        while (SDL_PollEvent(&event)) {
            // если пользователь нажал на крестик окна, то выходим из цикла
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            // если пользователь нажал на клавишу, то меняем направление движения змейки
            if (event.type == SDL_KEYDOWN) {
                if (snake.isAlive) {
                    snake.changeDirection(event.key.keysym.sym);
                }
                else {
                    if (event.type == SDL_KEYDOWN) {
                        snake.reset();
                        food.generate();
                    }
                }
            }
        }
        // обновляем состояние змейки
        snake.update();
        // проверяем столкновение змейки с едой
        if (snake.checkCollisionWithFood(food.getCell())) {
            // если есть столкновение, то змейка ест еду
            snake.eat();
            // и генерируем новую позицию для еды
            food.generate();
        }

        // очищаем экран черным цветом
        SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
        SDL_RenderClear(renderer);

        // рисуем змейку и еду на экране
        snake.render(renderer);
        food.render(renderer);

        // обновляем экран
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
        
    }
    // освобождаем ресурсы и выходим из программы
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}






