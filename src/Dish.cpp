//
// Created by erezgold on 08/11/18.
//

#include "../include/Dish.h"
#include <iostream>
#include <vector>


Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type):
id(d_id),name(d_name),price(d_price),type(d_type) {

}

//Dish::Dish(const Dish &other):
//id(other.id),name(other.name),price(other.price),type(other.type) {
//}

//Dish::~Dish()=default;

int Dish::getId() const {
    return this->id;
}

std::string Dish::getName() const {
    return this->name;
}

int Dish::getPrice() const {
    return this->price;
}

DishType Dish::getType() const {
    return this->type;
}

std::string Dish::typeToString() const {
    if(type == 0) return "VEG";
    if(type == 1) return "SPC";
    if(type == 2) return "BVG";
    return "ALC";
}