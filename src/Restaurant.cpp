//
// Created by erezgold on 09/11/18.
//
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "../include/Restaurant.h"
#include "../include/Dish.h"
#include "../include/Table.h"
#include "../include/Action.h"
#include "../include/Customer.h"

DishType Restaurant::setType(std::string type){
    if(type == "VEG") return VEG;
    if(type == "SPC") return SPC;
    if(type == "BVG") return BVG;
    return ALC;
}
Customer* Restaurant::createCustomerByType(int id, std::string name, std::string type){
    Customer* output;
    if (type == "alc"){
        output = new AlchoholicCustomer(name, id);
    }
    else if (type == "spc"){
        output = new SpicyCustomer(name, id);
    }
    else if (type == "chp"){
        return new CheapCustomer(name, id);
    }
    else {
        output = new VegetarianCustomer(name, id);
    }
    return output;
}
Restaurant::Restaurant(const std::string &configFilePath): open(true), tables(), menu(), actionsLog() {
    std::string line;
    std::ifstream file (configFilePath.c_str());
    int phase = 0;
    std::string dish_name;
    DishType dish_type;
    int dish_price;
    int counter = 0;
    if (file.is_open())
    {
        while ( getline (file,line) )
        {

            if(line.empty() || line == "\r" || line[0] == '#'){
                continue;
            } else {
                if(phase < 3){
                    phase++;
                }
            }
            if(phase == 1){

            }
            if(phase == 2){
                std::string token;
                while(line.find(',') != std::string::npos){
                    token = line.substr(0, line.find(','));
                    line = line.substr(line.find(',')+1, line.length());
                    tables.push_back(new Table(std::stoi(token)));
                }
                tables.push_back(new Table(std::stoi(line)));
            }
            if(phase == 3){
                dish_name = line.substr(0, line.find(','));
                line = line.substr(line.find(',')+1, line.length());
                dish_type = setType(line.substr(0, line.find(',')));
                line = line.substr(line.find(',')+1, line.length());
                dish_price = std::stoi(line);
                menu.emplace_back(Dish(counter, dish_name, dish_price, dish_type));
                counter++;
            }
        }
        file.close();
    }
    else std::cout << "Unable to open file";
}

//Copy Constructor
Restaurant::Restaurant(const Restaurant &other): open(true), tables(), menu(), actionsLog() {
    for (Table* const &table : other.tables) {
        tables.push_back(table->clone());
    }
    for (Dish const &dish : other.menu) {
        this->menu.push_back(dish);
    }
    for (BaseAction* const &action : other.getActionsLog())  {
        this->actionsLog.push_back(action->clone());
    }

}
//Destructor
Restaurant::~Restaurant() {

    for (Table* table : tables ) {
        delete table;
    }
    for (BaseAction* action : actionsLog ) {
        delete action;
    }
    tables.clear();
    actionsLog.clear();
    menu.clear();


}

//Copy Assignment Operator
Restaurant& Restaurant::operator=(const Restaurant &other) {
    if(this != &other) {
        //delete this
        for (Table* table : tables ) {
            delete table;
        }
        for (BaseAction* action : actionsLog ) {
            delete action;
        }
        tables.clear();
        actionsLog.clear();
        menu.clear();
        //assign other
        for (Table* const &table : other.tables) {
            tables.push_back(table->clone());
        }
        for (Dish const &dish : other.menu) {
            this->menu.push_back(dish);
        }
        for (BaseAction* const &action : other.getActionsLog())  {
            this->actionsLog.push_back(action->clone());
        }
    }
    return (*this);
}
//Copy Move Constructor
Restaurant::Restaurant(Restaurant &&other):open(other.open) ,tables(), menu(), actionsLog() {

    for (Table* const &table : other.tables) {
        tables.push_back(table->clone());
    }
    for (Dish const &dish : other.menu) {
        this->menu.push_back(dish);
    }
    for (BaseAction* const &action : other.getActionsLog())  {
        this->actionsLog.push_back(action->clone());
    }
    //delete other
    for (Table* table : other.tables ) {
        delete table;
    }
    for (BaseAction* action : other.actionsLog ) {
        delete action;
    }
    other.tables.clear();
    other.actionsLog.clear();
    other.menu.clear();

}
// Move Assignment
Restaurant& Restaurant::operator=(Restaurant &&other) {
    if(this != &other){
        //delete this
        for (Table* table : tables ) {
            delete table;
        }
        for (BaseAction* action : actionsLog ) {
            delete action;
        }
        tables.clear();
        actionsLog.clear();
        menu.clear();
        //assign other
        for (Table* const &table : other.tables) {
            tables.push_back(table->clone());
        }
        for (Dish const &dish : other.menu) {
            this->menu.push_back(dish);
        }
        for (BaseAction* const &action : other.getActionsLog())  {
            this->actionsLog.push_back(action->clone());
        }
        //delete other
        for (Table* table : other.tables ) {
            delete table;
        }
        for (BaseAction* action : other.actionsLog ) {
            delete action;
        }
        other.tables.clear();
        other.actionsLog.clear();
        other.menu.clear();
    }
    return (*this);
}

std::vector<Dish>& Restaurant::getMenu() {
    return menu;
}
const std::vector<BaseAction*>& Restaurant::getActionsLog() const {
    return actionsLog;
}

Table* Restaurant::getTable(int ind) {
    return tables[ind];
}

int Restaurant::getNumOfTables() const {
    return tables.size();
}

void Restaurant::start() {
    std::cout << "Restaurant is now open!" << std::endl;
    bool done = false;
    std::string input;
    int idCount = 0;
    while(!done){
        getline(std::cin, input);
        std::string log = input;
        BaseAction* action = nullptr;
        std::string actionStr = input.substr(0, input.find(' '));
        if(actionStr == "open"){
            std::string customerName;
            std::string customerType;
            std::vector<Customer*> customerList;
            input = input.substr(input.find(' ')+1, input.length());
            int tableId = std::stoi(input.substr(0, input.find(' ')));
            input = input.substr(input.find(' ')+1, input.length());
            while(input.find(' ') != std::string::npos){
                customerName = input.substr(0,input.find(' '));
                customerType = customerName.substr(input.find(',')+1,input.length());
                customerName = customerName.substr(0,input.find(','));
                Customer* Customer = createCustomerByType(idCount, customerName, customerType);
                customerList.push_back(Customer);
                idCount++;
                input = input.substr(input.find(' ')+1, input.length());
            }
            if(input.length() > 0){
                customerName = input;
                customerType = input.substr(input.find(',')+1,input.length());
                customerName = input.substr(0,input.find(','));
                Customer* Customer = createCustomerByType(idCount, customerName, customerType);
                customerList.push_back(Customer);
                idCount++;
            }
            action = new OpenTable(tableId, customerList);
        }
        if(actionStr == "order"){
            input = input.substr(input.find(' ')+1, input.length());
            int tableId = std::stoi(input.substr(0, input.find(' ')));
            action = new Order(tableId);
        }
        if(actionStr == "move"){
            input = input.substr(input.find(' ')+1, input.length());
            int srcTable = std::stoi(input.substr(0, input.find(' ')));
            input = input.substr(input.find(' ')+1, input.length());
            int dstTable = std::stoi(input.substr(0, input.find(' ')));
            input = input.substr(input.find(' ')+1, input.length());
            int customer = std::stoi(input);
            action = new MoveCustomer(srcTable, dstTable, customer);
        }
        if(actionStr == "close"){
            input = input.substr(input.find(' ')+1, input.length());
            int tableId = std::stoi(input.substr(0, input.find(' ')));
            action = new Close(tableId);
        }
        if(actionStr == "menu"){
            action = new PrintMenu();
        }
        if(actionStr == "status"){
            input = input.substr(input.find(' ')+1, input.length());
            int tableId = std::stoi(input.substr(0, input.find(' ')));
            action = new PrintTableStatus(tableId);
        }
        if(actionStr == "log"){
            action = new PrintActionsLog();
        }
        if(actionStr == "backup"){
            action = new BackupRestaurant();
        }
        if(actionStr == "restore"){
            action = new RestoreResturant();
        }
        if(actionStr == "closeall"){
            action = new CloseAll();
        }

        if(action != nullptr){
            action->act(*this);
            action->setString(log);
            actionsLog.push_back(action);
        }

        if(actionStr == "closeall"){
            done = true;
            this->open = false;
        }
    }
}