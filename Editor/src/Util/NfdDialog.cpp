#include "SEditor/Util/NfdDialog.h"
#include "nfd.h"
#include "spdlog/spdlog.h"
#include "iconv/iconv.h"
#include <iostream>
namespace SEditor::Util{


std::string NfdDialog::OpenFileDlg(){
    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_OpenDialog(NULL,NULL, &outPath);
    if ( result == NFD_OKAY ) {
        auto filepth = std::string(outPath);
        free(outPath);
        spdlog::info("[NFD] File Open Success! "+filepth);
        return filepth;
           
    }
    else if ( result == NFD_CANCEL ) {
        spdlog::info("[NFD] User pressed cancel.");
    }
    else {
        spdlog::error("[NFD] "+std::string(NFD_GetError()));
    }
    return "";
}
std::string NfdDialog::OpenFolderDlg(){
    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_PickFolder(NULL, &outPath);
    if ( result == NFD_OKAY ) {
        auto folderpth = std::string(outPath);
        free(outPath);

        // size_t srclen = folderpth.size();
        // size_t dstlen = 100;
        // char dst[100];
        // iconv_t conv = iconv_open("GBK","utf-8");
        // std::string tmpfolderpth(folderpth);
        // char* inptr = tmpfolderpth.data();
        // char* outptr = dst;
        // auto tmpres = iconv(conv,&inptr,&srclen,&outptr,&dstlen);
 
       
        spdlog::info("[NFD] Folder Open Success! "+folderpth);
        return folderpth;
           
    }
    else if ( result == NFD_CANCEL ) {
        spdlog::info("[NFD] User pressed cancel.");
    }
    else {
        spdlog::error("[NFD] "+std::string(NFD_GetError()));
    }
    return "";
}



}