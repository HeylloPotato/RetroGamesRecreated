#include <SDL2/SDL.h>
#include <iostream>

// Set the screen size and board rect
const uint16_t SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;
SDL_Rect boardRect;

// run every frame
void update(double deltaTime)
{

}

// render everything
void render(SDL_Renderer &renderer)
{
    // set background color
    SDL_SetRenderDrawColor(&renderer, 0, 0, 0, 255);
    SDL_RenderClear(&renderer);

    // Make the board i by j large
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            // set origin of the rectangle, offset each one by width and height
            // along which what triangle it is
            boardRect.x = (SCREEN_WIDTH / 2 - 100) + (boardRect.w * j);
            boardRect.y = (SCREEN_HEIGHT / 2 - 200) + (boardRect.h * i);
            //set the color then put it on the back buffer
            SDL_SetRenderDrawColor(&renderer, 0, 155, 0, 255);
            SDL_RenderDrawRect(&renderer, &boardRect);
        }
    }

    // swap forward and back buffer
    SDL_RenderPresent(&renderer);
}

int main(int argv, char** args)
{
    // initiliaze sdl
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Could not initialize SDL" << std::endl;
        return -1;
    }

    // create a window then check if it was created
    SDL_Window* window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window)
    {
        std::cout << "Could not create window " << SDL_GetError() << std::endl;  
        return -1;
    }

    // create a renderer from the window, then check if it was successful 
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        std::cout << "Could not create renderer " << SDL_GetError() << std::endl;  
        return -1;
    }

    // set size of board squares
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
            // check if the program was quit out of
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        update(deltaTime);
        render(*renderer);

        // calculate delta time
        previousElapsedTime = elapsedTime;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}