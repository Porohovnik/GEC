#ifndef URL_CURL_H
#define URL_CURL_H
#include <filesystem>
#include <iostream>
#include <curl/curl.h>

#include "tuple_utl.h"
namespace CeEngine {
class Curl_list{
public:
    curl_slist * list=nullptr;
    Curl_list(){}
    template<typename T>
    Curl_list(T array){
        for(auto &J:array){
            append(J);
        }
    }

    ~Curl_list(){
        if(list!=nullptr){
           //curl_slist_free_all(list);
        }
    }
    void append(std::string data){
        list=curl_slist_append(list,data.data());
    }

    operator curl_slist * (){
        return list;
    }
};


using Type_curl= tutl::Type_to_data<
    tutl::KAAT<CURLOPT_URL,std::string>,
    tutl::KAAT<CURLOPT_HEADER,std::size_t>,
    tutl::KAAT<CURLOPT_HTTPHEADER,Curl_list>,
    tutl::KAAT<CURLOPT_POST,int>,
    tutl::KAAT<CURLOPT_POSTFIELDSIZE,std::size_t>,
    tutl::KAAT<CURLOPT_POSTREDIR,int>,
    tutl::KAAT<CURLOPT_POSTFIELDS,std::string>,
    tutl::KAAT<CURLOPT_COOKIE,std::string>,
    tutl::KAAT<CURLOPT_COOKIEJAR,std::string>,

    tutl::KAAT<CURLOPT_TIMEOUT,std::size_t>,
    tutl::KAAT<CURLOPT_WRITEFUNCTION,size_t(*)(void *, size_t, size_t, void *)>,
    tutl::KAAT<CURLOPT_WRITEDATA,void *>
>;


class URL_curl{
    class Unit_curl{
        CURL *curl=nullptr;
    public:
        Unit_curl(){
            curl_global_init(CURL_GLOBAL_ALL);
            curl=curl_easy_init();
        }
        ~Unit_curl(){
            curl_easy_cleanup(curl);
        }
        operator auto(){
            return curl;
        }
    };

    inline static Unit_curl curl{};

    template<typename T>
    inline constexpr T & Type_to(T &t){
        return t;
    }
    inline auto Type_to(Curl_list &t){
        return t.operator curl_slist *();
    }

    inline auto Type_to(std::string  &t){
        return t.c_str();
    }
public:
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
        reinterpret_cast<std::string *>(userp)->append(reinterpret_cast<char*>(contents), size * nmemb);
        std::cout<<size<<"|"<<nmemb<<std::endl;
        return size * nmemb;
    }

    URL_curl(){}

    void set_atribute(Type_curl &info){
        auto data=info.get_data();
        tutl::TupleForeach(data,[this](auto &t){
            if(t.data.has_value()){
                curl_easy_setopt(curl, t.type,this->Type_to(*t.data));
            }
        });
    }

    void request(Type_curl &&info){
        set_atribute(info);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        //curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);

        auto res = curl_easy_perform(curl);
        if(res != CURLE_OK){
            std::cout<<"|: "<<curl_easy_strerror(res)<<std::endl;
        }
        curl_easy_reset(curl);
    }


    void ptint_test(){
        std::string readBuffer;

        curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        auto res = curl_easy_perform(curl);
        if(res != CURLE_OK){
            std::cout<<"|: "<<curl_easy_strerror(res)<<std::endl;
        }
        curl_easy_reset(curl);
        std::cout << readBuffer << std::endl;
    }
};
}// namespace CeEngine
#endif // URL_CURL_H
