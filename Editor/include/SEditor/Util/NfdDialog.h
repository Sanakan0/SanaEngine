#pragma once
#include <nfd.h>
#include <string>
#include <vector>
namespace SEditor::Util{

class NfdDialog{
public:
static std::string OpenFileDlg(const std::vector<nfdfilteritem_t>& filters,const std::string& default_pth="");
static std::string OpenFileDlg();
static std::string OpenFolderDlg();
static std::string OpenFolderDlg(const std::string& default_pth);
static std::string SaveDlg();
static std::string SaveDlg(const std::vector<nfdfilteritem_t>& filters,const std::string& defaultName,const std::string& defaultpath="");

};

}