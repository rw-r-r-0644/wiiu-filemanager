#include "MainFileView.h"

#include "fs/fs_ops.h"
#include "utils/logger.h"

MainFileView::MainFileView(s32 w, s32 h)
    : GuiMainWindowScreen(w, h)
    , width(w)
    , height(h)
	, img_bg_color(w, h, (GX2Color){0xEE, 0xEE, 0xEE, 0xFF})
	, filelist(w, h)
	, pathbar(w, h)
{
	append(&img_bg_color);
	
	// Append the file list
	filelist.FileEntryClick.connect(this, &MainFileView::OnFileEntryClick);
	filelist.FileEntrySelect.connect(this, &MainFileView::OnFileEntrySelect);
	append(&filelist);
	
	// Create a PathBar element
	append(&pathbar);
	
	// Create a navigation bar element
	//TODO...
}

MainFileView::~MainFileView()
{
    remove(&filelist);
	remove(&pathbar);
}

void MainFileView::Refresh()
{
	filelist.ClearItems();
	
	// List files in current directory
	DirList* dirList = new DirList(CurrentDirectory, NULL, DirList::Files | DirList::Dirs);
	dirList->SortList();
	
	// Add upper directry entry
	if (CurrentDirectory != DIR_ROOT)
		filelist.AddItem("..", true);
	
	// Add other entries
	for(s32 i = 0; i < dirList->GetFilecount(); i++)
		filelist.AddItem(dirList->GetFilename(i), dirList->IsDir(i));
	
	// Refresh pathbar
	pathbar.Refresh(CurrentDirectory);
	
	log_printf("MainFileView::Refresh: Refresh done\n");
}

void MainFileView::ChangeDir(std::string Dir)
{
	if (Dir == ".." && CurrentDirectory == DIR_ROOT)
		return;
	
	// Go tu upper directory or enter new directory
	CurrentDirectory = (Dir == "..") ? CurrentDirectory.substr(0, CurrentDirectory.find_last_of("/")) : CurrentDirectory + '/' + Dir;
	
	RefreshRequired = true;
	log_printf("MainFileView::ChangeDir: Directory is \"%s\"\n", CurrentDirectory.c_str());
}


void MainFileView::OnFileEntryClick(std::string entryName, bool IsDir)
{
	if (IsDir)
		ChangeDir(entryName);
}

void MainFileView::OnFileEntrySelect(std::string entryName, bool IsDir)
{
	filelist.SetEnable(0);
	
	file_options_menu = new FileOptionsMenu(width, height, entryName, IsDir);
	file_options_menu->Close.connect(this, &MainFileView::OnFileOptionsMenuClose);
	file_options_menu->OptionSelect.connect(this, &MainFileView::OnFileOptionsSelect);
	append(file_options_menu);
}

void MainFileView::OnFileOptionsMenuClose(int n)
{
	CloseOptionsMenu = true;
}

void MainFileView::OnFileOptionsSelect(std::string Name, u32 Option)
{
	std::string FullPath = CurrentDirectory + '/' + Name;
	log_printf("MainFileView::OnFileOptionsSelect: %s [%s]\n", Name.c_str(), FullPath.c_str());

	switch(Option)
	{
		case DIR_OPEN:
			ChangeDir(Name);
			break;
		case DIR_COPY:
			break;
		case DIR_DELETE:
			{
				DirList dirList(FullPath, 0, DirList::Files | DirList::Dirs | DirList::CheckSubfolders);
				for (int i = 0; i < dirList.GetFilecount(); i++)
					(dirList.IsDir(i)) ? fsRemoveDir(dirList.GetFilepath(i)) : fsRemove(dirList.GetFilepath(i));
				fsRemoveDir(FullPath.c_str());
				RefreshRequired = true;
				break;
			}
		case DIR_MOVE:
			break;
		case FILE_OPEN:
			break;
		case FILE_COPY:
			break;
		case FILE_DELETE:
			{
				fsRemove(FullPath.c_str());
				RefreshRequired = true;
				break;
			}
		case FILE_MOVE:
			break;
		default:
			break;
	}
}

void MainFileView::process(){
	GuiMainWindowScreen::process();
	
	if (RefreshRequired && filelist.StateEnabled)
	{
		Refresh();
		RefreshRequired = false;
	}
	
	if (CloseOptionsMenu)
	{
		remove(file_options_menu);
		delete file_options_menu;
		filelist.SetEnable(1);
		CloseOptionsMenu = false;
	}
}
