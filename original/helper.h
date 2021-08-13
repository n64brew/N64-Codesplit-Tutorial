#ifndef CODESPLIT_HELPER_H
#define CODESPLIT_HELPER_H

    /*********************************
                Functions
    *********************************/

    void rcp_init();
    void fb_clear(u8 r, u8 g, u8 b);
    void draw_sprite32b(void* sprite, int format, int x, int y, int w, int h, int scale);

#endif