//
// Created by erezgold on 09/11/18.
//

#include "../include/Action.h"
#include "../include/Table.h"
#include "../include/Restaurant.h"
#include <sstream>
#include <string>
#include <iostream>
extern Restaurant* backup;

//BaseAction

//Constructor
BaseAction::BaseAction():logMsg(), errorMsg(), status(PENDING)  {};
//Destructor
BaseAction::~BaseAction() = default;

//Methods
void BaseAction::setString(std::string const &logMsg) {
    this->logMsg = logMsg;
}
std::string BaseAction::getLogMsg() const {
    return logMsg;
}

ActionStatus BaseAction::getStatus() const {
    return status;
}
void BaseAction::complete() {
    status = COMPLETED;
}
void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = "ERROR: " + errorMsg;
    std::cout <<  this->errorMsg << std::endl;

}
std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}


//OpenTable
//Constructor
OpenTable::OpenTable(int id, std::vector<Customer*> &customersList):BaseAction(), tableId(id), customers(){
    for (Customer* customer:customersList) {
        customers.push_back(customer) ;
    }
}


//Copy Constructor
OpenTable::OpenTable(const OpenTable &other): BaseAction(other), tableId(other.tableId), customers(other.customers) {
}
//Destructor
OpenTable::~OpenTable(){
    customers.clear();
}
//Move Copy Constructor
OpenTable::OpenTable(OpenTable &&other):tableId(other.tableId), customers() {
    //copy other
    for (Customer* const &customer : other.customers ) {
        this->customers.push_back(customer->clone());
    }
    //delete other
    for (Customer* customer : other.customers ) {
        delete customer;
    }

    other.customers.clear();
}
//Methods
void OpenTable::act(Restaurant &restaurant) {
   Table* table (restaurant.getTable(tableId));
   if(table != nullptr && !table->isOpen()){
       table->openTable();
       for (Customer* const &customer:customers) {
           table->addCustomer(customer);
       }
     this->complete();
   }
   else{
       this->error("Table does not exist or is already open");
       for (Customer* const &customer:customers) {
           delete customer;
       }
   }

}
std::string OpenTable::toString() const {
    std::ostringstream oss;
    oss << getLogMsg();
    if(this->getStatus() == 1){
        oss << " " << "Completed";
    } else {
        oss << " " << this->getErrorMsg();
    }
    std::string output = oss.str();
    return output;
}
OpenTable* OpenTable::clone() const {
    return new OpenTable(*this);
}



//Order

//Constructor
Order::Order(int id):BaseAction(), tableId(id) {}
//Methods
void Order::act(Restaurant &restaurant) {
    Table* table(restaurant.getTable(tableId));
    if(table != nullptr && table->isOpen() ) {
        table->order(restaurant.getMenu());
        this->complete();
    }
    else {
        this->error("Table does not exist or is not open");
    }

}
std::string Order::toString() const {
    std::ostringstream oss;
    oss << getLogMsg();
    if(this->getStatus() == 1){
        oss << " " << "Completed";
    } else {
        oss << " " << this->getErrorMsg();
    }
    std::string output = oss.str();
    return output;
}
Order* Order::clone() const {
    return new Order(*this);
}

//Close

//Constructor
Close::Close(int id):BaseAction(), tableId(id) {}
//Methods
void Close::act(Restaurant &restaurant) {
    Table* table(restaurant.getTable(tableId));
    int bill;
    if(table!= nullptr && table->isOpen() ) {
        bill = table->getBill();
        table->closeTable();
        this->complete();
        std::cout << "Table " << tableId << " was closed. Bill " << std::to_string(bill) << "NIS" << std::endl;
    }
    else{
    this->error("Table does not exist or is already open");
    }
}
std::string Close::toString() const {
    std::ostringstream oss;
    oss << getLogMsg();
    if(this->getStatus() == 1){
        oss << " " << "Completed";
    } else {
        oss << " " << this->getErrorMsg();
    }
    std::string output = oss.str();
    return output;
}
Close* Close::clone() const {
    return new Close(*this);
}

//CloseAll

//Constructor
CloseAll::CloseAll(): BaseAction() {}
//Methods
void CloseAll::act(Restaurant &restaurant) {
    for(signed i =0 ; i< restaurant.getNumOfTables();i++){
        if(restaurant.getTable(i)->isOpen()){
            BaseAction* close = new Close(i);
            close->act(restaurant);
            complete();
            delete close;
        }
    }
}
std::string CloseAll::toString() const {
    std::ostringstream oss;
    oss << getLogMsg();
    if(this->getStatus() == 1){
        oss << " " << "Completed";
    } else {
        oss << " " << this->getErrorMsg();
    }
    std::string output = oss.str();
    return output;
}
CloseAll* CloseAll::clone() const {
    return new CloseAll(*this);
}

//PrintMenu

//Constructor
PrintMenu::PrintMenu(): BaseAction() {}


//Methods
void PrintMenu::act(Restaurant &restaurant) {
    for(Dish const &dish:restaurant.getMenu()){
        std::cout << dish.getName() << " " << dish.typeToString() << " " << dish.getPrice() << "NIS" << std::endl;
    }
    complete();
}
std::string PrintMenu::toString() const {
    std::ostringstream oss;
    oss << getLogMsg();
    if(this->getStatus() == 1){
        oss << " " << "Completed";
    } else {
        oss << " " << this->getErrorMsg();
    }
    std::string output = oss.str();
    return output;
}
PrintMenu* PrintMenu::clone() const {
    return new PrintMenu(*this);
}
//PrintActionsLog

//Constructor
PrintActionsLog::PrintActionsLog(): BaseAction() {}

//Methods
void PrintActionsLog::act(Restaurant &restaurant) {
    for(BaseAction* const &action:restaurant.getActionsLog()){
        if (action->toString() != "log"){
            std::cout << action->toString() << std::endl;
        }
    }
    this->complete();
}
std::string PrintActionsLog::toString() const {
    return getLogMsg();
}
PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}


//Move Customer

//Constructor
MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction() , srcTable(src), dstTable(dst), id (customerId)  {}

//Methods
void MoveCustomer::act(Restaurant &restaurant) {
    Table* tabledst (restaurant.getTable(dstTable));
    Table* tablesrc (restaurant.getTable(srcTable));
    size_t capacityOfdstTable= tabledst->getCapacity();
    //the dst and src tables are exist
    if(tabledst != nullptr && tablesrc != nullptr &&
       //the dst and src tables are open
       tabledst->isOpen() && tablesrc->isOpen() &&
       //the customer is found in the src table and there is available seat in the dst table
       tablesrc->customerFound(id) && tabledst->getCustomers().size()<capacityOfdstTable){
        std::vector<OrderPair> customerDishs = tablesrc->getCustomerDish(id);
        tabledst->addCustomer(tablesrc->getCustomer(id)->clone());
        tablesrc->removeCustomer(id);
        for(OrderPair const &order:customerDishs){
            tabledst->getOrders().push_back(order);
        }
        //if there is no customer left in the src table
        if(tablesrc->getCustomers().size()==0){
            tablesrc->closeTable();
        }
        customerDishs.clear();
        this->complete();
    }
    else{
        this->error("Cannot move customer");
    }
}

std::string MoveCustomer::toString() const {
    std::ostringstream oss;
    oss << getLogMsg();
    if(this->getStatus() == 1){
        oss << " " << "Completed";
    } else {
        oss << " " << this->getErrorMsg();
    }
    std::string output = oss.str();
    return output;
}

MoveCustomer* MoveCustomer::clone() const {
    return new MoveCustomer(*this);
}
//PrintTableStatus

//Constructor
PrintTableStatus::PrintTableStatus(int id):BaseAction(), tableId(id) {}

//Methods
void PrintTableStatus::act(Restaurant &restaurant) {
    Table* table (restaurant.getTable(tableId));
    if(table->isOpen()){
        std::cout << "Table "<< tableId << " status: open" << std::endl;
        std::cout << "Customers:" << std::endl;
        for (Customer* const &customer:table->getCustomers()) {
            std::cout << customer->getId() << " " << customer->getName() << std::endl;
        }
        std::cout << "Orders:" << std::endl;
        for(OrderPair const &order:table->getOrders()){
            std::cout << order.second.getName() << " " << order.second.getPrice() << "NIS " << order.first << std::endl;
        }
        std::cout << "Current Bill: " << table->getBill() << "NIS" << std::endl;
    } else {
        std::cout << "Table "<< tableId << " status: closed" << std::endl;
    }
    complete();
}
std::string PrintTableStatus::toString() const {
    std::ostringstream oss;
    oss << getLogMsg();
    if(this->getStatus() == 1){
        oss << " " << "Completed";
    } else {
        oss << " " << this->getErrorMsg();
    }
    std::string output = oss.str();
    return output;
}
PrintTableStatus* PrintTableStatus::clone() const {
    return new PrintTableStatus(*this);
}
//BackupRestaurant

//Constructor
BackupRestaurant::BackupRestaurant(): BaseAction() {}

//Methods
void BackupRestaurant::act(Restaurant &restaurant) {
    backup = new Restaurant(restaurant);
    complete();
}
std::string BackupRestaurant::toString() const {
    std::ostringstream oss;
    oss << getLogMsg();
    if(this->getStatus() == 1){
        oss << " " << "Completed";
    } else {
        oss << " " << this->getErrorMsg();
    }
    std::string output = oss.str();
    return output;
}
BackupRestaurant* BackupRestaurant::clone() const {
    return new BackupRestaurant(*this);
}

//RestoreResturant

//Constructor
RestoreResturant::RestoreResturant():BaseAction() {}

//Methods
void RestoreResturant::act(Restaurant &restaurant) {
    if (backup!= nullptr){
        restaurant = *backup;
        complete();
    }
    else{
        error("No backup available ");
    }

}
std::string RestoreResturant::toString() const {
    std::ostringstream oss;
    oss << getLogMsg();
    if(this->getStatus() == 1){
        oss << " " << "Completed";
    } else {
        oss << " " << this->getErrorMsg();
    }
    std::string output = oss.str();
    return output;
}
RestoreResturant* RestoreResturant::clone() const {
    return new RestoreResturant(*this);
}