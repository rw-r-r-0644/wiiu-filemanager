#pragma once

#include "resources/Resources.h"
#include "gui/Gui.h"
#include "gui/GuiFrame.h"
#include "gui/GuiDragListener.h"
#include "gui/GuiButton.h"
#include "gui/GuiSound.h"
#include "fs/DirList.h"

// Structure used to store a filesystem entry viewer
typedef struct
{
	std::string * name;
	bool isdir;
	GuiText * text;
	GuiImage * image;
	GuiImage * icon;
	GuiButton * button;
} FileListEntry_t;

class FileListViewer : public GuiFrame, public sigslot::has_slots<>
{
public:
	FileListViewer(s32 w, s32 h);
	virtual ~FileListViewer();
	
	// This functions are called by other elements so they must be public
	void OnFileEntryPressed(GuiButton *button, const GuiController *controller, GuiTrigger *trigger);
	void OnFileEntryPressedLong(GuiButton *button, const GuiController *controller, GuiTrigger *trigger);
	void OnDrag(GuiDragListener * listener, const GuiController * controller, GuiTrigger * trigger, s32 dx, s32 dy);
	
	void AddItem(std::string Name, bool IsDir);
	void ClearItems();
	
	void SetEnable(s32 enabled);
	
	// Whether this filelist is enabled
	s32 StateEnabled = 1;
	
	sigslot::signal2<std::string, bool> FileEntryClick;
	sigslot::signal2<std::string, bool> FileEntrySelect;
	
private:
	// The visible file entries
	std::list<FileListEntry_t *> ListElements;
	s32 CountElements = 0;
	
	// Image datas
	GuiImageData* ic_bg_div;
	GuiImageData* ic_folder;
	GuiImageData* ic_file;
	GuiImageData* ic_selected;

	// Triggers and drags
	GuiTrigger touchTrigger;
	GuiTrigger wpadTouchTrigger;
	GuiDragListener VPADDrag;
	
	// Button Click Sound
	GuiSound *buttonClickSound;
};