#include "SEditor/Util/NfdDialog.h"
#include <nfd.h>
#include "spdlog/spdlog.h"
#include "iconv/iconv.h"
#include <iostream>
#include <string>
namespace SEditor::Util{


std::string NfdDialog::OpenFileDlg(){
    NFD_Init();

    nfdchar_t* outPath = NULL;
    nfdfilteritem_t filterItem[2] = { { "Scene", "sanascene" }, { "Headers", "h,hpp" } };
    nfdresult_t result = NFD_OpenDialog(&outPath,filterItem,2,NULL);
    std::string filepth="";
    if ( result == NFD_OKAY ) {
        filepth = std::string(outPath);
        NFD_FreePath(outPath);
        outPath = nullptr;
        spdlog::info("[NFD] File Open Success! "+filepth);
    }
    else if ( result == NFD_CANCEL ) {
        spdlog::info("[NFD] User pressed cancel.");
    }
    else {
        spdlog::error("[NFD] "+std::string(NFD_GetError()));
    }

    NFD_Quit();
    return filepth;
}
std::string NfdDialog::OpenFolderDlg(){
    NFD_Init();

    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_PickFolder(&outPath,NULL);
    std::string folderpth = "";
    if ( result == NFD_OKAY ) {
        folderpth = std::string(outPath);
        NFD_FreePath(outPath);

        // size_t srclen = folderpth.size();
        // size_t dstlen = 100;
        // char dst[100];
        // iconv_t conv = iconv_open("GBK","utf-8");
        // std::string tmpfolderpth(folderpth);
        // char* inptr = tmpfolderpth.data();
        // char* outptr = dst;
        // auto tmpres = iconv(conv,&inptr,&srclen,&outptr,&dstlen);
 
       
        spdlog::info("[NFD] Folder Open Success! "+folderpth);
           
    }
    else if ( result == NFD_CANCEL ) {
        spdlog::info("[NFD] User pressed cancel.");
    }
    else {
        spdlog::error("[NFD] "+std::string(NFD_GetError()));
    }
    NFD_Quit();
    return folderpth;
}

std::string NfdDialog::SaveDlg(){
    NFD_Init();

    nfdchar_t *savePath = NULL;
    nfdfilteritem_t filterItem[1] = { { "Scene", "sanascene" } };
    nfdresult_t result = NFD_SaveDialog(  &savePath,filterItem,1,NULL,"New Scene" );
    std::string folderpth="";
    if ( result == NFD_OKAY )
    {   
        
        folderpth = std::string(savePath);
        
        NFD_FreePath(savePath);

    }
    else if ( result == NFD_CANCEL )
    {
        spdlog::info("[NFD] User pressed cancel.");
    }
    else 
    {
        spdlog::error("[NFD] "+std::string(NFD_GetError()));
    }

    NFD_Quit();
    return folderpth;

}



}