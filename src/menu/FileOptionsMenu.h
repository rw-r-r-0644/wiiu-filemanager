#pragma once

#include "resources/Resources.h"
#include "gui/Gui.h"
#include "gui/GuiDragListener.h"
#include "gui/GuiFrame.h"
#include "gui/GuiButton.h"
#include "fs/DirList.h"
#include "gui/sigslot.h"

typedef enum
{
	// Dirs
	DIR_OPEN,
	DIR_COPY,
	DIR_DELETE,
	DIR_MOVE,
	// Files
	FILE_OPEN,
	FILE_COPY,
	FILE_DELETE,
	FILE_MOVE
} FileOperations;

typedef struct
{
	u32 operation;
	std::string * name;
	GuiText * text;
	GuiButton * button;
} FileOption_t;

class FileOptionsMenu : public GuiFrame, public sigslot::has_slots<>
{
public:
	FileOptionsMenu(s32 w, s32 h, std::string fileName, bool isDir);
	virtual ~FileOptionsMenu();
	void update(GuiController * c);
	
	sigslot::signal2<std::string, u32> OptionSelect;
	sigslot::signal1<int> Close;

private:
	s32 waitRelease = 1;

	void AddOption(std::string Name, u32 Operation);
	void OnOptionPressed(GuiButton *button, const GuiController *controller, GuiTrigger *trigger);

	std::list<FileOption_t *> MenuOptions;
	s32 OptionsCount = 0;

	std::string FileName;
	bool IsDir;
	
	// Image datas
	GuiImageData* ic_menu_bg;
	GuiImage MenuBg;
	
	// Path
	GuiText FileNameText;
		
	// Triggers and drags
	GuiTrigger touchTrigger;
	GuiTrigger wpadTouchTrigger;
	GuiDragListener VPADDrag;
	
	// Button Click Sound
	GuiSound *buttonClickSound;
};