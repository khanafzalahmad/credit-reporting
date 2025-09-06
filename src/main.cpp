#include "credit_report.h"

int main() {
    std::string name, address, inquiry, keyword;
    std::cout << "Enter customer name: ";
    std::getline(std::cin, name);
    std::cout << "Enter address: ";
    std::getline(std::cin, address);
    std::cout << "Enter inquiry type: ";
    std::getline(std::cin, inquiry);
    std::cout << "Enter keyword: ";
    std::getline(std::cin, keyword);

    CreditReport report;
    report.fetchDetails(name, address, inquiry, keyword);
    report.display();

    return 0;
}