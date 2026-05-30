#pragma once

#include "parking_spot.hpp"
#include<chrono>

class Ticket {
private:
    string ticketId_;
    Vehicle* vehicle_;
    ParkingSpot* spot_;
    std::chrono::system_clock::time_point entryTime_;
    TicketState state_{TicketState::ISSUED};
    bool paid_{false};


    bool isValidTransition(TicketState from , TicketState to) {
        if(TicketState::ISSUED == from && TicketState::ACTIVE == to) return true;
        if(TicketState::ACTIVE == from && TicketState::PAYMENT_PENDING == to) return true;
        if(TicketState::PAYMENT_PENDING == from && TicketState::CLOSED == to) return true; 
        return false;
    }

public:
    Ticket(string id, Vehicle* vehicle , ParkingSpot* spot) :
    ticketId_(id) , vehicle_(vehicle), spot_(spot) , 
    entryTime_(std::chrono::system_clock::now()) {
        cout<<"Ticket Constructor Called for TicketId "<<id<<endl;
    }

    bool isPaid() {
        cout<<"isPaid was called"<<endl;
        return paid_;
    }

    void markPaid(){
        cout<<"Ticket with Id " <<ticketId_<<" was marked paid"<<endl;
        paid_ = true; 
    }

    const string& getTicketId() const {
        return ticketId_;
    }

    Vehicle* getVehicle() const {
        return vehicle_;
    }

    ParkingSpot* getSpot() const {
        return spot_;
    }

    std::chrono::system_clock::time_point getEntryTime() const { return entryTime_; }


    // why this ?
    double getParkedHours() const {
        auto now = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<3600>>>(now - entryTime_);
        return duration.count();
    }

    bool transitionTo(TicketState newState) {
        if(!isValidTransition(state_ , newState)){
            throw std::logic_error("Invalid ticket state transition");
        }
        state_ = newState;
        cout<<"Ticket state changed "<<endl;
        return true;
    }
};
