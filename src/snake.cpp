
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






