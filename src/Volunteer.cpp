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


string Volunteer::toString() const{
    string str=
    "customerId: "+to_string(getId())+"\n"+
    "customerName: "+getName() +"\n"+
    "compleatedOrderId: "+(getCompletedOrderId() < 0  ? "None" : to_string(getCompletedOrderId()))+"\n"+ 
    "ActiveOrderId: "+ (getActiveOrderId() < 0  ? "None" : to_string(getActiveOrderId())) +"\n";
    
    return str;
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
        return true;
    }
    return false;
}

void CollectorVolunteer::step(){
    decreaseCoolDown();
    if(timeLeft ==0){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

bool CollectorVolunteer::hasOrdersLeft() const { 
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const{ ///TODO: to Yonatan - why is this not making use of order
    return activeOrderId == NO_ORDER && timeLeft == 0; //was only timeleft==0
}

void CollectorVolunteer::acceptOrder(const Order &order) {
    if(canTakeOrder(order)){        
        timeLeft = coolDown;
        activeOrderId = order.getId(); 
    }
}

 

string CollectorVolunteer::toString() const{ ///TODO: fix Yonatan 
    // i deleted contents beacause it was litteraly a copy of ordertostring and it cause errors
}

//-----------------class LimitedCollectorVolunteer------------------

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown ,int maxOrders) 
: CollectorVolunteer(id,name,coolDown) , maxOrders(maxOrders) ,ordersLeft(maxOrders){}
 
 
LimitedCollectorVolunteer * LimitedCollectorVolunteer::clone() const{ ////notice yonatan we have default copy constructor
    LimitedCollectorVolunteer *retVol = new LimitedCollectorVolunteer(getId(), getName(), getCoolDown(), getMaxOrders());

    for(int i =0; i< (getCoolDown() - getTimeLeft()); i++){
            (*retVol).decreaseCoolDown();
    }// decreses time

    (*retVol).ordersLeft =  getNumOrdersLeft();

    return retVol;
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const {
    return getNumOrdersLeft !=0;
}


void LimitedCollectorVolunteer::acceptOrder(const Order &order) {
    CollectorVolunteer::acceptOrder(order);
    ordersLeft --;
}

int LimitedCollectorVolunteer::getMaxOrders() const{
    return maxOrders;
}
        
int LimitedCollectorVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
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
    if(getMaxDistance() <= order.getDistance() && hasOrdersLeft() && getDistanceLeft() == 0){
        return true;
    }
    return false;
}
void DriverVolunteer::acceptOrder(const Order &order) {
     if(canTakeOrder(order)){   
        activeOrderId = order.getId();     
         distanceLeft = order.getDistance();
        activeOrderId = order.getId(); 
    }
   
} // Assign distanceLeft to order's distance


void DriverVolunteer::step() {
   
    if( decreaseDistanceLeft() && activeOrderId != NO_ORDER){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
} 

string DriverVolunteer::toString() const {
    string str = Volunteer::toString() +
    "maxDIstance: "+ to_string(getMaxDistance()) +"\n" +
    "distancePerStep: "+ to_string(getDistancePerStep())+ "\n" +
    "distanceLeft: " +to_string(getDistanceLeft());

    return str;
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
    string strg = DriverVolunteer::toString() +
    "ordersLeft: "+ to_string(getNumOrdersLeft()) +"\n" +
    "maxOrders: "+ to_string(getMaxOrders())+ "\n" ;
}

