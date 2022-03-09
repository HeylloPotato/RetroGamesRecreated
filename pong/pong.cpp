#include <SDL2/SDL.h>
#include <iostream> 

// define what a paddle contains
struct paddle {
    float dy;
    SDL_Rect rect;
};

// define what a ball contains
// aswell as it's default parameters
struct ball{
    float dx = 0.5f, dy = 0.001f;
    SDL_FRect rect;
};

// define what the states of the game are
enum States{
    start,
    playing
};

// Create a ball and 2 paddle objects
ball Ball;
paddle LPaddle, RPaddle;

// Set score to 0 for each side
// Set to main menu/ start state
int score[] = {0, 0};
States state = start;

// create the surface objects for all of the game
// create a empty texture objects 
static SDL_Surface* numbers;
static SDL_Surface* startMenu;
static SDL_Surface* rPaddleWins;
static SDL_Surface* lPaddleWins;
SDL_Texture* numText;
SDL_Texture* startMenuText;
SDL_Texture* rPaddleWinsText;
SDL_Texture* lPaddleWinsText;

// Draw all the menus depending on state, and who wins
void DrawMenus(SDL_Renderer &renderer, SDL_Texture &text, int x, int y, int w, int h)
{
    SDL_Rect srcR, destR;

    // set the origin of the rect 
    srcR.x = 0;
    srcR.y = 0;
    srcR.w = w;
    srcR.h = h;

    // set where the rect is on the screen, and size
    destR.x = x;
    destR.y = y;
    destR.w = w;
    destR.h = h;

    // Render the rect @ position with the inputted renderer and texture
    // from the function
    SDL_RenderCopy(&renderer, &text, &srcR, &destR);
}

void DrawLeftScore(SDL_Renderer &renderer)
{
    SDL_Rect src, dest;

    // Set the origin of the x axis on the number.bmp based on score
    // This sets what number of the score
    src.x = score[0] * 32; // width
    src.y = 0;
    src.h = 32;
    src.w = 32;

    // Set the position of the text 
    dest.x = (800 / 2) - 80;
    dest.y = 0;
    dest.h = 64;
    dest.w = 64;

    SDL_RenderCopy(&renderer, numText, &src, &dest);
}

void DrawRightScore(SDL_Renderer &renderer)
{
    SDL_Rect src, dest;

    // Same as drawing left score
    //however using what the right paddles score is
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

// check for AABB Collision
bool CheckCollision(SDL_Rect pRect)
{
    // simulate a hitbox based on the paddle sizes
    int pLeftSide = pRect.x;
	int pRightSide = pRect.x + pRect.w;
	int pTopSide = pRect.y;
	int pBottomSide = pRect.y + pRect.h;

    // check if the ball is inside the simulated hitbox
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
    
    // Check if its colliding with left paddle
    bool collidingW1 = CheckCollision(LPaddle.rect);
    if (collidingW1)
    {
        // change direction the ball is moving on the x-axis
        Ball.dx = .5f;   
        // Random change the angle it's going at
        // Go down if the ball was previously going down, or vice versa
        // This was to make it not seem completely random     
        if (Ball.dy < 0)
            Ball.dy = -(float(rand())/float((RAND_MAX)) * 0.5f);
        else if (Ball.dy > 0)
            Ball.dy = (float(rand())/float((RAND_MAX)) * 0.5f);
    }
    // Same as checking for colliding with left paddle
    // but checking for the right and opposing directions
    bool collidingW2 = CheckCollision(RPaddle.rect);
    if (collidingW2)
    {
        Ball.dx = -.5f;
        if (Ball.dy < 0)
            Ball.dy = -(float(rand())/float((RAND_MAX)) * 0.5f);
        else if (Ball.dy > 0)
            Ball.dy = (float(rand())/float((RAND_MAX)) * 0.5f);
    }

    // If the ball hits the ceiling swap y-axis direction
    if (Ball.rect.y < 0)
    {
        Ball.rect.y = 0;
        Ball.dy = -Ball.dy;
    }

    // If the ball hits the floor swap y-axis direction
    if (Ball.rect.y > 500 - 20)
    {
        Ball.rect.y = 500 - 20;
        Ball.dy = -Ball.dy;
    }

    // If the ball goes off the right side
    // reset the game, set a random direction for the ball to start
    // +1 to the left sides score
    // Start the game
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

    // If the ball goes off the left side
    // reset the game, set a random direction for the ball to start
    // +1 to the right sides score
    // Start the game
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

    // intialize SDL
    // Create a window - check if window was made
    // Create a renderer with window - check if renderer was made
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

    // Set the paddles positions, and size
    LPaddle.rect.x = 30;
    LPaddle.rect.y = 500 / 2 - 75;
    LPaddle.rect.h = 150;
    LPaddle.rect.w = 30;

    RPaddle.rect.x = 740;
    RPaddle.rect.y = 500 / 2 - 75;
    RPaddle.rect.h = 150;
    RPaddle.rect.w = 30;

    // Set the balls position and size
    Ball.rect.x = 397;
    Ball.rect.y = 250;
    Ball.rect.h = 20;
    Ball.rect.w = 20;

    // Assign the previously initialized surfaces with the images
    // Then check if they were loaded 
    numbers = SDL_LoadBMP("pong/pongAssets/numbers.bmp");
    startMenu = SDL_LoadBMP("pong/pongAssets/StartMenu.bmp");
    lPaddleWins = SDL_LoadBMP("pong/pongAssets/LeftPlayerWins.bmp");
    rPaddleWins = SDL_LoadBMP("pong/pongAssets/RightPlayerWins.bmp");

    if (!numbers || !startMenu || !lPaddleWins || !rPaddleWins)
    {
        std::cout << "Couldnt load bmp\n";
    }

    // Create textures with the surfaces, that have the images in them
    numText = SDL_CreateTextureFromSurface(renderer, numbers);
    startMenuText = SDL_CreateTextureFromSurface(renderer, startMenu);
    lPaddleWinsText = SDL_CreateTextureFromSurface(renderer, lPaddleWins);
    rPaddleWinsText = SDL_CreateTextureFromSurface(renderer, rPaddleWins);

    // Free the surfaces, to free resources that are no longer needed
    SDL_FreeSurface(numbers);
    SDL_FreeSurface(startMenu);
    SDL_FreeSurface(lPaddleWins);
    SDL_FreeSurface(rPaddleWins);

    uint32_t elapsedTime = 0;
    uint32_t lastFrameTimeElapsed = 0;
    float deltaTime = 0.0f;
    float dTimeAsMilliseconds = 0.0f;

    // Main runnning loop, runs till program is stopped
    SDL_Event event;
    bool running = true;
    while (running)
    {
        // Number of milliseconds time has elapsed since start of the program
        elapsedTime = SDL_GetTicks();    
        
        while (SDL_PollEvent(&event))
        {
            // check if you quit out of the program
            // Then stop running the loop, and close
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        // Get the state of the keyboard keys
        const uint8_t* currentKeyStates = SDL_GetKeyboardState(NULL);
        // What to do during the game states
        switch (state)
        {
            // This is for the start state
            case start:
                // start playing the game if space is pressed
                if (currentKeyStates[SDL_SCANCODE_SPACE])
                {
                    score[0] = 0;
                    score[1] = 0;
                    state = playing;
                    break;
                }

                // Put black background in back buffer
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                // if there is a score when in start state
                // this only happens when one of the player wins
                // so check who wins, and display the winner
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
                // game hasn't been played
                else 
                {
                    // show first time start up menu
                    DrawMenus(*renderer, *startMenuText, 244, 234, 312, 21);
                }

                // render everything by swapping buffer
                SDL_RenderPresent(renderer);
                break;
            case playing:
                // Left paddle input, check for W/S
                // Go up if W is pressed; vice versa for S
                // otherwise don't move
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

                // Input for right paddle
                // Up arrow for up; Down arrow for down
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

                // change the left and right paddles y-axis positions
                RPaddle.rect.y += RPaddle.dy;
                LPaddle.rect.y += LPaddle.dy;

                BallMovement(dTimeAsMilliseconds);

                // Render everything
                // Set background color to black
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                // Clear the screen
                SDL_RenderClear(renderer);
                // make the background black
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                // Lines in the middle of the screen
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

                // Swap the front and back buffer; render everything
                SDL_RenderPresent(renderer);
                break;
        }

        // calculate delta time 
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