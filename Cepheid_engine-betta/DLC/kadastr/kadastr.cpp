#include <vector>
#include <string>
#include <iostream>
#include <numeric>
#include <unordered_map>

#include "CeExploer.h"
#include "Exel_use.h"
#include "ceplugin_engine.h"
#include "Model_FEU.h"

extern "C" {
    std::string get_name(){
        return "Меню расчёта";
        //return "назад";
    }

    std::vector<std::string> get_configuration_file(){
        return std::vector<std::string>{
            R"(\DLC\SOLAR-Solar_menu.json)",
            R"(\DLC\SOLAR-Kadastr-Solar_menu.json)",
            R"(\DLC\SOLAR-Optimal-A-Solar_menu.json)",
            R"(\DLC\SOLAR-Any_a-Solar_menu.json)",
            R"(\DLC\SOLAR-Temperature-Solar_menu.json)",
            R"(\DLC\SOLAR-E-Solar_menu.json)"
        };
    }
}

template<typename ...Arg>
struct Data_fun_get{
    std::function<void(Arg...)> fun=nullptr;

    inline void launch(Arg... arg){
        if(fun){
           fun(std::forward<Arg>(arg)...);
        }
    }
};

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

extern "C" {
    void kadastr_calk(Data_solar data);
}

inline static CeEngine::CeExploer exploer;



inline std::string to_string_accurate(float in, std::size_t accure=1){
    auto t=std::to_string(in);
    t.erase(t.size()-6+accure,t.size()-1);
    return t;
}
auto to_medium(std::vector<float> &t, float k,std::size_t crop_to=8760){
    std::vector<float> m;
    float sum=0.0f;
    if(crop_to==0){
        crop_to=t.size();
    }

    for (std::size_t i=0;i<crop_to;i+=24) {
        for (std::size_t j=i;j<i+24;j++) {
            sum+=t[j];
        }
        m.push_back(sum/k);
        sum=0.0f;
    }
    return m;
};

class Data_input{
public:
    Data_solar data_solar;

    std::vector<float> data_stage[7];

    std::unordered_map<std::string,float> data;
    std::unordered_map<std::string,Data_fun_get<std::string>> get_data;
    std::unordered_map<std::string,Data_fun_get<std::vector<float> &&,uint32_t,std::string,std::string,std::string,std::string>> get_data_graf;

    Data_fun_get<std::string> plate[3];
    Data_fun_get<std::vector<float> &&,uint32_t,std::string,std::string,std::string,std::string> grafic[2];
    std::shared_ptr<CeEngine::CePlagin_engine> plugin;

    inline static std::string current_path_=std::filesystem::current_path().string();

    Data_input(){
        plugin=std::make_shared<CeEngine::CePlagin_engine>("DLC\\libbase.dll");
        plugin->method<void,void(*)(Data_solar)>("set_dll",kadastr_calk);
    }

    Model_FEU_v2::Input_data get_input(){
        Model_FEU_v2::Input_data input_data;
        input_data.latitude=data_solar.latitude;
        input_data.longtitude=data_solar.longtitude;


        input_data.step_B=data["step_B_optimal_min_s_input"];
        if(static_cast<int>(input_data.step_B)<=0){
            input_data.step_B=1.0f;//изменить в интерфейсе
            get_data["step_B_optimal_min_s_input"].launch("1.0f");
        }

        input_data.step_Azimut=data["step_Azimut_optimal_min_s_input"];
        if(static_cast<int>(input_data.step_Azimut)<=0){
            input_data.step_Azimut=1.0f;//изменить в интерфейсе
            get_data["step_Azimut_optimal_min_s_input"].launch("1.0f");
        }

        input_data.B=data["B_сurrent_s_input"];
        input_data.Azimut=data["Azimut_сurrent_s_input"];

        input_data.NOST=data["NOST_s_input"];
        input_data.K_T=data["K_T_s_input"];

        input_data.KPD=data["КPD_optimal_s_input"];
        input_data.F=data["F_optimal_s_input"];
        input_data.E_start=data["E_allowance_optimal_s_input"];

        return input_data;
    }

    void model_launch_after_optimal_A(){


        auto [B,Azimut,B_Array]=optimal_A_find(get_input(),data_solar.R,data_solar.R_d,data_solar.albedo);
        get_data["B_optimal_input"].launch(to_string_accurate(B));
        get_data["Azimut_optimal_min_s_input"].launch(to_string_accurate(Azimut));

        data_stage[1]=B_Array;

        std::vector<float> B_Array_{};

        for(std::size_t i=0;i<B_Array.size()-1;i++){
            for (std::size_t j=0;j<get_input().step_B;j++) {
                B_Array_.push_back(B_Array[i+1]+(B_Array[i]-B_Array[i+1])/(-get_input().step_B)*(j-get_input().step_B));
            }
        }
        B_Array_.push_back(*(--B_Array.end()));

        get_data_graf["optimal_A_solar_Graf_1"].launch(std::move(B_Array_),0X4000FFFF,"optimal_A","R_a","B, гард","кВт*ч/м^2");

        get_data["B_сurrent_s_input"].launch(to_string_accurate(B));
        get_data["Azimut_сurrent_s_input"].launch(to_string_accurate(Azimut));


        //model_launch_befor_optimal_A();
    }

    void model_launch_befor_optimal_A(){
        auto t=get_input();
        std::cout<<"|||||T||||||"<<t.B<<"|"<<t.latitude<<"|"<<t.Azimut<<"|"<<t.KPD<<"|"<<std::endl;

        Model_FEU_v2 m(t);

        std::vector<float> R_=data_solar.R;
        std::vector<float> R_A=m.launch_A(data_solar.R,data_solar.R_d,data_solar.albedo);
        std::vector<float> R_A_T=m.launch_T(data_solar.T,R_A);
        data_stage[2]=R_A;
        data_stage[3]=R_A_T;

        std::cout<<"|||||R_d||||||"<<std::accumulate(data_solar.R_d.begin(),data_solar.R_d.end(),0)/1000.0f<<""<<std::endl;

        get_data_graf["optimal_Any_a_Graf_1"].launch(to_medium(R_A,1.0f),0X4000FFFF,"Any_a","R_v","t, дни","Вт*ч/м^2");
        get_data_graf["Temperature_solar_Graf_1"].launch(to_medium(R_A_T,1.0f),0X4000FFFF,"Temperature_solar","R_v","t, дни","Вт/м^2");

        std::cout<<"|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"<<std::endl;

        std::cout<<"|||||R_A||||||"<<std::accumulate(R_.begin(),R_.end(),0)/1000.0f<<""<<std::endl;

        m.launch_KPD(R_);
        m.launch_KPD(R_A);
        m.launch_KPD(R_A_T);

        data_stage[4]=R_;
        data_stage[5]=R_A;
        data_stage[6]=R_A_T;

        //get_data_graf["optimal_E_Graf_1"].launch(to_medium(data_solar.R,1.0f),0X4000FFFF,"result","R_v","t, дни","Вт/м^2");
        get_data_graf["optimal_E_Graf_1"].launch(to_medium(R_,1.0f),0X4000FFFF,"result","R_","t, дни","Вт/м^2");
        get_data_graf["optimal_E_Graf_1"].launch(to_medium(R_A,1.0f),0XFF00FFFF,"result","R_A","t, дни","Вт/м^2");
        get_data_graf["optimal_E_Graf_1"].launch(to_medium(R_A_T,1.0f),0X376b37FF,"result","R_A_T","t, дни","Вт/м^2");

        std::cout<<"|||||R_A||||||"<<std::accumulate(R_.begin(),R_.end(),0)/1000.0f<<""<<std::endl;


        get_data["E__s_input"].launch(to_string_accurate(std::accumulate(data_solar.R.begin(),data_solar.R.end(),0)/1000.0f));
        get_data["E_A_s_input"].launch(to_string_accurate(std::accumulate(R_.begin(),R_.end(),0)/1000.0f));
        get_data["E_T_s_input"].launch(to_string_accurate(std::accumulate(R_A.begin(),R_A.end(),0)/1000.0f));
        get_data["E_A_T_s_input"].launch(to_string_accurate(std::accumulate(R_A_T.begin(),R_A_T.end(),0)/1000.0f));
        std::cout<<"---4---"<<std::endl;
    }


};

inline static Data_input data_input;
inline static std::wstring current_path_=std::filesystem::current_path().wstring();

extern "C" {
    void get_data_stage_1(){
        exploer.open_exploer(".xlsx");
        std::cout<<exploer.get_path()<<std::endl;

        std::filesystem::path ppp=current_path_+ L"\\template_kadastr.xlsx";
        std::wstring l=exploer.get_path().wstring();
        //l+=L".xlsx";
        std::filesystem::path ppp1=l;
        try {
            std::filesystem::copy_file( ppp,ppp1);
        } catch(std::filesystem::filesystem_error& e) {
            std::cout << "Could not copy sandbox/abc: " << e.what() << '\n';
        }
        std::cout<<ppp1.string()<<std::endl;
        CeEngine::Exel_use exel(ppp1.string(),CeEngine::Exel_use::TYPE_FAILE_OPEN{});
        std::vector<float> hourse_summ(288,0);
        std::vector<float> hourse_p   (288,0);
        std::vector<float> hourse_dif (288,0);

        std::vector<float> hourse_all (52,0);

        std::size_t m=0;
        std::size_t day=0;
        for (std::size_t i=0;i<8760;i+=24) {
            m=tutl::to_month(day);
            //std::cout<<m<<"|"<<day<<"|"<<tutl::month_days<365>(m)<<"|"<<data_input.data_solar.R.size()<<std::endl;
            for(std::size_t j=0;j<24;j++){
                hourse_summ[j+m*24]+=data_input.data_solar.R[i+j]/static_cast<float>(tutl::month_days<365>(m)*1000.0f);
                hourse_dif[j+m*24]+=data_input.data_solar.R_d[i+j]/static_cast<float>(tutl::month_days<365>(m)*1000.0f);
                hourse_p[j+m*24]=hourse_summ[j+m*24]-hourse_dif[j+m*24];

                hourse_all[m+13*0]+=data_input.data_solar.R[i+j]/1000.0f;
                hourse_all[m+13*2]+=data_input.data_solar.R_d[i+j]/1000.0f;
                hourse_all[m+13*3]+=data_input.data_solar.albedo[i+j]/static_cast<float>(tutl::month_days<365>(m)*24.0f);
                hourse_all[m+13*1]=hourse_all[m+13*0]-hourse_all[m+13*2];
            }

            day++;
        }

        for(std::size_t i=0;i<12;i++){
            hourse_all[12]+=hourse_all[i+13*0];
            hourse_all[25]+=hourse_all[i+13*1];
            hourse_all[38]+=hourse_all[i+13*2];
            hourse_all[51]+=hourse_all[i+13*3]/12.0f;
        }



        std::cout<<m<<"|"<<day<<"|"<<tutl::month_days<365>(m)<<std::endl;

        auto list_save=exel.open_list("Sheet1");
        exel.save_data(CeEngine::Exel_use::range(list_save,3,11,3+3,11+12), &hourse_all,2);
        exel.save_data(CeEngine::Exel_use::range(list_save,31,2,31+11,2+23),&hourse_summ,2);
        exel.save_data(CeEngine::Exel_use::range(list_save,50,2,50+11,2+23),&hourse_p,2);
        exel.save_data(CeEngine::Exel_use::range(list_save,69,2,69+11,2+23),&hourse_dif,2);
    }
    void get_data_stage_2(){
        auto current_path_=std::filesystem::current_path().string();
        exploer.open_exploer(".xlsx");
        std::cout<<exploer.get_path()<<std::endl;
        CeEngine::Exel_use exel(exploer.get_path() ,CeEngine::Exel_use::TYPE_FAILE_CREATE{});
        std::vector<std::string> name_collumn={"B, degree","R(kWh*h/m^2)","R_пр(Wh/m^2)"};

        auto list_save=exel.open_list("Sheet1");
        exel.save_data(CeEngine::Exel_use::range(list_save,1,1,1,1+name_collumn.size()-1),&name_collumn);

        std::vector<int> B;

        auto step_B=data_input.get_input().step_B;
        for (std::size_t i=0;i<data_input.data_stage[1].size();i++) {
            B.emplace_back(step_B*i);
        }
        save_data_column(exel,list_save,6,0,
                            B,
                            data_input.data_stage[1]);
    }

    void get_data_stage_3(){
        auto current_path_=std::filesystem::current_path().string();
        exploer.open_exploer(".xlsx");
        std::cout<<exploer.get_path()<<std::endl;
        CeEngine::Exel_use exel(exploer.get_path() ,CeEngine::Exel_use::TYPE_FAILE_CREATE{});
        std::vector<std::string> name_collumn={"Год","Месяц","День","Час","R(Wh/m^2)"};

        auto list_save=exel.open_list("Sheet1");
        exel.save_data(CeEngine::Exel_use::range(list_save,1,1,1,1+name_collumn.size()-1),&name_collumn);
        save_data_column(exel,list_save,6,0,
                            data_input.data_solar.year,
                            data_input.data_solar.monthe,
                            data_input.data_solar.day,
                            data_input.data_solar.hour,
                            data_input.data_stage[2]);
    }
    void get_data_stage_4(){
        auto current_path_=std::filesystem::current_path().string();
        exploer.open_exploer(".xlsx");
        std::cout<<exploer.get_path()<<std::endl;
        CeEngine::Exel_use exel(exploer.get_path() ,CeEngine::Exel_use::TYPE_FAILE_CREATE{});
        std::vector<std::string> name_collumn={"Год","Месяц","День","Час","R(Wh/m^2)","T, цельсия"};

        auto list_save=exel.open_list("Sheet1");
        exel.save_data(CeEngine::Exel_use::range(list_save,1,1,1,1+name_collumn.size()-1),&name_collumn);
        save_data_column(exel,list_save,6,0,
                            data_input.data_solar.year,
                            data_input.data_solar.monthe,
                            data_input.data_solar.day,
                            data_input.data_solar.hour,
                            data_input.data_stage[3],
                            data_input.data_solar.T);
    }
    void get_data_stage_5(){
        auto current_path_=std::filesystem::current_path().string();
        exploer.open_exploer(".xlsx");
        std::cout<<exploer.get_path()<<std::endl;
        CeEngine::Exel_use exel(exploer.get_path() ,CeEngine::Exel_use::TYPE_FAILE_CREATE{});
        std::vector<std::string> name_collumn={"Год","Месяц","День","Час","R_sum_a_t(Wh/m^2)"};

        auto list_save=exel.open_list("Sheet1");
        exel.save_data(CeEngine::Exel_use::range(list_save,1,1,1,1+name_collumn.size()-1),&name_collumn);
        save_data_column(exel,list_save,6,0,
                            data_input.data_solar.year,
                            data_input.data_solar.monthe,
                            data_input.data_solar.day,
                            data_input.data_solar.hour,
                            data_input.data_stage[6]);
    }
}

extern "C" {
    void kadastr_calk(Data_solar data){
        float sum_E=std::accumulate(data.R.begin(),data.R.end(),0)/1000.0f;
        auto sum_E_s=std::to_string(sum_E);
        sum_E_s.erase(sum_E_s.size()-6+1,sum_E_s.size()-1);
        data_input.plate[0].launch(sum_E_s);

        float max_T=*std::max_element(data.T.begin(),data.T.end());
        float min_T=*std::min_element(data.T.begin(),data.T.end());

        auto max_T_s=std::to_string(max_T);
        auto min_T_s=std::to_string(min_T);

        max_T_s.erase(max_T_s.size()-6+1,max_T_s.size()-1);
        min_T_s.erase(min_T_s.size()-6+1,min_T_s.size()-1);

        data_input.plate[1].launch(max_T_s);
        data_input.plate[2].launch(min_T_s);

        std::vector<float> t=to_medium(data.T,24.0f);
        std::vector<float> r=to_medium(data.R,1.0f);
        std::vector<float> r_dif=to_medium(data.R_d,1.0f);

        data_input.grafic[1].launch(std::move(t),0X9F0000FF,"kadastr","Temperature","t, дни","T, Цельсий");
        data_input.grafic[0].launch(std::move(r),0X40FF00FF,"kadastr","All_R","t, дни","Вт*ч/м^2");
        data_input.grafic[0].launch(std::move(r_dif),0X202020FF,"kadastr","Defuse_R","t, дни","Вт*ч/м^2");


        data_input.data_solar=data;
        data_input.model_launch_after_optimal_A();
    }   
}



extern "C" {
    void set_data_interface(std::string data,std::string name){
        std::cout<<"---1---"<<std::endl;
        if(data.size()){
           data_input.data[name]=std::stof(data);
        }
        std::cout<<"---2---"<<std::endl;
        if(data_input.data_solar.R.size()){
            data_input.model_launch_befor_optimal_A();
        }
        std::cout<<"---3---"<<std::endl;
    }

    void set_data_interface_step(std::string data,std::string name){
        std::cout<<name<<std::endl;

        if(data.size()){
           data_input.data[name]=std::stof(data);
        }

        if(data_input.data_solar.R.size()){
            data_input.model_launch_after_optimal_A();
        }
    }

    void set_get_data_interface(std::function<void(std::string)> fun_,std::string name){
        std::cout<<name<<std::endl;
        data_input.get_data[name].fun=fun_;
        std::cout<<"|"<<std::endl;
    }

    void set_get_data_interface_graf(std::function<void(std::vector<float> &&,uint32_t,std::string,std::string,std::string,std::string)> fun_,std::string name){
        data_input.get_data_graf[name].fun=fun_;
    }
}


//callback - для отправки
extern "C" {
    void set_E_sum(std::function<void(std::string)> fun_,std::string name){
        data_input.plate[0].fun=fun_;
    }

    void set_T_max(std::function<void(std::string)> fun_,std::string name){
        data_input.plate[1].fun=fun_;

    }
    void set_T_min(std::function<void(std::string)> fun_,std::string name){
        data_input.plate[2].fun=fun_;
    }

    //убрать
    void set_grafics(std::function<void(std::vector<float> &&,uint32_t,std::string,std::string,std::string,std::string)> fun_,std::string name){
        data_input.grafic[0].fun=fun_;
    }
    void set_grafics_2(std::function<void(std::vector<float> &&,uint32_t,std::string,std::string,std::string,std::string)> fun_,std::string name){
        data_input.grafic[1].fun=fun_;
    }
}
