// SOLID Principles

// S — Single Responsibility Principle
// A class should have one reason to change. "One reason" maps to one business concern.

// SRP Violation — God class
// class ParkingLot {
//     void processEntry(Vehicle& v) {
//         auto spot = findSpot(v);        // spot logic
//         auto ticket = generateTicket(); // ticket logic
//         double fee = calculateFee();    // fee logic
//         processPayment(fee);            // payment logic
//         updateDisplay();                // display logic
//     }
// };

// SRP Correct — each class has one job
// class SpotAllocator   { /* finds/assigns spots */ };
// class TicketService   { /* creates/validates tickets */ };
// class FeeCalculator   { /* computes fees */ };
// class PaymentService  { /* processes payments */ };
// class DisplayService  { /* updates boards */ };

// O — Open/Closed Principle
// Classes should be open for extension, closed for modification. New behavior should be added by adding new code, not changing existing code.

// OCP Violation — adding "truck" breaks this
// double calculateFee(VehicleType t, double hours) {
//     if (t == VehicleType::CAR) return hours * 20.0;
//     if (t == VehicleType::MOTORCYCLE) return hours * 10.0;
//     // add TRUCK here — reopen and retest!
// }


// class IPricingStrategy {
// public:
//     virtual double calculate(double hours) const = 0;
//     virtual ~IPricingStrategy() = default;
// }

// Why const in calculate()?
// virtual double calculate(double hours) const = 0;

// The const at the end means:
// This function promises not to modify the object's state.

// class CarPricing : public IPricingStrategy {
//     double calculate(double h) const override { return h * 20.0; }
// };
// class TruckPricing : public IPricingStrategy {
//     double calculate(double h) const override { return h * 40.0; }
// };

// L — Liskov Substitution Principle
// Any derived class must be substitutable for its base class without altering program correctness.
// Every vehicle must be able to tell which parking spot it needs.
// Invalid 
// Car → returns COMPACT ✔
// Bike → returns MOTORCYCLE ✔
// Truck → returns LARGE ✔
// EV → throws exception ❌


// I — Interface Segregation Principle
// Clients should not be forced to depend on interfaces they don't use. Split fat interfaces into focused ones.

// ISP Violation — fat interface
// class IGateOperator {
//     virtual void openGate() = 0;
//     virtual void processPayment() = 0;   // exit only
//     virtual void generateTicket() = 0;   // entry only
//     virtual void generateReport() = 0;   // admin only
// };

// // ISP Correct — segregated interfaces
// class IEntryGateOperator {
//     virtual void openGate() = 0;
//     virtual void generateTicket() = 0;
// };
// class IExitGateOperator {
//     virtual void openGate() = 0;
//     virtual void processPayment() = 0;
// };
// class IAdminReporter {
//     virtual void generateReport() = 0;
// };


// D — Dependency Inversion Principle
// High-level modules should not depend on low-level modules — both should depend on abstractions. Inject dependencies through interfaces.
// High-level business logic should not depend on low-level implementation details.
// Both should depend on abstractions (interfaces).


// Don’t depend on concrete classes.
// Depend on contracts (interfaces).


// class ExitGate {
//     CreditCardProcessor processor; // concrete dependency

//     void processPayment(double fee) {
//         processor.charge(fee);
//     }
// };
// The ExitGate (high-level module) is directly tied to:
// CreditCardProcessor (low-level implementation)

// class ExitGate {
// private:
//     IPaymentProcessor& processor_;

// public:
//     explicit ExitGate(IPaymentProcessor& p)
//         : processor_(p) {}

//     void processPayment(double fee) {
//         processor_.processPayment(fee);
//     }
// };

// Why This Is Called “Dependency Inversion”
// High-level → Interface ← Low-level

#include<iostream>
#include<string>
#include<chrono>
#include<vector>
#include<memory>
#include<unordered_map>
#include<cmath>
using namespace std;
//Enum Classes
enum class SpotType{
    MOTORCYCLE,
    COMPACT,
    LARGE,
    ELECTRIC
};

enum class TicketState { 
    ISSUED,
    ACTIVE, 
    PAYMENT_PENDING, 
    CLOSED 
};

enum class VehicleType{
    MOTORCYCLE ,
    CAR,
    TRUCK, 
    ELECTRIC
};

//SingleTon Class 
class ParkingLot{
private:
    ParkingLot() = default; //Always create private contructor for singleTon class
    vector<ParkingFloor> floor_;
public:
    static ParkingLot& getInstance(){
        static ParkingLot instance;
        return instance;
    }
    //delete copy / move --> Confirms only 1 instance
    ParkingLot(const ParkingLot&) = delete;
    ParkingLot& operator=(const ParkingLot&) = delete;
};

class Vehicle {
private:
    string licensePlate_;
public:
    explicit Vehicle(string plate) : licensePlate_(move(plate)){}
    virtual VehicleType getVehicleType() const = 0;
    virtual ~Vehicle() = default; 
    const string getLicensePlate() const {
        return licensePlate_;
    }
};

class Car: public Vehicle{
public:
    explicit Car(const string plate) : Vehicle(move(plate)){}
    VehicleType getVehicleType() const override { return VehicleType::CAR; }
};

class MotorCycle : public Vehicle {
public: 
    explicit MotorCycle(const string plate) : Vehicle(move(plate)){}
    VehicleType getVehicleType() const override {
        return VehicleType::MOTORCYCLE;
    }
};

class Truck : public Vehicle {
public: 
    explicit Truck(const string plate) : Vehicle(move(plate)){}
    VehicleType getVehicleType() const override {
        return VehicleType::TRUCK;
    }
};


//Why not singleton class?
class ParkingSpot{
private:
    //Wby underscores?
    string spotId_;
    SpotType type_;
    Vehicle* vehicle_{nullptr};

protected:
    //why move(id) -> TRANSFER of ownership (no copy) -> O(n) memory copy -> spotId_ → "P12345"
    // id → empty / valid but unspecified
    ParkingSpot(string id , SpotType t) : spotId_(move(id)) , type_(t) {}
public:
    virtual ~ParkingSpot() = default;
    virtual bool canFitVehicle(VehicleType type) const = 0;

    bool isOccupied() const {
        return (vehicle_ != nullptr);
    }

    bool assignVehicle(Vehicle* v) {
        if(isOccupied()) return false;
        if(nullptr == v || !canFitVehicle(v->getVehicleType())) return false;
        vehicle_ = v; 
        return true;
    }

    void freeSpot() {
        vehicle_ = nullptr;
    }

    // why reference ? why 2 const ?
    // 1. Returned string is not modified and 2nd is the method doesnot modify its parameter as well
    // We return const reference to avoid unnecessary string copying and to 
    // prevent external modification. The method itself is const to guarantee it does not modify object state and can be called on const objects.
    const string& getSpotId() const {
        return spotId_;
    }

    // why not 2 const and reference ?
    // SpotType is a small enum, so returning by value is more efficient and simpler than returning a reference.
    // Reference returns are useful only for large objects to avoid copying overhead.
    SpotType getSpotType()  const {
        return type_;
    }
};


//SpotType Concrete Classes
class CompactSpot : public ParkingSpot {
public:
    // Why explicit , Explain this whole method in depth 
    // explicit prevents unintended implicit conversions when constructing CompactSpot
    // Invalid -> CompactSpot spot = "P1";
    // valid -> CompactSpot spot("P1"); // ✔ correct
    explicit  CompactSpot(string id) : ParkingSpot(move(id), SpotType::COMPACT){}

    bool canFitVehicle(VehicleType t) const override{
        return (t == VehicleType::CAR || t == VehicleType :: MOTORCYCLE);
    } 
};

class LargeSpot : public ParkingSpot {
public:
    explicit LargeSpot(string id) : ParkingSpot(move(id) , SpotType::LARGE) {}
    bool canFitVehicle(VehicleType v) const override{
        return true;
    }
};


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

//Observer 
class ISpotObserver {
public:
    virtual void onSpotChanges() = 0;
    virtual ~ISpotObserver() = default;
};


class ParkingFloor {
private:
    string floorId_;
    vector<unique_ptr<ParkingSpot>> spots_;
    vector<ISpotObserver*> observers_;
public:
    explicit ParkingFloor (string id) : floorId_(move(id)) {
        cout<<"Inside the Contructor of ParkingFloor"<<endl;
    }
    
    void addObserver(ISpotObserver* obs) {
        if(obs) {
            observers_.push_back(obs);
            cout<<"Observer got added to the list"<<endl;
        }
    }

    void notifyObserver(){
        for(auto *obs : observers_){
            obs->onSpotChanges();
        }
    }

    void addSpot(unique_ptr<ParkingSpot> spot) {
        if(spot) spots_.push_back(move(spot));
    }

    ParkingSpot* getFreeSlot(VehicleType type){
        for(const auto &spot:spots_) {
            if(!spot->isOccupied() && spot->canFitVehicle(type)){
                return spot.get();
            }
        }
        return nullptr;
    }

    int getAvailableCount(SpotType type) const {
        int count = 0;
        for(const auto& spot:spots_) {
            if(!spot->isOccupied() && (spot->getSpotType() == type)) count++;
        }
        return count;
    }

    const string getFloorId() const {
        return floorId_;
    }

    //Why No desturctor?
};


class DisplayBoard : public ISpotObserver{
private:
    // which strategy ?
    const ParkingFloor& floor_;
    unordered_map<SpotType, int> availableCounts_;
public:
    explicit DisplayBoard(ParkingFloor& id) : floor_(id) {}
    void onSpotChanges() override {
        update();
    }
    void update() {
        availableCounts_[SpotType::COMPACT] = floor_.getAvailableCount(SpotType::COMPACT);
        availableCounts_[SpotType::LARGE] = floor_.getAvailableCount(SpotType::LARGE);
        availableCounts_[SpotType::MOTORCYCLE] = floor_.getAvailableCount(SpotType::MOTORCYCLE);
        availableCounts_[SpotType::ELECTRIC] = floor_.getAvailableCount(SpotType::ELECTRIC);
    }

    void displayCount() {
        std::cout << floor_.getFloorId() << std::endl;
        std::cout << "Available Count" << std::endl;

        for (const auto& data : availableCounts_) {

            switch (data.first) {

                case SpotType::LARGE:
                    std::cout << "For Large Vehicles: ";
                    std::cout << data.second << std::endl;
                    break;

                case SpotType::COMPACT:
                    std::cout << "For Compact Vehicles: ";
                    std::cout << data.second << std::endl;
                    break;

                case SpotType::MOTORCYCLE:
                    std::cout << "For Motorcycle Vehicles: ";
                    std::cout << data.second << std::endl;
                    break;

                case SpotType::ELECTRIC:
                    std::cout << "For Electric Vehicles: ";
                    std::cout << data.second << std::endl;
                    break;

                default:
                    std::cout << "Key Not found" << std::endl;
                    break;
            }
        }
    }
};



// FeeCalculator acts as a Context

// In the Strategy Pattern there are usually 3 parts:

// Strategy Interface
//        ↑
// Concrete Strategies
//        ↑
// Context

// In your design:

// IPricingStrategy
//        ↑
// HourlyPricing
// WeekendPricing
// PremiumPricing
//        ↑
// FeeCalculator

// FeeCalculator is the Context.
class IPricingStrategy {
public:
    virtual double calculateFee(const Ticket& t) const = 0;
    virtual ~IPricingStrategy() = default;
};

class HourlyPricing : public IPricingStrategy {
private:
    double rate_;
public:
    explicit HourlyPricing(double ratePerHour) : rate_(ratePerHour) {
        cout<<"HourlyPricing Contructore created"<<endl;
    }

    double calculateFee(const Ticket& t) const override {
        return std::ceil(t.getParkedHours()) * rate_;
    }
};

class FeeCalculator {
private:
    unique_ptr<IPricingStrategy> strategy_;
public:
    explicit FeeCalculator(unique_ptr<IPricingStrategy> strategy) : strategy_(move(strategy)){}
    void setStrategy(unique_ptr<IPricingStrategy> strategy) {
        strategy_ = move(strategy);
    }
    double calculate(const Ticket& t) const { 
        return strategy_->calculateFee(t);
    }
};



