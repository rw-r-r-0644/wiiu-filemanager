#pragma once

#include "resources/Resources.h"
#include "gui/Gui.h"
#include "gui/GuiFrame.h"
#include "gui/GuiButton.h"
#include "fs/DirList.h"

#include "FileListViewer.h"

class PathBar : public GuiFrame
{
public:
	PathBar(s32 w, s32 h);
	virtual ~PathBar();
	void Refresh(std::string newPath);
private:	
	// Image datas
	GuiImageData* ic_top_bar;
	GuiImage TopBar;
	
	// Path
	GuiText PathText;
};