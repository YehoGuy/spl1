#pragma once
#include <string>
#include <vector>

#include "Order.h"
#include "Customer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer& getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        //--added for actions(Guy)--
        bool doesCustomerExist(int customerId) const;
        Customer& getCustomerById(int customerId) const;
        void addCustomer(Customer* customer);
        bool doesOrderExist(int orderId) const; //search
        int assignCustomerId();
        int assignVolunteerId();
        int assignOrderId();
        // (Yonatan)
        void deleteVolunteer(int volID);
        bool doesVolunteerExists(int id) const;
        vector<BaseAction*> getActionsLog() const;
        vector<Volunteer*> getVolunteers() const;
        vector<Order*> getPendingOrders() const;
        vector<Order*> getInProcessOrders() const;
        vector<Order*> getCompletedOrders() const;
        vector<Customer*> getCustomers() const;
        void mooveOrder(const Order & order); // moves order from vector to  next vector depending on its status (Collectenig to inProccses compleated to finished etc)


    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        //--added for actions(Guy)--
        int orderCounter; //For assigning unique order IDs

        //from the moodle forum - customers and orders are never deleted, volunteers do.
        //cant access a vector at id because assignId might was called without inserting accordingly?
};