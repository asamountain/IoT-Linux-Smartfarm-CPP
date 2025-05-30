CC = g++
CFLAGS = -std=c++17 -pthread
LIBS = -lcivetweb -ljsoncpp

# JSON 라이브러리 헤더 경로 (필요시 수정)
JSON_INCLUDE = -I/usr/include/nlohmann  # 경로 수정

all: sensor_simulator webserver

sensor_simulator: sensor_simulator.cpp
	$(CC) $(CFLAGS) $< -o $@

webserver: webserver.cpp csv_parser.hpp
	$(CC) $(CFLAGS) $(JSON_INCLUDE) $< -o $@ $(LIBS)


clean:
	rm -f sensor_simulator webserver

