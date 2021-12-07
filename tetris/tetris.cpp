#include <SDL2/SDL.h>
#include <iostream>

const uint16_t SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;
SDL_Rect boardRect;

void update(double deltaTime)
{

}

void render(SDL_Renderer &renderer)
{
    SDL_SetRenderDrawColor(&renderer, 0, 0, 0, 255);
    SDL_RenderClear(&renderer);

    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            boardRect.x = (SCREEN_WIDTH / 2 - 100) + (boardRect.w * j);
            boardRect.y = (SCREEN_HEIGHT / 2 - 200) + (boardRect.h * i);
            SDL_SetRenderDrawColor(&renderer, 0, 155, 0, 255);
            SDL_RenderDrawRect(&renderer, &boardRect);
        }
    }

    SDL_RenderPresent(&renderer);
}

int main(int argv, char** args)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Could not initialize SDL" << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window)
    {
        std::cout << "Could not create window " << SDL_GetError() << std::endl;  
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        std::cout << "Could not create renderer " << SDL_GetError() << std::endl;  
        return -1;
    }

    boardRect.w = 20;
    boardRect.h = 20;

    double elapsedTime = SDL_GetTicks(), 
    previousElapsedTime = 0, 
    deltaTime = 0.0f;

    SDL_Event event;
    bool running = true;
    while(running)
    {
        elapsedTime = SDL_GetTicks();
        deltaTime = elapsedTime - previousElapsedTime;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }
        // Process inputs



        // End of processing inputs

        update(deltaTime);
        render(*renderer);

        previousElapsedTime = elapsedTime;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}