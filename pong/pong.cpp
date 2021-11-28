#include <SDL2/SDL.h>
#include <iostream> 

struct paddle {
    float dy;
    SDL_Rect rect;
};

struct ball{
    float dx = 0.5f, dy = 0.001f;
    SDL_FRect rect;
};

enum States{
    start,
    playing
};

ball Ball;
paddle LPaddle, RPaddle;

int score[] = {0, 0};
States state = start;
static SDL_Surface* numbers;
static SDL_Surface* startMenu;
static SDL_Surface* rPaddleWins;
static SDL_Surface* lPaddleWins;
SDL_Texture* numText;
SDL_Texture* startMenuText;
SDL_Texture* rPaddleWinsText;
SDL_Texture* lPaddleWinsText;

void DrawMenus(SDL_Renderer &renderer, SDL_Texture &text, int x, int y, int w, int h)
{
    SDL_Rect srcR, destR;

    srcR.x = 0;
    srcR.y = 0;
    srcR.w = w;
    srcR.h = h;

    destR.x = x;
    destR.y = y;
    destR.w = w;
    destR.h = h;

    SDL_RenderCopy(&renderer, &text, &srcR, &destR);
}

void DrawLeftScore(SDL_Renderer &renderer)
{
    SDL_Rect src, dest;

    src.x = score[0] * 32;
    src.y = 0;
    src.h = 32;
    src.w = 32;

    dest.x = (800 / 2) - 80;
    dest.y = 0;
    dest.h = 64;
    dest.w = 64;

    SDL_RenderCopy(&renderer, numText, &src, &dest);
}

void DrawRightScore(SDL_Renderer &renderer)
{
    SDL_Rect src, dest;

    src.x = score[1] * 32;
    src.y = 0;
    src.h = 32;
    src.w = 32;

    dest.x = (800 / 2) + 28;
    dest.y = 0;
    dest.h = 64;
    dest.w = 64;

    SDL_RenderCopy(&renderer, numText, &src, &dest);
}

bool CheckCollision(SDL_Rect pRect)
{
    int pLeftSide = pRect.x;
	int pRightSide = pRect.x + pRect.w;
	int pTopSide = pRect.y;
	int pBottomSide = pRect.y + pRect.h;

	if (pLeftSide > Ball.rect.x + Ball.rect.w ||
        pRightSide < Ball.rect.x ||
        pTopSide > Ball.rect.y + Ball.rect.h ||
        pBottomSide < Ball.rect.y) {
		return false;
	}
    else 
        return true;
}

void BallMovement(float deltaTime)
{
    Ball.rect.x += Ball.dx * deltaTime;
    Ball.rect.y += Ball.dy * deltaTime;
    
    bool collidingW1 = CheckCollision(LPaddle.rect);
    if (collidingW1)
    {
        Ball.dx = .5f;        
        if (Ball.dy < 0)
            Ball.dy = -(float(rand())/float((RAND_MAX)) * 0.5f);
        else if (Ball.dy > 0)
            Ball.dy = (float(rand())/float((RAND_MAX)) * 0.5f);
    }
    bool collidingW2 = CheckCollision(RPaddle.rect);
    if (collidingW2)
    {
        Ball.dx = -.5f;
        if (Ball.dy < 0)
            Ball.dy = -(float(rand())/float((RAND_MAX)) * 0.5f);
        else if (Ball.dy > 0)
            Ball.dy = (float(rand())/float((RAND_MAX)) * 0.5f);
    }

    if (Ball.rect.y < 0)
    {
        Ball.rect.y = 0;
        Ball.dy = -Ball.dy;
    }

    if (Ball.rect.y > 500 - 20)
    {
        Ball.rect.y = 500 - 20;
        Ball.dy = -Ball.dy;
    }

    if (Ball.rect.x > 800)
    {
        Ball.dy = 0.001f;
        int randDir = rand() % 2;
        if (randDir == 0)
            Ball.dx = 0.5f;
        else if (randDir == 1)
            Ball.dx = -0.5f;
        Ball.rect.y = 250;
        Ball.rect.x = 397;
        LPaddle.rect.y = 500 / 2 - 75;
        RPaddle.rect.y = 500 / 2 - 75;
        score[0]++;
        if (score[0] >= 10)
            state = start;
    }

    if(Ball.rect.x < 0)
    {
        Ball.dy = 0.001f;
        int randDir = rand() % 2;
        if (randDir == 0)
            Ball.dx = 0.5f;
        else if (randDir == 1)
            Ball.dx = -0.5f;
        Ball.rect.y = 250;
        Ball.rect.x = 397;
        LPaddle.rect.y = 500 / 2 - 75;
        RPaddle.rect.y = 500 / 2 - 75;
        score[1]++;
        if (score[1] >= 10)
            state = start;
    }
}

int main(int argv, char** args)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Could not initialize SDL" << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 500, 0);
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

    LPaddle.rect.x = 30;
    LPaddle.rect.y = 500 / 2 - 75;
    LPaddle.rect.h = 150;
    LPaddle.rect.w = 30;

    RPaddle.rect.x = 740;
    RPaddle.rect.y = 500 / 2 - 75;
    RPaddle.rect.h = 150;
    RPaddle.rect.w = 30;

    Ball.rect.x = 397;
    Ball.rect.y = 250;
    Ball.rect.h = 20;
    Ball.rect.w = 20;

    numbers = SDL_LoadBMP("pong/numbers.bmp");
    startMenu = SDL_LoadBMP("pong/StartMenu.bmp");
    lPaddleWins = SDL_LoadBMP("pong/LeftPlayerWins.bmp");
    rPaddleWins = SDL_LoadBMP("pong/RightPlayerWins.bmp");

    if (!numbers || !startMenu || !lPaddleWins || !rPaddleWins)
    {
        std::cout << "Couldnt load bmp";
    }

    numText = SDL_CreateTextureFromSurface(renderer, numbers);
    startMenuText = SDL_CreateTextureFromSurface(renderer, startMenu);
    lPaddleWinsText = SDL_CreateTextureFromSurface(renderer, lPaddleWins);
    rPaddleWinsText = SDL_CreateTextureFromSurface(renderer, rPaddleWins);

    SDL_FreeSurface(numbers);
    SDL_FreeSurface(startMenu);
    SDL_FreeSurface(lPaddleWins);
    SDL_FreeSurface(rPaddleWins);

    uint32_t elapsedTime = 0;
    uint32_t lastFrameTimeElapsed = 0;
    float deltaTime = 0.0f;
    float dTimeAsMilliseconds = 0.0f;

    SDL_Event event;
    bool running = true;
    while (running)
    {
        elapsedTime = SDL_GetTicks();    
        
        // Process Input
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        const uint8_t* currentKeyStates = SDL_GetKeyboardState(NULL);
        switch (state)
        {
            case start:
                if (currentKeyStates[SDL_SCANCODE_SPACE])
                {
                    score[0] = 0;
                    score[1] = 0;
                    state = playing;
                    break;
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                if (score[0] != 0 || score[1] != 0)
                {
                    //Show win menu
                    if (score[0] == 10)
                    {
                        // Show left paddle wins
                        DrawMenus(*renderer, *lPaddleWinsText, 273, 204, 255, 46);
                    }
                    else if (score[1] == 10)
                    {
                        // show right paddle wins
                        DrawMenus(*renderer, *rPaddleWinsText, 261, 204, 279, 46);
                    }
                }
                else 
                {
                    // Show start menu
                    DrawMenus(*renderer, *startMenuText, 244, 234, 312, 21);
                }

                SDL_RenderPresent(renderer);
                break;
            case playing:
                if (currentKeyStates[SDL_SCANCODE_W])
                {
                    if (LPaddle.rect.y > 0)
                        LPaddle.dy = -1 * dTimeAsMilliseconds;
                    else
                        LPaddle.rect.y = 0;
                }
                else if (currentKeyStates[SDL_SCANCODE_S])
                {
                    if (LPaddle.rect.y < 500 - 150)
                        LPaddle.dy = 1 * dTimeAsMilliseconds;
                    else
                        LPaddle.rect.y = 500 - 150;
                }
                else
                {
                    LPaddle.dy = 0;
                }

                if (currentKeyStates[SDL_SCANCODE_UP])
                {
                    if (RPaddle.rect.y > 0)
                        RPaddle.dy = -1 * dTimeAsMilliseconds;
                    else
                        RPaddle.rect.y = 0;
                }
                else if (currentKeyStates[SDL_SCANCODE_DOWN])
                {
                    if (RPaddle.rect.y < 500 - 150)
                        RPaddle.dy = 1 * dTimeAsMilliseconds;
                    else
                        RPaddle.rect.y = 500 - 150;
                }
                else
                {
                    RPaddle.dy = 0;
                }

                //Update everything like paddle positions, and ball
                RPaddle.rect.y += RPaddle.dy;
                LPaddle.rect.y += LPaddle.dy;

                BallMovement(dTimeAsMilliseconds);

                // Render everything
                // Set background color to black
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                // Clear the screen
                SDL_RenderClear(renderer);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                for (int i = 0; i < 20; i++)
                {
                    SDL_Rect lineRect;
                    lineRect.x = 400;
                    lineRect.y = i * 25;
                    lineRect.h = 15;
                    lineRect.w = 15;
                    SDL_RenderFillRect(renderer, &lineRect);
                }

                DrawRightScore(*renderer);
                DrawLeftScore(*renderer);

                SDL_RenderFillRect(renderer, &LPaddle.rect);
                SDL_RenderFillRect(renderer, &RPaddle.rect);
                SDL_RenderFillRectF(renderer, &Ball.rect);

                // Swap the front and back buffer
                SDL_RenderPresent(renderer);
                break;
        }


        deltaTime = (elapsedTime - lastFrameTimeElapsed) / 1000.0f;
        dTimeAsMilliseconds = deltaTime * 1000.0f;
        
        lastFrameTimeElapsed = elapsedTime;      
    }

    // Cleanup 
    SDL_DestroyTexture(numText);
    SDL_DestroyTexture(startMenuText);
    SDL_DestroyTexture(lPaddleWinsText);
    SDL_DestroyTexture(rPaddleWinsText);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}