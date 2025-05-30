/**
 * @file csv_parser.hpp
 * @brief CSV 파일 파싱 유틸리티
 * @details 팀 내 표준 CSV 처리 로직 재사용
 */

#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

struct SensorData {
    std::string timestamp;
    double temperature;
    double humidity;
};

std::vector<SensorData> parse_csv(const std::string& filename) {
    std::vector<SensorData> data;
    std::ifstream file(filename);
    std::string line;

    // 헤더 스킵
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        SensorData entry;
        std::getline(ss, entry.timestamp, ',');
        ss >> entry.temperature;
        ss.ignore(); // 콤마 무시
        ss >> entry.humidity;
        data.push_back(entry);
    }

    return data;
}

