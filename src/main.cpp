#include <iostream>
#include "../include/credit_report_server.h"

void runTests() {
    std::cout << "Running basic tests..." << std::endl;

    // Example test
    if (true) {  // Replace with actual test condition
        std::cout << "Test passed!" << std::endl;
    } else {
        std::cerr << "Test failed!" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--test") {
        runTests();
        return 0;
    }

    CreditReportServer server;
    server.run();
    return 0;
}
