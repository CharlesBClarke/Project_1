#pragma once
#include <sys/socket.h>

class SocketStream {
public:
	explicit SocketStream(const char *ip, const char *port, int buf_size);

	~SocketStream();

	template<int N>
	SocketStream &operator<<(const char (&lit)[N]){
		int total_sent=0;
		int sent_size=0;
		do{
			sent_size = send(this->socket_, lit+total_sent, N-total_sent-1,0);
			total_sent+=sent_size;
		}while(total_sent<N-1&&sent_size!=0);
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
