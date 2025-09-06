#include "credit_report.h"
#include <sqlite3.h>
#include <algorithm>

static std::string trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) start++;
    auto end = s.end();
    do { end--; } while (std::distance(start, end) > 0 && std::isspace(*end));
    return std::string(start, end + 1);
}

void CreditReport::fetchDetails(const std::string& name, const std::string& address,
                                const std::string& inquiry, const std::string& keyword) {
    sqlite3* db;
    sqlite3_open("data/customers.db", &db);

    std::string sql = "SELECT credit_history FROM customers WHERE name=? COLLATE NOCASE AND address=? COLLATE NOCASE;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    std::string trimmedName = trim(name);
    std::string trimmedAddress = trim(address);

    sqlite3_bind_text(stmt, 1, trimmedName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, trimmedAddress.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        customerData = "Name: " + trimmedName + "\nAddress: " + trimmedAddress +
                       "\nInquiry: " + inquiry + "\nKeyword: " + keyword +
                       "\nCredit History: " + std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    } else {
        customerData = "Customer not found.";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void CreditReport::display() {
    std::cout << "Customer Credit Report:\n" << customerData << std::endl;
}