#include <vector>
#include <string>
#include <iostream>
#include <functional>

#include "ceplugin_engine.h"
#include "Exel_use.h"
#include "CeExploer.h"
#include "URL_curl.h"

#include <ctime>
#include <iomanip>
#include <sstream>
#include "csv.hpp"


extern "C" {

    std::string get_name(){
        return "Загрузка данных";
    }

    std::vector<std::string> get_configuration_file(){
        return std::vector<std::string>{R"(\DLC\config_down_solar.json)"};
    }
}



struct Data_solar{
    float latitude=0.0f;
    float longtitude=0.0f;

    std::vector<int> year;
    std::vector<int> monthe;
    std::vector<int> day;
    std::vector<int> hour;
    std::vector<float> R;
    std::vector<float> R_d;
    std::vector<float> T;
    std::vector<float> albedo;
    std::vector<float> V;
};

template<typename ...Arg>
struct Data_fun_get{
    std::function<void(Arg...)> fun=nullptr;
    inline void launch(Arg... arg){
        if(fun){
           fun(arg...);
        }
    }
};

#include <unordered_map>


class Down_data_Solar{
public:
    Data_solar data;
    std::string begin_time;
    std::string end_time;
    Data_fun_get<std::string> fun_get_status;

    Data_fun_get<std::string> fun_to_map_data[2];

    std::vector<std::function<void(Data_solar)>> callback_to_dll;

    std::unordered_map<std::string,int> data_input;

    inline static CeEngine::URL_curl curl;
    inline static CeEngine::CeExploer exploer;
    Down_data_Solar(){}
    std::string convert_data_time(std::string data){
        return data.substr(6,4)+data.substr(3,2)+data.substr(0,2);
    }
    std::string convert_data_value(float data){
        std::string data_S=std::to_string(data);
        auto pos=data_S.find_first_of(',');
        data_S[pos]='.';
        data_S.pop_back();
        data_S.pop_back();
        return data_S;
    }

    void down_data(){
        CeEngine::Type_curl atribute;
        //std::string https(R"(https://power.larc.nasa.gov/api/temporal/hourly/point?Time=LST&parameters=CLRSKY_SFC_SW_DWN,ALLSKY_SFC_SW_DWN,T2M,SZA&community=RE&longitude=80.0000&latitude=60.0000&start=20210101&end=20210331&format=CSV&user=anonymous)");

        //обобщить
        std::string https_start(R"(https://power.larc.nasa.gov/api/temporal/hourly/point?Time=LST&parameters=ALLSKY_SFC_SW_DWN,ALLSKY_SFC_SW_DIFF,T2M,ALLSKY_SRF_ALB,WS10M&community=RE)");
        std::string https_end( R"(&format=CSV&user=anonymous)");
        std::string https_data="&longitude="+convert_data_value(data.longtitude)+
                               "&latitude="+convert_data_value(data.latitude)+
                               "&start="+convert_data_time(begin_time)+
                               "&end="+convert_data_time(end_time);

        atribute.set_data<CURLOPT_URL>(https_start+https_data+https_end);

        std::cout<<https_start+https_data+https_end<<std::endl;

        std::string s;
        auto Callback=[](void *contents, size_t size, size_t nmemb, void *userp)-> std::size_t{
            reinterpret_cast<std::string*>(userp)->append(reinterpret_cast<char*>(contents), size * nmemb);
            return size * nmemb;
        };
        atribute.set_data<CURLOPT_WRITEFUNCTION>(Callback);
        atribute.set_data<CURLOPT_WRITEDATA>(&s);
        curl.request(std::move(atribute));


        std::stringstream ss;
        ss<<s;
        csv::CSVFormat format;
        format.delimiter(',');
        format.no_header();
        format.header_row(13);

        csv::CSVReader sstream_reader(ss, format);

        data.year.clear();
        data.monthe.clear();
        data.day.clear();
        data.hour.clear();
        data.R.clear();
        data.R_d.clear();
        data.T.clear();
        data.albedo.clear();
        data.V.clear();



        for (auto& T: sstream_reader) {
            data.year.emplace_back   (T[0].get<int>());
            data.monthe.emplace_back (T[1].get<int>());
            data.day.emplace_back    (T[2].get<int>());
            data.hour.emplace_back   (T[3].get<int>());
            data.R.emplace_back      (T[4].get<float>());
            data.R_d.emplace_back    (T[5].get<float>());
            data.T.emplace_back      (T[6].get<float>());
            data.albedo.emplace_back (T[7].get<float>());
            data.V.emplace_back      (T[8].get<float>());
        }

        fun_get_status.launch("200");

        for(auto &T:data.R){
            if(T<0){
                std::cout<<T<<"|$$$$$$$$$$"<<std::endl;
                T=0;
            }
        }
        for(auto &T:data.R_d){
            if(T<0){
                std::cout<<T<<"|$$$$$$$$$$"<<std::endl;
                T=0;
            }
        }

        for(auto &T:data.albedo){
            if(T<0){
                T=0;
            }
        }
        for(auto &T: callback_to_dll){
            if(T){
                T(data);
            }
        }

    }
    void get_data(){
        auto current_path_=std::filesystem::current_path().string();
        exploer.open_exploer(".xlsx");
        std::cout<<exploer.get_path()<<std::endl;
        CeEngine::Exel_use exel(exploer.get_path() ,CeEngine::Exel_use::TYPE_FAILE_CREATE{});
        auto list_save=exel.open_list("Sheet1");

        std::vector<std::string> name_collumn={"Год","Месяц","День","Час","R_sum(Wh/m^2)","R_d(Wh/m^2)","T, цельсия","Коэффицент отражения поверхности, о.е.","V, м/с"};
        std::vector<std::string> line_datasheet={"Широта",
                                                 "Долгота",
                                                 "Начало: ",
                                                 "Конец: ",
                                                 " ",
                                                 "R_sum - сумарная солнечная радиация",
                                                 "R_d   - дифузная солнечная радиация",
                                                 "V   - дифузная солнечная радиация",
                                                 "Коэффицент отражения поверхности   - альбедо поверхности",
                                                 "T   - температура окружающей среды"
                                                };
        std::vector<std::string> line_datasheet_cord={std::to_string(data.latitude),
                                                      std::to_string(data.longtitude),
                                                      begin_time,
                                                      end_time
                                                     };



        exel.save_data(CeEngine::Exel_use::range(list_save,12,1,12,1+name_collumn.size()-1),&name_collumn);
        exel.save_data(CeEngine::Exel_use::range(list_save,1,1,1+line_datasheet.size()-1,1),&line_datasheet);
        exel.save_data(CeEngine::Exel_use::range(list_save,1,2,1+line_datasheet_cord.size()-1,2),&line_datasheet_cord);
        save_data_column(exel,list_save,12,0,
                         data.year,
                         data.monthe,
                         data.day,
                         data.hour,
                         data.R,
                         data.R_d,
                         data.T,
                         data.albedo,
                         data.V);
    }

    void down_data_file(){
        exploer.open_exploer(".xlsx");
        std::cout<<exploer.get_path()<<std::endl;
        CeEngine::Exel_use exel(exploer.get_path().string(),CeEngine::Exel_use::TYPE_FAILE_OPEN{});
        auto list_1=exel.open_list("Sheet1");

        data.year.clear();
        data.monthe.clear();
        data.day.clear();
        data.hour.clear();
        data.R.clear();
        data.R_d.clear();
        data.T.clear();
        data.albedo.clear();
        data.V.clear();


        exel.read_data(exel.range(list_1,2,1+data_input["down_file_name_param_A_input"],2+8760,1+data_input["down_file_name_param_A_input"]),&data.year);

        for(auto &[T,K]:data_input){
            std::cout<<T<<"|"<<K<<std::endl;
        }

        exel.read_data(exel.range(list_1,2,1+data_input["down_file_name_param_B_input"],2+8760,1+data_input["down_file_name_param_B_input"]),&data.monthe);
        exel.read_data(exel.range(list_1,2,1+data_input["down_file_name_param_C_input"],2+8760,1+data_input["down_file_name_param_C_input"]),&data.day);
        exel.read_data(exel.range(list_1,2,1+data_input["down_file_name_param_D_input"],2+8760,1+data_input["down_file_name_param_D_input"]),&data.hour);
        exel.read_data(exel.range(list_1,2,1+data_input["down_file_name_param_E_input"],2+8760,1+data_input["down_file_name_param_E_input"]),&data.R);
        exel.read_data(exel.range(list_1,2,1+data_input["down_file_name_param_F_input"],2+8760,1+data_input["down_file_name_param_F_input"]),&data.R_d);
        exel.read_data(exel.range(list_1,2,1+data_input["down_file_name_param_Albedo_input"],2+8760,1+data_input["down_file_name_param_Albedo_input"]),&data.T);
        exel.read_data(exel.range(list_1,2,1+data_input["down_file_name_param_Albedo_input"],2+8760,1+data_input["down_file_name_param_Albedo_input"]),&data.albedo);


        data.albedo.clear();
        data.albedo=std::vector<float>(8780,0.0f);

        for(auto &T: callback_to_dll){
            if(T){
                T(data);
            }
        }
    }
};
inline static Down_data_Solar down;

extern "C" {
    void set_input(std::string t,std::string name){
        std::cout<<" IN: "<<t<<std::endl;
        down.data_input[name]=t[0]-'A';
    }
}

//получение данных с интерфейса
extern "C" {
    void set_lan(std::string t,std::string name){
        std::cout<<" IN: "<<t<<std::endl;
        down.data.latitude =std::stof(t);
    }
    void set_long(std::string t,std::string name){
        down.data.longtitude =std::stof(t);
    }
    void set_begin_time(std::string t,std::string name){
        down.begin_time=t;
    }
    void set_end_time(std::string t,std::string name){
        down.end_time=t;
    }

    void set_lan_long_map(std::string lan_,std::string long_){
        down.fun_to_map_data[0].launch(std::to_string(std::stoi(lan_)));
        down.fun_to_map_data[1].launch(std::to_string(std::stoi(long_)));

        down.data.latitude=std::stof(lan_);
        down.data.longtitude=std::stof(long_);
    }

}

//активаторы
extern "C" {
    void down_data(std::string name){
        down.down_data();
    }
    void get_data(std::string name){
        down.get_data();
    }
    void input_data(std::string name){
        down.down_data_file();
    }
}

//callback - для отправки
extern "C" {
    void set_get_status(std::function<void(std::string)>fun_,std::string name){
        down.fun_get_status.fun=fun_;
    }
    void set_lan_to_map(std::function<void(std::string)>fun_,std::string name){
        down.fun_to_map_data[0].fun=fun_;
    }
    void set_long_to_map(std::function<void(std::string)>fun_,std::string name){
        down.fun_to_map_data[1].fun=fun_;;
    }

    void set_dll(void(*fun_)(Data_solar)){
        down.callback_to_dll.emplace_back(fun_) ;
    }
}




//ТЕСТЫ

static void(*fun_test_2)(std::string,int)=nullptr;
static int id_text_2=-1;


static void(*fun_test)(std::string,int)=nullptr;
static int id_text=-1;

static int  t=15;

extern "C" {
    void test_0(){
        std::cout<<"IN: "<<t<<std::endl;
        if(fun_test){
            fun_test(std::to_string(t),id_text);
        }
        if(fun_test_2){
            fun_test_2("std::to_string(t-1)",id_text_2);
        }
        t++;

        CeEngine::Exel_use exel(R"(C:\Users\ARHIV\Desktop\)"+std::to_string(t)+".xlsx",CeEngine::Exel_use::TYPE_FAILE_CREATE{});
        std::vector<std::string> namber  {1,std::to_string(t)};

        auto list_save=exel.open_list("Sheet1");
        exel.save_data(exel.range(list_save,3-2,2,3-2,2),&namber);
    }
    void test_1(void(*fun_test_)(std::string,int),int id_text_){
        fun_test=*fun_test_;
        id_text=id_text_;
    }

}

