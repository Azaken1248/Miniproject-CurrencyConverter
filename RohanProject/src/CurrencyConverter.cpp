#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

double getExchangeRate(const std::string& baseCurrency, const std::string& targetCurrency) {
    CURL* curl;
    CURLcode res;
    std::string url = "http://api.exchangeratesapi.io/v1/latest?access_key=68e6afe9e1ed12672c57dff62e5d544d&base=" + baseCurrency;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    double exchangeRate = 0.0;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {

            json j = json::parse(readBuffer);

            if (j["rates"].contains(targetCurrency)) {
                exchangeRate = j["rates"][targetCurrency];
            } else {
                std::cout << "Limited Access Error!Please Upgrade API Key To Use This Base." << std::endl;
            }
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return exchangeRate;
}

int main() {
    char choice;

    do {
        std::string baseCurrency, targetCurrency;
        double amount;

        std::cout << "======================" << std::endl;
        std::cout << "Currency Converter Menu" << std::endl;
        std::cout << "======================" << std::endl;

        std::cout << "Enter base currency code (e.g., EUR): ";
        std::cin >> baseCurrency;

        std::cout << "Enter target currency code (e.g., USD): ";
        std::cin >> targetCurrency;

        std::cout << "Enter amount to convert: ";
        std::cin >> amount;

        double exchangeRate = getExchangeRate(baseCurrency, targetCurrency);

        if (exchangeRate != 0.0) {
            double convertedAmount = amount * exchangeRate;
            std::cout << "======================" << std::endl;
            std::cout << "Converted amount: " << convertedAmount << " " << targetCurrency << std::endl;
            std::cout << "======================" << std::endl;
        }

        std::cout << "Do you want to convert another amount? (y/n): ";
        std::cin >> choice;

    } while (choice == 'y' || choice == 'Y');

    return 0;
}
       

