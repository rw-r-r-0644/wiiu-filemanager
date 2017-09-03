#include "FileOptionsMenu.h"

#include <sys/stat.h>
#include "utils/logger.h"

FileOptionsMenu::FileOptionsMenu(s32 w, s32 h, std::string fileName, bool isDir)
	: GuiFrame(w, h)
	, FileName(fileName)
	, IsDir(isDir)
	, ic_menu_bg(Resources::GetImageData("ic_menu_bg.png"))
	, MenuBg(ic_menu_bg)
	, FileNameText(fileName.c_str(), 32, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
	, touchTrigger(GuiTrigger::CHANNEL_1, GuiTrigger::VPAD_TOUCH)
	, wpadTouchTrigger(GuiTrigger::CHANNEL_2 | GuiTrigger::CHANNEL_3 | GuiTrigger::CHANNEL_4 | GuiTrigger::CHANNEL_5, GuiTrigger::BUTTON_A)
	, VPADDrag(w, h)
	, buttonClickSound(Resources::GetSound("button_click.mp3"))
{	
	MenuBg.setAlignment(ALIGN_CENTERED);
	FileNameText.setAlignment(ALIGN_CENTERED);
	FileNameText.setPosition(0, 210);
	append(&MenuBg);
	append(&FileNameText);
	
	if (IsDir)
	{
		AddOption("Open", DIR_OPEN);
		AddOption("Copy", DIR_COPY);
		AddOption("Delete", DIR_DELETE);
		AddOption("Move", DIR_MOVE);	
	}
	else
	{
		//AddOption("Open", FILE_OPEN);
		AddOption("Copy", FILE_COPY);
		AddOption("Delete", FILE_DELETE);
		AddOption("Move", FILE_MOVE);	
	}
}

FileOptionsMenu::~FileOptionsMenu()
{
	// TODO: free the list
	
	remove(&MenuBg);
	remove(&FileNameText);
}

void FileOptionsMenu::AddOption(std::string Name, u32 Operation)
{	
	log_printf("FileOptionsMenu::AddOption: Adding entry %s (IsDir: %d)\n", Name.c_str(), IsDir);

	FileOption_t *Option = (FileOption_t *)malloc(sizeof(FileOption_t));
	
	Option->name = new std::string(Name);
	Option->operation = Operation;
	
	Option->text = new GuiText(Name.c_str(), 32, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	Option->button = new GuiButton(600, 75);
	Option->button->setLabel(Option->text);
	Option->button->setAlignment(ALIGN_TOP_CENTER);
	Option->button->setPosition(0, -225 - OptionsCount * 75);
	Option->button->setTrigger(&touchTrigger);
	Option->button->setTrigger(&wpadTouchTrigger);
	Option->button->setSoundClick(buttonClickSound);
	Option->button->pressed.connect(this, &FileOptionsMenu::OnOptionPressed);
		
	append(Option->button);
	MenuOptions.push_back(Option);
	OptionsCount++;
}

void FileOptionsMenu::OnOptionPressed(GuiButton *button, const GuiController *controller, GuiTrigger *trigger)
{
	// Find out the button that was clicked
	FileOption_t *Option;
	for (std::list<FileOption_t *>::iterator it = MenuOptions.begin(); it != MenuOptions.end(); ++it)
	{
		Option = (FileOption_t *)*it;
		if (Option->button == button)
		{
			OptionSelect(FileName, Option->operation);
			break;
		}
	}
	
	Close(1);
}

void FileOptionsMenu::update(GuiController * c)
{
	GuiFrame::update(c);
	
	// Prevent the menu from closing immediatly
	if(c->data.validPointer)
	{
		if (!(MenuBg.isInside(c->data.x, c->data.y)) && !waitRelease)
			Close(1);
	}
	else if (waitRelease)
		waitRelease = 0;
}