#pragma once
#include<iostream>
#include<string>
#include<cmath>
#include<memory>
#include<vector>
#include<unordered_map>
using namespace std;

enum class VehicleType{
    MOTORCYCLE, 
    CAR,
    TRUCK, 
    ELECTRIC,
};

enum class SpotType {
    MOTORCYCLE, 
    COMPACT,
    LARGE, 
    ELECTRIC,
};

enum class PaymentMethod {
    CASH, 
    CREDIT_CARD, 
    UPI,
};

enum class TicketState{
    ISSUED,
    ACTIVE,
    PAYMENT_PENDING,
    CLOSED,
};