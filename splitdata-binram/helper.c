/***************************************************************
                           helper.c
                               
Contains some convenience functions
***************************************************************/

#include <nusys.h>
#include "config.h"


/*==============================
    rcp_init
    Initializes the RCP before drawing anything.
==============================*/

void rcp_init()
{
    // Set the segment register
    gSPSegment(glistp++, 0, 0);
    
    // Initialize the RSP and RDP with a display list
    gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(rspinit_dl));
    gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(rdpinit_dl));
}


/*==============================
    fb_clear
    Initializes the framebuffer and Z-buffer
    @param The red value
    @param The green value
    @param The blue value
==============================*/

void fb_clear(u8 r, u8 g, u8 b)
{
    // Clear the Z-buffer
    gDPSetDepthImage(glistp++, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
    gDPSetCycleType(glistp++, G_CYC_FILL);
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b,SCREEN_WD, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
    gDPSetFillColor(glistp++,(GPACK_ZDZ(G_MAXFBZ,0) << 16 | GPACK_ZDZ(G_MAXFBZ,0)));
    gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
    gDPPipeSync(glistp++);

    // Clear the framebuffer
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, osVirtualToPhysical(nuGfxCfb_ptr));
    gDPSetFillColor(glistp++, (GPACK_RGBA5551(r, g, b, 1) << 16 | 
                               GPACK_RGBA5551(r, g, b, 1)));
    gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
    gDPPipeSync(glistp++);
}


/*==============================
    draw_sprite
    Draws a sprite on the screen
    @param A pointer to the sprite
    @param The image format of the sprite
    @param The X coordinate to draw the sprite on
    @param The Y coordinate to draw the sprite on
    @param The sprite width 
    @param The sprite height
    @param The scale to apply to the sprite
==============================*/

void draw_sprite32b(void* sprite, int format, int x, int y, int w, int h, int scale)
{
    gDPSetCycleType(glistp++, G_CYC_1CYCLE);
    gDPSetCombineMode(glistp++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    gDPSetRenderMode(glistp++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE);
    gDPSetTexturePersp(glistp++, G_TP_NONE);
    gDPLoadTextureBlock(glistp++, sprite, format, G_IM_SIZ_32b, w, h, 0, G_TX_CLAMP, G_TX_CLAMP, sqrtf((float)w), sqrtf((float)h), G_TX_NOLOD, G_TX_NOLOD);
    gSPTextureRectangle(glistp++, (x-w*scale/2)<<2, (y-h*scale/2)<<2, (x+w*scale/2)<<2, (y+h*scale/2)<<2, G_TX_RENDERTILE, 0 << 5, 0 << 5, (1 << 10) / scale + 0.5, (1 << 10) / scale + 0.5);
    gDPPipeSync(glistp++);
}