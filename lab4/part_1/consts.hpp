#include <iostream>
#include <windows.h>
#include <ctime>
#include <windows.h>
#include <string>
#include <fstream>
#define WAITING 0
#define RW_OPERATION 1
#define REALISED 2
#define PATH_TO_LOGS R"(C:\Users\Keltasar\CLionProjects\etu_2024_04_os_lab\lab4\part_1)"
const int pageSize = 4096,
        numberOfPages = 19,
        numberOfReaders = 10,
        numberOfWriters = 10,
        rwDelay_ms = 500,
        rwDelayDiv_ms = 1000;
const std::string mutexName = "IOMutex",
        mapName = "mapped_file",
        fileName = "file";
void logProcessEvent(int id, bool is_reader, int event_type, int page) {
    std::string processTypeName = "writer";
    if (is_reader) processTypeName = "reader";
    std::ofstream logFile(std::string(PATH_TO_LOGS) + processTypeName + "_" + std::to_string(id) + ".txt", std::ios::app);
    if (logFile.is_open()) {
        DWORD time = GetTickCount();
        logFile << event_type << " " << time << " " << page << std::endl;
        logFile.close();
    }
}
void logProcessEvent(int id, bool is_reader, int event_type) {
    logProcessEvent(id, is_reader, event_type, -1);
}