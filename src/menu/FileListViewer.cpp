#include "FileListViewer.h"

#include <sys/stat.h>
#include "utils/logger.h"

FileListViewer::FileListViewer(s32 w, s32 h)
	: GuiFrame(w, h)
	, ic_bg_div(Resources::GetImageData("ic_bg_div.png"))
	, ic_folder(Resources::GetImageData("ic_folder.png"))
	, ic_file(Resources::GetImageData("ic_file.png"))
	, ic_selected(Resources::GetImageData("ic_selected.png"))
	, touchTrigger(GuiTrigger::CHANNEL_1, GuiTrigger::VPAD_TOUCH)
	, wpadTouchTrigger(GuiTrigger::CHANNEL_2 | GuiTrigger::CHANNEL_3 | GuiTrigger::CHANNEL_4 | GuiTrigger::CHANNEL_5, GuiTrigger::BUTTON_A)
	, VPADDrag(w, h)
	, buttonClickSound(Resources::GetSound("button_click.mp3"))
{
	VPADDrag.setTrigger(&touchTrigger);
	VPADDrag.setTrigger(&wpadTouchTrigger);
	VPADDrag.dragged.connect(this, &FileListViewer::OnDrag);
	append(&VPADDrag);
}

void FileListViewer::AddItem(std::string Name, bool IsDir)
{
	log_printf("FileListViewer::AddItem: Adding entry %s (IsDir: %d)\n", Name.c_str(), IsDir);
		
	// Create a new FileEntry
	FileListEntry_t *FileEntry = (FileListEntry_t *)malloc(sizeof(FileListEntry_t));
	
	// Add infos
	FileEntry->name = new std::string(Name);
	FileEntry->isdir = IsDir;
	
	// Truncate the name if too long
	if (Name.length() > 28)
	{
		Name = Name.substr(0, 28);
		Name += "...";
	}
	
	// Prepares things for the button
	FileEntry->text = new GuiText(Name.c_str(), 32, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	FileEntry->icon = new GuiImage((IsDir) ? ic_folder : ic_file);
	FileEntry->icon->setPosition(-300, 0);
	FileEntry->image = new GuiImage(ic_bg_div);
		
	// Create the button
	FileEntry->button = new GuiButton(ic_bg_div->getWidth(), ic_bg_div->getHeight());
	FileEntry->button->setLabel(FileEntry->text);
	FileEntry->button->setIcon(FileEntry->icon);
	FileEntry->button->setImage(FileEntry->image);
	FileEntry->button->setAlignment(ALIGN_TOP_CENTER);
	FileEntry->button->setPosition(0, -100 - CountElements * (ic_bg_div->getHeight() + 10));
	FileEntry->button->setTrigger(&touchTrigger);
	FileEntry->button->setTrigger(&wpadTouchTrigger);
	FileEntry->button->setEffectGrow();
	FileEntry->button->setRumble(0.6f);
	FileEntry->button->setSoundClick(buttonClickSound);
	FileEntry->button->pressed.connect(this, &FileListViewer::OnFileEntryPressed);
	FileEntry->button->pressedLong.connect(this, &FileListViewer::OnFileEntryPressedLong);
		
	// Add the button to the screen and to the list
	append(FileEntry->button);
	ListElements.push_back(FileEntry);
	CountElements++;
}

void FileListViewer::ClearItems()
{
	if (!ListElements.empty())
	{
		for (std::list<FileListEntry_t *>::iterator it = ListElements.begin(); it != ListElements.end(); ++it)
		{
			FileListEntry_t *FileEntry = (FileListEntry_t *)*it;
			log_printf("FileListViewer::ClearItems: Removing entry \"%s\"\n", FileEntry->name->c_str());
			
			// Remove and delete objects
			remove(FileEntry->button);
			delete(FileEntry->button);
			delete(FileEntry->image);
			delete(FileEntry->icon);
			delete(FileEntry->text);
			delete(FileEntry->name);
			
			// Free the structure
			free(FileEntry);
		}
		
		// Clear the list (which now contains bad pointers)
		ListElements.clear();
		CountElements = 0;
	}
}

void FileListViewer::OnFileEntryPressed(GuiButton *button, const GuiController *controller, GuiTrigger *trigger)
{
	if (StateEnabled)
	{
		// Find out the button that was clicked
		FileListEntry_t *FileEntry;
		for (std::list<FileListEntry_t *>::iterator it = ListElements.begin(); it != ListElements.end(); ++it)
		{
			FileEntry = (FileListEntry_t *)*it;
			if (FileEntry->button == button)
			{
				FileEntryClick(*FileEntry->name, FileEntry->isdir);
				break;
			}
		}
	}
}

void FileListViewer::OnFileEntryPressedLong(GuiButton *button, const GuiController *controller, GuiTrigger *trigger)
{
	if (StateEnabled)
	{
		// Find out the button that was clicked
		FileListEntry_t *FileEntry;
		for (std::list<FileListEntry_t *>::iterator it = ListElements.begin(); it != ListElements.end(); ++it)
		{
			FileEntry = (FileListEntry_t *)*it;
			if (FileEntry->button == button)
			{
				FileEntrySelect(*FileEntry->name, FileEntry->isdir);
				break;
			}
		}
	}
}

void FileListViewer::OnDrag(GuiDragListener * listener, const GuiController * controller, GuiTrigger * trigger, s32 dx, s32 dy)
{
	if (StateEnabled)
	{
		for (std::list<FileListEntry_t *>::iterator it = ListElements.begin(); it != ListElements.end(); ++it)
		{
			FileListEntry_t *FileEntry = (FileListEntry_t *)*it;
			FileEntry->button->setPosition(0, FileEntry->button->getOffsetY() + dy);
		}
	}
}

void FileListViewer::SetEnable(s32 enabled)
{
	StateEnabled = enabled;
	
	for (std::list<FileListEntry_t *>::iterator it = ListElements.begin(); it != ListElements.end(); ++it)
	{
		FileListEntry_t *FileEntry = (FileListEntry_t *)*it;
		FileEntry->button->setSelectable(enabled == 1);
		FileEntry->button->setClickable(enabled == 1);
		FileEntry->button->setHoldable(enabled == 1);
	}
}

FileListViewer::~FileListViewer()
{
	ClearItems();
	
	// Remove other elements
	Resources::RemoveImageData(ic_bg_div);
	Resources::RemoveImageData(ic_folder);
	Resources::RemoveImageData(ic_file);
	Resources::RemoveSound(buttonClickSound);
	remove(&VPADDrag);
}