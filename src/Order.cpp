#include "../include/Order.h"
using std::to_string;
//---------------class Order------------------

Order::Order(int id, int customerId, int distance) : 
id(id), customerId(customerId), distance(distance),status(OrderStatus::PENDING),
 collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}

int Order::getId() const{ return id; }
int Order::getCustomerId() const{ return customerId; }
void Order::setStatus(OrderStatus status){ this->status = status; }
void Order::setCollectorId(int collectorId){ this->collectorId = collectorId; }
void Order::setDriverId(int driverId){ this->driverId = driverId; }
int Order::getCollectorId() const{ return collectorId; }
int Order::getDriverId() const{ return driverId; }
OrderStatus Order::getStatus() const{ return status; }

int Order::getDistance() const {
    return distance;
}

const string Order::toString() const{
    string status_str = "";
    switch (status)
    {
    case OrderStatus::PENDING:
        status_str = "PENDING";
        break;
    case OrderStatus::COLLECTING:
        status_str = "COLLECTING";
        break;
    case OrderStatus::DELIVERING:
        status_str = "DELIVERING";
        break;
    case OrderStatus::COMPLETED:
        status_str = "COMPLETED";
        break;
    }
    string str=
    "orderId: "+to_string(id)+"\n"+
    "orderStatus: "+status_str+"\n"+
    "customerId: "+to_string(customerId)+"\n"+ 
    "collectorId: "+ (collectorId < 0  ? "None" : to_string(collectorId)) +"\n"+
    "driverId: "+ (driverId < 0 ? "None" : to_string(driverId)) +"\n";

    return str;
}