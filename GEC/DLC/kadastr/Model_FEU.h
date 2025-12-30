#include <ctime>
#include <numeric>
#include <cmath>
#include <tuple_utl.h>
#include <algorithm>

inline static float GRAD_RAD=3.14f/180.0f;

#include <iostream>

class Model_FEU_v2{


public:
    struct Input_data{
        float B=0.0f;
        float Azimut=0.0f;

        float K_T=0.0f;
        float NOST=47.0f;
        float KPD=0.0f;
        float F=0.0f;
        float E_start=0.0f;

        float latitude=0.0f;
        float longtitude=0.0f;

        float step_B=5.0f;
        float step_Azimut=5.0f;
    };

    Input_data input_data;

    struct Data_K{
        float delta_malay=0.0f;
        float A=0.0f;
        float B=0.0f;
        float C=0.0f;

        float w_z=0.0f;
        float w_b=0.0f;

        float w_z_B=0.0f;
        float w_b_B=0.0f;
    };

    Data_K data_K[366];

    float kpr[8760];

    Model_FEU_v2(Input_data input_data_):input_data(input_data_){

        //(SIN(РАДИАНЫ($E$1))*COS(РАДИАНЫ($B$1))-COS(РАДИАНЫ($E$1))*SIN(РАДИАНЫ($B$1))*COS(РАДИАНЫ($B$2)))*SIN(РАДИАНЫ(C6))
        float a_intermediate=std::sin(GRAD_RAD*input_data.latitude)*std::cos(GRAD_RAD*input_data.B)-
                             std::cos(GRAD_RAD*input_data.latitude)*std::sin(GRAD_RAD*input_data.B)*std::cos(GRAD_RAD*input_data.Azimut);

        //=(COS(РАДИАНЫ($E$1))*COS(РАДИАНЫ($B$1))+SIN(РАДИАНЫ($E$1))*SIN(РАДИАНЫ($B$1))*COS(РАДИАНЫ($B$2)))*COS(РАДИАНЫ(C6))
        float b_intermediate=std::cos(GRAD_RAD*input_data.latitude)*std::cos(GRAD_RAD*input_data.B)+
                             std::sin(GRAD_RAD*input_data.latitude)*std::sin(GRAD_RAD*input_data.B)*std::cos(GRAD_RAD*input_data.Azimut);

        //SIN(РАДИАНЫ($B$1))*SIN(РАДИАНЫ($B$2))*COS(РАДИАНЫ(C6))
        float c_intermediate= std::sin(GRAD_RAD*input_data.B)*std::sin(GRAD_RAD*input_data.Azimut);

        //float lat_minus_grad=input_data.latitude-input_data.B;??
        for(std::size_t i=0;i<366;i++){
            data_K[i].delta_malay=23.45f*std::sin(GRAD_RAD*360.0f/365.0f*(284.0f+i));
            data_K[i].A=a_intermediate*std::sin(GRAD_RAD*data_K[i].delta_malay);
            data_K[i].B=b_intermediate*std::cos(GRAD_RAD*data_K[i].delta_malay);
            data_K[i].C=c_intermediate*std::cos(GRAD_RAD*data_K[i].delta_malay);


            data_K[i].w_z= std::acos(-std::tan(GRAD_RAD*input_data.latitude)*std::tan(GRAD_RAD*data_K[i].delta_malay))/GRAD_RAD;
            data_K[i].w_b=-data_K[i].w_z;

            float abc=std::sqrt(data_K[i].C*data_K[i].C-data_K[i].A*data_K[i].A+data_K[i].B*data_K[i].B);

            //2*ATAN((-F6 - КОРЕНЬ(F6^2 - D6^2 + E6^2)) / (D6 - E6))
            float ABC_minus=2*std::atan((-data_K[i].C-abc)/(data_K[i].A-data_K[i].B))/GRAD_RAD;

            //2*ATAN((-F6 + КОРЕНЬ(F6^2 - D6^2 + E6^2)) / (D6 - E6))
            float ABC_plus= 2*std::atan((-data_K[i].C+abc)/(data_K[i].A-data_K[i].B))/GRAD_RAD;

            data_K[i].w_z_B=std::max(data_K[i].w_z,ABC_minus);
            data_K[i].w_b_B=std::max(data_K[i].w_b,ABC_plus);
            //std::cout<<data_K[i].A<<"|"<<data_K[i].B<<"|"<<data_K[i].C<<"||||"<<ABC_minus*GRAD_RAD<<"|"<<ABC_plus*GRAD_RAD<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& "<<data_K[i].w_z_B<<"|"<<data_K[i].w_b_B<<std::endl;
        };

        int deviation=input_data.longtitude/15;
        if(deviation<0){
            deviation=24+deviation;
        }

        std::cout<<deviation<<"************************************************"<<input_data.latitude<<std::endl;

        std::size_t begin=0+deviation;
        std::size_t end=8760+deviation;


        std::size_t i=0;
        for (; begin <= end; begin+=1) {
            std::size_t current_horse=(begin)%8760;
            std::size_t current_day=(begin/24)%365;
            std::size_t current_horse_to_day=current_horse-current_day*24;

            //МИН(H6; МАКС(I6; 15*(M6-12)))
            float w_1=std::min(data_K[current_day].w_z, std::max(data_K[current_day].w_b, 15.0f*(current_horse_to_day-12.0f)));
            //МИН(H6; МАКС(I6; 15*(N6-12)))
            float w_2=std::min(data_K[current_day].w_z, std::max(data_K[current_day].w_b, 15.0f*(current_horse_to_day+1.0f-12.0f)));

            //МИН(P6; МАКС(Q6; 15*(M6-12)))
            float w_1_B=std::min(data_K[current_day].w_z_B, std::max(data_K[current_day].w_b_B, 15.0f*(current_horse_to_day-12.0f)));
            //МИН(P6; МАКС(Q6; 15*(N6-12)))
            float w_2_B=std::min(data_K[current_day].w_z_B, std::max(data_K[current_day].w_b_B, 15.0f*(current_horse_to_day+1.0f-12.0f)));

            //E6 * (SIN(РАДИАНЫ(U6)) - SIN(РАДИАНЫ(T6)))
            float Kpr1=data_K[current_day].B*(std::sin(GRAD_RAD*w_2_B)-std::sin(GRAD_RAD*w_1_B));
            //D6*3,1415/180*(U6-T6)
            float Kpr2=data_K[current_day].A*GRAD_RAD*(w_2_B-w_1_B);
            //F6 * (COS(РАДИАНЫ(U6))-COS(РАДИАНЫ(T6)))
            float Kpr3=data_K[current_day].C*(std::cos(GRAD_RAD*w_2_B)-std::cos(GRAD_RAD*w_1_B));

            //COS(РАДИАНЫ($E$1))*COS(РАДИАНЫ(C6)) * (SIN(РАДИАНЫ(S6)) - SIN(РАДИАНЫ(R6)))
            float Kpr4=std::cos(GRAD_RAD*input_data.latitude)*std::cos(GRAD_RAD*data_K[current_day].delta_malay)*(std::sin(GRAD_RAD*w_2)-std::sin(GRAD_RAD*w_1));
            //SIN(РАДИАНЫ($E$1)) * SIN(РАДИАНЫ(C6)) * (S6-R6) * 3,1415/180
            float Kpr5=std::sin(GRAD_RAD*input_data.latitude)*std::sin(GRAD_RAD*data_K[current_day].delta_malay)*(w_2-w_1)*GRAD_RAD;

            //ЕСЛИ(Z6+AA6<>0; (W6+X6-Y6) / (Z6+AA6); 0)
            float Kpr=0.0f;
            if(static_cast<std::size_t>(Kpr5+Kpr4*1000000)!=0){
                Kpr=(Kpr1+Kpr2-Kpr3)/(Kpr5+Kpr4);
                if(Kpr>5){
                    Kpr=5;
                }
                if(Kpr<0){
                    Kpr=0;
                }
            }
//            if(begin<10){
//                std::cout<<current_horse_to_day<<"|"<<w_1<<"|"<<w_2<<"||"<<w_1_B<<"|"<<w_2_B<<"|"<<std::endl;
//                std::cout<<current_horse<<"|"<<Kpr1<<"|"<<Kpr2<<"|"<<Kpr3<<"|"<<Kpr4<<"|"<<Kpr5<<std::endl;
//                std::cout<<current_horse<<"|"<<current_day<<"|"<<current_horse_to_day<<"|"<<Kpr<<std::endl;
//            }


            kpr[i]=Kpr;
//            if(current_horse<deviation){
//                std::size_t m=8760-static_cast<int>(deviation-current_horse);

////                std::cout<<m<<"|"<<current_horse<<"|"<<current_day<<"|"<<current_horse_to_day<<"|"<<Kpr<<std::endl;
////                std::cout<<"|"<<kpr[m]<<std::endl;
//            }else {
//                kpr[current_horse-deviation ]=Kpr;
//                //std::cout<<current_horse-deviation<<"|"<<current_horse<<"|"<<current_day<<"|"<<current_horse_to_day<<"|"<<Kpr<<std::endl;
//            }

            i++;
        }
        std::cout<<"|"<<kpr[8759]<<std::endl;
    }

    std::vector<float> launch_A(std::vector<float> & data_in_All,std::vector<float> & data_in_d,std::vector<float> & albedo){
        std::vector<float> data_out;

        float l=0;

        for(std::size_t i=0;i<8760;i++){
            //auto R=(data_in_All[i]-data_in_d[i])*kpr[i]+data_in_d[i]*(180.0f-input_data.B)/180.0f+0.5f*albedo[i]*data_in_All[i]*std::sin(GRAD_RAD*input_data.B);
            auto R=(data_in_All[i]-data_in_d[i])*kpr[i]+data_in_d[i]*(180.0f-input_data.B)/180.0f;
            data_out.emplace_back(R);
            l+=kpr[i];
        }

        std::cout<<l<<"|"<<kpr[8759]<<std::endl;



        return data_out;
    }

    std::vector<float> launch_T(std::vector<float> & data_in_T,std::vector<float> & data_in_All){
        std::vector<float> data_out;
        for(std::size_t i=0;i<data_in_All.size();i++){
            //ЕСЛИ(AR6<25;$AW$1; ($AW$1*(1-$AW$2*(AS6-25))))
            if(data_in_T[i]<25.0f){
                data_out.emplace_back(data_in_All[i]);
            }else {
                float Tc=data_in_T[i]+data_in_All[i]*(input_data.NOST-20)/800.0f;
                data_out.emplace_back((1-input_data.K_T*(Tc-25))*data_in_All[i]);
            }
        }

        return data_out;
    }

    void launch_KPD(std::vector<float> & data_in_All){
        for(auto&T:data_in_All){
            if(T<input_data.E_start){
               T=0;
            }else{
               T*=input_data.KPD*input_data.F;
            }
        }
    }
};
#include <map>

static std::tuple<float, float,std::vector<float>> optimal_A_find(Model_FEU_v2::Input_data in,std::vector<float> & data_in_All,std::vector<float> & data_in_d,std::vector<float> & albedo){
    std::map<float,std::pair<float, float>> angles;

    std::vector<float> B_a;
    for (float Azimut=0;Azimut<=360;Azimut+=in.step_Azimut) {
        for (float B=0;B<=90;B+=in.step_B) {
            in.B=B;
            in.Azimut=Azimut;
            Model_FEU_v2 m(in);
            auto t=m.launch_A(data_in_All,data_in_d,albedo);
            float sum=0;

            for (auto T:t) {
                sum+=T;
            }
            //std::cout<<in.latitude<<"|"<<B<<"|"<<Azimut<<" sum: "<<sum<<std::endl;

            angles[sum]={B,Azimut};

            if(static_cast<int>(Azimut)==0){
                B_a.emplace_back(sum/1000.0f);
            }
        }
    }


    return  std::tuple_cat(std::max_element(angles.begin(),angles.end())->second,std::tuple<std::vector<float>>(B_a));

}
