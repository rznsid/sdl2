//Using SDL and standard IO
#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Key press surface constants
enum KeyPressTextures
{
    KEY_PRESS_TEXTURE_DEFAULT,
    KEY_PRESS_TEXTURE_UP,
    KEY_PRESS_TEXTURE_LEFT,
    KEY_PRESS_TEXTURE_RIGHT,
    KEY_PRESS_TEXTURE_DOWN,
    KEY_PRESS_TEXTURE_TOTAL
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture *loadTexture(std::string path);

//The window we'll be rendering to
SDL_Window *gWindow = NULL;

//The window renderer
SDL_Renderer *gRenderer = NULL;

//The images that correspond to a keypress
SDL_Texture *gKeyPressTextures[KEY_PRESS_TEXTURE_TOTAL];

//Current displayed render
SDL_Texture *gTexture = NULL;

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                //Intialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initalize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not intialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load default texture
    gKeyPressTextures[KEY_PRESS_TEXTURE_DEFAULT] = loadTexture("res/default.png");
    if (gKeyPressTextures[KEY_PRESS_TEXTURE_DEFAULT] == NULL)
    {
        printf("Failed to load default image!\n");
        success = false;
    }

    //Load up texture
    gKeyPressTextures[KEY_PRESS_TEXTURE_UP] = loadTexture("res/up.png");
    if (gKeyPressTextures[KEY_PRESS_TEXTURE_UP] == NULL)
    {
        printf("Failed to load up image!\n");
        success = false;
    }

    //Load down texture
    gKeyPressTextures[KEY_PRESS_TEXTURE_DOWN] = loadTexture("res/down.png");
    if (gKeyPressTextures[KEY_PRESS_TEXTURE_DOWN] == NULL)
    {
        printf("Failed to load down image!\n");
        success = false;
    }

    //Load left texture
    gKeyPressTextures[KEY_PRESS_TEXTURE_LEFT] = loadTexture("res/left.png");
    if (gKeyPressTextures[KEY_PRESS_TEXTURE_LEFT] == NULL)
    {
        printf("Failed to load left image!\n");
        success = false;
    }

    //Load right texture
    gKeyPressTextures[KEY_PRESS_TEXTURE_RIGHT] = loadTexture("res/right.png");
    if (gKeyPressTextures[KEY_PRESS_TEXTURE_RIGHT] == NULL)
    {
        printf("Failed to load right image!\n");
        success = false;
    }

    return success;
}

void close()
{
    //Destroy texture
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

    //Destroy renderer
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    //Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    //Quit SDL subsystem
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture *loadTexture(std::string path)
{
    //The final texture
    SDL_Texture *newTexture = NULL;

    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

int main(int argc, char *args[])
{
    //Start up SDL and created window
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        //Load media
        if (!loadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //Set default current surface
            gTexture = gKeyPressTextures[KEY_PRESS_TEXTURE_DEFAULT];

            //While application is running
            while (!quit)
            {
                //Handle events on queue
                while (SDL_PollEvent(&e) != 0)
                {
                    //User requests quite
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    //User presses a key
                    else if (e.type == SDL_KEYDOWN)
                    {
                        //Select surface based on keypress
                        switch (e.key.keysym.sym)
                        {
                        case SDLK_UP:
                            gTexture = gKeyPressTextures[KEY_PRESS_TEXTURE_UP];
                            break;

                        case SDLK_DOWN:
                            gTexture = gKeyPressTextures[KEY_PRESS_TEXTURE_DOWN];
                            break;

                        case SDLK_LEFT:
                            gTexture = gKeyPressTextures[KEY_PRESS_TEXTURE_LEFT];
                            break;

                        case SDLK_RIGHT:
                            gTexture = gKeyPressTextures[KEY_PRESS_TEXTURE_RIGHT];
                            break;

                        default:
                            gTexture = gKeyPressTextures[KEY_PRESS_TEXTURE_DEFAULT];
                            break;
                        }
                    }
                }

                //Clear screen
                SDL_RenderClear(gRenderer);

                //Render texture to screen
                SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

                //Update the surface
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}