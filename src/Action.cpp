#include "../include/Action.h"
#include <iostream>

//----------BaseAction----------

BaseAction::BaseAction(){}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    std::cout << "Error: " << errorMsg << std::endl;
}

string BaseAction::getErrorMsg() const {
    return errorMsg;
}

//----------SimulateStep----------

//TODO: implement 

//----------AddOrder----------

AddOrder::AddOrder(int id) : BaseAction::BaseAction(), customerId(id) {}

void AddOrder::act(WareHouse &wareHouse) {
    if(!wareHouse.doesCustomerExist(customerId)){
        error("Cannot place this order");
    }
    else if(!wareHouse.getCustomerById(customerId).canMakeOrder()){
        error("Cannot place this order");
    }else{
        int orderId = wareHouse.assignOrderId();
        Customer& customer = wareHouse.getCustomerById(customerId);
        int distance = customer.getCustomerDistance();
        Order* order = new Order(orderId, customerId, distance);
        if(customer.addOrder(orderId) == -1){
            error("Cannot place this order");
        }else{
            wareHouse.addOrder(order);
            complete();
        }
    } 
}

string AddOrder::toString() const {
    return "customerId: "+std::to_string(customerId);
}

AddOrder *AddOrder::clone() const {
    return new AddOrder(*this);
}

//----------AddCustomer----------

AddCustomer::AddCustomer(const string& customerName, const string& customerType, int distance, int maxOrders):
    BaseAction::BaseAction(),
    customerName(customerName),
    customerType(customerType == "soldier" ? CustomerType::Soldier : CustomerType::Civilian), 
    distance(distance),
    maxOrders(maxOrders)
    {}

void AddCustomer::act(WareHouse &wareHouse){
    int customerId = wareHouse.assignCustomerId();
    Customer* customer;
    switch(customerType){
        case CustomerType::Soldier:
            customer = new SoldierCustomer(customerId, customerName, distance, maxOrders);
            break;
        case CustomerType::Civilian:
            customer = new CivilianCustomer(customerId, customerName, distance, maxOrders);
            break;
    }
    wareHouse.addCustomer(customer);
    complete();  
}

AddCustomer *AddCustomer::clone() const {
    return new AddCustomer(*this);
}

string AddCustomer::toString() const{
    return "customerName: "+customerName+", customerType: "+(customerType == CustomerType::Soldier ? "Soldier" : "Civilian")+", distance: "+std::to_string(distance)+", maxOrders: "+std::to_string(maxOrders);
}

//----------PrintOrderStatus----------

PrintOrderStatus::PrintOrderStatus(int id) : BaseAction::BaseAction(), orderId(id) {}

void PrintOrderStatus::act(WareHouse& warehouse){
    if(!warehouse.doesOrderExist(orderId)){
        error("Order doesn't exist");
    }else{
        Order& order = warehouse.getOrder(orderId);
        std::cout << "OrderId: " << order.getId() << std::endl;
        switch (order.getStatus())
        {
        case OrderStatus::PENDING:
            std::cout << "OrderStatus: PENDING" << std::endl;
            break;
        case OrderStatus::COLLECTING:
            std::cout << "OrderStatus: COLLECTING" << std::endl;
            break;
        case OrderStatus::DELIVERING:
            std::cout << "OrderStatus: DELIVERING" << std::endl;
            break;
        case OrderStatus::COMPLETED:
            std::cout << "OrderStatus: COMPLETED" << std::endl;
            break;
        default:
            break;
        }
        std::cout << "CustomerID: " << order.getCustomerId() << std::endl;
        if(order.getCollectorId() == -1)
            {std::cout << "Collector: " << "NONE" << std::endl;}
        else 
            {std::cout << "Collector: " << order.getCollectorId() << std::endl;}
        if(order.getDriverId() == -1)
            {std::cout << "Driver: " << "NONE" << std::endl;}
        else 
            {std::cout << "Driver: " << order.getDriverId() << std::endl;}
        
        complete();
    }
}

PrintOrderStatus *PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const {
    return "orderId: "+std::to_string(orderId);
}

//----------PrintCustomerStatus----------

PrintCustomerStatus::PrintCustomerStatus(int customerId) : BaseAction::BaseAction(), customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse& warehouse){
    if(!warehouse.doesCustomerExist(customerId)){
        error("Customer doesn't exist");
    }else{
        Customer& customer = warehouse.getCustomerById(customerId);
        std::cout << "CustomerID: " << customer.getId() << std::endl;
        for(int orderId : customer.getOrdersIds()){
            std::cout << "OrderID: " << orderId << std::endl;
            if(!warehouse.doesOrderExist(orderId)){
                std::cout << "Order doesn't exist in warehouse" << std::endl;
            }else{
                Order& order = warehouse.getOrder(orderId);
                switch (order.getStatus())
                {
                case OrderStatus::PENDING:
                    std::cout << "OrderStatus: PENDING" << std::endl;
                    break;
                case OrderStatus::COLLECTING:
                    std::cout << "OrderStatus: COLLECTING" << std::endl;
                    break;
                case OrderStatus::DELIVERING:
                    std::cout << "OrderStatus: DELIVERING" << std::endl;
                    break;
                case OrderStatus::COMPLETED:
                    std::cout << "OrderStatus: COMPLETED" << std::endl;
                    break;
                default:
                    break;
                }
            }
        }
        std::cout << "numOrdersLeft: " << customer.getMaxOrders() - customer.getNumOrders() << std::endl;
        complete();
    }
}

PrintCustomerStatus *PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const {
    return "customerId: "+std::to_string(customerId);
}

//----------PrintVolunteerStatus----------
PrintVolunteerStatus::PrintVolunteerStatus(int id): BaseAction(), volunteerId(id){}

void PrintVolunteerStatus::act(WareHouse &wareHouse){
    if(!wareHouse.doesVolunteerExists(volunteerId)){
        error("Volunteer doesn't exist");
    }else{
        string s = wareHouse.getVolunteer(volunteerId).toString();//doesnt work for some reson
        std::cout << s << std::endl;
         //change vol to string to help
    }
} 

PrintVolunteerStatus *PrintVolunteerStatus::clone() const{
    return new PrintVolunteerStatus(*this);
}

 string PrintVolunteerStatus::toString() const {
    return "volunteerID: " + std::to_string(volunteerId);
 }
//----------PrintActionsLog----------
    PrintActionsLog::PrintActionsLog(): BaseAction(){}

    void PrintActionsLog::act(WareHouse& wareHouse){
        for(BaseAction* action : wareHouse.getActions()){
            std::cout << action->toString() << std::endl;
        }
    }  

    string PrintActionsLog::toString() const {
        return ""; //what do we need to write here?
    }
//----------Close----------
    Close::Close() :BaseAction(){}

    void Close::act(WareHouse &warehouse){
        for(Order* o : warehouse.getInProcessOrders()){
            std::cout << "OrderID: " + std::to_string(o->getId()) +", CustomerID: " +std::to_string(o->getCustomerId())+
            ", OrderStatus: Collecting" << std::endl;
        }

        for(Order* o : warehouse.getPendingOrders()){
            std::cout << "OrderID: " + std::to_string(o->getId()) +", CustomerID: " +std::to_string(o->getCustomerId())+
            ", OrderStatus: Pending" << std::endl;
        }

        for(Order* o : warehouse.getCompletedOrders()){
            std::cout << "OrderID: " + std::to_string(o->getId()) +", CustomerID: " +std::to_string(o->getCustomerId())+
            ", OrderStatus: Completed" << std::endl;
        }

        for(Volunteer* v : warehouse.getVolunteers()){
            delete v;
        }

        for(Customer* c : warehouse.getCustomers()){
            delete c;
        }
    }

    Close* Close::clone() const{
        return new Close(*this);
    }

    string Close::toString() const {
        return ""; //what do we need to write here?
    }
//----------BackupWareHouse----------

    BackupWareHouse::BackupWareHouse() : BaseAction(){}

    void BackupWareHouse::act(WareHouse &wareHouse){
        backup = new WareHouse(wareHouse);
    }

     BackupWareHouse * BackupWareHouse::clone() const {
        return new BackupWareHouse(*this);
 
     }

     string Close::toString() const {
        return ""; //what do we need to write here?
    }
//----------RestoreWareHouse----------
    RestoreWareHouse::RestoreWareHouse() : BaseAction(){}

    void RestoreWareHouse::act(WareHouse &wareHouse){
        if(backup == nullptr){
            error("Backup is not avalible");
        }else{
            wareHouse = (*backup);
        }
       
        
    }

     RestoreWareHouse * RestoreWareHouse::clone() const {
        return new RestoreWareHouse(*this);
     }

     string Close::toString() const {
        return ""; //what do we need to write here?
    }