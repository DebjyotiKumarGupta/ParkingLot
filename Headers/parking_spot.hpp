#pragma once

#include "vehicle.hpp"


class ParkingSpot{
private:
    string spotId_;
    Vehicle* vehicle_{nullptr} ;
    SpotType type_;

protected:
    ParkingSpot(string id, SpotType t):spotId_(move(id)), type_(t){
        cout<<"ParkingSpot Contructor Called"<<endl;
    }

public:
    virtual ~ParkingSpot() = default;
    virtual bool canFitVehicle(VehicleType t) const noexcept = 0; 
    const Vehicle* getVehicle() const noexcept {
        return vehicle_;
    }
    bool isOccupied() const noexcept {
        return (vehicle_ != nullptr);
    }
    bool assignVehicle(Vehicle* v) noexcept {
        if(!v || isOccupied() || !canFitVehicle(v->getVehicleType())) return false;
        cout<<"Congrats Your Vehicle got Assigned"<<endl;
        vehicle_ = v;
        return true;
    }
    bool freeSpot() noexcept {
        cout<<"Spot got freed"<<endl;
        vehicle_ = nullptr;
    }
    SpotType getSpotType() const noexcept{
        return type_;
    }
    const string& getSpotId() const noexcept {
        return spotId_;
    }
};


class MotorCycleSpot : public ParkingSpot {
public:
    explicit MotorCycleSpot (string id) : ParkingSpot(move(id), SpotType::MOTORCYCLE) {
        cout<<"Contructor called for MotorCylce"<<endl;
    }
    bool canFitVehicle (VehicleType t) const noexcept override{
        return (VehicleType::MOTORCYCLE == t);
    }
};

class CompactSpot : public ParkingSpot {
public:
    explicit CompactSpot (string id) : ParkingSpot(move(id), SpotType::MOTORCYCLE) {
        cout<<"Contructor called for CompactSpot"<<endl;
    }
    bool canFitVehicle (VehicleType t) const noexcept override{
        return (VehicleType::CAR == t || VehicleType::MOTORCYCLE == t);
    }
};

class LargeSpot : public ParkingSpot {
public:
    explicit LargeSpot (string id) : ParkingSpot(move(id), SpotType::MOTORCYCLE) {
        cout<<"Contructor called for LargeSpot"<<endl;
    }
    bool canFitVehicle (VehicleType t) const noexcept override{
        return true;
    }
};

class ElecticSpot : public ParkingSpot {
private:
    bool chargerAvailable_{true};
public:
    explicit ElecticSpot(string id) : ParkingSpot(move(id), SpotType::ELECTRIC) {
        cout<<"ElectricSpot created Successfully"<<endl;
    }
    bool canFitVehicle(VehicleType t) const noexcept override {
        return (VehicleType::ELECTRIC == t);
    }
    bool isChargerAvailable() const noexcept {
        return chargerAvailable_;
    }
 };

