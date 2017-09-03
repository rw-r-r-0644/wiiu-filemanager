/****************************************************************************
 * Loadiine resource files.
 * This file is generated automatically.
 * Includes 9 files.
 *
 * NOTE:
 * Any manual modification of this file will be overwriten by the generation.
 ****************************************************************************/
#ifndef _FILELIST_H_
#define _FILELIST_H_

#include <gctypes.h>

typedef struct _RecourceFile
{
	const char *filename;
	const u8   *DefaultFile;
	const u32  &DefaultFileSize;
	u8		   *CustomFile;
	u32		    CustomFileSize;
} RecourceFile;

extern const u8 bgMusic_ogg[];
extern const u32 bgMusic_ogg_size;

extern const u8 button_click_mp3[];
extern const u32 button_click_mp3_size;

extern const u8 font_ttf[];
extern const u32 font_ttf_size;

extern const u8 ic_bg_div_png[];
extern const u32 ic_bg_div_png_size;

extern const u8 ic_file_png[];
extern const u32 ic_file_png_size;

extern const u8 ic_folder_png[];
extern const u32 ic_folder_png_size;

extern const u8 ic_menu_bg_png[];
extern const u32 ic_menu_bg_png_size;

extern const u8 ic_selected_png[];
extern const u32 ic_selected_png_size;

extern const u8 ic_top_bar_png[];
extern const u32 ic_top_bar_png_size;

static RecourceFile RecourceList[] =
{
	{"bgMusic.ogg", bgMusic_ogg, bgMusic_ogg_size, NULL, 0},
	{"button_click.mp3", button_click_mp3, button_click_mp3_size, NULL, 0},
	{"font.ttf", font_ttf, font_ttf_size, NULL, 0},
	{"ic_bg_div.png", ic_bg_div_png, ic_bg_div_png_size, NULL, 0},
	{"ic_file.png", ic_file_png, ic_file_png_size, NULL, 0},
	{"ic_folder.png", ic_folder_png, ic_folder_png_size, NULL, 0},
	{"ic_menu_bg.png", ic_menu_bg_png, ic_menu_bg_png_size, NULL, 0},
	{"ic_selected.png", ic_selected_png, ic_selected_png_size, NULL, 0},
	{"ic_top_bar.png", ic_top_bar_png, ic_top_bar_png_size, NULL, 0},
	{NULL, NULL, 0, NULL, 0}
};

#endif
