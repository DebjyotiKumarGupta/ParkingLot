#pragma once

#include "parking_floor.hpp"
#include "ticket.hpp"
#include "pricing.hpp"
#include "payment.hpp"
#include <unordered_map>
#include <mutex>




class ParkingLot {
private:
    ParkingLot() = default;
    ParkingLot& operator=(const ParkingLot&) = delete;
    ParkingLot(const ParkingLot&) = delete;
    vector<unique_ptr<ParkingFloor>> floors_;
    unordered_map<string , Ticket*> activeTickets_;
    mutex mutex_;
    int ticketCount_{0};

    string generateTicketId() {
        return "TKT-" + to_string(++ticketCount_);
    }

    // vector<unique_ptr
public:
    static ParkingLot& getInstance(){
        static ParkingLot instance;
        return instance;
    }
    ParkingLot& operator=(const ParkingLot&) = delete;
    ParkingLot(const ParkingLot&) = delete;

    void addFloor(unique_ptr<ParkingFloor> floor){
        if(floor) floors_.push_back(move(floor));
    }
};