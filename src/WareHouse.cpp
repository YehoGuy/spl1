#include "../include/WareHouse.h"
#include "../include/Action.h"
#include <fstream>
#include <sstream>
#include <iostream>

//-----------------class WareHouse-----------------

WareHouse::WareHouse(const string& configFilePath): 
isOpen(false), customerCounter(0), volunteerCounter(0), orderCounter(0), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers()
{
    string line;
    std::ifstream file(configFilePath);
    while (getline (file, line)) {
        std::istringstream iss(line);
            std::vector<string> words;
            string word;
            while (iss >> word) {
                words.push_back(word);
            }
            if(words[0]=="customer")
            {
                AddCustomer* addCustomer = new AddCustomer(words[1], words[2], std::stoi(words[3]), std::stoi(words[4]));
            }
            if(words[0]=="volunteer")
            {
                if(words[2] == "collector")
                {
                    CollectorVolunteer *collectorVolunteer = new CollectorVolunteer(assignVolunteerId(), words[1], std::stoi(words[3]));
                }
                else if(words[2] == "limited_collector")
                {
                    LimitedCollectorVolunteer *limitedCollectorVolunteer = new LimitedCollectorVolunteer(assignVolunteerId(), words[1], std::stoi(words[3]), std::stoi(words[4]));
                }
                else if(words[2] == "driver")
                {
                    DriverVolunteer *driverVolunteer = new DriverVolunteer(assignVolunteerId(), words[1],std::stoi(words[3]),std::stoi(words[4]));
                }
                else if(words[2] == "limited_driver")
                {
                    LimitedDriverVolunteer *limitedDriverVolunteer = new LimitedDriverVolunteer(assignVolunteerId(), words[1],std::stoi(words[3]),std::stoi(words[4]),std::stoi(words[5]));
                }
                else
                {
                    throw std::invalid_argument("WareHouse constructor: volunteer type is not valid");
                }
                
            }
            // Process the words as needed
    }

    file.close();
}




void WareHouse::start() {
    std::cout << "Warehouse is open!" << std::endl;
    while (isOpen)
    {
        string input;
        BaseAction* action;
        std::getline(std::cin, input);
        std::istringstream iss(input);

        std::vector<string> line;
        string word;
        while (iss >> word) {
            line.push_back(word);
        }

        word = line[0];
        
        if(word == "step"){
            action = new SimulateStep(std::stoi(line[1]));
            action->act((*this));
            actionsLog.push_back(action);
        }else if( word == "order"){
            action = new AddOrder(std::stoi(line[1]));
            action->act((*this));
            actionsLog.push_back(action);
        }else if(word == "customer"){
            action = new AddCustomer(line[1], line[2],std::stoi(line[3]),std::stoi(line[4]));
            action->act((*this));
            actionsLog.push_back(action);   
        }else if(word == "orderStatus"){
            action = new PrintOrderStatus(std::stoi(line[1]));
            action->act((*this));
            actionsLog.push_back(action);
        }else if(word == "customerStatus"){
            action = new PrintCustomerStatus(std::stoi(line[1]));
            action->act((*this));
            actionsLog.push_back(action);
        }else if(word == "volunteerStatus"){
            action = new PrintVolunteerStatus(std::stoi(line[1]));
            action->act((*this));
            actionsLog.push_back(action);
        }else if(word == "log"){
            action = new PrintActionsLog();
            action->act((*this));
            actionsLog.push_back(action);
        }else if(word == "close"){
            action = new Close();
            action->act((*this));
            actionsLog.push_back(action);
        }else if(word == "backup"){
            action = new BackupWareHouse();
            action->act((*this));
            actionsLog.push_back(action);
        }else if(word == "restore"){
            action = new RestoreWareHouse();
            action->act((*this));
            actionsLog.push_back(action);
        }
        else{
            printf("Invalid input\n");
        }
        
    }
}

void WareHouse::addOrder(Order* order) {
    pendingOrders.push_back(order);
}

void WareHouse::addAction(BaseAction* action) { //we need to add actions into actionslog
    actionsLog.push_back(action);
}

Customer& WareHouse::getCustomer(int customerId) const {
    for (Customer* customer : customers) {
        if (customer->getId() == customerId) {
            return *customer;
        }
    }
    throw std::invalid_argument("Customer not found");
}

Volunteer& WareHouse::getVolunteer(int volunteerId) const {
    for (Volunteer* volunteer : volunteers) {
        if (volunteer->getId() == volunteerId) {
            return *volunteer;
        }
    }
    throw std::invalid_argument("Volunteer not found");
}

Order& WareHouse::getOrder(int orderId) const {
    for (Order* order : pendingOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    for (Order* order : inProcessOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    for (Order* order : completedOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    throw std::invalid_argument("Order not found");
}

const vector<BaseAction*>& WareHouse::getActions() const {
    return actionsLog;
}

void WareHouse::close() {
    isOpen = false; //??
}

void WareHouse::open() {
    isOpen = true; 
    this -> start();
}

//-----------------added for actions(Guy)-----------------

bool WareHouse::doesCustomerExist(int customerId) const {
    for (Customer* customer : customers) {
        if (customer->getId() == customerId) {
            return true;
        }
    }
    return false;
}

void WareHouse::addCustomer(Customer* customer) {
    customers.push_back(customer);
}

bool WareHouse::doesOrderExist(int orderId) const {
    for (Order* order : pendingOrders) {
        if (order->getId() == orderId) {
            return true;
        }
    }
    for (Order* order : inProcessOrders) {
        if (order->getId() == orderId) {
            return true;
        }
    }
    for (Order* order : completedOrders) {
        if (order->getId() == orderId) {
            return true;
        }
    }
    return false;
}

int WareHouse::assignCustomerId() {
    int id = customerCounter;
    customerCounter++;
    return id;
}

int WareHouse::assignVolunteerId() {
    int id = volunteerCounter;
    volunteerCounter++;
    return id;
}

int WareHouse::assignOrderId() {
    int id = orderCounter;
    orderCounter++;
    return id;
}

Order* WareHouse::removePendingOrder(int orderId) {
    for (int i = 0; i < pendingOrders.size(); i++) {
        if (pendingOrders[i]->getId() == orderId) {
            Order* order = pendingOrders[i];
            pendingOrders.erase(pendingOrders.begin() + i);
            return order;
        }
    }
    throw std::invalid_argument("removePendingOrder: Order not found");
}

Order* WareHouse::removeInProcessOrder(int orderId) {
    for (int i = 0; i < inProcessOrders.size(); i++) {
        if (inProcessOrders[i]->getId() == orderId) {
            Order* order = inProcessOrders[i];
            inProcessOrders.erase(inProcessOrders.begin() + i);
            return order;
        }
    }
    throw std::invalid_argument("removeInProcessOrder: Order not found");
}

//-----------------added for actions(Yonatan)-----------------

void WareHouse::deleteVolunteer(int volID) {
    for (int i = 0; i < volunteers.size(); i++) {
        if (volunteers[i]->getId() == volID) {
            delete volunteers[i];
            volunteers.erase(volunteers.begin() + i);
            return;
        }
    }
}

bool WareHouse::doesVolunteerExists(int id) const {
    for (Volunteer* volunteer : volunteers) {
        if (volunteer->getId() == id) {
            return true;
        }
    }
    return false;
}

vector<BaseAction*> WareHouse::getActionsLog() const { //we already have a getter for actionsLog
    return actionsLog;
}

vector<Volunteer*> WareHouse::getVolunteers() const {
    return volunteers;
}

vector<Order*> WareHouse::getPendingOrders() const {
    return pendingOrders;
}

vector<Order*> WareHouse::getInProcessOrders() const {
    return inProcessOrders;
}

vector<Order*> WareHouse::getCompletedOrders() const {
    return completedOrders;
}

vector<Customer*> WareHouse::getCustomers() const {
    return customers;
}

void WareHouse::pickedUpByCollector(int orderId) {
    Order* order = removePendingOrder(orderId);
    inProcessOrders.push_back(order);
}

void WareHouse::finishedCollecting(int orderId) {
    Order* order = removeInProcessOrder(orderId);
    pendingOrders.push_back(order);
}

void WareHouse::pickedUpByDriver(int orderId) {
    Order* order = removePendingOrder(orderId);
    inProcessOrders.push_back(order);
}

void WareHouse::finishedDelivering(int orderId) {
    Order* order = removeInProcessOrder(orderId);
    completedOrders.push_back(order);
}



//TODO: rule of 5

