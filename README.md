# IoT Linux SmartFarm (C++ Edition)

센서 시뮬레이터와 C++ 기반 REST API, Chart.js 대시보드를 활용한 IoT 데이터 파이프라인 데모 프로젝트

---
## 주요 기능

- C++로 임의 센서 데이터(온도, 습도) 생성 및 CSV 저장
- C++ CivetWeb 기반 REST API 서버(`/api/data`)에서 실시간 JSON 데이터 제공
- Chart.js 기반 시각화 대시보드: 실시간 온습도 데이터 5초 간격 시각화
- 자동화 스크립트(run_iot.sh, stop_iot.sh)로 손쉬운 실행/종료

---

## 특징

- 모든 주요 기능(센서 시뮬레이션, 파일 입출력, REST API, 멀티스레딩, 에러처리 등)을 C++로 구현
- CivetWeb, nlohmann/json 등 실무에서 널리 쓰이는 라이브러리 활용 경험
- 자동화 스크립트와 Makefile로 팀원 누구나 쉽게 빌드·실행 가능하도록 설계
- 코드에 상세 주석 및 모듈화로 협업과 유지보수 용이성 확보

---

## 실행 방법

1. **의존성 설치**
   ```bash
   sudo apt update
   sudo apt install g++ libcivetweb-dev nlohmann-json3-dev
   ```

2. **빌드**
   ```bash
   make clean && make
   ```

3. **실행**
   ```bash
   ./run_iot.sh
   ```

4. **브라우저에서 접속**
   - API: `http://localhost:5001/api/data`
   - 대시보드: `http://localhost:5001/`

5. **종료**
   ```bash
   ./stop_iot.sh
   ```

---

## 실행 결과 예시

- API 응답(JSON)  
<img width="537" alt="Screenshot 2025-05-30 at 8 29 58 PM" src="https://github.com/user-attachments/assets/f8bfb666-94c1-4902-b8f6-1aa90189f99e" />

```cpp
// sensor_simulator.cpp
/**
 * @brief 센서 데이터를 5초마다 CSV 파일에 기록하는 시뮬레이터
 * @details
 * - 파일이 없으면 헤더를 자동 생성
 * - 랜덤 온습도 값을 생성하여 CSV에 추가
 * - 파일 접근 동시성 문제를 방지하기 위해 std::mutex 사용
 */
```

<img width="1440" alt="Screenshot 2025-05-30 at 11 13 57 PM" src="https://github.com/user-attachments/assets/232a8725-4b88-49af-b577-ccfb862aebf4" />


```cpp
// webserver.cpp
/**
 * @brief CivetWeb 기반 REST API 및 대시보드 서버
 * @details
 * - /api/data: 센서 CSV 파일을 파싱해 JSON 배열로 반환
 * - /: dashboard.html(Chart.js 대시보드) 서빙
 * - HTTP 응답은 Content-Length를 명시해 chunked encoding 문제 방지
 * - 파일 읽기 실패 시 404 반환
 */
```

```cpp
// csv_parser.hpp
/**
 * @brief CSV 파일을 파싱해 SensorData 벡터로 반환하는 유틸리티
 * @details
 * - 헤더 스킵, 각 필드별 파싱, 구조체로 데이터 관리
 * - 팀 내 표준 데이터 포맷 유지
 */
```
---

## 문제 해결 경험 (이슈 → 해결 과정)

> 개발 과정에서 포트 충돌, HTTP chunked encoding 오류, 파일 경로 문제, Multipass 네트워크/포트포워딩 등 다양한 실전 이슈를 경험
> 각 문제 상황마다 lsof, curl, 브라우저 개발자 도구, 로그 분석 등 다양한 도구로 원인을 진단  
> HTTP 응답 헤더 직접 작성, 파일 위치 일치, 자동화 스크립트 도입 등으로 해결  

