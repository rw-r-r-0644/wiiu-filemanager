#include "PathBar.h"

#include <sys/stat.h>
#include "utils/logger.h"

PathBar::PathBar(s32 w, s32 h)
	: GuiFrame(w, h)
	, ic_top_bar(Resources::GetImageData("ic_top_bar.png"))
	, TopBar(ic_top_bar)
	, PathText("sd:/", 32, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
{	
	TopBar.setAlignment(ALIGN_TOP_CENTER);
	TopBar.setPosition(0, 0);
	PathText.setAlignment(ALIGN_TOP_LEFT);
	PathText.setPosition(30, -55);
	append(&TopBar);
	append(&PathText);
}

PathBar::~PathBar()
{
	remove(&TopBar);
	remove(&PathText);
}

void PathBar::Refresh(std::string newPath)
{
	PathText.setText(newPath.c_str());
}