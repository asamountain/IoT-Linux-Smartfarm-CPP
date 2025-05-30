#!/bin/bash

echo "=== IoT 프로젝트 시작 ==="

# 기존 프로세스 종료 (혹시 남아있다면)
echo "기존 프로세스 정리 중..."
pkill -f sensor_simulator 2>/dev/null
pkill -f webserver 2>/dev/null
sleep 1

# 센서 시뮬레이터를 백그라운드에서 실행
echo "센서 시뮬레이터 시작..."
nohup ./sensor_simulator > sensor_simulator.log 2>&1 &
SENSOR_PID=$!
echo "센서 시뮬레이터 PID: $SENSOR_PID"

# 잠시 대기 (sensor_data.csv 파일이 생성될 시간)
sleep 2

# 웹서버를 백그라운드에서 실행
echo "웹서버 시작..."
nohup ./webserver > webserver.log 2>&1 &
WEBSERVER_PID=$!
echo "웹서버 PID: $WEBSERVER_PID"

# PID를 파일에 저장 (나중에 종료할 때 사용)
echo $SENSOR_PID > .sensor_pid
echo $WEBSERVER_PID > .webserver_pid

echo ""
echo "=== 실행 완료! ==="
echo "센서 시뮬레이터: PID $SENSOR_PID (로그: sensor_simulator.log)"
echo "웹서버: PID $WEBSERVER_PID (로그: webserver.log)"
echo ""
echo "접속 주소:"
echo "- API: http://localhost:5001/api/data"
echo "- 대시보드: http://localhost:5001/"
echo ""
echo "종료하려면: ./stop_iot.sh 실행"

