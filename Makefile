CXX = g++
SRC = src/web_server.cpp src/credit_report.cpp
OUT = credit-report-web
CFLAGS = -I./include -lsqlite3 -pthread

all:
	$(CXX) $(SRC) $(CFLAGS) -o $(OUT)
	