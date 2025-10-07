#include "PIDController.cpp"
#include <iostream>
#include <fstream>
#include <vector>

int main(){
    PIDController pid(1000, 10, 0.1, 0.5);

    std::fstream file;
    file.open("PIDControllerInput.txt", std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Kann nicht die Datei oeffnen" << std::endl;
        return 0;
    }

    std::vector<std::pair<double, double>> input;
    double a, b;
    while(file >> a >> b)   input.emplace_back(a, b);

    file.close();

    for(auto i = 0; i < input.size(); i++){
        pid.CalculateU(input[i].first, input[i].second);
        std::cout << pid.GetU() << std::endl;
    }

    return 0;
}