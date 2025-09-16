#include "SocketStream.h"
#include <cstddef>

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

SocketStream::SocketStream(const char *ip, const char *port, int buf_size)
    : socket_(-1), last_recv_size_(0), buf_size_(buf_size){
	struct addrinfo hints{};
	struct addrinfo *rp, *result;
	int s;

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	if ((s = getaddrinfo(ip, port, &hints, &result)) != 0) {
		std::cerr <<  "stream-talk-client: getaddrinfo: %s\n" << gai_strerror(s) << std::endl;
		this->socket_ = -1;
	}

	for (rp = result; rp != NULL; rp = rp->ai_next){
		if ((s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1)
			continue;

		if (connect(s, rp->ai_addr, rp->ai_addrlen) != -1)
			break;

		close(s);
	}
	if (rp == NULL) {
		std::cerr << "stream-talk-client: connect" << std::endl;
		this->socket_ = -1;
	}
	freeaddrinfo(result);

	this->socket_ = s;
}

SocketStream::~SocketStream() {
	close(this->socket_);
}

SocketStream &SocketStream::operator>>(char *buff) {
	int total_recvd=0;
	int recvd_size=0;
	do{
		total_recvd+=recvd_size;
		recvd_size = recv(this->socket_, buff+total_recvd, this->buf_size_-total_recvd,0);
	}while(total_recvd<this->buf_size_&&recvd_size!=0);
	last_recv_size_ = total_recvd;
	return *this;
}

int SocketStream::lastRecv() const {
    return last_recv_size_;
}

