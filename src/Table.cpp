

#include "../include/Table.h"
#include <iostream>

#include "../include/Customer.h"
#include <vector>
//Constructor
Table::Table(int t_capacity) :capacity(t_capacity), open(false), customersList(), orderList(){}

//Copy Constructor
Table::Table(const Table& other):capacity(other.getCapacity()), open(other.open),customersList(), orderList(){
    for (Customer* const &customer : other.customersList ) {
        this->customersList.push_back(customer->clone());
    }
    for (OrderPair const &order : other.orderList ) {
        this->orderList.push_back(order);
    }

}
//Destructor
Table::~Table() {
    for (Customer* customer : customersList ) {
        delete customer;
    }
    customersList.clear();
    orderList.clear();


}
//Copy Assignment Operator
Table& Table:: operator=(const Table &other) {
    if(this != &other) {
        //delete this
        for (Customer* customer : customersList ) {
            delete customer;
        }
        customersList.clear();
        orderList.clear();
        //assign other
        capacity = other.getCapacity();
        open = other.open;
        for (Customer* const &customer : other.customersList ) {
            this->customersList.push_back(customer->clone());
        }
        for (OrderPair const &order : other.orderList ) {
            this->orderList.push_back(order);
        }

    }
    return (*this);
}
//Copy Move Constructor
Table::Table(Table &&other): capacity(other.getCapacity()), open(other.open), customersList(), orderList()  {
    //copy other
    for (Customer* const &customer : other.customersList ) {
        this->customersList.push_back(customer->clone());
    }
    for (OrderPair const &order : other.orderList ) {
        this->orderList.push_back(order);
    }
    //delete other
    for (Customer* customer : other.customersList ) {
        delete customer;
    }

    other.customersList.clear();
    other.orderList.clear();
    other.capacity = 0 ;
}
// Move Assignment
Table& Table::operator=(Table &&other){
    if(this != &other){
        //delete this
        for (Customer* customer : customersList ) {
            delete customer;
        }
        customersList.clear();
        orderList.clear();
        //assign other
        capacity = other.getCapacity();
        open = other.open;
        for (Customer* const &customer : other.customersList ) {
            this->customersList.push_back(customer->clone());
        }
        for (OrderPair const &order : other.orderList ) {
            this->orderList.push_back(order);
        }
        //delete other
        for (Customer* customer : other.customersList ) {
            delete customer;
        }

        other.customersList.clear();
        other.orderList.clear();
        other.capacity = 0 ;

    }
    return (*this);
}

Table* Table::clone() {
    return new Table(*this);
}

//Methods
int Table::getCapacity() const{
    return capacity;
}

void Table::addCustomer(Customer* customer){

        customersList.push_back(customer);
}



void Table::removeCustomer(int id) {
    bool found = false;
    for (size_t i = 0; !found && i < getCustomers().size(); i++) {
        if (customersList[i]->getId() == id){
            delete customersList[i];
            customersList.erase(customersList.begin() + i);
            found = true;
        }
    }
    if(found){
        size_t size = getOrders().size();
        std::vector<OrderPair> oldOrderList = orderList;
        orderList.clear();
        for ( size_t i=0; i<size ;i++) {
             if(oldOrderList[i].first != id){
                 orderList.push_back(oldOrderList[i]);
             }
        }
        oldOrderList.clear();
    }
}
Customer* Table::getCustomer(int id){
    for ( size_t i=0; i<getCustomers().size(); i++) {
        if (customersList[i]->getId() == id) {
            return customersList[i];
        }
    }
    return nullptr;
}
std::vector<Customer*>& Table::getCustomers(){
    return customersList;
}

std::vector<OrderPair>& Table::getOrders() {
    return orderList;
}
bool Table::isOpen()  {
    return open;
}
bool Table::customerFound(int id) {
    bool found = false;
    for ( size_t i = 0; !found && i < getCustomers().size(); i++) {
        if (customersList[i]->getId() == id) {
            found = true;
        }
    }
    return found;
}

std::vector<OrderPair> Table::getCustomerDish(int id) {
    std::vector<OrderPair> customerDishs;
    for (OrderPair const &order:orderList) {
        if (order.first == id) {
            customerDishs.push_back(order);
        }
    }

    return customerDishs;
}
//Action
void Table::openTable() {
        open = true;

}
int Table::getBill() {
    int bill =0 ;
    for (std::pair<int, Dish> const &order:getOrders()){
        bill = bill + order.second.getPrice();
    }
    return bill;
}

void Table::closeTable() {
    //reset the table
    for ( size_t i = 0; i <getCustomers().size() ; i++) {
        delete customersList[i];
    }
    customersList.clear();
    orderList.clear();
    open = false;


}

void Table::order(const std::vector<Dish> &menu) {
    for (Customer* const &customer:customersList) {
           int customerId = customer->getId();
           std::vector<int> dishsId = customer->order(menu);
           for (int id :dishsId) {
               OrderPair p(customerId,menu[id]);
               orderList.push_back(p);
               std::cout << customer->getName() << " ordered " << menu[id].getName() << std::endl;
               }
            dishsId.clear();
           }

}
