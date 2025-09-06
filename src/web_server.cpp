#include "crow_all.h"
#include "credit_report.h"

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([](){
        crow::response res;
        res.code = 200;
        res.set_header("Content-Type", "text/html");
        res.body =
            "<html><body>"
            "<h2>Credit Report Inquiry</h2>"
            "<form action='/report' method='post'>"
            "Name: <input name='name'><br>"
            "Address: <input name='address'><br>"
            "Inquiry: <input name='inquiry'><br>"
            "Keyword: <input name='keyword'><br>"
            "<input type='submit' value='Get Report'>"
            "</form></body></html>";
        return res;
    });

    CROW_ROUTE(app, "/report").methods("POST"_method)
    ([](const crow::request& req){
        // Parse form data from req.body
        std::string body = req.body;
        std::string name, address, inquiry, keyword;

        auto url_decode = [](std::string value) {
            std::string result;
            for (size_t i = 0; i < value.length(); ++i) {
                if (value[i] == '%') {
                    if (i + 2 < value.length()) {
                        std::string hex = value.substr(i + 1, 2);
                        char ch = static_cast<char>(std::strtol(hex.c_str(), nullptr, 16));
                        result += ch;
                        i += 2;
                    }
                } else if (value[i] == '+') {
                    result += ' ';
                } else {
                    result += value[i];
                }
            }
            return result;
        };

        auto get_value = [url_decode](const std::string& body, const std::string& key) {
            size_t start = body.find(key + "=");
            if (start == std::string::npos) return std::string();
            start += key.length() + 1;
            size_t end = body.find("&", start);
            std::string value = body.substr(start, end - start);
            return url_decode(value);
        };

        name = get_value(body, "name");
        address = get_value(body, "address");
        inquiry = get_value(body, "inquiry");
        keyword = get_value(body, "keyword");

        CreditReport report;
        report.fetchDetails(name, address, inquiry, keyword);

        crow::response res;
        res.code = 200;
        res.set_header("Content-Type", "text/html");
        res.body = "<html><body><pre>" + report.getCustomerData() + "</pre><a href='/'>Back</a></body></html>";
        return res;
    });

    app.port(8080).multithreaded().run();
    return 0;
}