/*

g++ traffic_controller.cpp -o traffic_controller
./traffic_controller > simulation_results.csv

*/

#include<fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <iomanip>

// 1. Triangular Membership Function
class TriangularMF {
private:
    double a, b, c;

public:
    TriangularMF(double a, double b, double c) : a(a), b(b), c(c) {}

    double getMembership(double x) const {
        if (x <= a || x >= c) return 0.0;
        if (x == b) return 1.0;
        if (x > a && x < b) return (x - a) / (b - a);
        return (c - x) / (c - b);
    }
    
    double getPeak() const { return b; }
};

// 2. Fuzzy Variable
class FuzzyVariable {
public:
    std::string name;
    std::map<std::string, TriangularMF> memberships;

    FuzzyVariable(std::string name) : name(name) {}

    void addMF(std::string linguisticTerm, double a, double b, double c) {
        memberships.insert({linguisticTerm, TriangularMF(a, b, c)});
    }

    double getFuzzifiedValue(std::string linguisticTerm, double crispInput) {
        return memberships.at(linguisticTerm).getMembership(crispInput);
    }
};

// 3. Fuzzy Rule (Mamdani Inference)
class FuzzyRule {
public:
    std::string densityTerm;
    std::string flowTerm;
    std::string cycleLengthTerm;

    FuzzyRule(std::string d, std::string f, std::string c) 
        : densityTerm(d), flowTerm(f), cycleLengthTerm(c) {}

    double evaluate(FuzzyVariable& density, double densityInput, 
                    FuzzyVariable& flow, double flowInput) {
        double densityWeight = density.getFuzzifiedValue(densityTerm, densityInput);
        double flowWeight = flow.getFuzzifiedValue(flowTerm, flowInput);
        return std::min(densityWeight, flowWeight); // Mamdani AND
    }
};

// 4. Main Traffic Controller Engine
class TrafficSignalController {
private:
    FuzzyVariable* density;
    FuzzyVariable* flow;
    FuzzyVariable* cycleLength;
    std::vector<FuzzyRule> rules;

public:
    TrafficSignalController() {
        density = new FuzzyVariable("Density");
        flow = new FuzzyVariable("Flow");
        cycleLength = new FuzzyVariable("CycleLength");

        // Hardcoding Density ranges (0 - 100+)
        density->addMF("VS", 0, 20, 40);
        density->addMF("S", 20, 40, 60);
        density->addMF("M", 40, 60, 80);
        density->addMF("L", 60, 80, 100);
        density->addMF("VL", 80, 100, 120);
        
        // Hardcoding Flow ranges (0 - 1800+)
        flow->addMF("VS", 0, 400, 800);
        flow->addMF("S", 400, 800, 1200);
        flow->addMF("M", 800, 1200, 1600);
        flow->addMF("L", 1200, 1600, 2000);
        flow->addMF("VL", 1600, 2000, 2400);

        // Hardcoding Cycle Length ranges (0 - 100 seconds)
        cycleLength->addMF("VS", 0, 16.67, 33.33);
        cycleLength->addMF("S", 16.67, 33.33, 50.0);
        cycleLength->addMF("M", 33.33, 50.0, 66.67);
        cycleLength->addMF("L", 50.0, 66.67, 83.33);
        cycleLength->addMF("VL", 66.67, 83.33, 100.0);

        // Initializing the 25 Rules from Table 1 of the paper
        // VS Density Rules
        rules.push_back(FuzzyRule("VS", "VS", "VS"));
        rules.push_back(FuzzyRule("VS", "S", "S"));
        rules.push_back(FuzzyRule("VS", "M", "S"));
        rules.push_back(FuzzyRule("VS", "L", "M"));
        rules.push_back(FuzzyRule("VS", "VL", "L"));
        // S Density Rules
        rules.push_back(FuzzyRule("S", "VS", "VS"));
        rules.push_back(FuzzyRule("S", "S", "S"));
        rules.push_back(FuzzyRule("S", "M", "S"));
        rules.push_back(FuzzyRule("S", "L", "M"));
        rules.push_back(FuzzyRule("S", "VL", "L"));
        // M Density Rules
        rules.push_back(FuzzyRule("M", "VS", "S"));
        rules.push_back(FuzzyRule("M", "S", "S"));
        rules.push_back(FuzzyRule("M", "M", "M"));
        rules.push_back(FuzzyRule("M", "L", "M"));
        rules.push_back(FuzzyRule("M", "VL", "L"));
        // L Density Rules
        rules.push_back(FuzzyRule("L", "VS", "S"));
        rules.push_back(FuzzyRule("L", "S", "M"));
        rules.push_back(FuzzyRule("L", "M", "M"));
        rules.push_back(FuzzyRule("L", "L", "L"));
        rules.push_back(FuzzyRule("L", "VL", "L"));
        // VL Density Rules
        rules.push_back(FuzzyRule("VL", "VS", "VS"));
        rules.push_back(FuzzyRule("VL", "S", "S"));
        rules.push_back(FuzzyRule("VL", "M", "M"));
        rules.push_back(FuzzyRule("VL", "L", "L"));
        rules.push_back(FuzzyRule("VL", "VL", "VL"));
    }

    ~TrafficSignalController() {
        delete density;
        delete flow;
        delete cycleLength;
    }

    double computeFuzzyCycleLength(double currentDensity, double currentFlow) {
        std::map<std::string, double> aggregatedOutputs = {
            {"VS", 0.0}, {"S", 0.0}, {"M", 0.0}, {"L", 0.0}, {"VL", 0.0}
        };

        // Fuzzification & Rule Evaluation
        for (FuzzyRule& rule : rules) {
            double ruleStrength = rule.evaluate(*density, currentDensity, *flow, currentFlow);
            // Aggregation (Max operator)
            std::string outputTerm = rule.cycleLengthTerm;
            aggregatedOutputs[outputTerm] = std::max(aggregatedOutputs[outputTerm], ruleStrength);
        }

        // Defuzzification (Weighted Average / Center of Gravity approx)
        double numerator = 0.0;
        double denominator = 0.0;

        for (const auto& pair : aggregatedOutputs) {
            std::string term = pair.first;
            double strength = pair.second;
            
            if (strength > 0) {
                double peak = cycleLength->memberships.at(term).getPeak();
                numerator += peak * strength;
                denominator += strength;
            }
        }

        if (denominator == 0.0) return 0.0;
        return numerator / denominator;
    }

    // Webster's Baseline Formula
    double computeWebsterCycleLength(double flowRate) {
        double L = 10.0; // Lost time is approx 10 seconds
        double saturationFlow = 1950.0; // Vehicles per hour
        double Y = flowRate / saturationFlow; // Critical flow ratio
        
        if (Y >= 1.0) return 120.0; // Cap cycle length if over-saturated
        
        double C = ((1.5 * L) + 5.0) / (1.0 - Y);
        return C;
    }
};

#include <fstream> // Add this at the top with the other includes

// ... [rest of your classes stay exactly the same] ...

int main() {
    TrafficSignalController controller;
    
    // Create and open a text file
    std::ofstream outFile("simulation_results.csv");

    // Output standard CSV headers to the FILE, not the screen
    outFile << "Density,Flow,Fuzzy_Cycle_Length,Webster_Cycle_Length\n";

    std::vector<std::pair<double, double>> testData = {
        {5, 100}, {10, 200}, {15, 300}, {20, 400}, {25, 500},
        {30, 600}, {35, 700}, {40, 800}, {45, 900}, {50, 1000},
        {55, 1100}, {60, 1200}, {65, 1300}, {70, 1400}, {75, 1500}, {80, 1600}
    };

    // Run simulation and output to the file
    for (const auto& data : testData) {
        double density = data.first;
        double flow = data.second;
        
        double fuzzyLength = controller.computeFuzzyCycleLength(density, flow);
        double websterLength = controller.computeWebsterCycleLength(flow);
        
        outFile << std::fixed << std::setprecision(2)
                << density << "," 
                << flow << "," 
                << fuzzyLength << "," 
                << websterLength << "\n";
    }

    // Close the file when done
    outFile.close();
    
    // Print a confirmation to the screen so you know it worked
    std::cout << "Success! Data saved to simulation_results.csv" << std::endl;

    return 0;
}