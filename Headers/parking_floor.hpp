
#pragma once 

#include "types.hpp"
#include "parking_spot.hpp"
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
