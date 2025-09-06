#pragma once
#include <string>
#include <iostream>

class CreditReport {
public:
    void fetchDetails(const std::string& name, const std::string& address,
                      const std::string& inquiry, const std::string& keyword);
    void display();
    std::string getCustomerData() const { return customerData; }
private:
    std::string customerData;
};

