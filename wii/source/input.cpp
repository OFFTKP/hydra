#include <grrlib.h>
#include <wiiuse/wpad.h>
#include "menu.h"
#include "player1_point_png.h"
#include "player1_grab_png.h"

static GRRLIB_texImg* cursors[4];
static GRRLIB_texImg* cursors_grab[4];

void InitializeInput()
{
    WPAD_Init();
    cursors[0] = GRRLIB_LoadTexture(player1_point_png);
    cursors_grab[0] = GRRLIB_LoadTexture(player1_grab_png);
    WPAD_SetDataFormat(WPAD_CHAN_ALL,WPAD_FMT_BTNS_ACC_IR);
	WPAD_SetVRes(WPAD_CHAN_ALL, screen_width, screen_height);
}

bool UpdateInput()
{
    WPAD_ScanPads();
    if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) return true;
    auto data = WPAD_Data(0);
    GRRLIB_DrawImg(data->ir.x, data->ir.y, cursors[0], data->ir.angle, 1, 1, 0xFFFFFFFF);

    return false;
}