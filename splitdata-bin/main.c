/***************************************************************
                            main.c
                               
Program entrypoint.
***************************************************************/

#include <nusys.h>
#include "config.h"
#include "stages.h"
#include "assets.h"


/*********************************
        Function Prototypes
*********************************/

static void callback_stage00(int callback_vsync);
static void callback_stage01(int callback_vsync);
static void callback_prenmi(void);


/*********************************
             Globals
*********************************/

volatile int global_stage;
NUContData   controller;
char         texture[4096];


/*==============================
    mainproc
    Initializes the game
==============================*/

void mainproc(void * dummy)
{
    // Start by selecting the proper television
    switch(TV_TYPE)
    {
        case PAL:
            osViSetMode(&osViModeTable[OS_VI_FPAL_LAN1]);
            osViSetYScale(0.833);
            nuPreNMIFuncSet((NUScPreNMIFunc)callback_prenmi);
            break;
        case MPAL:
            osViSetMode(&osViModeTable[OS_VI_MPAL_LAN1]);
            break;
        default:
            break;
    }

    // Initialize the game
    nuGfxInit();
    nuContInit();
    debug_initialize();
    global_stage = 0;
    
    // Create a callback function for when the reset button is pressed
    nuPreNMIFuncSet((NUScPreNMIFunc)callback_prenmi);
    
    // Loop forever to keep the idle thread busy
    while(1)
    {
        // If we had a stage change, then initialize it and set the callback function
        switch (global_stage)
        {
            case 0:
                stage00_init();
                nuGfxFuncSet((NUGfxFunc)callback_stage00);
                break;
            case 1:
                stage01_init();
                nuGfxFuncSet((NUGfxFunc)callback_stage01);
                break;
            default:
                break;
        }
        
        // Turn on the screen and reset the stage variable
        nuGfxDisplayOn();
        global_stage = -1;
        
        // Spin until the stage changes
        while (global_stage == -1)
            ;
                   
        // Turn off the screen and print that we're changing stage
        debug_printf("Changing stage to %02d\n", global_stage);
        nuGfxDisplayOff();
    }
}


/*==============================
    callback_stage00
    Code that runs on the graphics thread for stage 00
    @param The number of tasks left to execute
==============================*/

void callback_stage00(int callback_vsync)
{
    // Update the stage
    stage00_update();

    // If the RCP ran out of drawing tasks, redraw the stage
    if (callback_vsync < 1)
        stage00_draw();
}


/*==============================
    callback_stage01
    Code that runs on the graphics thread for stage 01
    @param The number of tasks left to execute
==============================*/

void callback_stage01(int callback_vsync)
{
    // Update the stage
    stage01_update();

    // If the RCP ran out of drawing tasks, redraw the stage
    if (callback_vsync < 1)
        stage01_draw();
}


/*==============================
    callback_prenmi
    Code that runs when the reset button
    is pressed. Exists to prevent crashing 
    when the reset button is pressed on PAL 
    systems.
==============================*/

void callback_prenmi(void)
{
    nuGfxDisplayOff();
    osViSetYScale(1);
}