#include "pe/Hacks/FSHacks.h"
#include "helpers/fsHelper.h"
#include "hook/trampoline.hpp"
#include "nn/fs.h"

namespace pe {

HOOK_DEFINE_TRAMPOLINE(OpenFile) { static nn::Result Callback(nn::fs::FileHandle * out, const char* filePath, int mode); };

nn::Result OpenFile::Callback(nn::fs::FileHandle* out, const char* filePath, int mode)
{
    static bool sdCardMounted = false;
    if (!sdCardMounted) {
        nn::fs::MountSdCardForDebug("sd");
        sdCardMounted = true;
    }

    if (std::strncmp(filePath, "content:/", 9) == 0) {
        char path[1024] { 0 };
        strcat(path, "sd:/Peepa/romfs/");
        strcat(path, filePath + 9);
        if (FsHelper::isFileExist(path))
            return Orig(out, path, mode);
    }

    /*if (std::strncmp(filePath, "save:/", 6) == 0)
    {
        char path[1024] { 0 };
        strcat(path, "sd:/Peepa/save/");
        strcat(path, filePath + 6);
        return Orig(out, path, mode);
    }*/

    return Orig(out, filePath, mode);
}

void installFSHacks()
{
    OpenFile::InstallAtSymbol("_ZN2nn2fs8OpenFileEPNS0_10FileHandleEPKci");
}

} // namespace pe
