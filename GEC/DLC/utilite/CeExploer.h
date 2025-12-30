#ifndef CEEXPLOER_H
#define CEEXPLOER_H
#include <filesystem>

#include "winsock2.h"
#include <windows.h>
namespace CeEngine {
class CeExploer{
    std::filesystem::path path="";
public:
    CeExploer(){
        //std::cout<<"CeExploer"<<std::endl;
        if(sizeof (wchar_t)==2){
            std::setlocale(LC_ALL,"Russian");
            std::setlocale(LC_ALL,"ru_RU.UTF8");
        }else{
            std::setlocale(LC_ALL,"ru_RU.UTF8");
        }
    }

    std::filesystem::path get_path(){
        return path;
    }

    void open_exploer(std::string name=""){
        if(sizeof (wchar_t)==2){
            std::setlocale(LC_ALL,"Russian");
        }

        #ifdef WIN32
            OPENFILENAME ofn;
            name.resize(1024);

            memset(&ofn,0,sizeof(ofn));
            ofn.lStructSize     = sizeof(ofn);
            ofn.hwndOwner       = nullptr;
            ofn.hInstance       = nullptr;
            ofn.lpstrFilter     = nullptr;
            ofn.lpstrFile       = name.data();
            ofn.nMaxFile        = MAX_PATH;
            ofn.lpstrTitle      = nullptr;
            ofn.Flags           =0;
            GetSaveFileName(&ofn);

            //std::cout<<name<<std::endl;

            std::size_t result_u = MultiByteToWideChar(1251,0, name.data(),-1,nullptr,0);
            std::vector<wchar_t> ures(result_u);

            if (!result_u || !MultiByteToWideChar(1251,0,name.data(),-1, ures.data(),result_u)){
               return;
            }

            std::size_t result_c = WideCharToMultiByte(CP_UTF8,0,ures.data(), -1,nullptr,0, nullptr, nullptr);
            std::string cres(" ",result_c);

            if(!result_c || !WideCharToMultiByte(CP_UTF8,0,ures.data(), -1,cres.data(),result_c,nullptr, nullptr)){
                return;
            }

            //std::cout<<cres<<std::endl;
            path=name;
        #else
            char filename[1024];
            fgets(filename, 1024, popen("zenity --file-selection", "r"));
            path=filename;
        #endif
            if(sizeof (wchar_t)==2){
                std::setlocale(LC_ALL,"Russian");
                std::setlocale(LC_ALL,"ru_RU.UTF8");
            }else{
                std::setlocale(LC_ALL,"ru_RU.UTF8");
            }
    }
};
}// namespace CeEngine
#endif // CEEXPLOER_H
