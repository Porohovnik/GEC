#ifndef EXEL_USE_H
#define EXEL_USE_H
#include <OpenXLSX.hpp>
#include <filesystem>
namespace CeEngine {
class Exel_use{
    OpenXLSX::XLDocument doc;
public:
    class TYPE_FAILE_OPEN{};
    class TYPE_FAILE_CREATE{};

    Exel_use(std::string patch_faile,TYPE_FAILE_OPEN){
        doc.open(patch_faile);
        std::cout<<"Open"<<std::endl;
    }

    Exel_use(std::filesystem::path patch_faile,TYPE_FAILE_CREATE){
        doc.create(patch_faile.string());
    }
    Exel_use(){}
    Exel_use(const Exel_use & t){}
    auto open_list(std::string name_list){
        return  doc.workbook().worksheet(name_list);

    }

    inline static OpenXLSX::XLCellRange range(OpenXLSX::XLWorksheet & wks,std::size_t begin_cell,std::size_t begin_columns,std::size_t end_cell,std::size_t end_columns){
        auto begin=OpenXLSX::XLCellReference(begin_cell,begin_columns);
        auto end=OpenXLSX::XLCellReference(end_cell,end_columns);
        return wks.range(begin, end);
    }

    template<typename DATA>
    inline static void read_data(OpenXLSX::XLCellRange range,std::vector<DATA> * data){
        data->resize(range.numRows()*range.numColumns());
        std::cout<<range.numRows()*range.numColumns()<<std::endl;
        std::size_t i=0;
        for(auto &T:range){
            if constexpr(std::is_same_v<float,DATA>){
                auto s=T.value().asString();
                auto f= s.find_first_of(".");
                if(f!= std::string::npos){
                    s[f]=',';
                }

                (*data)[i++]=std::stof(s);

            }
            if constexpr(std::is_same_v<int,DATA>){
                (*data)[i++]=std::stoi(T.value().asString());
            }
            if constexpr(std::is_same_v<std::string,DATA>){
                (*data)[i++]=T.value().asString();
            }

        }
    }

    template<typename DATA>
    inline void save_data(OpenXLSX::XLCellRange range,std::vector<DATA> * data,std::size_t accurate=2){
        std::size_t i=0;

        std::setlocale(LC_ALL, "C");

        for(auto &T:range){
            if constexpr(std::is_same_v<std::string,DATA>){
                std::string s=(*data)[i];
                T.value()=s;
            }

            if constexpr(std::is_same_v<int,DATA>){
                T.value().set((*data)[i]);
            }

            if constexpr(std::is_same_v<float,DATA>){
                std::string s=std::to_string((*data)[i]);
                s.erase(s.size()-6+accurate,s.size()-1);
                if(accurate==0){
                    s.erase(s.find_first_of(','));
                }
                double d=(*data)[i];
                T.value()=d;
                //T.value()=s;
            }

            i++;
            if(i>data->size()){
               break;
            }
        }
        doc.save();
        if(sizeof (wchar_t)==2){
            std::setlocale(LC_ALL,"Russian");
            std::setlocale(LC_ALL,"ru_RU.UTF8");
        }else{
            std::setlocale(LC_ALL,"ru_RU.UTF8");
        }
    }

    ~Exel_use(){
        //doc.save();
        doc.close();
    }
};

//сделать версию для tuple
template<typename ...Arg>
static void save_data_column(Exel_use & exel,OpenXLSX::XLWorksheet &list_save,std::size_t begin_line,std::size_t begin,Arg &...arg){
    std::size_t k=begin;
    ((std::cout<<(++k)<<"|"<<1+arg.size()<<"|"<<k<<std::endl),...);
    k=begin;
    ((exel.save_data(Exel_use::range(list_save,1+begin_line,(++k),1+begin_line+arg.size()-1,k),&arg)),...);


}


template <typename T, size_t... i>
inline void save_data_column__t(Exel_use & exel,OpenXLSX::XLWorksheet &list_save,std::size_t begin,T&& t,std::index_sequence<i...>){
    save_data_column(exel,list_save,begin,std::get<i>(t)...);
}

template<typename T>
inline void save_data_column_t(Exel_use & exel,OpenXLSX::XLWorksheet &list_save,std::size_t begin,T t) {
    save_data_column__t(exel,list_save,begin,std::forward<T>(t),std::make_index_sequence<std::tuple_size<std::remove_reference_t<T>>::value>());
}
}// namespace CeEngine
#endif // EXEL_USE_H
