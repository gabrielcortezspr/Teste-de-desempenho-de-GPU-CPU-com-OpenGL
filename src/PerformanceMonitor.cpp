#include "PerformanceMonitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

PerformanceMonitor::PerformanceMonitor() {
    startTime = std::chrono::high_resolution_clock::now();
}

PerformanceMonitor::~PerformanceMonitor() {
}

void PerformanceMonitor::initialize() {
    cpuInfo = getCPUInfo();
    gpuInfo = getGPUInfo();
    cpuCores = getCPUCoreCount();
    
    std::cout << "\n=== Informações do Sistema ===" << std::endl;
    std::cout << "CPU: " << cpuInfo << std::endl;
    std::cout << "GPU: " << gpuInfo << std::endl;
    std::cout << "Núcleos CPU: " << cpuCores << std::endl;
    std::cout << "===============================" << std::endl;
}

void PerformanceMonitor::update() {
    // Esta função será chamada a cada frame para monitoramento contínuo
    // Por enquanto, apenas registra o timestamp
}

void PerformanceMonitor::addDataPoint(double fps, int triangleCount, bool hasLighting, bool hasTextures) {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
    
    PerformanceData data;
    data.timestamp = duration.count() / 1000.0; // Converter para segundos
    data.fps = fps;
    data.cpuUsage = getCPUUsage();
    data.gpuUsage = getGPUUsage();
    data.triangleCount = triangleCount;
    data.hasLighting = hasLighting;
    data.hasTextures = hasTextures;
    
    dataPoints.push_back(data);
    
    std::cout << "Dados registrados - FPS: " << fps 
              << ", Triângulos: " << triangleCount
              << ", CPU: " << data.cpuUsage << "%"
              << ", GPU: " << data.gpuUsage << "%" << std::endl;
}

std::string PerformanceMonitor::getCPUInfo() {
    std::ifstream file("/proc/cpuinfo");
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.find("model name") != std::string::npos) {
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                return line.substr(pos + 2); // +2 para pular ": "
            }
        }
    }
    
    return "CPU não identificado";
}

std::string PerformanceMonitor::getGPUInfo() {
    // Tentar obter informações da GPU usando lspci
    FILE* pipe = popen("lspci | grep -i vga", "r");
    if (!pipe) {
        return "GPU não identificada";
    }
    
    char buffer[256];
    std::string result;
    
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    
    pclose(pipe);
    
    if (result.empty()) {
        return "GPU não identificada";
    }
    
    // Limpar a string resultante
    result.erase(result.find_last_not_of(" \n\r\t") + 1);
    return result;
}

int PerformanceMonitor::getCPUCoreCount() {
    return sysconf(_SC_NPROCESSORS_ONLN);
}

double PerformanceMonitor::getCPUUsage() {
    static double lastTotal = 0, lastIdle = 0;
    
    std::ifstream file("/proc/stat");
    std::string line;
    std::getline(file, line);
    
    std::istringstream iss(line);
    std::string cpu;
    double user, nice, system, idle, iowait, irq, softirq, steal;
    
    iss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    
    double total = user + nice + system + idle + iowait + irq + softirq + steal;
    double totalIdle = idle + iowait;
    
    if (lastTotal == 0) {
        lastTotal = total;
        lastIdle = totalIdle;
        return 0.0;
    }
    
    double totalDiff = total - lastTotal;
    double idleDiff = totalIdle - lastIdle;
    
    lastTotal = total;
    lastIdle = totalIdle;
    
    if (totalDiff == 0) return 0.0;
    
    return 100.0 * (totalDiff - idleDiff) / totalDiff;
}

double PerformanceMonitor::getGPUUsage() {
    // Tentar usar nvidia-smi se disponível
    FILE* pipe = popen("nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader,nounits 2>/dev/null", "r");
    if (pipe) {
        char buffer[16];
        if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            pclose(pipe);
            return std::stod(buffer);
        }
        pclose(pipe);
    }
    
    // Tentar usar radeontop se disponível
    pipe = popen("radeontop -d- -l1 | grep -o '[0-9]*\\.[0-9]*%' | head -1 | sed 's/%//' 2>/dev/null", "r");
    if (pipe) {
        char buffer[16];
        if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            pclose(pipe);
            return std::stod(buffer);
        }
        pclose(pipe);
    }
    
    // Se não conseguir obter dados da GPU, retornar 0
    return 0.0;
}

std::string PerformanceMonitor::getSystemInfo() {
    std::ostringstream oss;
    oss << "CPU: " << cpuInfo << "\n";
    oss << "GPU: " << gpuInfo << "\n";
    oss << "Núcleos CPU: " << cpuCores << "\n";
    return oss.str();
}

void PerformanceMonitor::generateCSVReport(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao criar arquivo CSV: " << filename << std::endl;
        return;
    }
    
    // Cabeçalho
    file << "Timestamp,FPS,CPU_Usage,GPU_Usage,Triangle_Count,Has_Lighting,Has_Textures\n";
    
    // Dados
    for (const auto& data : dataPoints) {
        file << data.timestamp << ","
             << data.fps << ","
             << data.cpuUsage << ","
             << data.gpuUsage << ","
             << data.triangleCount << ","
             << (data.hasLighting ? "true" : "false") << ","
             << (data.hasTextures ? "true" : "false") << "\n";
    }
    
    file.close();
    std::cout << "Relatório CSV gerado: " << filename << std::endl;
}

void PerformanceMonitor::generatePerformanceReport(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao criar relatório: " << filename << std::endl;
        return;
    }
    
    file << "=== RELATÓRIO DE DESEMPENHO OPENGL ===\n\n";
    file << "Data: " << std::chrono::system_clock::now().time_since_epoch().count() << "\n\n";
    
    file << "=== CONFIGURAÇÃO DO HARDWARE ===\n";
    file << getSystemInfo() << "\n";
    
    file << "=== METODOLOGIA DOS EXPERIMENTOS ===\n";
    file << "Este relatório apresenta os resultados dos testes de desempenho realizados\n";
    file << "com diferentes configurações de renderização OpenGL.\n\n";
    
    file << "Parâmetros testados:\n";
    file << "- Número de triângulos renderizados\n";
    file << "- Presença de iluminação (omnidirecional e spotlight)\n";
    file << "- Presença de texturas\n\n";
    
    file << "=== DADOS COLETADOS ===\n";
    file << "Total de medições: " << dataPoints.size() << "\n\n";
    
    if (!dataPoints.empty()) {
        // Calcular estatísticas básicas
        double avgFPS = 0, maxFPS = 0, minFPS = 999999;
        double avgCPU = 0, maxCPU = 0, minCPU = 999999;
        double avgGPU = 0, maxGPU = 0, minGPU = 999999;
        
        for (const auto& data : dataPoints) {
            avgFPS += data.fps;
            avgCPU += data.cpuUsage;
            avgGPU += data.gpuUsage;
            
            if (data.fps > maxFPS) maxFPS = data.fps;
            if (data.fps < minFPS) minFPS = data.fps;
            
            if (data.cpuUsage > maxCPU) maxCPU = data.cpuUsage;
            if (data.cpuUsage < minCPU) minCPU = data.cpuUsage;
            
            if (data.gpuUsage > maxGPU) maxGPU = data.gpuUsage;
            if (data.gpuUsage < minGPU) minGPU = data.gpuUsage;
        }
        
        avgFPS /= dataPoints.size();
        avgCPU /= dataPoints.size();
        avgGPU /= dataPoints.size();
        
        file << "FPS - Média: " << avgFPS << ", Máximo: " << maxFPS << ", Mínimo: " << minFPS << "\n";
        file << "CPU - Média: " << avgCPU << "%, Máximo: " << maxCPU << "%, Mínimo: " << minCPU << "%\n";
        file << "GPU - Média: " << avgGPU << "%, Máximo: " << maxGPU << "%, Mínimo: " << minGPU << "%\n\n";
    }
    
    file << "=== ANÁLISE DOS RESULTADOS ===\n";
    file << "Os gráficos e tabelas detalhadas estão disponíveis nos arquivos CSV gerados.\n";
    file << "Recomenda-se o uso de ferramentas como Python/matplotlib ou Excel para\n";
    file << "visualização dos dados coletados.\n\n";
    
    file << "=== CÓDIGO-FONTE ===\n";
    file << "O código-fonte completo está disponível nos arquivos do projeto.\n";
    
    file.close();
    std::cout << "Relatório de performance gerado: " << filename << std::endl;
}
