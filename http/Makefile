.PHONY:all
all:HttpServer
HttpServer:HttpServer.cc
	g++ -g -o $@ $^ -std=c++11 -lpthread -g
.PHONY:clean
clean:
	rm -f HttpServer

