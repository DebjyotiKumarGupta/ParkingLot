#pragma once

#include "types.hpp" 

class IPaymentProcessor {
public:
    virtual ~IPaymentProcessor () = default;
    virtual bool processPayment(double amount , PaymentMethod method) = 0;
};

class CreditCradProcessor : public IPaymentProcessor {
public:
    bool processPayment(double amount , PaymentMethod method) override {
        if(PaymentMethod::CREDIT_CARD != method) return false;
        //integrate real payment gateway here
        return true;
    }
};


class UPIProcessor : public IPaymentProcessor {
public:
    bool processPayment(double amount , PaymentMethod method)  override {
        if(PaymentMethod::UPI != method) return false;
        return true;  
    }
};