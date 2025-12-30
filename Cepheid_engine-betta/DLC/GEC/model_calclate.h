#ifndef MODEL_CALCLATE_H
#define MODEL_CALCLATE_H
#include<vector>
#include <iostream>
#include <ctime>
#include <numeric>
#include <cmath>
#include <tuple_utl.h>
#include <algorithm>

struct Storage_capacity{
    int charding_time{0};
    int charding_time_do=0;
    bool charding_deferent=0;

    float V_NTE=0;
    float V_Acum=0;
    float V_SC=0;
    float V_VSAE=0;

    std::vector<float> V_NTE_end_month;
    std::vector<float> V_Acum_end_month;
    std::vector<float> V_SC_end_month;
    std::vector<float> V_VSAE_end_month;

    std::vector<float> max_V_NTE;
    std::vector<float> max_V_Acum;
    std::vector<float> max_V_SC;
    std::vector<float> max_V_VSAE;

    bool charding_SC=true;
    bool charding_Li_in=true;
    bool charding_VSAE=true;

    int on_DGU=0;
    bool on_off_dgu=false;
    int timer=0;

    void calculate(std::size_t &current_month){
        if(V_SC>max_V_SC[current_month]){
            max_V_SC[current_month]=V_SC;
        }
        V_SC_end_month[current_month]=V_SC;

        if(V_Acum>max_V_Acum[current_month]){
            max_V_Acum[current_month]=V_Acum;
        }
        V_Acum_end_month[current_month]=V_Acum;

        if(V_VSAE>max_V_VSAE[current_month]){
            max_V_VSAE[current_month]=V_VSAE;
        }
        V_VSAE_end_month[current_month]=V_VSAE;

    }
};

inline static float GRAD_RAD=3.14f/180.0f;

enum TYPE_DATA{second, hours};
template<int TYPE>
inline static auto averaging_data(std::vector<float> & data,std::size_t curren_hourse){
    if constexpr (TYPE ==second){
        return static_cast<float>(rand())/static_cast<float>(RAND_MAX)*(data[curren_hourse+1] -  data[curren_hourse]+0.1f)+data[curren_hourse];
    }
    if constexpr (TYPE ==hours){
        return data[curren_hourse];
    }
}


enum Type_object{VEU_V=2,GEU=1,PMT=3,DIESEL_GENERATOR=4,GNE=0,SUPERCAPACITOR=10,BATTARY_BLOCK=11,VSAE=12,GAS=7,CCT=8,NTE=6,PTE=9,INVERTOR=11,PEE=5,SETTING=2};

class PMT_model{
public:
    struct Input_data{
        float I_max=0.0f;
        float I_kz=0.0f;
        float K_i_t=0.0f;

        float U_max=0.0f;
        float U_xx=0.0f;
        float K_u_t=0.0f;

        float A_PMT=0.0f;
        float ALBEDO=0.0f;

        std::size_t n_PMT=0;
        float N_one=0.0f;

        float KPD=0.0f;

        float cost=0.0f;
        float cost_inv=0.0f;
        float service_life_inv=0.0f;

        float CN=0.0f;
        float service_life=0.0f;
    };

    Input_data input_data;

    float coefficent_BAX=0.0f;
    int  n[12]={17,47,75,105,135,162,198,228,258,288,318,344};
    float K_sum_B[12];

    float const_part_coefficent_PMT=0;

    PMT_model(Input_data input_data_,float latitude,std::vector<std::vector<float>> & data_E0,std::vector<float> data_Eg):
        input_data(input_data_){


        coefficent_BAX=
         input_data.I_max *input_data.U_max/
        (input_data.I_kz*input_data.U_xx);

        const_part_coefficent_PMT=(coefficent_BAX*298.15f/1000.0f)/std::log(std::pow(10.0f,6.0f)*1000.0f)*input_data.n_PMT*input_data.KPD;

        for(std::size_t i=0;i<12;i++){
            float delta_malay=23.45f*std::sin(GRAD_RAD*360.0f/365.0f*(284.0f+n[i]));

            float lat_minus_grad=latitude-input_data.A_PMT;

            float w_g=std::acos(-std::tan(GRAD_RAD*latitude)*std::tan(GRAD_RAD*delta_malay))/GRAD_RAD;
            float w_b=std::min(w_g,std::acos(-std::tan(GRAD_RAD*lat_minus_grad)*std::tan(GRAD_RAD*delta_malay))/GRAD_RAD);


            float K_pi=(std::cos(GRAD_RAD*lat_minus_grad)*std::cos(GRAD_RAD*delta_malay)*std::sin(GRAD_RAD*w_b)+GRAD_RAD*w_b*std::sin(GRAD_RAD*lat_minus_grad)*std::sin(GRAD_RAD*delta_malay))/
            //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                             (std::cos(GRAD_RAD*latitude)*std::cos(GRAD_RAD*delta_malay)*std::sin(GRAD_RAD*w_g)+GRAD_RAD*w_g*std::sin(GRAD_RAD*latitude)*std::sin(GRAD_RAD*delta_malay));


            float E0=data_E0[static_cast<std::size_t>(latitude/5)][i];


            float K_air=data_Eg[i]/E0;

            float Kgd=1.39f-4.03f*K_air+5.53f*std::pow(K_air,2.0f)-3.11f*std::pow(K_air,3.0f)+0.13f ;
            //(reg_rad[current_region].diffuse_radiation[i.month])/(reg_rad[current_region].amount_diffuse_radiation[i.month]+reg_rad[current_region].diffuse_radiation[i.month]);

            K_sum_B[i]=(1-Kgd)*K_pi+Kgd*(1+std::cos(GRAD_RAD*input_data.A_PMT))/2.0f+input_data.ALBEDO*(1-std::cos(GRAD_RAD*input_data.A_PMT))/2.0f;

            //std::cout<<std::endl;
//            std::cout<<"delta_malay:"<<delta_malay<<std::endl;
//            std::cout<<"lat_minus_grad:"<<lat_minus_grad<<std::endl;
//            std::cout<<"w_g:"<<w_g <<std::endl;
//            std::cout<<"w_b:"<<w_b<<std::endl;
//            std::cout<<"K_pi:"<<K_pi<<std::endl;
//            std::cout<<"data_Eg:"<<data_Eg[i]<<std::endl;
            //std::cout<<"E0:"<<E0<<std::endl;
           // std::cout<<i+1<<":"<<"K_air:"<<K_air<<std::endl;
//            std::cout<<"Kgd:"<<Kgd<<std::endl;
            //std::cout<<"K_sum_B:"<<K_sum_B[i]<<std::endl;
        }
    }


    inline float launch(float current_rad_colar_second,float current_temp_okr_sred_second,std::size_t current_month){
        float R_B=0;
        float delta_Temp_PMT=0;
        float Temp_PMT=0;

        float coefficent_PMT=0;

        R_B=current_rad_colar_second*K_sum_B[current_month];
        delta_Temp_PMT=6*std::pow(10.0f,-6.0f)*std::pow(current_rad_colar_second,2.0f)+0.021f*current_rad_colar_second-2.39f;

        Temp_PMT= current_temp_okr_sred_second+delta_Temp_PMT+273.15f;
        coefficent_PMT=const_part_coefficent_PMT*(input_data.I_kz+input_data.K_i_t*(Temp_PMT-298.15f))*(input_data.U_xx+input_data.K_u_t*(Temp_PMT-298.15f));

//         if(Diagnostic){
//    //        std::cout<<"0: curren_hourse: "           <<curren_hourse<<std::endl;
//        std::cout<<"1: current_rad_colar_second: "       <<current_rad_colar_second<<std::endl;
//        std::cout<<"2: current_month: "    <<current_month<<std::endl;
 //       std::cout<<"3: PMT: "   <<coefficent_PMT*R_B*1.0f*std::log(std::pow(10.0f,6.0f)*R_B)/ Temp_PMT<<std::endl;
//    //        std::cout<<"4: Temp_okr_sred_current_pp: "<<temp_okr_sred_current<<std::endl;
//            std::cout<<"5: current_rad_colar_second: "<<current_rad_colar_second<<std::endl;
//    //        std::cout<<"6: current_temp_okr_sred_second: "<<current_temp_okr_sred_second<<std::endl;
//            std::cout<<"7: R_B: "<<R_B<<std::endl;
//    //        std::cout<<"8: Temp_PMT: "<<Temp_PMT<<std::endl;
//    //        std::cout<<"9: coefficent_PMT: "<<coefficent_PMT<<std::endl;
//        }

        if(R_B>0.0001f){
            return (coefficent_PMT*R_B*1.0f*std::log(std::pow(10.0f,6.0f)*R_B)/ Temp_PMT);
        }else{
            return 0.0f;
        }
    }
};


class VEU_model{


    float K_otk=0.0f;
    float h_priv=0.0f;
    float const_part_N_VIE=0.0f;
    float delta_VEU=0.0f;

    float  N_VIE_past=0.0f;
public:
    struct Input_data{
        float Class=0.0f;
        float Nnom=0.0f;
        float H_fluger=0.0f;
        float H_vk=0.0f;
        float Inerc_const=0.0f;
        float V_rach=0.0f;
        float Min_V=0.0f;
        float Max_V=0.0f;

        std::size_t n_VEU=0;

        float cost=0.0f;
        float CN=0.0f;
        float service_life=0.0f;
    };

    Input_data input_data;

    VEU_model(Input_data input_data_):input_data(input_data_){
        K_otk=0.2113f*input_data.Class+0.155f;
        h_priv=input_data.H_vk/input_data.H_fluger;

        const_part_N_VIE=input_data.Nnom/(input_data.V_rach-input_data.Min_V);
        delta_VEU=input_data.Nnom/input_data.Inerc_const;
    }

    template<int TYPE>
    inline float launch(float speed_veter_f){
        float m=0;
        float speed_vetrak=0;
        float N_VIE=0;

        //по секундно
        m=0.5551f*std::pow(speed_veter_f,-0.7275f);
        speed_vetrak=speed_veter_f*K_otk*std::pow(h_priv,m);

        if(input_data.Min_V<=speed_vetrak && speed_vetrak<=input_data.V_rach){
            N_VIE=const_part_N_VIE*(speed_vetrak-input_data.Min_V)*input_data.n_VEU*1000.0f;
        }

        if(input_data.V_rach<=speed_vetrak && speed_vetrak <=input_data.Max_V){
             N_VIE=input_data.Nnom*1000.0f*input_data.n_VEU;
        }

        if( speed_vetrak<input_data.Min_V || input_data.Max_V<speed_vetrak){
             N_VIE=0;
        }

//        if(Diagnostic){
//            std::cout<<"3: speed_veter: "            <<speed_veter_f<<std::endl;
//            std::cout<<"4: m: "                      <<m<<std::endl;
//            std::cout<<"5: speed_vetrak: "           <<speed_vetrak<<std::endl;
//            std::cout<<"6: N_VIE: "                  <<N_VIE<<std::endl;
//        }


        if constexpr(TYPE==second){
            if(std::abs(N_VIE_past-N_VIE)>delta_VEU*1000.0f){
                N_VIE=N_VIE_past-std::copysign(delta_VEU*1000.0f,N_VIE_past-N_VIE);
            }
            N_VIE_past=N_VIE;
        }

        return  N_VIE;
    }
};

class PTE_model{
public:
    inline float launch(float data){
        return data*1000.0f;
    }
};

class PEE_model{
public:
    inline float launch(float data){
        return data*1000.0f;
    }
};



class CCT_model{
    int  n[12]={17,47,75,105,135,162,198,228,258,288,318,344};
    float K_sum_B[12];


public:
    struct Input_data{
        float S=0.0f;
        float K_passing=0.0f;
        float K_absorbing=0.0f;
        float K_T_CK=0.0f;
        float T_C_CK=0.0f;

        float A_PMT=0.0f;
        float ALBEDO=0.0f;

        float KPD=0.0f;
    };
    Input_data input_data;


    CCT_model(Input_data input_data_,float latitude,std::vector<std::vector<float>> & data_E0,std::vector<float> data_Eg):
        input_data(input_data_){
        for(std::size_t i=0;i<12;i++){
            float delta_malay=23.45f*std::sin(GRAD_RAD*360.0f/365.0f*(284.0f+n[i]));

            float lat_minus_grad=latitude-input_data.A_PMT;

            float w_g=std::acos(-std::tan(GRAD_RAD*latitude)*std::tan(GRAD_RAD*delta_malay))/GRAD_RAD;
            float w_b=std::min(w_g,std::acos(-std::tan(GRAD_RAD*lat_minus_grad)*std::tan(GRAD_RAD*delta_malay))/GRAD_RAD);


            float K_pi=(std::cos(GRAD_RAD*lat_minus_grad)*std::cos(GRAD_RAD*delta_malay)*std::sin(GRAD_RAD*w_b)+GRAD_RAD*w_b*std::sin(GRAD_RAD*lat_minus_grad)*std::sin(GRAD_RAD*delta_malay))/
            //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                             (std::cos(GRAD_RAD*latitude)*std::cos(GRAD_RAD*delta_malay)*std::sin(GRAD_RAD*w_g)+GRAD_RAD*w_g*std::sin(GRAD_RAD*latitude)*std::sin(GRAD_RAD*delta_malay));


            float E0=data_E0[static_cast<std::size_t>(latitude/5)][i];


            float K_air=data_Eg[i]/E0;

            float Kgd=1.39f-4.03f*K_air+5.53f*std::pow(K_air,2.0f)-3.11f*std::pow(K_air,3.0f)+0.13f ;
            //(reg_rad[current_region].diffuse_radiation[i.month])/(reg_rad[current_region].amount_diffuse_radiation[i.month]+reg_rad[current_region].diffuse_radiation[i.month]);

            K_sum_B[i]=(1-Kgd)*K_pi+Kgd*(1+std::cos(GRAD_RAD*input_data.A_PMT))/2.0f+input_data.ALBEDO*(1-std::cos(GRAD_RAD*input_data.A_PMT))/2.0f;

            //std::cout<<std::endl;
//            std::cout<<"delta_malay:"<<delta_malay<<std::endl;
//            std::cout<<"lat_minus_grad:"<<lat_minus_grad<<std::endl;
//            std::cout<<"w_g:"<<w_g <<std::endl;
//            std::cout<<"w_b:"<<w_b<<std::endl;
//            std::cout<<"K_pi:"<<K_pi<<std::endl;
//            std::cout<<"data_Eg:"<<data_Eg[i]<<std::endl;
            //std::cout<<"E0:"<<E0<<std::endl;
           // std::cout<<i+1<<":"<<"K_air:"<<K_air<<std::endl;
//            std::cout<<"Kgd:"<<Kgd<<std::endl;
            //std::cout<<"K_sum_B:"<<K_sum_B[i]<<std::endl;
        }
    }

    inline float launch(float current_rad_colar_second,float current_temp_okr_sred_second,std::size_t current_month){

        float R_B=0;
        float delta_Temp_PMT=0;
        float Temp_CCT=0;


        R_B=current_rad_colar_second*K_sum_B[current_month];
        delta_Temp_PMT=6.0f*std::pow(10.0f,-6.0f)*std::pow(current_rad_colar_second,2.0f)+0.021f*current_rad_colar_second-2.39f;

    //    Temp_okr_sred= input[SETTING][0].date.value+0.5*input[SETTING][1].date.value*
    //                    std::cos(3.14*2.0f*GRAD_RAD*( input[SETTING][3].date.value- input[SETTING][2].date.value)/(i.hours*1.0f));

        Temp_CCT= current_temp_okr_sred_second+delta_Temp_PMT+273.15f;

//        if(Diagnostic){
//            std::cout<<"5: current_rad_colar_second: "<<current_rad_colar_second<<std::endl;
//            std::cout<<"6: current_temp_okr_sred_second: "<<current_temp_okr_sred_second<<std::endl;
//            std::cout<<"7: R_B: "<<R_B<<std::endl;
//            std::cout<<"8: Temp_CCTT: "<<Temp_CCT<<std::endl;
//            std::cout<<"9: coefficent_PMT: "<<coefficent_PMT<<std::endl;
//        }

        if(R_B>0.0001f){
            return (input_data.S*(R_B*input_data.K_passing*input_data.K_absorbing-input_data.K_T_CK*(input_data.T_C_CK- Temp_CCT))*input_data.KPD);
        }else{
            return 0.0f;
        }
    }
};


class NTE_model{
public:
    struct Input_data{
        float KPD=0.0f;
    };
    Input_data input_data;


    NTE_model(Input_data input_data_):input_data(input_data_){}

    inline float launch(std::vector<std::vector<float>> &out_data,Storage_capacity &storage_capacity,std::size_t curren_time,std::size_t current_month){
        float W=0.0f;
        W=(out_data[PTE][curren_time]-out_data[CCT][curren_time]);

        if(W<0){
          W=W*input_data.KPD;
        }

        storage_capacity.V_NTE=storage_capacity.V_NTE-W/3600;

        if(storage_capacity.V_NTE<0){
            W=0;
        }

        if(storage_capacity.V_NTE<=0){
           storage_capacity.V_NTE=0;
        }


        //статистика
        if(storage_capacity.V_NTE>storage_capacity.max_V_NTE[current_month]){
            storage_capacity.max_V_NTE[current_month]=storage_capacity.V_NTE;
        }
        storage_capacity.V_NTE_end_month[current_month]=storage_capacity.V_NTE;

        return W;
    }
};



class EK_model{


public:
    struct Input_data{
        float KPD=0.0f;
    };
    Input_data input_data;

    EK_model(Input_data input_data_):input_data(input_data_){}

    inline float launch(std::vector<std::vector<float>> &out_data,std::size_t curren_time){
        float N=(out_data[PTE][curren_time]-out_data[CCT][curren_time]-out_data[NTE][curren_time])/input_data.KPD;
        if(N>0){
            return N;
        }else{
            return 0;
        }
    }
};



class GEU_model{

    float N_GEU_past=0.0f;
public:
    struct Input_data{
        float H=0.0f;
        float KPD=0.0f;
        float N_yst=0.0f;
        float N_min=0.0f;
        float N_speed_max=0.0f;

        std::size_t n_GEU=0;

        float cost=0.0f;
        float service_life=0.0f;
    };
    Input_data input_data;

    GEU_model(Input_data input_data_):input_data(input_data_){}

    //по часам всегда
    template<int TYPE>
    inline float launch(float expenditure,std::vector< std::vector<float>> &out_data,std::size_t curren_time){
        float N_GEU_1=0;
        float N_GEU_2=0;
        float N_GEU=0;

        N_GEU_1=out_data[PEE][curren_time]+out_data[GAS][curren_time]-out_data[PMT][curren_time]-out_data[VEU_V][curren_time];
        N_GEU_2=9.81f*expenditure*input_data.H*input_data.KPD*1000.0f;

        //std::cout<<N_GEU_2<<std::endl;
        N_GEU=std::min(N_GEU_1,std::min(N_GEU_2,input_data.N_yst*1000.0f)*input_data.n_GEU);
        if(input_data.N_min>N_GEU){
            N_GEU=0;
            return N_GEU;
        }



        if constexpr(TYPE==second){
            if(std::abs(N_GEU_past-N_GEU)>input_data.N_speed_max*1000.0f){
                N_GEU=N_GEU_past-std::copysign(input_data.N_speed_max*1000.0f,N_GEU_past-N_GEU);
            }

            N_GEU_past=N_GEU;
        }

        return N_GEU;
    }
};




struct N_capasiter {
    float N_SC=0;
    float N_BB=0;
    float N_VSAE=0;
};

class GNE_model{
public:
    struct Input_data{

        float KPD_VSAE=0.0f;
        float KPD_BB=0.0f;
        float KPD_SC=0.0f;

        float cost_VSAE=0.0f;
        float service_life_VSAE=0.0f;
    };
    Input_data input_data;

    GNE_model(Input_data input_data_):input_data(input_data_){}

    inline N_capasiter launch(std::vector<std::vector<float>> &out_data,Storage_capacity &storage_capacity,std::size_t curren_time){

        N_capasiter n_capasiter;

        storage_capacity.charding_time_do=storage_capacity.charding_time;


        float N_Potreb=0;
        float N_Generate=0;

        N_Potreb=out_data[PEE][curren_time]+out_data[GAS][curren_time];
        N_Generate=-out_data[PMT][curren_time]-out_data[VEU_V][curren_time]-out_data[GEU][curren_time];



        float N=N_Potreb+N_Generate;
        if(N<0 && !storage_capacity.charding_SC){
            storage_capacity.charding_time=0;
            storage_capacity.charding_SC=true;
            storage_capacity.charding_Li_in=true;
            storage_capacity.charding_VSAE=true;
        }
        if(N>0 && storage_capacity.charding_SC){
            storage_capacity.charding_time=0;
            storage_capacity.charding_SC=false;
            storage_capacity.charding_Li_in=false;
            storage_capacity.charding_VSAE=false;
        }


        if( storage_capacity.charding_time<60*5 ){
            if(storage_capacity.charding_SC){
                n_capasiter.N_SC=N;
            }
            if(!storage_capacity.charding_SC && storage_capacity.V_SC>0){
               n_capasiter.N_SC=N;
               float N_ex=storage_capacity.V_SC-n_capasiter.N_SC/3600.0f;
               if(N_ex<0){
                   n_capasiter.N_SC=storage_capacity.V_SC*3600.0f;
               }
            }
            if(!storage_capacity.charding_SC && storage_capacity.V_SC<=0){
                if(!storage_capacity.charding_deferent){
                    storage_capacity.charding_time=60*5;
                }
                n_capasiter.N_SC=0;
            }

            if(static_cast<int>(input_data.KPD_SC*100)==0){
                storage_capacity.charding_time=60*5;
                n_capasiter.N_SC=0;
            }

            n_capasiter.N_BB=0.0f;
            n_capasiter.N_VSAE=0.0f;
        }




        if((60*5<= storage_capacity.charding_time &&  storage_capacity.charding_time<60*60*3)||((static_cast<int>(input_data.KPD_SC*100)==0) && (storage_capacity.charding_time>=60*60*3))){
            if(storage_capacity.charding_Li_in){
                n_capasiter.N_BB=N;
            }
            if( !storage_capacity.charding_Li_in && storage_capacity.V_Acum>=0){
                n_capasiter.N_BB=N;
                float N_ex=storage_capacity.V_Acum-n_capasiter.N_BB/3600.0f;
                if(N_ex<0){
                    n_capasiter.N_BB=storage_capacity.V_Acum*3600.0f;
                }
            }
            if(!storage_capacity.charding_Li_in && storage_capacity.V_Acum<0){
                if(!storage_capacity.charding_deferent){
                    storage_capacity.charding_time=60*60*3;
                }
            }
            if(static_cast<int>(input_data.KPD_BB*100)==0){
                storage_capacity.charding_time=60*60*3;
                n_capasiter.N_BB=0;
            }
            n_capasiter.N_VSAE=0.0f;
        }



        if((storage_capacity.charding_time>=60*60*3)||((static_cast<int>(input_data.KPD_SC*100)==0) && static_cast<int>(input_data.KPD_BB*100)==0)){

            if( storage_capacity.charding_VSAE) {
                n_capasiter.N_VSAE=N;
                //std::cout<<N_Potreb<<"|"<<n_capasiter.N_VSAE<<std::endl;
            }
            if(!storage_capacity.charding_VSAE && storage_capacity.V_VSAE >0){
               n_capasiter.N_VSAE=N;
               float N_ex=storage_capacity.V_VSAE-n_capasiter.N_VSAE/3600.0f;
               if(N_ex<0){

                   n_capasiter.N_VSAE=storage_capacity.V_VSAE*3600.0f;
               }
            }

            if(static_cast<int>(input_data.KPD_VSAE*100)==0){
               n_capasiter.N_VSAE=0;
               //std::cout<<":::::::::"<<std::endl;
            }

            if(!storage_capacity.charding_VSAE && storage_capacity.V_VSAE<0 ){
                //std::cout<<"############"<<std::endl;
                n_capasiter.N_VSAE=0.0f;

            //     std::cout<<"[[[["<<"|||||||||||"<<"]]]]"<<std::endl;
            }
        }



        // std::cout<<N<<" :storage_capacity.charding_time: "<<storage_capacity.charding_time<<std::endl;
        //std::cout<<"storage_capacity.V_SC:"<<storage_capacity.V_SC<<std::endl;
       // std::cout<<"storage_capacity.V_Acum:"<<storage_capacity.V_Acum<<std::endl;
        //std::cout<<"storage_capacity.V_VSAE:"<<storage_capacity.V_VSAE<<std::endl;

        //std::cout<<"|E_N_SC:"<<n_capasiter.N_SC<<"|E_N_Li_in:"<<n_capasiter.N_BB<<"|E_N_VSAE:"<<n_capasiter.N_VSAE<<std::endl;

        return  n_capasiter;
    }
};


class DGY_model{
    float N_DGY_past=0.0f;
public:
    struct Input_data{
        float DGY_CN=0.0f;
        float DGY_Nyst=0.0f;
        float DGY_speed=0.0f;

        float Q_disel=0.0f;

        float service_life=0.0f;
        float cost=0.0f;
        float cost_disel=0.0f;
    };
    Input_data input_data;

    DGY_model(Input_data input_data_):input_data(input_data_){}

    template<int TYPE>
    inline float launch(std::vector<std::vector<float>> &out_data,Storage_capacity &storage_capacity,std::size_t current_time){

        float N_Potreb=0;
        float N_Generate=0;
        float N_Copasiter=0;
        float N_DGU=0;

//        float N_exess=0;
//        bool  compens=false;
        bool  izlishec=false;

        N_Potreb=out_data[PEE][current_time]+out_data[GAS][current_time];
        N_Generate=-out_data[PMT][current_time]-out_data[VEU_V][current_time]-out_data[GEU][current_time];
        N_Copasiter=-(out_data[SUPERCAPACITOR][current_time]+out_data[BATTARY_BLOCK][current_time]+out_data[VSAE][current_time]);

        N_DGU=N_Potreb+N_Generate+N_Copasiter;
        if(N_DGU>0.1f){
            if(storage_capacity.on_off_dgu){
                storage_capacity.on_DGU++;
                //std::cout<<N_DGU<<" storage_capacity.on_DGU:"<<storage_capacity.on_DGU<<std::endl;
            }
            N_DGU+=input_data.DGY_CN*input_data.DGY_Nyst*1000.0f;

            //std::cout<<storage_capacity.timer<<"||||||||||||||||| "<<storage_capacity.on_DGU<<std::endl;
            storage_capacity.on_off_dgu=false;

        }else{
            storage_capacity.on_off_dgu=true;
            //std::cout<<storage_capacity.timer<<"#########|||\\\\########## "<<storage_capacity.on_DGU<<std::endl;
            if(storage_capacity.on_DGU>2){
                N_DGU=input_data.DGY_CN*input_data.DGY_Nyst*1000.0f;

                if((-(N_Potreb+N_Generate-N_DGU))>(-N_Copasiter)){
                    izlishec=true;
                }
            }else{
                N_DGU=0;
            }
        }

        storage_capacity.timer++;
        if(storage_capacity.timer==24*60*60){
            storage_capacity.on_DGU=0;
            storage_capacity.timer=0;
        }


        if constexpr(TYPE==second){
            if((N_DGY_past-N_DGU)>input_data.DGY_speed*1000.0f){
                N_DGU=N_DGY_past-input_data.DGY_speed*1000.0f;
            };

            N_DGY_past=N_DGU;
        }

        return N_DGU;
    }

};




class Model_Gibrid_complex{
    Storage_capacity storage_capacity;

    PMT_model pmt;
    CCT_model cct;
    VEU_model veu;
    NTE_model nte;
    EK_model  ek;
    GEU_model geu;
    GNE_model gne;
    DGY_model dgy;

    PEE_model pee;
    PTE_model pte;

    std::tm tm_start={};
    std::tm tm_end={};

    float yers_cost_model=0.0f;
    float ee_cost=0.0f;
    float discount_rate=0.0f;
    float income_tax=0.0f;
public:
    struct Input_data{
        PMT_model::Input_data pmt_input_data;
        VEU_model::Input_data veu_input_data;
        CCT_model::Input_data cct_input_data;
        NTE_model::Input_data nte_input_data;
        EK_model:: Input_data ek_input_data;
        GEU_model::Input_data geu_input_data;
        GNE_model::Input_data gne_input_data;
        DGY_model::Input_data dgy_input_data;

        float latitude;
        std::tm tm_start;
        std::tm tm_end;

        //придумать хранилище
        float yers_cost_model=0.0f;
        float ee_cost=0.0f;
        float discount_rate=0.0f;
        float income_tax=0.0f;
    };

    Model_Gibrid_complex(Input_data input_data,std::vector<std::vector<float>> & data_E0,std::vector<float> data_Eg):
    pmt(input_data.pmt_input_data,input_data.latitude,data_E0,data_Eg),
    cct(input_data.cct_input_data,input_data.latitude,data_E0,data_Eg),
    veu(input_data.veu_input_data),
    nte(input_data.nte_input_data),
    ek (input_data. ek_input_data),
    geu(input_data.geu_input_data),
    gne(input_data.gne_input_data),
    dgy(input_data.dgy_input_data),
    tm_start(input_data.tm_start),
    tm_end  (input_data.tm_end),
    yers_cost_model(input_data.yers_cost_model),
    ee_cost        (input_data.ee_cost),
    discount_rate  (input_data.discount_rate),
    income_tax     (input_data.income_tax)
    {
        for (int i=0;i<12;i++) {
            storage_capacity.max_V_SC.push_back(-0.0001f);
            storage_capacity.max_V_Acum.push_back(-0.0001f);
            storage_capacity.max_V_VSAE.push_back(-0.0001f);

            storage_capacity.max_V_NTE.push_back(-0.0001f);

            storage_capacity.V_SC_end_month.push_back(-0.0001f);
            storage_capacity.V_Acum_end_month.push_back(-0.0001f);
            storage_capacity.V_VSAE_end_month.push_back(-0.0001f);

            storage_capacity.V_NTE_end_month.push_back(-0.0001f);
        }
    }


public:
    enum dowmold_data{N_PEE=0,N_PTE=1,SOLAR=2,VETER=3,WATER=4,TEMPERATURE=5};
    template<int TYPE>
    inline std::vector<std::vector<float>> model_calculet(std::vector<std::vector<float>> & data_downold,bool choise[10]){
        std::vector<std::vector<float>> out_data(13);

        std::size_t step_ = 1;
        if constexpr(TYPE==second){
            step_ = 1;
        }
        if constexpr(TYPE==hours){
            step_ = 3600;
        }
        const std::size_t step=step_;


        N_capasiter N;


        auto begin=std::mktime(&tm_start);
        auto end=std::mktime(&tm_end);
        auto distanse=end-begin;

        std::size_t size_buffer=distanse/step+1;

        for (auto &T:out_data) {
            T.resize(size_buffer);
        }

        for (; begin< end; begin+=step) {
            std::size_t i=static_cast<std::size_t>(distanse-(end-begin));
            //std::size_t current_month=(static_cast<std::size_t>(tm_start.tm_mon)+tutl::to_month(i/86400))%12;
            std::size_t current_month=(tutl::to_month((tutl::to_day((static_cast<std::size_t>(tm_start.tm_mon)))+i/86400)%365))%12;
            std::size_t current_horse=((tutl::to_day(static_cast<std::size_t>(tm_start.tm_mon))+static_cast<std::size_t>(tm_start.tm_mday))*24+i/3600-1)%8760+1;

            i=i/step;

            float solar_R=averaging_data<TYPE>(data_downold[SOLAR],current_horse);
            float temperature=averaging_data<TYPE>(data_downold[TEMPERATURE],current_horse);
            float wind_speed=averaging_data<TYPE>(data_downold[VETER],current_horse);

            float pee_N=averaging_data<TYPE>(data_downold[N_PEE],current_horse);
            float pte_N=averaging_data<TYPE>(data_downold[N_PTE],current_horse);



            out_data[PEE][i]=pee.launch(pee_N);//+
            out_data[PTE][i]=pte.launch(pte_N);//+



            if(choise[PMT]){
                out_data[PMT][i]=pmt.launch(solar_R,temperature,current_month);//+
                //std::cout<<out_data[PMT].back()<<std::endl;
            }else{
                out_data[PMT][i]=0.0f;
            }

            if(choise[VEU_V]){
                out_data[VEU_V][i]=veu.launch<TYPE>(wind_speed);//+-
            }else{
                out_data[VEU_V][i]=0.0f;
            }

            if(choise[CCT]){
                out_data[CCT][i]=cct.launch(solar_R,temperature,current_month);//+
            }else{
                out_data[CCT][i]=0.0f;
            }

            if(out_data[PEE].size()==1){
                out_data[PEE][0]=(0.0f);//+
                out_data[PTE][0]=(0.0f);//+
                out_data[PMT][0]=(0.0f);
                out_data[VEU_V][0]=(0.0f);
                out_data[CCT][0]=(0.0f);
            }
        }



        std::cout<<out_data[PEE].size()<<" DD: "<<storage_capacity.V_NTE<<std::endl;


        begin=std::mktime(&tm_start);
        for (; begin< end; begin+=step) {
            std::size_t i=static_cast<std::size_t>(distanse-(end-begin));
            //std::size_t current_month=(static_cast<std::size_t>(tm_start.tm_mon)+tutl::to_month(i/86400))%12;
            std::size_t current_month=(tutl::to_month((tutl::to_day((static_cast<std::size_t>(tm_start.tm_mon)))+i/86400)%365))%12;
            std::size_t current_horse=((tutl::to_day(static_cast<std::size_t>(tm_start.tm_mon))+static_cast<std::size_t>(tm_start.tm_mday))*24+i/3600-1)%8760+1;

            i=i/step;

            float water_Q=averaging_data<TYPE>(data_downold[WATER],current_horse);

            if(choise[NTE]){
                out_data[NTE][i]=nte.launch(out_data,storage_capacity,i,current_month);//+(CCT)
            }else{
                out_data[NTE][i]=0.0f;
            }

            if(choise[GAS]){
                out_data[GAS][i]=ek.launch(out_data,i);//+(CCT, НТЕ)
            }else{
                out_data[GAS][i]=0.0f;
            }

            if(choise[GEU]){
                out_data[GEU][i]=geu.launch<TYPE>(water_Q,out_data,i);//+-
            }else{
                out_data[GEU][i]=0.0f;
            }

            if(choise[GNE]){
                N=gne.launch(out_data,storage_capacity,i);//(PMT, VEU, GEU )/++
                out_data[SUPERCAPACITOR][i]=N.N_SC;
                out_data[BATTARY_BLOCK][i]=N.N_BB;
                out_data[VSAE][i]=N.N_VSAE;
            }else{
                out_data[SUPERCAPACITOR][i]=0.0f;
                out_data[BATTARY_BLOCK][i]=0.0f;
                out_data[VSAE][i]=0.0f;
            }

            if(choise[DIESEL_GENERATOR]){
                out_data[DIESEL_GENERATOR][i]=dgy.launch<TYPE>(out_data,storage_capacity,i);
            }else{
                out_data[DIESEL_GENERATOR][i]=0.0f;
            }


            if(out_data[SUPERCAPACITOR][i]<0){
                storage_capacity.V_SC+=-out_data[SUPERCAPACITOR][i]/(3600.0f/(step*1.0f));
            }else {
                if(static_cast<int>(gne.input_data.KPD_SC*100)!=0){
                    storage_capacity.V_SC+=-out_data[SUPERCAPACITOR][i]/((3600.0f/(step*1.0f))*gne.input_data.KPD_SC);
                }
            }
            if(out_data[BATTARY_BLOCK][i]<0){
                storage_capacity.V_Acum+=-out_data[BATTARY_BLOCK][i]/(3600.0f/(step*1.0f));
            }else {
                if(static_cast<int>(gne.input_data.KPD_BB*100)!=0){
                    storage_capacity.V_Acum+=-out_data[BATTARY_BLOCK][i]/((3600.0f/(step*1.0f))*gne.input_data.KPD_BB);
                }
            }
            if(out_data[VSAE][i]<0){
                 std::cout<<out_data[VSAE].back()<<" :storage_capacity.charding_time: "<<storage_capacity.charding_time<<std::endl;
               storage_capacity.V_VSAE+=-out_data[VSAE][i]/(3600.0f/(step*1.0f));
            }else {
                if(static_cast<int>(gne.input_data.KPD_VSAE*100)!=0){
                    storage_capacity.V_VSAE+=-out_data[VSAE][i]/((3600.0f/(step*1.0f))*gne.input_data.KPD_VSAE);
                }
            }

            storage_capacity.charding_time+=step;

            //std::cout<<storage_capacity.charding_time<<std::endl;
            storage_capacity.calculate(current_month);
            i=i+1;
            if(out_data[NTE].size()==1){
                out_data[NTE][0]=(0.0f);
                out_data[GAS][0]=(0.0f);//+
                out_data[SUPERCAPACITOR][0]=(0.0f);
                out_data[BATTARY_BLOCK][0]=(0.0f);
                out_data[VSAE][0]=(0.0f);

                out_data[DIESEL_GENERATOR][0]=(0.0f);
            }
        }
        std::cout<<"DD"<<std::endl;
        return  out_data;
    }

    template<int TYPE>
    std::vector<std::vector<float>> indicators_v1(std::vector<std::vector<float>> & out_data){
         std::vector<std::vector<float>> data;

         for (std::size_t i=0;i<21;i++) {
             data.emplace_back(std::vector<float>{});
             for (std::size_t j=0;j<14;j++){
                 data[i].emplace_back(0.0f);
             }
         }

         for (auto &T:data) {
             for (auto &J:T) {
                 J=0.0f;
             }
         }


         std::size_t step_ = 1;
         float delit=0;
         if constexpr(TYPE==second){
             delit = 3600.0f*1000.0f;
             step_ = 1;
         }
         if constexpr(TYPE==hours){
             delit = 1000.0f;
             step_ = 3600;
         }
         const std::size_t step=step_;

         auto begin=std::mktime(&tm_start);
         auto end=std::mktime(&tm_end);
         auto distanse=end-begin;


         float max_N_dgy[13];
         for (auto &J:max_N_dgy) {
             J=0.0f;
         }

         std::size_t i=0;
         for (; begin< end; begin+=step) {
             i=static_cast<std::size_t>(distanse-(end-begin));
             //std::size_t current_horse=i/3600;
             //std::size_t current_month=(static_cast<std::size_t>(tm_start.tm_mon)+tutl::to_month(i/86400))%12;
             std::size_t current_month=(tutl::to_month((tutl::to_day((static_cast<std::size_t>(tm_start.tm_mon)))+i/86400)%365))%12;
             i=i/step;
             data[0][current_month]+=out_data[PEE][i];
             data[1][current_month]+=out_data[PTE][i];
             data[2][current_month]+=out_data[PMT][i];
             data[3][current_month]+=out_data[VEU_V][i];
             data[4][current_month]+=out_data[GEU][i];
             data[5][current_month]+=out_data[DIESEL_GENERATOR][i];

             data[6][current_month]+=out_data[CCT][i];
             data[7][current_month]+=out_data[GAS][i];

             if(max_N_dgy[current_month]<out_data[DIESEL_GENERATOR][i]){
                 max_N_dgy[current_month]=out_data[DIESEL_GENERATOR][i];
             }
         }



         for(int i=0;i<9;i++){
              for(std::size_t j=0;j<12;j++){
                 data[i][j]=data[i][j]/delit+0.0001f;
                 data[i][12]+=data[i][j];
              }
         }

         for(std::size_t i=0;i<13;i++){
             float time=0.0f;
             if(i!=12){
                 time=(tutl::to_day(i+1)-tutl::to_day(i))*24.0f*step_;
             }else {
                time=365*24.0f*step_;
             }
             data[8][i]=std::max(0.0001f,storage_capacity.max_V_VSAE[i])/1000.0f;
             data[9][i]=std::max(0.0001f,storage_capacity.V_VSAE_end_month[i])/1000.0f;

             data[10][i]=std::max(0.0001f,storage_capacity.max_V_Acum[i])/1000.0f;
             data[11][i]=std::max(0.0001f,storage_capacity.V_Acum_end_month[i])/1000.0f;

             data[12][i]=std::max(0.0001f,storage_capacity.max_V_SC[i])/1000.0f;
             data[13][i]=std::max(0.0001f,storage_capacity.V_SC_end_month[i])/1000.0f;

             data[14][i]=std::max(0.0001f,storage_capacity.max_V_NTE[i])/1000.0f;
             data[15][i]=std::max(0.0001f,storage_capacity.V_NTE_end_month[i])/1000.0f;

             if(i==12){
                 data[8][i]=*std::max_element(storage_capacity.max_V_VSAE.begin(),storage_capacity.max_V_VSAE.end()).base()/1000.0f;
                 data[9][i]=std::max(0.0001f,storage_capacity.V_VSAE_end_month[static_cast<std::size_t>(tm_end.tm_mon)])/1000.0f;

                 data[10][i]=*std::max_element(storage_capacity.max_V_Acum.begin(),storage_capacity.max_V_Acum.end()).base()/1000.0f;
                 data[11][i]=std::max(0.0001f,storage_capacity.V_Acum_end_month[static_cast<std::size_t>(tm_end.tm_mon) ])/1000.0f;

                 data[12][i]=*std::max_element(storage_capacity.max_V_SC.begin(),storage_capacity.max_V_SC.end()).base()/1000.0f;
                 data[13][i]=std::max(0.0001f,storage_capacity.V_SC_end_month[static_cast<std::size_t>(tm_end.tm_mon) ])/1000.0f;

                 data[14][i]=*std::max_element(storage_capacity.max_V_NTE.begin(),storage_capacity.max_V_NTE.end()).base()/1000.0f;
                 data[15][i]=std::max(0.0001f,storage_capacity.V_NTE_end_month[static_cast<std::size_t>(tm_end.tm_mon) ])/1000.0f;
             }

             if(pmt.input_data.N_one*pmt.input_data.n_PMT!=0){
                data[16][i]=data[2][i]/(time*pmt.input_data.N_one*pmt.input_data.n_PMT)*100.0f;
             }
             std::cout<<data[2][i]<<"time*pmt.input_data.N_one*pmt.input_data.n_PMT"<<time*pmt.input_data.N_one*pmt.input_data.n_PMT<<std::endl;

             if(veu.input_data.Nnom*veu.input_data.n_VEU!=0){
                data[17][i]=data[3][i]/(time*veu.input_data.Nnom*veu.input_data.n_VEU)*100.0f;
             }

             if(geu.input_data.N_yst*geu.input_data.n_GEU!=0){
                 data[18][i]=data[4][i]/(time*geu.input_data.N_yst*geu.input_data.n_GEU)*100.0f;
             }
             if(dgy.input_data.DGY_Nyst!=0){
                 data[19][i]=data[5][i]/(time*dgy.input_data.DGY_Nyst)*100.0f;
             }

             data[20][i]=(1-(data[5][i])/data[0][i])*100.0f;
         }




         std::cout<<"L"<<std::endl;
         return data;
    }


    template<int TYPE>
    std::vector<float> indicators_v2(std::vector<std::vector<float>> & out_data){
         std::vector<float> output(17);


        float data[6][13];
        for (auto &T:data) {
            for (auto &J:T) {
                J=0.0f;
            }
        }

        std::cout<<"L"<<std::endl;

        std::size_t step_ = 1;
        float delit=0;
        if constexpr(TYPE==second){
            delit = 3600.0f*1000.0f;
            step_ = 1;
        }
        if constexpr(TYPE==hours){
            delit = 1000.0f;
            step_ = 3600;
        }
        const std::size_t step=step_;

        auto begin=std::mktime(&tm_start);
        auto end=std::mktime(&tm_end);
        auto distanse=end-begin;

        for (; begin< end; begin+=step) {
            std::size_t i=static_cast<std::size_t>(distanse-(end-begin));
            //std::size_t current_horse=i/3600;
            std::size_t current_month=(static_cast<std::size_t>(tm_start.tm_mon)+tutl::to_month(i/86400))%12;
            i=i/step;
            data[0][current_month]+=out_data[PEE][i];
            data[1][current_month]+=out_data[BATTARY_BLOCK][i]+out_data[VSAE][i]+out_data[SUPERCAPACITOR][i];
            data[2][current_month]+=out_data[PMT][i];
            data[3][current_month]+=out_data[VEU_V][i];
            data[4][current_month]+=out_data[GEU][i];
            data[5][current_month]+=out_data[DIESEL_GENERATOR][i];
        }
        std::cout<<"L"<<std::endl;

        for(int i=0;i<6;i++){
             for(std::size_t j=0;j<12;j++){
                data[i][j]=data[i][j]/delit+0.0001f;
                data[i][12]+=data[i][j];
             }
        }
        std::cout<<"L"<<std::endl;

        output[0]=*std::max_element(out_data[DIESEL_GENERATOR].begin(),out_data[DIESEL_GENERATOR].end()).base()/1000.0f;
        output[1]=pmt.input_data.N_one*pmt.input_data.n_PMT;
        output[2]=veu.input_data.Nnom*veu.input_data.n_VEU;
        output[3]=geu.input_data.N_yst*geu.input_data.n_GEU;
        output[4]=(*std::max_element(out_data[PMT].begin(),out_data[PMT].end()).base()/1000.0f)*1.1f;
        std::cout<<"L"<<std::endl;

        output[5]=                   *std::min_element(out_data[VSAE].begin(),out_data[VSAE].end()).base();
        output[5]=std::min(output[5],*std::min_element(out_data[SUPERCAPACITOR].begin(),out_data[SUPERCAPACITOR].end()).base());
        output[5]=std::min(output[5],*std::min_element(out_data[BATTARY_BLOCK].begin(),out_data[BATTARY_BLOCK].end()).base());

//        float a=             *std::min_element(out_data[VSAE].begin(),out_data[VSAE].end()).base();
//        a=std::min(output[5],*std::min_element(out_data[SUPERCAPACITOR].begin(),out_data[SUPERCAPACITOR].end()).base());
//        a=std::min(output[5],*std::min_element(out_data[BATTARY_BLOCK].begin(),out_data[BATTARY_BLOCK].end()).base());

//        output[5]=std::max(output[5],-a);
        output[5]/=-1000.0f;



        output[6]=  *std::max_element(storage_capacity.max_V_VSAE.begin(),storage_capacity.max_V_VSAE.end()).base()+
                    *std::max_element(storage_capacity.max_V_Acum.begin(),storage_capacity.max_V_Acum.end()).base()+
                    *std::max_element(storage_capacity.max_V_SC.begin(),storage_capacity.max_V_SC.end()).base();

//        float b=    *std::min_element(storage_capacity.max_V_VSAE.begin(),storage_capacity.max_V_VSAE.end()).base()+
//                    *std::min_element(storage_capacity.max_V_Acum.begin(),storage_capacity.max_V_Acum.end()).base()+
//                    *std::min_element(storage_capacity.max_V_SC.begin(),storage_capacity.max_V_SC.end()).base();

//        output[6]=std::max(output[5],-b);
        output[6]/=1000.0f;

        std::cout<<"T"<<std::endl;
        output[7]=data[5][12]*dgy.input_data.Q_disel;

        output[8]=data[5][12];
        output[9]=data[2][12];
        output[10]=data[3][12];
        output[11]=data[4][12];
        std::cout<<"T"<<std::endl;
        output[12]=data[2][12]/(8760.0f*output[1])*100.0f;
        output[13]=data[3][12]/(8760.0f*output[2])*100.0f;
        output[14]=data[4][12]/(8760.0f*output[3])*100.0f;
        output[15]=(1-data[5][12]/data[0][12])*100.0f;
        std::cout<<"T"<<std::endl;
        std::cout<<"L"<<data[5][12]<<std::endl;

        output[16]=data[0][12];
        return output;
    }



    struct Data_cost{
        float sum_cost=0.0f;
        float profit_cost=0.0f;
        float pure=0.0f;
        float discont_sum=0.0f;
        float discont_years=0.0f;
        std::vector<float> dis;
    };

    template<int TYPE>
    Data_cost indicators_cost(std::vector<float> & output){
         Data_cost data_cost;

         if(static_cast<std::size_t>(yers_cost_model*1000.0f)==0){
             return data_cost;
         }

         auto cout_bay_to_year=[](std::size_t years,std::size_t old_yers){
             if(0==years){
                 return true;
             }

             if(old_yers==0){
                 return false;
             }

             return (0==years%old_yers);
         };

         std::cout<<"||||0||"<<std::endl;


         //чистая прибыль



std::vector<float> dis_;
         for(std::size_t i=0;i<yers_cost_model;i++){

             float cost_dgy=dgy.input_data.cost*output[0]*cout_bay_to_year(i,dgy.input_data.service_life);

             float cost_pmt=pmt.input_data.cost*pmt.input_data.n_PMT*(1+pmt.input_data.CN)*cout_bay_to_year(i,pmt.input_data.service_life);;
             float cost_veu=veu.input_data.cost*veu.input_data.n_VEU*(1+veu.input_data.CN)*cout_bay_to_year(i,veu.input_data.service_life);;
             float cost_geu=geu.input_data.N_yst*geu.input_data.n_GEU*geu.input_data.cost*cout_bay_to_year(i,geu.input_data.service_life);
             float cost_invertor=pmt.input_data.cost_inv*output[4]*cout_bay_to_year(i,pmt.input_data.service_life_inv);
             float cost_cne=gne.input_data.cost_VSAE*output[6]*cout_bay_to_year(i,gne.input_data.service_life_VSAE);

             float cost_dgy_fuel=dgy.input_data.cost_disel*output[7];
             float sum_cost=cost_dgy+cost_pmt+cost_veu+cost_geu+cost_invertor+cost_cne;


             float sum_profit=(output[16]*ee_cost)*(1-income_tax)- cost_dgy_fuel;

              std::cout<<sum_profit<<":"<<output[7]<<":"<<dgy.input_data.cost_disel<<":"<<std::endl;


             float sum_profit_discont=(sum_profit-sum_cost)*(1/std::pow((1+discount_rate),i+1));
             dis_.emplace_back(sum_profit_discont);
             std::cout<<sum_profit_discont<<std::endl;

             data_cost.sum_cost+=sum_cost;
             data_cost.profit_cost+=sum_profit;
         }

         data_cost.dis.emplace_back(dis_[0]);
        for(std::size_t i=1;i<yers_cost_model;i++){
            data_cost.dis.emplace_back(data_cost.dis[i-1]+dis_[i]);
        }

         float y1=0.0f;
         float y2=0.0f;

         float x1=0.0f;
         float x2=0.0f;


         for (std::size_t i=1;i<yers_cost_model;i++) {
             if(data_cost.dis[i-1]<0 && data_cost.dis[i]>=0){
                 y2=data_cost.dis[i];
                 y1=data_cost.dis[i-1];

                 x1=i-1;
                 x2=i;
                 break;
             }
         }

         float k = (y1 - y2) / (x1 - x2);
         float b = y2 - k * x2;
         data_cost.discont_years=-b/k;//пересечени y=0

         data_cost.discont_sum=data_cost.dis.back();


         data_cost.sum_cost/=1000000.0f;
         data_cost.profit_cost/=1000000.0f*yers_cost_model;
         data_cost.pure=data_cost.profit_cost/data_cost.sum_cost*100.0f;

         data_cost.discont_sum/=1000000.0f;

         for(auto &T:data_cost.dis){
             T/=1000000.0f;
         }

         return data_cost;
    }


};

static auto create_Model_Gibrid_complex(Model_Gibrid_complex::Input_data input_data,std::vector<std::vector<float>> & data_E0,std::vector<std::vector<float>> & data_downold){
    std::vector<float> data_Eg(12);
    std::size_t cout_hourse=0;
    std::size_t  n[12]={17,47,75,105,135,162,198,228,258,288,318,344};

    for (std::size_t i=0;i<12;i++) {
        cout_hourse=n[i]*24;
        for (std::size_t j=cout_hourse;j<(cout_hourse+24);j++) {
            data_Eg[i]+=data_downold[Model_Gibrid_complex::SOLAR][j];
        }
        data_Eg[i]/=1000.0f;
        std::cout<<n[i]<<"::::"<< data_Eg[i]<<std::endl;
    }

    return Model_Gibrid_complex(input_data,data_E0,data_Eg);
}

#endif // MODEL_CALCLATE_H
