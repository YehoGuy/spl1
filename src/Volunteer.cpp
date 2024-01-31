#include "../include/Volunteer.h"
using std::to_string;

//-----------------class Volunteer------------------

Volunteer::Volunteer(int id, const string &name) : id(id) ,name(name) ,completedOrderId(NO_ORDER), activeOrderId(NO_ORDER){}

int Volunteer::getId() const{ 
    return id;
}

const string& Volunteer::getName() const{
    return name;
}

int Volunteer::getActiveOrderId() const{
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const{
    return completedOrderId;
}

bool Volunteer::isBusy() const{
    return activeOrderId != NO_ORDER;
}



//-----------------class CollectorVolunteer------------------

CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown) 
    : Volunteer(id, name), coolDown(coolDown), timeLeft(0){} 

CollectorVolunteer * CollectorVolunteer::clone() const{
    return new CollectorVolunteer(*this);
}

int CollectorVolunteer::getCoolDown() const{
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const{
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown(){
    if(timeLeft > 0){
        timeLeft --;
    }
    return getTimeLeft() == 0;
}

void CollectorVolunteer::step(){
    
    if(decreaseCoolDown() && activeOrderId != NO_ORDER){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

bool CollectorVolunteer::hasOrdersLeft() const { 
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const{
    return activeOrderId == NO_ORDER && timeLeft == 0; //was only timeleft==0
}

void CollectorVolunteer::acceptOrder(const Order &order) {
    if(canTakeOrder(order)){        
        timeLeft = coolDown;
        activeOrderId = order.getId(); 
    }
}

 

string CollectorVolunteer::toString() const{  
    return "volunteerID: "+to_string(getId())+"\n"+
    "isBusy: "+ (isBusy ? "True" : "False")+"\n"+
    "OrderId: "+(!isBusy() ? "None": to_string(getActiveOrderId()))+"\n"+ 
    "TimeLeft: "+ (getTimeLeft() == 0  ? "None" : to_string(getTimeLeft())) +"\n"+
    "OrdersLeft: "+ "No Limit" +"\n";

}

//-----------------class LimitedCollectorVolunteer------------------

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown ,int maxOrders) 
: CollectorVolunteer(id,name,coolDown) , maxOrders(maxOrders) ,ordersLeft(maxOrders){}
 
 
LimitedCollectorVolunteer * LimitedCollectorVolunteer::clone() const{
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const {
    return getNumOrdersLeft !=0;
}


void LimitedCollectorVolunteer::acceptOrder(const Order &order) {
    if(canTakeOrder(order)){
        CollectorVolunteer::acceptOrder(order);
    }
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const{
    return activeOrderId == NO_ORDER && getTimeLeft() == 0 && getNumOrdersLeft() >0;
}

int LimitedCollectorVolunteer::getMaxOrders() const{
    return maxOrders;
}
        
int LimitedCollectorVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const{
    return "volunteerID: "+to_string(getId())+"\n"+
    "isBusy: "+ (isBusy ? "True" : "False")+"\n"+
    "OrderId: "+(!isBusy() ? "None": to_string(getActiveOrderId()))+"\n"+ 
    "TimeLeft: "+ (getTimeLeft() == 0  ? "None" : to_string(getTimeLeft())) +"\n"+
    "OrdersLeft: "+ to_string(getNumOrdersLeft()) +"\n";
}

//-----------------class DriverVolunteer------------------

DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep) 
: Volunteer(id,name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0){}

DriverVolunteer * DriverVolunteer::clone() const{
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const{
    return distanceLeft;
}
int DriverVolunteer::getMaxDistance() const{            
    return maxDistance;
}
        
int DriverVolunteer::getDistancePerStep() const{
    return distanceLeft;
} 

void DriverVolunteer::setDistanceLeft(int newDisLeft){
    if(distanceLeft ==0){
        distanceLeft = newDisLeft;
    }
}

bool DriverVolunteer::decreaseDistanceLeft(){
    if(distanceLeft > distancePerStep){
        distanceLeft =- distancePerStep;
    }else{
        distanceLeft =0;
    }
    return distanceLeft ==0;
}

bool DriverVolunteer::hasOrdersLeft() const {
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const {
    if(getMaxDistance() >= order.getDistance() && hasOrdersLeft() 
        && getDistanceLeft() == 0 && getActiveOrderId() ==NO_ORDER){
        return true;
    }
    return false;
}
void DriverVolunteer::acceptOrder(const Order &order) {
     if(canTakeOrder(order)){   
        activeOrderId = order.getId();     
        distanceLeft = order.getDistance();
    }
   
} // Assign distanceLeft to order's distance


void DriverVolunteer::step() {
   
    if( decreaseDistanceLeft() && activeOrderId != NO_ORDER){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
} 

string DriverVolunteer::toString() const {
    return 
    "volunteerID: "+to_string(getId())+"\n"+
    "isBusy: "+ (isBusy ? "True" : "False")+"\n"+
    "OrderId: "+(!isBusy() ? "None": to_string(getActiveOrderId()))+"\n"+ 
    "TimeLeft: "+ (getDistanceLeft() == 0  ? "None" : to_string(getDistanceLeft())) +"\n"+
    "OrdersLeft: "+ "No Limit" +"\n";
}

//-----------------class LimitedDriverVolunteer------------------

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders) :
    DriverVolunteer(id,name,maxDistance,distancePerStep) , maxOrders(maxOrders), ordersLeft(maxOrders){}



LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const{
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const {
    return ordersLeft !=0;
}
    
bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const {
    return DriverVolunteer::canTakeOrder(order) && hasOrdersLeft;
} // Signal if the volunteer is not busy, the order is within the maxDistance.

void LimitedDriverVolunteer::acceptOrder(const Order &order) {
    DriverVolunteer::acceptOrder(order);
    ordersLeft--;
} // Assign distanceLeft to order's distance and decrease ordersLeft

string LimitedDriverVolunteer::toString() const {
    return "volunteerId: "+to_string(getId())+"\n"+
    "isBusy: "+ (isBusy ? "True" : "False")+"\n"+
    "OrderId: "+(!isBusy() ? "None": to_string(getActiveOrderId()))+"\n"+ 
    "TimeLeft: "+ (getDistanceLeft() == 0  ? "None" : to_string(getDistanceLeft())) +"\n"+
    "OrdersLeft: "+ to_string(getNumOrdersLeft()) +"\n";
}

