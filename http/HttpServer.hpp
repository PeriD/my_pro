#pragma once

#include <iostream>
#include <pthread.h>
#include "Util.hpp"
#include "Protocol.hpp"
#include "ThreadPool.hpp"
using namespace std;

class Sock{
	private:
		int sock;
		int port; //duankou

	public:
		Sock(const int &port_):port(port_),sock(-1)
		{	}

		void Socket()     //chnagjiantaojiezi
		{
			sock = socket(AF_INET,SOCK_STREAM,0);
			if(sock < 0){
				cerr << "sock error" << endl;
				exit(2);
			}
			int opt = 1;
			setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		}
		void Bind()   //bangding
		{
			struct sockaddr_in local;
			local.sin_family =AF_INET;
			local.sin_addr.s_addr = htonl(INADDR_ANY);
			local.sin_port = htons(port);
		
			if(bind(sock, (struct sockaddr*)&local, sizeof(local)))< 0 ){
				cerr << "bind error" << endl;
				exit(3);
			}
		}
		void Listen()    //jianting
		{
			const int backlog = 10;
			if(listen(sock, backlog) < 0){
				cerr << "listen error" << endl;
				exit(4);
			}
		}
		int Accept()
		{
			struct sockaddr_in peer;
			socklen_t len = sizeof(peer);
			int fd =accept(sock,(struct sockaddr*)&peer, &len);
			if(fd< 0){
				cerr << "accept error" << endl;
				return -1;
			}
			cout << "get a new link ... done" << endl;
			return fd;
		}
		~Sock()
		{
			if(sock >= 0){
					close(sock);
				}
		}
};


#define DEFAULT_PORT 8080

class HttpServer{
	private:
		Sock sock;
	public:
		HttpServer(int port = DEFAULT_PORT):sock(port)
		{
		
		}
		void InitHttpServer(){
		sock.Socket();
		sock.Bind();
		sock.Listen();
											//tp = new ThreadPool(8);
													//tp->InitThreadPool();
		}
void Start(){
	     for (;;){
		int sock_ = sock.Accept();
				if (sock_ >= 0)
				{
				pthread_t tid;
				int *p=new int(sock_);
				//Task t(p,Entry::HandlerRequest);
				//tp ->PushTask(t);
			    pthread_create(&tid, nullptr, Entry::HandlerRequest, p);		                }
		 }
}
};
	
