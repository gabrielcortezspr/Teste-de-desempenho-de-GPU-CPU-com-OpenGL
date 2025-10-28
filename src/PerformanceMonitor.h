#ifndef PERFORMANCEMONITOR_H
#define PERFORMANCEMONITOR_H

#include <string>
#include <vector>
#include <fstream>
#include <chrono>

struct PerformanceData {
    double timestamp;
    double fps;
    double cpuUsage;
    double gpuUsage;
    int triangleCount;
    bool hasLighting;
    bool hasTextures;
};

class PerformanceMonitor {
private:
    std::vector<PerformanceData> dataPoints;
    std::chrono::high_resolution_clock::time_point startTime;
    
    // Informações do sistema
    std::string cpuInfo;
    std::string gpuInfo;
    int cpuCores;
    
    // Métodos para obter informações do sistema
    std::string getCPUInfo();
    std::string getGPUInfo();
    int getCPUCoreCount();
    double getCPUUsage();
    double getGPUUsage();

public:
    PerformanceMonitor();
    ~PerformanceMonitor();
    
    void initialize();
    void update();
    void addDataPoint(double fps, int triangleCount, bool hasLighting = false, bool hasTextures = false);
    
    // Geração de relatórios
    void generateCSVReport(const std::string& filename);
    void generatePerformanceReport(const std::string& filename);
    
    // Getters para informações do sistema
    std::string getSystemInfo();
    std::vector<PerformanceData> getDataPoints() const { return dataPoints; }
};

#endif
