
#include "../include/Action.h"
#include "../include/Volunteer.h"
#include "../include/WareHouse.h"
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

string BaseAction::getStrStatus() const{
    return (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR"); 
}

//----------SimulateStep----------

SimulateStep::SimulateStep(int numOfSteps) :BaseAction(), numOfSteps(numOfSteps){} 

void SimulateStep::act(WareHouse &wareHouse){
    for(int i =0; i< numOfSteps; i++){

        //iterate on all of the orders in the pending list
        //and handle accordingly, either assign a collector or a driver
        for(Order * o : wareHouse.getPendingOrders()){
            for(Volunteer * v: wareHouse.getVolunteers()){
                if(v->canTakeOrder(*o)){
                    v->acceptOrder(*o);
                    o->changeStatus();
                    if(o->getStatus() == OrderStatus::COLLECTING){
                        wareHouse.pickedUpByCollector(o->getId());
                    }else if(o->getStatus() == OrderStatus::DELIVERING){
                        wareHouse.pickedUpByDriver(o->getId());
                    }
                    }
                    break;
                }
            }
        
        //perform a step for each volunteer
        for(Volunteer * v: wareHouse.getVolunteers()){
                v->step();
        }

        //check if any volunteer finished his order
        //and if so handle accordingly
        for (Volunteer * v : wareHouse.getVolunteers()){
            if(v->getHasJustFinished()){
                if(wareHouse.getOrder(v->getCompletedOrderId()).getStatus() == OrderStatus::COLLECTING){
                    wareHouse.finishedCollecting(v->getCompletedOrderId());
                }
                else if(wareHouse.getOrder(v->getCompletedOrderId()).getStatus() == OrderStatus::DELIVERING){
                    wareHouse.finishedDelivering(v->getCompletedOrderId());
                }
                
            }
        }

        //delete limited volunteers who reached limit
        for (Volunteer * v : wareHouse.getVolunteers()){
            if(!v->hasOrdersLeft() && !v->isBusy()){
                wareHouse.deleteVolunteer(v->getId());
            }
        }

    }
}

string SimulateStep::toString() const{
    return "step " + std::to_string(numOfSteps) + " "+ getStrStatus();
}

SimulateStep * SimulateStep::clone() const{
    return new SimulateStep(*this);
}

//----------AddOrder----------

AddOrder::AddOrder(int id) : BaseAction::BaseAction(), customerId(id) {}

void AddOrder::act(WareHouse &wareHouse) {
    if(!wareHouse.doesCustomerExist(customerId)){
        error("Cannot place this order");
    }
    else if(!wareHouse.getCustomer(customerId).canMakeOrder()){
        error("Cannot place this order");
    }else{
        int orderId = wareHouse.assignOrderId();
        Customer& customer = wareHouse.getCustomer(customerId);
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
    return "order "+std::to_string(customerId) +" "+ getStrStatus();
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
    switch((int)customerType){
        case 0:
            customer = new SoldierCustomer(customerId, customerName, distance, maxOrders);
            break;
        case 1:
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
    return "customer "+customerName+" "+(customerType == CustomerType::Soldier ? "soldier" : "civilian")+" "+std::to_string(distance)+" "+std::to_string(maxOrders)+ " " + getStrStatus();
}

//----------PrintOrderStatus----------

PrintOrderStatus::PrintOrderStatus(int id) : BaseAction::BaseAction(), orderId(id) {}

void PrintOrderStatus::act(WareHouse& warehouse){
    if(!warehouse.doesOrderExist(orderId)){
        error("Order doesn't exist");
    }else{
        Order& order = warehouse.getOrder(orderId);
        std::cout << "OrderId: " << order.getId() << std::endl;
        switch ((int)order.getStatus())
        {
        case 0:
            std::cout << "OrderStatus: PENDING" << std::endl;
            break;
        case 1:
            std::cout << "OrderStatus: COLLECTING" << std::endl;
            break;
        case 2:
            std::cout << "OrderStatus: DELIVERING" << std::endl;
            break;
        case 3:
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
    return "orderStatus "+std::to_string(orderId) + " " + getStrStatus();
}

//----------PrintCustomerStatus----------

PrintCustomerStatus::PrintCustomerStatus(int customerId) : BaseAction::BaseAction(), customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse& warehouse){
    if(!warehouse.doesCustomerExist(customerId)){
        error("Customer doesn't exist");
    }else{
        Customer& customer = warehouse.getCustomer(customerId);
        std::cout << "CustomerID: " << customer.getId() << std::endl;
        for(int orderId : customer.getOrdersIds()){
            std::cout << "OrderID: " << orderId << std::endl;
            if(!warehouse.doesOrderExist(orderId)){
                std::cout << "Order doesn't exist in warehouse" << std::endl;
            }else{
                Order& order = warehouse.getOrder(orderId);
                switch ((int)order.getStatus())
                {
                case 0:
                    std::cout << "OrderStatus: PENDING" << std::endl;
                    break;
                case 1:
                    std::cout << "OrderStatus: COLLECTING" << std::endl;
                    break;
                case 2:
                    std::cout << "OrderStatus: DELIVERING" << std::endl;
                    break;
                case 3:
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
    return "customerStatus "+std::to_string(customerId)+ " "+ getStrStatus();
}

//----------PrintVolunteerStatus----------
PrintVolunteerStatus::PrintVolunteerStatus(int id): BaseAction(), volunteerId(id){}

void PrintVolunteerStatus::act(WareHouse &wareHouse){
    if(!wareHouse.doesVolunteerExists(volunteerId)){
        error("Volunteer doesn't exist");
    }else{
        std::cout << (wareHouse.getVolunteer(volunteerId).toString()) << std::endl;
    }
    complete();
} 

PrintVolunteerStatus *PrintVolunteerStatus::clone() const{
    return new PrintVolunteerStatus(*this);
}

 string PrintVolunteerStatus::toString() const {
    return "volunteerStatus " + std::to_string(volunteerId) +" "+ getStrStatus();
 }
//----------PrintActionsLog----------
    PrintActionsLog::PrintActionsLog(): BaseAction(){}

    void PrintActionsLog::act(WareHouse& wareHouse){
        for(BaseAction* action : wareHouse.getActions()){
            std::cout << action->toString() << std::endl;
        }
        complete();
    }  

    PrintActionsLog * PrintActionsLog::clone() const{
        return new PrintActionsLog(*this);
    }
    

    string PrintActionsLog::toString() const {
        return "log"; 
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

        warehouse.close();
    }

    Close* Close::clone() const{
        return new Close(*this);
    }

    string Close::toString() const {
        return "close"; //what do we need to write here?
    }
//----------BackupWareHouse----------

    BackupWareHouse::BackupWareHouse() : BaseAction(){}
    
    extern WareHouse* backup;

    void BackupWareHouse::act(WareHouse &wareHouse){
        backup = new WareHouse(wareHouse);
    }

     BackupWareHouse * BackupWareHouse::clone() const {
        return new BackupWareHouse(*this);
 
     }
    //string toString() const override;
     string BackupWareHouse::toString() const {
        return "backup"; //what do we need to write here?
    }
//----------RestoreWareHouse----------
    RestoreWareHouse::RestoreWareHouse() : BaseAction(){}

    void RestoreWareHouse::act(WareHouse &wareHouse){
        if(backup == nullptr){
            error("No backup available");
        }else{
            wareHouse = (*backup);
        }
       
    }

     RestoreWareHouse * RestoreWareHouse::clone() const {
        return new RestoreWareHouse(*this);
     }

     string RestoreWareHouse::toString() const {
        return "restore"; //what do we need to write here?
    }