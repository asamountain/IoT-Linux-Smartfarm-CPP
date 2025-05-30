/**
 * @file sensor_simulator.cpp
 * @brief CSV 파일에 센서 데이터를 주기적으로 기록하는 프로그램
 * @details
 * - 주기: 5초
 * - 데이터: 타임스탬프, 온도(20~30°C), 습도(40~60%)
 * - 파일: sensor_data.csv (헤더 자동 생성)
 * @team 협업 포인트: CSV 포맷 표준화, 파일 잠금 메커니즘 사용
 */
#include <fstream>      // 파일 입출력 스트림 사용
#include <iostream>     // 표준 입출력 스트림 사용
#include <random>       // 랜덤값 생성 라이브러리
#include <chrono>       // 시간 관련 라이브러리
#include <ctime>        // C 스타일 시간 함수
#include <thread>       // sleep 등 스레드 관련 함수
#include <mutex>        // 뮤텍스(동기화) 사용

std::mutex file_mutex;  // 파일 접근 동시성 제어용 뮤텍스 선언

// 센서 데이터를 생성하고 파일에 기록하는 함수
void generate_data() {
    std::ofstream file;                     // 파일 출력 스트림 객체 선언
    std::string filename = "sensor_data.csv"; // 파일 이름 지정

    // 파일이 존재하지 않으면 헤더를 작성
    {
        std::ifstream check(filename);      // 파일 존재 여부 확인용 입력 스트림
        if (!check.good()) {                // 파일이 없으면
            std::lock_guard<std::mutex> lock(file_mutex); // 파일 접근 동기화
            file.open(filename, std::ios::out);           // 파일 생성(쓰기 모드)
            file << "timestamp,temperature,humidity\n";   // 헤더 작성
            file.close();                                 // 파일 닫기
        }
    }

    // 무한 루프: 5초마다 센서 데이터 생성 및 파일에 기록
    while (true) {
        auto now = std::chrono::system_clock::now();      // 현재 시간(시스템 클록)
        std::time_t timestamp = std::chrono::system_clock::to_time_t(now); // time_t로 변환

        std::random_device rd;                            // 랜덤 디바이스(시드)
        std::mt19937 gen(rd());                           // 메르센 트위스터 엔진
        std::uniform_real_distribution<> temp_dist(20.0, 30.0); // 온도 범위 20~30
        std::uniform_real_distribution<> humi_dist(40.0, 60.0); // 습도 범위 40~60

        double temp = round(temp_dist(gen) * 100) / 100;  // 온도 소수점 2자리
        double humi = round(humi_dist(gen) * 100) / 100;  // 습도 소수점 2자리

        // 파일에 데이터 추가 (동기화 보장)
        {
            std::lock_guard<std::mutex> lock(file_mutex); // 파일 접근 동기화
            file.open(filename, std::ios::app);           // 파일 열기(추가 모드)
            file << std::ctime(&timestamp) << ","         // 타임스탬프 기록
                 << temp << ","                           // 온도 기록
                 << humi << "\n";                         // 습도 기록
            file.close();                                 // 파일 닫기
        }

        std::this_thread::sleep_for(std::chrono::seconds(5)); // 5초 대기
    }
}

int main() {
    generate_data(); // 센서 데이터 생성 함수 실행
    return 0;        // 프로그램 종료
}

