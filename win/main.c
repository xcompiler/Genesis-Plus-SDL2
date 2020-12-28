#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include "shared.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define WINDOW_X_POSITION 0
#define WINDOW_Y_POSITION 0

int timer_count = 0;
int old_timer_count = 0;
int paused = 0;
int frame_count = 0;

int update_input(void);
const Uint8 *keystate;
int key_count;

SDL_Window *window;

Uint32 fps_callback(Uint32 interval, void *param)
{
	if(paused) return 1000/60;
	timer_count++;
	if(timer_count % 60 == 0)
	{
		int fps = frame_count;
		char caption[32];
                sprintf(caption, "Genesis Plus/SDL    FPS=%d", fps);
                SDL_SetWindowTitle(window, caption);
		frame_count = 0;
	}
	return 1000/60;
}


int main (int argc, char **argv)
{
	int running = 1;

    SDL_Rect viewport, src;
    SDL_Surface *bmp, *screen;
    SDL_Event event;

    error_init();

    /* Print help if no game specified */
	if(argc < 2)
	{
		char caption[256];
                sprintf(caption, "Genesis Plus\nby Charles MacDonald\nWWW: http://cgfm2.emuviews.com\nusage: %s gamename\n", argv[0]);
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Information", caption, NULL);
                printf("%s\n", caption); /* print console output as well, in case the message box fails to display -- e.g. on LXDE platforms */
		exit(1);
	}

	/* Load game */
    if(!load_rom(argv[1]))
	{
		char caption[256];
		sprintf(caption, "Error loading file `%s'.", argv[1]);
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Information", caption, NULL);
                printf("%s\n", caption); /* print console output as well, in case the message box fails to display -- e.g. on LXDE platforms */
		exit(1);
	}
        
	viewport.x = 0;
	viewport.y = 0;
	viewport.w = 256;
	viewport.h = 224;

	src.x = 32;
	src.y = 0;
	src.w = viewport.w;
	src.h = viewport.h;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        exit(1);
    }

    window = SDL_CreateWindow("Genesis Plus/SDL", WINDOW_X_POSITION, WINDOW_Y_POSITION, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    viewport.x = 0;
    viewport.y = 0;


    /* The SDL2 documentation states that the first argument passed to SDL_CreateRGBSurface should be 0, and not SDL_SWSURFACE or anything else */
    bmp = SDL_CreateRGBSurface(0, 1024, 512, 16, 0xF800, 0x07E0, 0x001F, 0x0000);



    memset(&bitmap, 0, sizeof(t_bitmap));
    bitmap.width  = 1024;
    bitmap.height = 512;
    bitmap.depth  = 16;
    bitmap.granularity = 2;
    bitmap.pitch = (bitmap.width * bitmap.granularity);
    bitmap.data   = (unsigned char *)bmp->pixels;
    bitmap.viewport.w = 256;
    bitmap.viewport.h = 224;
    bitmap.viewport.x = 0x20;
    bitmap.viewport.y = 0x00;
    bitmap.remap = 1;

    system_init();
    system_reset();

    SDL_AddTimer(1000/60, fps_callback, 0);

	while(running)
	{

		SDL_Delay(1); /* Prevent the program from running too quickly, and also save CPU usage; this value can be adjusted depending on the CPU speed */

		running = update_input();

		while (SDL_PollEvent(&event)) 
		{
			switch(event.type) 
			{
				case SDL_QUIT: /* Windows was closed */
					running = 0;
					break;

				/* SDL2 doesn't seem to recognise the SDL_ACTIVEEVENT flag */

				/*
				case SDL_ACTIVEEVENT: // Window focus changed or was minimized
					if(event.active.state & (SDL_APPINPUTFOCUS | SDL_APPACTIVE))
					{
						paused = !event.active.gain;
					}
					break;
				*/

				default:
					break;
			}
		}

		if(!paused)
		{
			frame_count++;

            update_input();

            if(!system_frame(0))
                system_reset();

            if(bitmap.viewport.changed)
            {
                bitmap.viewport.changed = 0;
                src.w = bitmap.viewport.w;
                src.h = bitmap.viewport.h;
                viewport.w = bitmap.viewport.w;
                viewport.h = bitmap.viewport.h;
                /*
                SDL_DestroyWindow(window);
                window = SDL_CreateWindow("Genesis Plus/SDL", WINDOW_X_POSITION, WINDOW_Y_POSITION, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
                */
                screen = SDL_GetWindowSurface(window);
            }

            SDL_BlitScaled(bmp, &src, screen, NULL);
            SDL_UpdateWindowSurface(window);
        }
    }

    system_shutdown();
    SDL_Quit();
    error_shutdown();

    return 0;
}


/* Check if a key is pressed */
int check_key(int code)
{
    static char lastbuf[0x100] = {0};

    if((!keystate[code]) && (lastbuf[code] == 1))
        lastbuf[code] = 0;

    if((keystate[code]) && (lastbuf[code] == 0))
    {
        lastbuf[code] = 1;
        return (1);
    }                                                                    

    return (0);
}

int update_input(void)
{
    int running = 1;

    keystate = SDL_GetKeyboardState(&key_count);

    memset(&input, 0, sizeof(t_input));
    if(keystate[SDL_SCANCODE_UP])     input.pad[0] |= INPUT_UP;
    else
    if(keystate[SDL_SCANCODE_DOWN])   input.pad[0] |= INPUT_DOWN;
    if(keystate[SDL_SCANCODE_LEFT])   input.pad[0] |= INPUT_LEFT;
    else
    if(keystate[SDL_SCANCODE_RIGHT])  input.pad[0] |= INPUT_RIGHT;

    if(keystate[SDL_SCANCODE_A])      input.pad[0] |= INPUT_A;
    if(keystate[SDL_SCANCODE_S])      input.pad[0] |= INPUT_B;
    if(keystate[SDL_SCANCODE_D])      input.pad[0] |= INPUT_C;
    if(keystate[SDL_SCANCODE_F])      input.pad[0] |= INPUT_START;
    if(keystate[SDL_SCANCODE_RETURN]) input.pad[0] |= INPUT_START;
    if(keystate[SDL_SCANCODE_Z])      input.pad[0] |= INPUT_X;
    if(keystate[SDL_SCANCODE_X])      input.pad[0] |= INPUT_Y;
    if(keystate[SDL_SCANCODE_C])      input.pad[0] |= INPUT_Z;
    if(keystate[SDL_SCANCODE_V])      input.pad[0] |= INPUT_MODE;
 
    if(keystate[SDL_SCANCODE_TAB])    system_reset();

    if(keystate[SDL_SCANCODE_ESCAPE]) running = 0;
    return (running);
}


