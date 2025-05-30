#!/bin/bash

echo "=== IoT 프로젝트 종료 ==="

# PID 파일에서 프로세스 ID 읽기
if [ -f .sensor_pid ]; then
    SENSOR_PID=$(cat .sensor_pid)
    echo "센서 시뮬레이터 종료 (PID: $SENSOR_PID)..."
    kill $SENSOR_PID 2>/dev/null
    rm .sensor_pid
fi

if [ -f .webserver_pid ]; then
    WEBSERVER_PID=$(cat .webserver_pid)
    echo "웹서버 종료 (PID: $WEBSERVER_PID)..."
    kill $WEBSERVER_PID 2>/dev/null
    rm .webserver_pid
fi

# 혹시 남아있는 프로세스도 정리
pkill -f sensor_simulator 2>/dev/null
pkill -f webserver 2>/dev/null

echo "모든 프로세스가 종료되었습니다."

