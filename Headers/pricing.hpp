#pragma once

#include "ticket.hpp"


class IPricingStrategy {
public:
    virtual ~IPricingStrategy() = default;
    virtual double calculateFee(const Ticket& ticket) const = 0;
};


class HourlyPricing : public IPricingStrategy {
private:
    double rate_;
public:
    HourlyPricing(double ratePerHour) : rate_(move(ratePerHour)){
        cout<<"Contructor for HourlyPricing called"<<endl;
    }
    virtual double calculateFee(const Ticket& ticket) const { 
        double hours = ceil(ticket.getParkedHours());
        return hours*rate_;
    }
};

class DailyCapPricing : public IPricingStrategy {
private:
    double ratePerDay_; 
    double hourlyRate_;

public:
    DailyCapPricing(double ratePerDay, double hourlyRate) : ratePerDay_(move(ratePerDay)), hourlyRate_(move(hourlyRate)){
        cout<<"Constructor for DailyCapPricing was called"<<endl;
    }

    double calculateFee(const Ticket& ticket) const {
        double hours = ticket.getParkedHours();
        int fullDay = static_cast<int>(hours/24);
        double remainingHours = hours - (fullDay)*24;
        return min(ceil(remainingHours*ratePerDay_), hourlyRate_ ) + (fullDay*ratePerDay_);
    }
};