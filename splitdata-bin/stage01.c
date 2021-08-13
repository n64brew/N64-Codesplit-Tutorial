/***************************************************************
                           stage01.c
                               
Handles the second level of the game.
***************************************************************/

#include <nusys.h>
#include "config.h"
#include "helper.h"
#include "debug.h"
#include "assets.h"


/*==============================
    stage01_init
    Initialize the stage
==============================*/

void stage01_init(void)
{
    u32 seg_start = (u32)_spr_burgerSegmentRomStart;
    u32 seg_size = _spr_burgerSegmentRomEnd - _spr_burgerSegmentRomStart;

    // Read the texture from ROM
    debug_printf("Reading sprite from ROM location 0x%08x, size %d bytes\n", seg_start, seg_size);
    nuPiReadRom(seg_start, texture, seg_size);
    
    // Finish initializing
    debug_printf("Initialized stage 01\n");
}


/*==============================
    stage01_update1
    Update stage variables every frame
==============================*/

void stage01_update(void)
{
    // Read the controller
    nuContDataGetEx(&controller, 0);
    
    // If we pressed A, change to stage 0
    if (controller.trigger & A_BUTTON)
        global_stage = 0;
}


/*==============================
    stage01_draw
    Draw the stage
==============================*/

void stage01_draw(void)
{
    // Assign our glist pointer to our glist array for ease of access
    glistp = glist;

    // Initialize the RCP and framebuffer
    rcp_init();
    fb_clear(255, 0, 0);
    
    // Draw our burger sprite
    draw_sprite32b(texture, G_IM_FMT_RGBA, SCREEN_WD/2, SCREEN_HT/2, 32, 32, 3);
    
    // Syncronize the RCP + CPU and specify that our display list has ended
    gDPFullSync(glistp++);
    gSPEndDisplayList(glistp++);
    
    // Ensure we haven't gone over the display list size and start the graphics task
    debug_assert((glistp-glist) < GLIST_LENGTH);
    nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx), NU_GFX_UCODE_F3DEX, NU_SC_SWAPBUFFER);
}