#include <stdio.h>
#include <unistd.h>

int fsRemove(const char* path)
{
	return remove(path);
}

int fsRemoveDir(const char* path)
{
	return rmdir(path);
}