//
// Created by erezgold on 08/11/18.
//

#include "../include/Customer.h"
#include <iostream>
#include <sstream>
#include <algorithm>

//Constructor
Customer::Customer(std::string c_name, int c_id):name(c_name), id(c_id) {}
//Destructor
Customer::~Customer() = default;
//Methods
std::string Customer::toString() const {
    std::ostringstream oss;
    oss << this->name << "(" << this->id << ")";
    std::string var = oss.str();
    return "This is a Customer";
}
int Customer::getId() const {
    return this->id;
}
std::string Customer::getName() const {
    return this->name;
}


//Veggie

//Constructor
VegetarianCustomer::VegetarianCustomer(std::string name, int id):Customer(name, id) {}
//Destructor
VegetarianCustomer::~VegetarianCustomer(){}
//Methods
VegetarianCustomer* VegetarianCustomer::clone() const {
    return new VegetarianCustomer(*this);
}
std::string VegetarianCustomer::toString() const {
    return "a string";
}
std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> output;
    if(menu.size() > 0){
        bool vegFound = false;
        int minId= -1;
        int index = -1;
        for(size_t i = 0; i < menu.size(); i++){
            if(menu[i].getType() == 0 && minId == -1){
                minId = menu[i].getId();
                index = i;

            }
            if(menu[i].getType() == 0 && minId > menu[i].getId()){
                minId = menu[i].getId();
                index = i;
            }
        }
        if(index != -1){
            output.push_back(menu[index].getId());
            vegFound = true;
        }
        int maxPrice= -1;
        index = -1;
        for(size_t j = 0; j < menu.size() && vegFound; j++){
            if(menu[j].getType() == 2 && maxPrice == -1){
                maxPrice = menu[j].getPrice();
                index = j;

            }
            if(menu[j].getType() == 2 && maxPrice < menu[j].getPrice()){
                maxPrice = menu[j].getPrice();
                index = j;
            }
        }
        if(index != -1){
            output.push_back(menu[index].getId());
        }
    }
    return output;
}



//Cheap

//Constructor
CheapCustomer::CheapCustomer(std::string name, int id):Customer(name, id),ordered(false) {}
//Destructor
CheapCustomer::~CheapCustomer(){}
//Methods
CheapCustomer* CheapCustomer::clone() const {
    return new CheapCustomer(*this);
}
std::string CheapCustomer::toString() const {
    return "a string";
}
std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> output;
    if(menu.size() > 0 && !this->ordered){
        int minPrice = menu[0].getPrice();
        int index = 0;
        for(size_t  i = 0; i < menu.size(); i++){
            if( minPrice > menu[i].getPrice()){
                minPrice = menu[i].getPrice();
                index = i;
            }
        }
        output.push_back(menu[index].getId());
        this->ordered = true;
    }
    return output;
}


//Spicy

//Constructor
SpicyCustomer::SpicyCustomer(std::string name, int id):Customer(name, id), ordered(false) {}
//Destructor
SpicyCustomer::~SpicyCustomer(){
}
//Methods
SpicyCustomer* SpicyCustomer::clone() const {
    return new SpicyCustomer(*this);
}
std::string SpicyCustomer::toString() const {
    return "a string";
}
std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> output;
    if(menu.size() > 0){
        int minPrice= -1;
        int index = -1;
        for(size_t i = 0; i < menu.size() && ordered; i++){
            if(menu[i].getType() == 2 && minPrice == -1){
                minPrice = menu[i].getPrice();
                index = i;

            }
            if(menu[i].getType() == 2 && minPrice > menu[i].getPrice()){
                minPrice = menu[i].getPrice();
                index = i;
            }
        }
        if(index != -1){
            output.push_back(menu[index].getId());
        }
        int maxPrice = -1;
        index = -1;
        for(size_t j = 0; j < menu.size() && !ordered; j++){
            if(menu[j].getType() == 1 && maxPrice == -1){
                maxPrice = menu[j].getPrice();
                index = j;

            }
            if(menu[j].getType() == 1 && maxPrice < menu[j].getPrice()){
                maxPrice = menu[j].getPrice();
                index = j;
            }
        }
        if(index != -1){
            output.push_back(menu[index].getId());
            ordered = true;
        }
    }
    return output;
}

//AlchoholicCustomer
//Constructor
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id):Customer(name, id), currentPrice(-1) {}
//Destructor
AlchoholicCustomer::~AlchoholicCustomer(){
}
//Methods
AlchoholicCustomer* AlchoholicCustomer::clone() const {
    return new AlchoholicCustomer(*this);
}
std::string AlchoholicCustomer::toString() const {
    return "a string";
}
std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> output;
    if(menu.size() > 0 ){
        int minPrice= -1;
        int index = -1;
        for(size_t i = 0; i < menu.size(); i++){
            if(menu[i].getType() == 3 && minPrice == -1 && currentPrice < menu[i].getPrice()){
                minPrice = menu[i].getPrice();
                index = i;

            }
            if(menu[i].getType() == 3 && minPrice > menu[i].getPrice() && currentPrice < menu[i].getPrice()){
                minPrice = menu[i].getPrice();
                index = i;
            }
        }
        if(index != -1){
            output.push_back(menu[index].getId());
            currentPrice = minPrice;
        }
    }
    return output;
}