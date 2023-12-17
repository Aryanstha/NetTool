#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cstdio>

void printColorBanner() {
    std::cout << "\033[1;32m"; // Set text color to green
    std::cout << "=== Network Tools ===" << std::endl;
    std::cout << "\033[0m";    // Reset text color to default
}

std::string executeCommand(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = _popen(cmd, "r");
    if (!pipe) throw std::runtime_error("_popen() failed!");

    try {
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
    } catch (...) {
        _pclose(pipe);
        throw;
    }

    _pclose(pipe);
    return result;
}

void ping(const std::string& target, std::ofstream& report) {
    std::string command = "ping -n 4 " + target;
    report << "Executing: " << command << std::endl;
    std::string result = executeCommand(command.c_str());
    report << result << std::endl;
}

void tracert(const std::string& target, std::ofstream& report) {
    std::string command = "tracert " + target;
    report << "Executing: " << command << std::endl;
    std::string result = executeCommand(command.c_str());
    report << result << std::endl;
}

void nslookup(const std::string& target, std::ofstream& report) {
    std::string command = "nslookup " + target;
    report << "Executing: " << command << std::endl;
    std::string result = executeCommand(command.c_str());
    report << result << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <hostname or IP address>" << std::endl;
        return 1;
    }

    const std::string target = argv[1];
    const std::string reportFileName = "report_" + target + ".txt";

    std::ofstream report(reportFileName);
    if (!report.is_open()) {
        std::cerr << "Failed to open " << reportFileName << std::endl;
        return 1;
    }

    printColorBanner();

    ping(target, report);
    tracert(target, report);
    nslookup(target, report);

    report.close();

    std::cout << "\033[1;32m"; // Set text color to green
    std::cout << "Report generated successfully: " << reportFileName << std::endl;
    std::cout << "\033[0m";    // Reset text color to default

    return 0;
}
