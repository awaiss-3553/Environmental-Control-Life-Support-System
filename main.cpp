#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// Abstract Base Class
class LifeSupportModule {
protected:
    string moduleID;
    double corePowerDrawKW;

public:
    // Static tracker: Active modules operating in the habitat
    static int activeOnlineModules;
    static double totalPowerConsumptionKW;

    LifeSupportModule(string id, double powerDraw)
        : moduleID(id), corePowerDrawKW(powerDraw) {
        activeOnlineModules++;
        totalPowerConsumptionKW += powerDraw;
    }

    // Virtual Destructor ensures derived state is cleaned safely
    virtual ~LifeSupportModule() {
        cout << "[DECOMMISSION] Module " << moduleID << " offline. Power restored." << endl;
        activeOnlineModules--;
        totalPowerConsumptionKW -= corePowerDrawKW;
    }

    // Pure Virtual Interfaces
    virtual double calculateEfficiencyRating() const = 0;
    virtual void runDiagnostics() const = 0;

    string getModuleID() const { return moduleID; }
    double getPowerDraw() const { return corePowerDrawKW; }
};

// Static initializations outside class boundary
int LifeSupportModule::activeOnlineModules = 0;
double LifeSupportModule::totalPowerConsumptionKW = 0.0;

// Derived Class 1: Oxygen Generation Subsystem (OGS)
class OxygenGenerator : public LifeSupportModule {
private:
    double litersPerMinuteProduction;
    double electrolysisWaterLevelLiters;

public:
    OxygenGenerator(string id, double powerDraw, double lpm, double waterLiters)
        : LifeSupportModule(id, powerDraw),
          litersPerMinuteProduction(lpm),
          electrolysisWaterLevelLiters(waterLiters) {}

    ~OxygenGenerator() override {
        cout << " -> Purging high-pressure O2 line for " << moduleID << "..." << endl;
    }

    double calculateEfficiencyRating() const override {
        // Higher output per kilowatt indicates greater efficiency
        return (litersPerMinuteProduction * 60.0) / corePowerDrawKW;
    }

    void runDiagnostics() const override {
        cout << "\n==============================================" << endl;
        cout << "  MODULE: " << moduleID << " [OXYGEN GENERATOR]" << endl;
        cout << "==============================================" << endl;
        cout << "  Core Power Draw : " << corePowerDrawKW << " kW" << endl;
        cout << "  O2 Flow Rate    : " << litersPerMinuteProduction << " L/min" << endl;
        cout << "  Electrolyte Res : " << electrolysisWaterLevelLiters << " Liters" << endl;
        cout << "  Efficiency Index: " << fixed << setprecision(2)
             << calculateEfficiencyRating() << " L/kWh" << endl;
    }
};

// Derived Class 2: Carbon Dioxide Removal Assembly (CDRA)
class CarbonScrubber : public LifeSupportModule {
private:
    double scrubbingPurityPercentage;
    int bedDesiccantCycles;

public:
    CarbonScrubber(string id, double powerDraw, double purityPct, int cycles)
        : LifeSupportModule(id, powerDraw),
          scrubbingPurityPercentage(purityPct),
          bedDesiccantCycles(cycles) {}

    ~CarbonScrubber() override {
        cout << " -> Sealing zeolite adsorbent beds for " << moduleID << "..." << endl;
    }

    double calculateEfficiencyRating() const override {
        // Efficiency scales with purity achieved per energy consumed
        return (scrubbingPurityPercentage * 10.0) / corePowerDrawKW;
    }

    void runDiagnostics() const override {
        cout << "\n==============================================" << endl;
        cout << "  MODULE: " << moduleID << " [CO2 SCRUBBER ASSEMBLY]" << endl;
        cout << "==============================================" << endl;
        cout << "  Core Power Draw : " << corePowerDrawKW << " kW" << endl;
        cout << "  Air Purity Level: " << scrubbingPurityPercentage << " %" << endl;
        cout << "  Zeolite Cycles  : " << bedDesiccantCycles << " cycles" << endl;
        cout << "  Efficiency Index: " << fixed << setprecision(2)
             << calculateEfficiencyRating() << " pts/kWh" << endl;
    }
};

int main() {
    cout << "\n>>> INITIALIZING ORBITAL HABITAT ECLSS BUS <<<\n" << endl;

    // Array of base class pointers managing polymorphic derived types
    const int CAPACITY = 2;
    LifeSupportModule* stationGrid[CAPACITY];

    // Heap allocation
    stationGrid[0] = new OxygenGenerator("OGS-ALPHA-01", 12.5, 5.4, 250.0);
    stationGrid[1] = new CarbonScrubber("CDRA-BETA-02", 8.2, 99.4, 1420);

    // Dynamic dispatch via base pointers
    for (int i = 0; i < CAPACITY; i++) {
        stationGrid[i]->runDiagnostics();
    }

    cout << "\n----------------------------------------------" << endl;
    cout << "Active Habitat Modules Online : " << LifeSupportModule::activeOnlineModules << endl;
    cout << "Total Grid Power Consumption  : " << LifeSupportModule::totalPowerConsumptionKW << " kW" << endl;
    cout << "----------------------------------------------\n" << endl;

    cout << ">>> COMMENCING HABITAT MAINTENANCE SHUTDOWN <<<\n" << endl;

    // Controlled heap deallocation
    for (int i = 0; i < CAPACITY; i++) {
        delete stationGrid[i];
        stationGrid[i] = nullptr;
    }

    cout << "\n----------------------------------------------" << endl;
    cout << "Active Habitat Modules Online : " << LifeSupportModule::activeOnlineModules << endl;
    cout << "Total Grid Power Consumption  : " << LifeSupportModule::totalPowerConsumptionKW << " kW" << endl;
    cout << "----------------------------------------------\n" << endl;

    return 0;
}
