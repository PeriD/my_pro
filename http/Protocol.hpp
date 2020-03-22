#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <utility>
#include <unordered_map>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/sendfile.h>
#include "Util.hpp"

using namespace std;

#define WWW "./wwwroot"
class HttpRequest{
	private:
		string request_line;
		string request_header;
		string request_blank;
		string request_body;
	private:
	    string method;
		string uri;
		string version;
	        unordered_map<string, string> header_kv;
		string path;
		string query_string;   															int recource_size;
			bool cgi;
			string suffix;
			//bool done;
	public:
	HttpRequest() :request_blank("\n"), path(WWW), recource_size(0), cgi(false)
		{}
		string &GetRequestLine()
		{
			return request_line;
			}
		string &GetRequestHeader()
		{
			return request_header;
		    }
		string &GetRequestBody()
		{
			return request_body;
			}																		string GetPath()
			{
			return path;
			}
		string &GetQueryString()	
		{
			return query_string;
			}
			bool MethodIsLegal()
			{
				//GET ||POST
		if (method != "GET"&&method != "POST"){
			return false;	
			}
			return true;
			}
        string GetSuffix()
		   {
			   return suffix;
		   }
		   int GetRecourceSize()
		   {
			   return recource_size;
		   }
		   void RequestLineParse()
		   {
			   //get/index.html http/1.1
			   stringsteam ss(request_line);
			   ss >> method >> uri >>version;
			   cout << "mrthod:"<< method << endl;
			   cout << "uri: " << uri << endl;
			   cout << "version: " << version << endl;
		   }
		   void RequestHeaderParse()
		   {
			   vector<string> v;
			   Util::TansfromToVector(request_header, v);
			   auto it = v.begin();
			   for (; it != v.end(); it++){
				   string k;
				   string v;
				   Util::MakeKV(*it, k, v);
				   header_kv.insert(make_pair(k, v));
				   cout << "key:" << k << endl;
				   cout << "value:" << v << endl;
			   }
		   }

		   //yes -> true no ->false
		   bool IsNeedRecv()
		   {
			   return method == "POST";
		   }
		   int GetContentLength()
		   {
			   auto it = header_kv.find("Content-Length");
			   if (it == header_kv.end()){
				   return  -1;
			   }
			   return Util::StringToInt(it->second);
		   }
		   void UriParse()
		   {
			   if (method == "POST"){
				   cgi = true;
				   path += uri;
			   }
			   else{
				   size_t pos = uri.find('?');
				   if (string::npos == pos){
					   path += uri;
				   }
				   else{
					   cgi=true;
					   path += uri.substr(0, pos);
					   query_string = uri.substr(pos + 1);
				   }
			   }
			   if (path[path.size() - 1] == '/'){
				   path += "index.html";
			   }
			     size_t pos=path.rfind(".");
				  if(string::npos==pos){
					   suffix=".html"; 
				  }
				  else{
					  suffix=path.substr(pos);  
				  }
				  cout<<"debug : suffix"<<suffix<<endl;
		   }
		   bool IsPathLegal()
		   {
			   //path="/";
			   bool IsPathLegal()
			   if (S_ISDIR(st.st_mode)){
				   path += "/index.html";
			   }
			   else{
				   if ((st.st_mode&S_IXUSR) || \
					   (st.st_mode&S_IXUSR) || \
					   (st.st_mode&S_IXUSR)){
					   cgi = true;
				   }
			   }
			   recource_size = st.st_size;
			   return tur
		   }
		   else{
			   return false;
		   }
}
bool IsCgi()
{
	return cgi;
}
~HttpRequest()
{}
};
class HttpResponse{
	private:
	string response_line;
	string response_header;
	string response_blank;
	string response_body;
	private:
	int fd;
	int size;
	public:
	HttpResponse() :response_blank("\r\n"), fd(-1)
	{}
	void MakeResponseLine(int code)
	{
		//string version = "HTTP/1.0";
		response_line += "HTTP/1.1";
		response_line += " ";
		response_line += Util::IntToString(code);
		response_line += " ";
		response_line += Util::CodeToDec(code);
	    response_line += "\r\n";
	}
	void MakeResponseHeader(vector<string> &v)
	{
		string connection="Connection: close";
		v.push_back(connection);
		auto it = v.begin();
		for (; it != v.end(); it++){
			response_header += *it;
			response_header += "\r\n";
		}
	}
	void MakeResponse(HttpRequest *rq, int code, bool cgi)
	{
		MakeResponseLine(code);
		string suffix = rq->GetSuffix();
		vector<string> v;
		size=rq->GetRecourceSize();
		//string s = "Content-Type: text/html";
		//v.push_back(s);
		//MakeResponseHeader(v);
		//string path = rq->GetPath();
		//fd = open(path.c_str(), O_RDONLY);
		//size=rq->GetRecourceSize();
		if(cgi){
			//cgi
			 string ct=Util::SuffixToType("");
			 v.push_back(ct);
			 string cl="Content_Length: ";
			 cl+=Util::IntToString(response_body.size());
			 v.push_back(cl);
			 MakeResponseHeader(v);
			 }
			 else{
				 string suffix=rq->GetSuffix();
				 size=rq->GetRecourceSize();			  
				 string ct=Util::SuffixToType(suffix);
				 v.push_back(ct);
				 string cl="Content-Length: ";
				 cl+=Util::IntToString(size);
				 v.push_back(cl);
				 MakeResponseHeader(v);
				 string path=rq->GetPath();
				 cout<<"debug :"<<path<<endl;
				 fd=open(path.c_str(),O_RDONLY);
			 }
	}
	string &GetResponseLine()
	{
		return response_line;
	}
	string &GetResponsHeader()
	{
		return response_header;
	}
	string &GetResponseBlank()
	{
		return response_blank;
	}
	string &GetResponseBody()
	{
		return response_body;
	}
	int GetFd()
	{
		return fd;
	}
	int GetRecourceSize()
	{
		return size;
	}
	~HttpResponse()
	{
		if (fd = -1){
			close(fd);
		}
	}
};
class EndPoint{
	private:
	int sock;
	public:
	EndPoint(int sock_) :sock(sock_)
	{}
	int RecvLine(string &line){
		//\n \r \r\n ->\n
		char c = 'X';
		while (c != '\n'){
			ssize_t s = recv(sock, &c, 1, 0);
			if (s>0){
				if (c == '\r'){
					if (recv(sock, &c, 1, MSG_PEEK)>0){
						if (c == '\n'){//\r\n
							recv(sock, &c, 1, 0);
						}
						else{//\r
							c = '\n';
						}
					}
					line.push_back(c);
				}
				else{
					c = '\n';
					cout << "recv error: " << s << "sock: " << sock << endl;
					line.push_back(c);
				}
			}
			return line.size();
		}
		void RecvRequestLine(HttpRequest *rq)
		{
			RecvLine(rq->GetRequestLine());   //\n \r \n\r -> \n
		}
		void RecvRequestHeader(HttpRequest *rq)
		{
			string &rh = rq->GetRequestHeader();
			do{
				string line = "";
				RecvLine(line);
				if (line == "\n"){
					break;
				}
				rh += line;
			} while (1);
		}
		void RecvRequestBody(HttpRequest *rq)
		{
			int len = rq->GetContentLength();
			string &body = rq->GetRequestBody();
			char c;
			while (len--){
				if (recv(sock, &c, 1, 0)>0){
					body.push_back(c);
				}
			}
			cout << "body: " << body << endl;
		}
		void SendResponse(HttpResponse *rsp, bool cgi)
		{
			if (cgi){
				//string &response_body=rsp->GetResponseBody();
				//send(sock,response_body.c_str(),response_body.size(),0);
			}
			else{
				////non cgi ->GET, no args;  
				string &response_line = rsp->GetResponseLine();
				string &response_header = rsp->GetResponsHeader();
				string &response_blank = rsp->GetResponseBlank();
				int fd = rsp->GetFd();
				int size = rsp->GetRecourceSize();
				send(sock, response_line.c_str(), response_line.size(), 0);
				send(sock, response_header.c_str(), response_header.size(), 0);
				send(sock, response_blank.c_str(), response_blank.size(), 0);
				sendfile(sock, fd, NULL, size);
			}
		}
		~EndPoint()
		{
			close(sock);
		}
	};
	class Entry{
		public:
		static void *HandlerRequest(void *args){
			int code = 200;
			int *p = (int*)args;
			int sock = *p;
			EndPoint *ep = new EndPoint(sock);
			HttpRequest *rq = new HttpRequest();
			HttpResponse *rsp = new HttpResponse();
			ep->RecvRequestLine(rq);
			rq->RequestLineParse();
			if (!rq->MethodIsLegal()){
				code = 400;
				goto end;
			}
			ep->RecvRequestHeader(rq);
			rq->RequestHeaderParse();
			if (rq->IsNeedRecv()){
				ep->RecvRequestBody(rq);
			}
			//rq->SetDone(true);
			rq->UriParse();
			if (!rq->IsPathLegal()){
				//TODO
				code = 404;
				goto end;
			}
			//method path ,query_string,body,version,cgi
			if (rq->IsCgi()){
				//DO CGI
				//code=ProcessCgi(rq,rsp);
				//if(code==200){
				//  rsp->MakeResponse(rq,code,true);
				//  ep->SendResponse(rsp,true);
				}
				else{//GET,no args
					rsp->MakeResponse(rq, code, false);
					ep->SendResponse(rsp, false);
				}


				end:
				delete rq;
			    delete rsp;
				delete ep;
				delete p;
				}
		};



