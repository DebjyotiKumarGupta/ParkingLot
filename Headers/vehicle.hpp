#pragma once

#include "types.hpp"




// why not IVehicle?

//Base class
class Vehicle{
private:
    string color_; 
    string plate_;

protected:
    Vehicle(string plate, string color) : color_(move(color)), plate_(move(color)){
        cout<<"Vehicle Constuctor was created"<<endl;
    }

public:
    // noexcept -> it will never throw an exception
    virtual VehicleType getVehicleType() const noexcept = 0;
    const string& getLicensePlate() const noexcept {
        return plate_;
    }
    const string& getColor() const noexcept {
        return color_;
    }
};

class Car : public Vehicle{ 
public:
    //Why not explicit because implicit assignment is not possible
    Car(string plate , string color) : Vehicle(move(plate), move(color)){
        cout<<"Car Contructor created "<<endl;
    }
    VehicleType getVehicleType() const noexcept override {
        return VehicleType::CAR;
    }
};

class Truck : public Vehicle{ 
public:
    //Why not explicit because implicit assignment is not possible
    Truck(string plate , string color) : Vehicle(move(plate), move(color)){
        cout<<"Truck Contructor created "<<endl;
    }
    VehicleType getVehicleType() const noexcept override {
        return VehicleType::TRUCK;
    }
};

class Electric : public Vehicle{ 
public:
    //Why not explicit because implicit assignment is not possible
    Electric(string plate , string color) : Vehicle(move(plate), move(color)){
        cout<<"Electric Contructor created "<<endl;
    }
    VehicleType getVehicleType() const noexcept override {
        return VehicleType::ELECTRIC;
    }
};


class MotorCycle : public Vehicle{ 
public:
    //Why not explicit because implicit assignment is not possible
    MotorCycle(string plate , string color) : Vehicle(move(plate), move(color)){
        cout<<"MotorCycle Contructor created "<<endl;
    }
    VehicleType getVehicleType() const noexcept override {
        return VehicleType::MOTORCYCLE;
    }
};

