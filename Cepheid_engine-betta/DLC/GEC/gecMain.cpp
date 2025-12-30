#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>
#include "model_calclate.h"


#include "Exel_use.h"

#include "ceplugin_engine.h"

template<typename ...Arg>
struct Data_fun_get{
    std::function<void(Arg...)> fun=nullptr;

    inline void launch(Arg... arg){
        if(fun){
           fun(std::forward<Arg>(arg)...);
        }
    }

//    template <>
//    inline void launch(std::vector<float> && d,uint32_t t1,std::string t2,std::string t3,std::string t4,std::string t5){
//        if(fun){
//           fun(d,t1,t2,t3,t4,t5);
//        }
//    }
};

extern "C" {

    std::string get_name(){
        return "Назад";
    }


    std::vector<std::string> get_configuration_file(){
        return std::vector<std::string>{
            R"(\DLC\GEC-main.json)",
            R"(\DLC\GEC-table.json)",
            R"(\DLC\GEC-graf.json)",

            R"(\DLC\GEC-input_veu.json)",
            R"(\DLC\GEC-input_solar.json)",
            R"(\DLC\GEC-input_geu.json)",
            R"(\DLC\GEC-input_cct.json)",
            R"(\DLC\GEC-input_gne.json)",
            R"(\DLC\GEC-input_nte.json)",
            R"(\DLC\GEC-input_ek.json)",
            R"(\DLC\GEC-input_dgy.json)",
            R"(\DLC\GEC-input_setting.json)",

            R"(\DLC\GEC-input_calc_1.json)",
            R"(\DLC\GEC-input_calc_1_inf.json)"
        };
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
    std::vector<float> t_solar;
    std::vector<float> V;
};
extern "C" {
    void gec_calk(Data_solar data);
    void calc_swith(std::string name);
}

class Data_input{
public:
    std::unordered_map<std::string,float> data;
    Data_fun_get<std::vector<float> &&,uint32_t,std::string,std::string,std::string,std::string> grafic;

    std::unordered_map<std::string,Data_fun_get<std::vector<float> &&,uint32_t,std::string,std::string,std::string,std::string>> grafics_win;

    Data_fun_get<std::string> table_1[5][16];
    Data_fun_get<std::string> table_2[21][14];
    float calc_param[21][14];
    bool choise[15];

    std::string current_path_screenshot="";

    std::unordered_map<std::string,Data_fun_get<std::string>> cost_parametr;
    std::unordered_map<std::string,Data_fun_get<std::string>> main_parametr;
    std::size_t n_equpment[3][5];


    std::vector<std::vector<float>> data_param_current_second;
    std::vector<Model_Gibrid_complex::Data_cost> curren_cost_param;
    std::size_t curren_cost_param_namber=1;
    std::unordered_map<std::string,float> fun_out_table;


    std::vector<std::vector<float>> data_down{6};
    std::vector<std::vector<float>> E0{18};

    std::shared_ptr<CeEngine::CePlagin_engine> plugin;

    Data_input(){
        plugin=std::make_shared<CeEngine::CePlagin_engine>("DLC\\libbase.dll");
        plugin->method<void,void(*)(Data_solar)>("set_dll",gec_calk);

        CeEngine::Exel_use exel("E0.xlsx",CeEngine::Exel_use::TYPE_FAILE_OPEN{});
        auto list_1=exel.open_list("Лист1");
        for(std::size_t i=0;i<18;i++){
            exel.read_data(exel.range(list_1,1+i,1,1+i,1+12-1),&E0[i]);
        }

        for(auto &T:choise){
            T=true;
        }
    }
    Model_Gibrid_complex::Input_data get_input_data(){
        Model_Gibrid_complex::Input_data input_data;

        //ФЭУ
        input_data.pmt_input_data.I_kz=data["I_kz_input"];
        input_data.pmt_input_data.U_xx=data["U_xx_input"];
        input_data.pmt_input_data.I_max=data["I_max_input"];
        input_data.pmt_input_data.U_max=data["U_max_input"];

        input_data.pmt_input_data.K_i_t=data["K_T_kz_input"];
        input_data.pmt_input_data.K_u_t=data["K_T_xx_input"];

        input_data.pmt_input_data.A_PMT=data["A_PMT_input"];
        input_data.pmt_input_data.n_PMT=data["n_PMT_input"];
        input_data.pmt_input_data.ALBEDO=data["ALBEDO_input"];
        input_data.pmt_input_data.KPD=data["KPD_inv_input"];

        input_data.pmt_input_data.N_one=data["N_yst_input"];

        input_data.pmt_input_data.CN=data["СMP_cost_input"];
        input_data.pmt_input_data.service_life=data["PMT_year_old_max_input"];
        input_data.pmt_input_data.service_life_inv=data["Inv_year_old_max_input"];
        input_data.pmt_input_data.cost=data["PMT_cost_input"];
        input_data.pmt_input_data.cost_inv=data["Inv_cost_input"];



        //ВЭУ
        input_data.veu_input_data.H_vk=data["H_vk_input"];
        input_data.veu_input_data.H_fluger=data["H_fluger_input"];
        input_data.veu_input_data.Inerc_const=data["Inerc_const_input"];
        input_data.veu_input_data.Class=data["Class_input"];

        input_data.veu_input_data.Max_V=data["Max_V_input"];
        input_data.veu_input_data.V_rach=data["V_rach_input"];
        input_data.veu_input_data.Min_V=data["Min_V_input"];

        input_data.veu_input_data.Nnom=data["Nnom_input"];
        input_data.veu_input_data.n_VEU=data["n_VEU_input"];

        input_data.veu_input_data.CN=data["cost_VEU_CMP_input"];
        input_data.veu_input_data.cost=data["cost_VEU_input"];
        input_data.veu_input_data.service_life=data["VEU_year_old_max_input"];

        //ГЭУ
        input_data.geu_input_data.H=data["H_input"];
        input_data.geu_input_data.N_min=data["N_min_GEU_input"];
        input_data.geu_input_data.N_yst=data["N_yst_GEU_input"];
        input_data.geu_input_data.KPD=data["KPD_GEU_input"];
        input_data.geu_input_data.n_GEU=data["n_GEU_input"];
        input_data.geu_input_data.N_speed_max=data["N_speed_max_input"];

        input_data.geu_input_data.cost=data["GEU_cost_input"];
        input_data.geu_input_data.service_life=data["GEU_year_old_max_input"];
        //ДГУ
        input_data.dgy_input_data.DGY_CN=data["DGY_CN_input"];
        input_data.dgy_input_data.DGY_Nyst=data["DGY_Nyst_input"];
        input_data.dgy_input_data.DGY_speed=data["DGY_speed_input"];
        input_data.dgy_input_data.Q_disel=data["Fuel_consumption_input"];

        input_data.dgy_input_data.cost=data["DGY_cost_input"];
        input_data.dgy_input_data.cost_disel=data["Disel_cost_input"];
        input_data.dgy_input_data.service_life=data["DGY_year_old_max_input"];
        //ГСНЭ
        input_data.gne_input_data.KPD_SC=data["KPD_CuperK_input"];

        input_data.gne_input_data.KPD_BB=data["KPD_LITI-ON_input"];
        input_data.gne_input_data.KPD_VSAE=data["KPD_VCAE_input"];
        input_data.gne_input_data.cost_VSAE=data["cost_VCAE_input"];
        input_data.gne_input_data.service_life_VSAE=data["year_old_max_VCAE_input"];
        //CСК
        input_data.cct_input_data.S=data["S_CK_input"];
        input_data.cct_input_data.KPD=data["KPD_CK_input"];
        input_data.cct_input_data.A_PMT=data["A_CK_input"];
        input_data.cct_input_data.ALBEDO=data["ALBEDO_input"];
        input_data.cct_input_data.K_T_CK=data["K_T_CK_input"];
        input_data.cct_input_data.T_C_CK=data["T_C_CK_input"];

        input_data.cct_input_data.K_passing=data["K_passing_input"];
        input_data.cct_input_data.K_absorbing=data["K_absorbing_input"];

        //ЭК
        input_data.ek_input_data.KPD=data["KPD_EK_input"];
        //СНТ
        input_data.nte_input_data.KPD=data["KPD_NTE_input"];


        input_data.yers_cost_model=data["yers_cost_model_s_input"];
        input_data.ee_cost=data["ee_cost_s_input"];
        input_data.income_tax=data["income_tax_s_input"];
        input_data.discount_rate=data["discount_rate_s_input"];


        input_data.latitude=data["Lat_input"];

        std::size_t day=data["Times_input"];

        std::tm tm_start={};
        tm_start.tm_year=110;
        tm_start.tm_mon=data["Monthe_input"]-1;
        tm_start.tm_mday=data["Day_input"];



        std::tm tm_end={};
        tm_end.tm_year=110+day/365;
        tm_end.tm_mon=(tutl::to_month((tutl::to_day((static_cast<std::size_t>(tm_start.tm_mon)))+day)%365))%12;
        tm_end.tm_mday=static_cast<std::size_t>((tutl::to_day((static_cast<std::size_t>(tm_start.tm_mon)))+day)%365-tutl::to_day(static_cast<std::size_t>(tm_end.tm_mon)))%365+1;

        std::cout<<tm_start.tm_mon<<"%|%"<<day<<"%|%"<<tm_start.tm_mday<<std::endl;
        std::cout<<tm_end.tm_mon<<"%|%"<<day<<"%|%"<<tm_end.tm_mday<<std::endl;
        auto begin=std::mktime(&tm_start);
        auto end=std::mktime(&tm_end);

        input_data.tm_start=tm_start;
        input_data.tm_end=tm_end;
        std::cout<<std::asctime(std::localtime(&begin))<<"|"<<day<<"|"<<std::asctime(std::localtime(&end))<<std::endl;
        return input_data;
    }



    void model_launch(){

        std::cout<<"T"<<std::endl;

        //подключение из интерфейса
//        choise[PMT]=1;
//        choise[VEU_V]=1;
//        choise[GEU]=1;
//        choise[DIESEL_GENERATOR]=1;

        if(!data_down[0].size()){
            return;
        }
        auto in=get_input_data();


        auto model=create_Model_Gibrid_complex(in,E0,data_down);
        auto data_output=model.model_calculet<second>(data_down,choise);
        auto t=model.indicators_v1<second>(data_output);


        for(std::size_t i=0;i<21;i++){
            for(std::size_t j=0;j<13;j++){
            auto s=std::to_string(t[i][j]);
                if(s.size()>6){
                   s.erase(s.size()-5,s.size()-1);
                }

                table_2[i][j+1].fun(s);

                std::cout<<j+1<<"T: "<<t[i][j]<<std::endl;
            }
        }
        data_param_current_second=t;
        grafics_win["PEE"].launch(std::move(data_output[PEE]),0X4000FFFF,"list_1","PEE","t, секунд","Вт");
        grafics_win["PTE"].launch(std::move(data_output[PTE]),0X4000FFFF,"list_1","PTE","t, секунд","Вт");
        grafics_win["PMT"].launch(std::move(data_output[PMT]),0X4000FFFF,"list_1","PMT","t, секунд","Вт");
        grafics_win["VEU_V"].launch(std::move(data_output[VEU_V]),0X4000FFFF,"list_1","VEU_V","t, секунд","Вт");

        grafics_win["GEU"].launch(std::move(data_output[GEU]),0X4000FFFF,"list_1","GEU","t, секунд","Вт");
        grafics_win["DIESEL_GENERATOR"].launch(std::move(data_output[DIESEL_GENERATOR]),0X4000FFFF,"list_1","DIESEL_GENERATOR","t, секунд","Вт");
        grafics_win["GAS"].launch(std::move(data_output[GAS]),0X4000FFFF,"list_1","GAS","t, секунд","Вт");
        grafics_win["SUPERCAPACITOR"].launch(std::move(data_output[SUPERCAPACITOR]),0X4000FFFF,"list_1","SUPERCAPACITOR","t, секунд","Вт");

        grafics_win["BATTARY_BLOCK"].launch(std::move(data_output[BATTARY_BLOCK]),0X4000FFFF,"list_1","BATTARY_BLOCK","t, секунд","Вт");
        grafics_win["VSAE"].launch(std::move(data_output[VSAE]),0X4000FFFF,"list_1","VSAE","t, секунд","Вт");
        grafics_win["CCT"].launch(std::move(data_output[CCT]),0X4000FFFF,"list_1","CCT","t, секунд","Вт");
        grafics_win["NTE"].launch(std::move(data_output[NTE]),0X4000FFFF,"list_1","NTE","t, секунд","Вт");
    }



    void model_cost_launch(){

        bool choise_[15];
        for(auto &T:choise_){
            T=false;
        }

        choise_[PMT]=1;
        choise_[VEU_V]=1;
        choise_[GEU]=1;
        choise_[DIESEL_GENERATOR]=1;
        choise_[GNE]=1;

        std::tm tm_start={};
        tm_start.tm_year=110;
        tm_start.tm_mon=0;
        tm_start.tm_mday=1;


        std::tm tm_end={};
        tm_end.tm_year=111;
        tm_end.tm_mon=0;
        tm_end.tm_mday=1;

        curren_cost_param.clear();
        for (std::size_t j=0;j<5;j++) {

            auto in=get_input_data();

            in.tm_start=tm_start;
            in.tm_end=tm_end;

            in.pmt_input_data.n_PMT=n_equpment[0][j];
            in.veu_input_data.n_VEU=n_equpment[1][j];
            in.geu_input_data.n_GEU=n_equpment[2][j];

            std::cout<<"T"<<std::endl;
            auto model=create_Model_Gibrid_complex(in,E0,data_down);
            auto data_output=model.model_calculet<hours>(data_down,choise_);
            auto t=model.indicators_v2<hours>(data_output);

            auto t3=model.indicators_cost<hours>(t);

            t[0]/=1000.0f;
            t[1]/=1000.0f;
            t[2]/=1000.0f;
            t[3]/=1000.0f;
            t[4]/=1000.0f;
            t[5]/=1000.0f;
            t[6]/=1000000.0f;
            t[8]/=1000000.0f;
            t[9]/=1000000.0f;
            t[10]/=1000000.0f;
            t[11]/=1000000.0f;

            for(std::size_t i=0;i<16;i++){
                auto s=std::to_string(t[i]);
                if(s.size()>6 && i!=6){
                   s.erase(s.size()-5,s.size()-1);
                }


                if(s.size()>10 && i==6){
                   s.erase(s.size(),s.size()-1);
                }
                table_1[j][i].fun(s);

                std::cout<<i<<"T: "<<t[i]<<std::endl;
            }


            curren_cost_param.emplace_back(t3);
        }
        calc_swith(std::to_string(curren_cost_param_namber));
    }
};

#include "CeExploer.h"
inline static Data_input data_input;
inline static CeEngine::CeExploer exploer;
inline static std::wstring current_path_=std::filesystem::current_path().wstring();

extern "C" {
    void gec_calk(Data_solar data){
        std::cout<<"############"<<std::endl;
        std::vector<float> R_sum;
        for(std::size_t i=0;i<data.R.size();i++){
            R_sum.emplace_back(data.R[i]+data.R_d[i]);
        }
        data_input.data_down[2]=R_sum;
        data_input.data_down[3]=data.V;
        data_input.data_down[5]=data.T;
    }
}



extern "C" {
    void set_path_screnshots(std::string name){
       exploer.open_exploer(".xlsx");
       std::filesystem::create_directory(exploer.get_path());
       data_input.current_path_screenshot=exploer.get_path().string().c_str();
    }

    std::string get_path_screnshots(){
        return data_input.current_path_screenshot;
    }
}

extern "C" {
    void set_choise(std::string name){
        data_input.choise[std::stoi(name)]=!data_input.choise[std::stoi(name)];
        std::cout<<name<<"|"<<data_input.choise[std::stoi(name)]<<std::endl;
    }

    void calc_swith(std::string name){
        auto t3=data_input.curren_cost_param[std::stoi(name)-1];
        data_input.curren_cost_param_namber=std::stoi(name);
        std::cout<<name<<std::endl;

        auto to_float=[](float t){
            auto s=std::to_string(t);
            if(s.size()>6){
               s.erase(s.size()-5,s.size()-1);
            }
            std::cout<<t<<"S: "<<s<<std::endl;
            return s;
        };

        data_input.cost_parametr["V_inv_input"].launch(to_float(t3.sum_cost));
        data_input.cost_parametr["D_max_input"].launch(to_float(t3.profit_cost));
        data_input.cost_parametr["R_cost_input"].launch(to_float(t3.pure));
        data_input.cost_parametr["D_D_D_max_input"].launch(to_float(t3.discont_sum));
        data_input.cost_parametr["D_D_D_old_input"].launch(to_float(t3.discont_years));
        data_input.grafic.launch(std::move(t3.dis),0X4000FFFF,"list_1","DIS","t, лет","милл. руб");
    }

    void down_data_param_second(std::string name){
        auto current_path_=std::filesystem::current_path().string();
        exploer.open_exploer(".xlsx");
        std::cout<<exploer.get_path()<<std::endl;
        CeEngine::Exel_use exel(exploer.get_path() ,CeEngine::Exel_use::TYPE_FAILE_CREATE{});
        auto list_save=exel.open_list("Sheet1");

        std::vector<std::string> name_row_table={
            "Расчётные данные",
            "Объем потребления электрической энергии,кВт*ч ",
            "Объем потребления тепла,кВт*ч ",
            "Объем выработки ФЭУ,кВт*ч ",
            "Объем выработки ВЭУ,кВт*ч ",
            "Объем выработки ГЭУ,кВт*ч ",
            "Объем выработки ДГУ,кВт*ч ",
            "Отпуск тепла ССТ,кВт*ч ",
            "Отпуск тепла ЭК,кВт*ч ",
            "Емкость ВСАЭ, кВт*ч ",
            "Остаточная емкость ВСАЭ, кВт*ч ",
            "Емкость ЛИА, кВт*ч ",
            "Остаточная емкость ЛИА, кВт*ч ",
            "Емкость СК кВт*ч ",
            "Остаточная емкость СК, кВт*ч ",
            "Емкость НТЭ кВт*ч",
            "Остаточная емкость НТЭ, кВт*ч",
            "КИУМ ФЭУ, %",
            "КИУМ ВЭУ, %",
            "КИУМ ГЭУ, %",
            "КИУМ ДГУ, %",
            "Доля замещения ДГУ, %"
        };//21

        std::vector<std::string> name_collum_table={
            "I    ",
            "II   ",
            "III  ",
            "IV   ",
            "V    ",
            "VI   ",
            "VII  ",
            "VIII ",
            "IX   ",
            "X   ",
            "XI   ",
            "XII  ",
            "Год  ",
        };

        exel.save_data(exel.range(list_save,1,2,1,2+12),&name_collum_table);
        exel.save_data(exel.range(list_save,1,1,1+21,1),&name_row_table);
        if(data_input.data_param_current_second.size()){
            std::size_t i=2;
            for(auto &T:data_input.data_param_current_second){
                exel.save_data(exel.range(list_save,i,2,i,2+12),&T);
                i++;
            }
        }

    }

    void model_launch_s(std::string name){
        data_input.model_launch();
    }

    void test(std::string name){
        std::cout<<name<<std::endl;
        //data_input.model_launch();
        //data_input.grafic.launch(t[PMT],0X9F0000FF,"list_1","Temperature","t, лет","руб");
    }


    void loading_data(std::string name){
        exploer.open_exploer(".xlsx");
        std::cout<<exploer.get_path()<<std::endl;

        CeEngine::Exel_use exel(exploer.get_path().string(),CeEngine::Exel_use::TYPE_FAILE_OPEN{});
        auto list_1=exel.open_list("Лист1");

        for(std::size_t i=0;i<6;i++){
            exel.read_data(exel.range(list_1,2,2+i,2+8760-1,2+i),&data_input.data_down[i]);
        }
        for (auto &T:data_input.data_down) {
            T.emplace_back(T.back());
        }
    }

    void down_template(std::string name){
        exploer.open_exploer(".xlsx");
        std::cout<<exploer.get_path()<<std::endl;

        std::filesystem::path ppp=current_path_+ L"\\template.xlsx";
        std::wstring l=exploer.get_path().wstring();
        l+=L".xlsx";
        std::filesystem::path ppp1=l;
        try {
            std::filesystem::copy_file( ppp,ppp1);
        } catch(std::filesystem::filesystem_error& e) {
            std::cout << "Could not copy sandbox/abc: " << e.what() << '\n';
        }
    }

}


extern "C" {
    void set_data_interface(std::string data,std::string name){
        if(data.size()){
           data_input.data[name]=std::stof(data);
        }
        if(name=="Day_input" || name=="Monthe_input" || name=="Yers_input" || name=="Times_input"){
            auto in=data_input.get_input_data();
            in.tm_end.tm_year=in.tm_end.tm_year-in.tm_start.tm_year-1900+data_input.data["Yers_input"];
            in.tm_start.tm_year=-1900+data_input.data["Yers_input"];

            std::stringstream start;
            start<<std::put_time(&in.tm_start, "start : %Y.%m.%d");

            std::stringstream finish;
            finish<<std::put_time(&in.tm_end, "finish: %Y.%m.%d");
            data_input.main_parametr["start_time"].launch(start.str());
            data_input.main_parametr["end_time"].launch(finish.str());
        }
        std::cout<<name<<": "<<data_input.data[name]<<std::endl;
    }

    void set_data_interface_cout(std::string data,std::string name){
        if(data.size()){
           data_input.data[name]=std::stof(data);
        }
        if(name.substr(0,3)=="FEU"){
            data_input.n_equpment[0][std::stoi(std::string{name.back()})-1]=std::stof(data);
        }
        if(name.substr(0,3)=="VEU"){
            data_input.n_equpment[1][std::stoi(std::string{name.back()})-1]=std::stof(data);
        }
        if(name.substr(0,3)=="GEU"){
            data_input.n_equpment[2][std::stoi(std::string{name.back()})-1]=std::stof(data);
        }
        if(data_input.data_down[0].size()){
            data_input.model_cost_launch();
        }
    }

    void cost_parametr(std::function<void(std::string)>fun_,std::string name){
        data_input.cost_parametr[name].fun=fun_;
    }

    void main_parametr(std::function<void(std::string)>fun_,std::string name){
        data_input.main_parametr[name].fun=fun_;
    }

    void set_table_1(std::function<void(std::string)>fun_,std::string name){
        //"tabl_2_1"
        std::cout<<std::stoi(std::string{name[5]})<<"|"<<std::stoi(std::string{name.substr(7)})<<std::endl;
        data_input.table_1[std::stoi(std::string{name[5]})-2][std::stoi(std::string{name.substr(7)})-1].fun=fun_;
    }
    void set_table_2(std::function<void(std::string)>fun_,std::string name){
        //"tabl_2_1"
        std::cout<<"||||||||||"<<std::string{{name[6],name[7]}}<<"|"<<name.substr(8)<<std::endl;
        if(('1'<=name[6] && name[6]<='9')&& ( '0'<=name[7] && name[7]<='9')){
            data_input.table_2[std::stoi(std::string{{name[6],name[7]}})-1][std::stoi(std::string{name.substr(9)})-1].fun=fun_;
        }else {
            data_input.table_2[std::stoi(std::string{name[6]})-1][std::stoi(std::string{name.substr(8)})-1].fun=fun_;
        }
    }

    void set_grafics(std::function<void(std::vector<float> &&,uint32_t,std::string,std::string,std::string,std::string)> fun_,std::string name){
        data_input.grafic.fun=fun_;
    }

    void set_grafics_win(std::function<void(std::vector<float> &&,uint32_t,std::string,std::string,std::string,std::string)> fun_,std::string name){
        data_input.grafics_win[name].fun=fun_;
    }
}
