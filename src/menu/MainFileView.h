#pragma once

#include "gui/Gui.h"
#include "gui/GuiMainWindowScreen.h"
#include "gui/GuiParticleImage.h"
#include "FileListViewer.h"
#include "FileOptionsMenu.h"
#include "PathBar.h"

#define DIR_ROOT "sd:/"

class CVideo;
class MainFileView : public GuiMainWindowScreen, public sigslot::has_slots<>
{
public:
    MainFileView(s32 w, s32 h);
    virtual ~MainFileView();
	
	void Refresh();
	void ChangeDir(std::string Dir);
	void OnFileEntryClick(std::string entryName, bool IsDir);
	void OnFileEntrySelect(std::string entryName, bool IsDir);
	void OnFileOptionsMenuClose(int n);
	void OnFileOptionsSelect(std::string FileName, u32 Option);
	
private:
    s32 width, height;
	
	// The background
	GuiImage img_bg_color;
	
	FileListViewer filelist;
	PathBar pathbar;
	
	FileOptionsMenu *file_options_menu;
	
	// Currently browsed directory
	std::string CurrentDirectory = DIR_ROOT;
	
	// Whether a refresh is required
	bool RefreshRequired = true;
	bool CloseOptionsMenu = false;
	
    void process();
};
