#include "dialog-util.h"

const nfdchar_t* SceneFilterList = "Scene files\0scene.scene\0All files\0*\0";


void DialogUtil::openFileDialog()
{
}
std::string DialogUtil::saveFileDialog(const std::string &defaultPath, const nfdchar_t *filterList)
{
    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_SaveDialog("scene", defaultPath.c_str(), &outPath);
    if (result == NFD_OKAY)
    {
        std::string path(outPath);
        free(outPath);
        std::cout << "DialogUtil::saveFileDialog: " << path << std::endl;
        return path;
    }
    else if (result == NFD_CANCEL)
    {
        std::cout << "DialogUtil::saveFileDialog: cancel" << std::endl;
        return "";
    }
    else
    {
        // printf("Error: %s\n", NFD_GetError());
        std::cout << "DialogUtil::saveFileDialog: error "<< NFD_GetError() << std::endl;
        return "";
    }
}