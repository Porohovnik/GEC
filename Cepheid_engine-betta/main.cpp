#include <iostream>

#include <type_resource.h>
#include <milieu.h>
#include <utiliti_json.h>

#include <charconv>
#include <string>

//const float GRAD_RAD =3.14f/180.0f;

static Milieu_<Milieu> milieu{};

#ifdef WIN32
    inline static std::string extension_dlc=".dll";
#else
    inline static std::string extension_dlc=".so";
#endif

class CeMain: public CeObject{
    //std::shared_ptr<CeButtom<Engine>>  bottom_NASA;
    std::shared_ptr<CeBoxElement> box;

    void * list_2;
    std::string current_path_=std::filesystem::current_path().string();
public:
    template<typename Engine>
    CeMain(Engine *win_,typename Engine::List * celist_,void * data){
        list_2=win_->resource.create_list("list_3");
        std::vector <std::string> boxs_name_list;

        for (auto &p:std::filesystem::recursive_directory_iterator("DLC")){
            if(p.path().extension()==extension_dlc){
                CePlagin_engine * plugin =milieu.plugin_menedger.add_Shared_modul(p.path());

                boxs_name_list.emplace_back(plugin->method<std::string>("get_name"));
                auto name_jsons=plugin->method<std::vector<std::string>>("get_configuration_file");

                for(auto &T:name_jsons){
                    std::cout<<T<<std::endl;
                    std::ifstream json_file(current_path_+T);
                    nlohmann::json json;
                    json_file >> json;

                    load_data_json<CeObject,CePlagin_menedger,Engine>(&milieu.plugin_menedger,win_,json);
                }
            }
        }

        //блок выбора действия
        auto fun_position_2 = [=](int k){
            return Position{-0.74f+0.5f/3.0f*(1+k*1.05f),0.0f, -0.5f, 0.5f/3.0f, 0.75f};
        };
        box= Engine::objects.template create_object<CeBoxElement>("plate",win_,celist_,typename CeBoxElement::Data_initialization(fun_position_2));
        auto pos_ch=box->change_position();

        pos_ch.position->scale(glm::vec3{2.0f, 0.09f, 1.0f});
        pos_ch.position->move(glm::vec3{0.0f, -0.96f, 0.99f});

        for(auto &S:boxs_name_list){
            std::string name=S;

            auto data=typename CeButtom::Data_initialization("fonts/SourceCodePro-Regular.ttf",S,"");
            std::shared_ptr<CeButtom> bottom = Engine::objects.template create_object<CeButtom>(name+"mc",win_,celist_,data);

            bottom->connect_fun([win_,celist_,name,bottom](){
                win_->resource.status_change_all(Type_Status::NOVISEBLE);
                celist_->set_status(Type_Status::VISEBLE);

                if(bottom->pass){
                    if(name=="Назад"){
                        win_->resource.get_list("list_1")->set_status(Type_Status::VISEBLE);
                    }else {
                        win_->resource.get_list(name)->set_status(Type_Status::VISEBLE);
                    }

                }
            },CeButtom::TYPE_FUNCTION::REPEAT_LEFT);

            box->add_element(bottom);
        }

        std::cout<<"CeMain1"<<std::endl;
    }


};

#include "CeFPS.h"

#include "CePlate.h"
#include "CeText.h"
#include "CeLine.h"

#include "CeWorldMap.h"

#include <bezier_curves.h>
#include "CeObjectSVG.h"
int main(){
    //SVG_mesh(R"(C:\Users\ARHIV\YandexDisk-telegram-2\Cepheid_engine-betta\lib\CepheidEngine\lib\nanosvg\example\nano.svg)");

    //auto Win_base=milieu.insert_context("GEC",static_cast<int>(1920/1.5),static_cast<int>(1024/1.5),Color{0.2f-0.05f, 0.4f-0.05f, 0.3f-0.05f, 0.0f});
    auto Win_base = milieu.insert_context("GEC",static_cast<int>(1920/1.5),static_cast<int>(1024/1.5),Color{0.29f, 0.30f, 0.39f, 0.0f});
    //auto Win_base=milieu.insert_context("GEC",static_cast<int>(1920/1.5),static_cast<int>(1024/1.5),Color{1.0f, 1.0f, 1.0f, 1.0f});

    // milieu.insert_window("TEST",static_cast<int>(1920/1.7),static_cast<int>(1024/1.7),glm::vec4{1.0f, 1.0f,1.0f, 1.0f});
    Win_base->make_context();
    //glfwSetInputMode(milieu.get_indef_win("TEST"), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    GL_layer::Enable<GL_layer::TYPE_SETTING_GL::DEPTH_TEST>();
    //GL_layer::Enable<GL_layer::TYPE_SETTING_GL::CULL_FACE>();//схренали всё ломает??
    GL_layer::Enable<GL_layer::TYPE_SETTING_GL::BLEND>();
    GL_layer::Enable<GL_layer::TYPE_SETTING_GL::MULTISAMPLE>();
    GL_layer::DepthFunc<GL_layer::TYPE_SETTING_DEPTH_FUN::LESS>();
    GL_layer::BlendFuncSeparate(GL_layer::get_value_GL<GL_layer::TYPE_SETTING_DEPTH_FUN_BLEND::SRC_ALPHA>(),
                                GL_layer::get_value_GL<GL_layer::TYPE_SETTING_DEPTH_FUN_BLEND::ONE_MINUS_SRC_ALPHA>(),
                                GL_layer::get_value_GL<GL_layer::TYPE_SETTING_DEPTH_FUN_BLEND::ONE>(),
                                GL_layer::get_value_GL<GL_layer::TYPE_SETTING_DEPTH_FUN_BLEND::ZERO>()
                                );

    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto list_1=Win_base->resource.create_list("list_0");


    Milieu_<Milieu>::Context::objects.add_type_fabric<Milieu_<Milieu>::Context,CeWorldMap>("CeMap");

    //auto main_obj=Milieu_<Milieu>::Context::objects.template create_object<CeMain>("Main",Win_base,list_1,nullptr);

    CeMain main_(Win_base,list_1,nullptr);

    std::string path=R"(C:\Users\ARHIV\YandexDisk-telegram-2\Cepheid_engine-betta\lib\CepheidEngine\lib\nanosvg\example\6.svg)";
    //CeObjectSVG svg(Win_base,list_1,CeObjectSVG::Data_initialization(path));


//    std::vector<float> vertices{
//            -1,0.9f,
//            0,0.5f,
//            1,0.0f,
//            2,0.5f,
//            3,0.0f,
//            4,0.5f,
//            5,0.5f,
//            6,0.0f,
//            7,0.5f,
//            8,0.2f,
//            9,0.5f,
//            10,0.9f,
//            -1,0.9f
//    };

//    std::vector<float> vertices1{
//            -1,1.8f,
//            0,1.8f,
//            1,0.8f,
//            2,0.8f,
//            3,0.8f,
//            4,0.8f,
//            5,1.8f,
//            6,0.8f,
//            7,0.8f,
//            8,0.8f,
//            9,0.8f,
//            10,0.8f
//    };


//    std::vector<float> vertices{
//            0.9f,
//            0.5f,
//            0.0f,
//            0.5f,
//            0.0f,
//            0.5f,
//            0.5f,
//            0.0f,
//            0.5f,
//            0.2f,
//            0.5f,
//            0.9f
//    };

//        std::vector<float> vertices{
//            0,0.2f,
//            2,0.2f,
//            2,0.7f,
//            0,0.7f,
//            0,0.2f,
//            2,0.2f,
//        };

//        std::vector<float> vertices{
//            0,0.0f,
//            2,0.0f,
//            2,0.5f,
//            0,0.5f,
//            0,0.0f,
//            2,0.0f,
//        };

//    std::shared_ptr<Mesh_data> data=std::make_shared<Mesh_data>(std::move(vertices));

//    data->set_marking_vertices<GL_layer::TYPE_data::position>(1);
//    data->set_marking_vertices<GL_layer::TYPE_data::normal>(1);

//    CeLine_xy line(list_1,CeLine_xy::Data(data,"lines",0.05f));
//    {
//        line.template change <Position>().get_data()->move(glm::vec3{-0.75f,-0.70f,0.0f});
//        line.template change <Position>().get_data()->scale(glm::vec3{0.15f,0.8f,1.0f});
//    }

//    std::vector<float> vertices{
//        0,0.0f,
//        1,0.0f
//    };

//    CePlate plate(list_1,CePlate::Data(line_mesh(vertices,0.1f,TYPE_JOINT::OBLIQUE),"lines"));
//    {
//        plate.template change <Position>().get_data()->move(glm::vec3{-0.75f,-0.70f,0.0f});
//        plate.template change <Position>().get_data()->scale(glm::vec3{0.15f,0.8f,1.0f});
//    }

//    std::shared_ptr<Mesh_data> data1=std::make_shared<Mesh_data>(std::move(vertices1));

//    data1->set_marking_vertices<GL_layer::TYPE_data::position>(1);
//    data1->set_marking_vertices<GL_layer::TYPE_data::normal>(1);

//    CeLine_xy line1(list_1,CeLine_xy::Data(data1,"lines2",0.05f));
//    {
//        line1.template change <Position>().get_data()->move(glm::vec3{-0.75f,-0.60f,0.0f});
//        line1.template change <Position>().get_data()->scale(glm::vec3{0.15f,0.9f,1.0f});
//    }

    typename CeFPS::Data_initialization data_t("fonts/SourceCodePro-Regular.ttf","FPS: ");
    data_t.color={0.0f, 0.0f, 0.0f, 1.0f};
    //data_t.pos=glm::vec3{-0.95f,0.8f,2.0f};
    data_t.pos=glm::vec3{-0.95f,0.8f,0.0f};
    data_t.pos.scale(glm::vec4{0.2f, 0.2f, 1.0f, 1.0f});
    //auto fps=Milieu_<Milieu>::Context::objects.template create_object<CeFPS>("t",Win_base,list_1,data_t);

    //Engine::objects.set_template_object(R"(C:\\Users\\ARHIV\\Desktop\\novy_1.xml)");
    //Engine::objects.set_template_object(R"(C:\\Users\\ARHIV\\Desktop\\main.xml)");

    auto t_=Milieu_<Milieu>::Context::objects.template create_to_template_object<CePlate,CeText,CeLine>(Win_base,list_1,"Main",{});
    std::cout<<"<<<<<<<<"<<std::endl;
    //auto destine=Engine::objects->template create_object<Stroke_of_fate>("Destine",win_,celist_,nullptr);

    Win_base->resource.status_change_all(Type_Status::NOVISEBLE);
    list_1->set_status(Type_Status::VISEBLE);
    Win_base->resource.get_list("list_1")->set_status(Type_Status::VISEBLE);

    std::cout<<sizeof (Milieu_<Milieu>)<<"|||||||||БЮ|||||||||"<<std::endl;

    //GL_layer::PolygonMode<GL_layer::TYPE_POLYGON_MOD::LINE>();


    milieu.DISPLAY();
    Win_layer::Win::Terminate();
    return 0;
}

