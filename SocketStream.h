#pragma once
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <iostream>

class SocketStream {
public:
	explicit SocketStream(const char *ip, const char *port, int buf_size);

	~SocketStream();

	template<int N>
	SocketStream &operator<<(const char (&lit)[N]){
		int size = N-1;
		int total_sent=0;
		int sent_size=0;
		do{
			sent_size = send(this->socket_, lit+total_sent, size-total_sent,0);
		}while(sent_size>0&&(total_sent+= sent_size)<size);

		if (sent_size == -1) std::cerr << "send error: " << strerror(errno);
		return *this;
	}

	SocketStream &operator>>(char *buff);

	int lastRecv() const;
	//void setName(const std::string &n);        // setter

private:
	int socket_;
	int last_recv_size_;
	int buf_size_;
};
