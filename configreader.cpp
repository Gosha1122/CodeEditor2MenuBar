#include "ConfigReader.h"
#include <vector>

ConfigReader::ConfigReader(const std::string& path):path(path){
    in.open(path);
    if(!in.is_open()){
        throw ConfException("ReadConf");
    }
    std::string str1;
    std::string key, value;
    int begin;
    int end1, end2;
    while(!in.eof()){
        std::getline(in, str1);
        if((str1[0] == '/' && str1[1] == '/') || str1 == "\0"){
            continue;
        }
        begin = str1.find_first_not_of(" ");
        end1 = str1.find_last_not_of(" ");
        end2 = str1.find_first_of("=");
        key = str1.substr(begin, end2 - begin);
        value = str1.substr(end2 + 1, end1 - end2 + 1);
        mpConf[key] = value;
    }
    in.close();
}

ConfigReader::~ConfigReader(){

}

Conf ConfigReader::ReadAll(){
    return mpConf;
}

std::string ConfigReader::ReadOne(const std::string& key){
    return mpConf[key];
}

void ConfigReader::IzmZn(std::string key, std::string value){
    mpConf[key] = value;
    std::vector<std::string> vec;
    std::ifstream in(path);
    if(!in.is_open()){
        throw ConfException("IzmZnRead");
    }
    std::string strt;
    int index;
    int i = 0;
    while(!in.eof()){
        getline(in, strt);
        vec.push_back(strt);
        if(strt != "\0" && strt[0] != '/' && strt[1] != '/'){
            if(key == strt.substr(strt.find_first_not_of(" "), strt.find_first_of("=") - strt.find_first_not_of(" ") + 1)){
                index = i;
            }
        }
        i++;
    }
    in.close();
    std::ofstream f(path);
    if(!f.is_open()){
        throw ConfException("IzmZnWrite");
    }
    vec[index] = value;
    i = 0;
    while(!f.eof()){
        f << vec[i];
        i++;
    }
    f.close();
}
