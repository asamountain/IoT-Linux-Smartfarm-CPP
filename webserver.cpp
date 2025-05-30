/**
 * @file webserver.cpp
 * @brief CivetWeb를 사용한 REST API 서버 및 대시보드 호스팅
 * @details
 * - API 엔드포인트: /api/data (GET)
 * - 대시보드: / (GET)
 * @team 협업 포인트: API 스펙 문서화, 안전한 멀티스레딩 처리
 */
#include <iostream>         // 표준 입출력 스트림
#include <thread>           // 스레드 관련 함수
#include <chrono>           // 시간 관련 함수
#include <civetweb.h>       // CivetWeb 라이브러리(웹서버)
#include <fstream>          // 파일 입출력
#include <sstream>          // 문자열 스트림
#include <vector>           // 벡터(동적 배열)
#include "csv_parser.hpp"   // CSV 파싱 유틸리티(구조체 등)
#include "nlohmann/json.hpp"// JSON 라이브러리

using json = nlohmann::json; // json 타입 별칭

// CSV 데이터를 JSON 문자열로 변환하는 함수
std::string get_sensor_data() {
    std::vector<SensorData> data = parse_csv("sensor_data.csv"); // CSV 파싱
    json result;                                                 // 결과 JSON 배열

    for (const auto& entry : data) {         // 각 행마다
        json item;                           // JSON 객체 생성
        item["timestamp"] = entry.timestamp; // 타임스탬프 저장
        item["temperature"] = entry.temperature; // 온도 저장
        item["humidity"] = entry.humidity;       // 습도 저장
        result.push_back(item);              // 결과 배열에 추가
    }

    return result.dump();                    // JSON 문자열 반환
}

// /api/data 엔드포인트 핸들러
int api_handler(struct mg_connection* conn, void* cbdata) {
    std::string json_data = get_sensor_data(); // 센서 데이터 JSON 생성
    mg_printf(conn,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json; charset=utf-8\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        json_data.size(), json_data.c_str()); // HTTP 응답 전송
    return 200; // 상태 코드 반환
}

// / (대시보드) 엔드포인트 핸들러
int dashboard_handler(struct mg_connection* conn, void* cbdata) {
    std::ifstream html_file("dashboard.html"); // 대시보드 HTML 파일 열기
    if (!html_file.is_open()) {                // 파일 열기 실패 시
        mg_printf(conn,
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html; charset=utf-8\r\n"
            "Content-Length: 23\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<h1>File Not Found</h1>");
        return 404;                            // 404 반환
    }
    std::stringstream buffer;                  // 파일 내용을 담을 스트림
    buffer << html_file.rdbuf();               // 파일 전체 읽기
    std::string html_content = buffer.str();   // 문자열로 변환

    mg_printf(conn,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=utf-8\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        html_content.size(), html_content.c_str()); // HTTP 응답 전송
    return 200; // 상태 코드 반환
}

int main() {
    const char* options[] = {
        "document_root", ".",      // 문서 루트 디렉토리
        "listening_ports", "5001", // 리스닝 포트
        NULL                      // 옵션 끝
    };

    mg_init_library(0); // CivetWeb 라이브러리 초기화
    mg_context* ctx = mg_start(NULL, 0, options); // 서버 시작
    mg_set_request_handler(ctx, "/api/data", api_handler, NULL); // /api/data 핸들러 등록
    mg_set_request_handler(ctx, "/", dashboard_handler, NULL);   // / 핸들러 등록

    std::cout << "Server running at http://localhost:5001\n"; // 서버 실행 메시지 출력
    while (true) std::this_thread::sleep_for(std::chrono::seconds(1)); // 무한 대기

    mg_stop(ctx);         // 서버 정지
    mg_exit_library();    // CivetWeb 라이브러리 종료
    return 0;             // 프로그램 종료
}

