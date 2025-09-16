#include <iostream>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "SocketStream.h"

int lookup_and_connect( const char *host, const char *service );

int main(int argc, char* argv[]) {
	const char *host = "www.ecst.csuchico.edu";
	const char *port = "80";
	int chunk_size = std::stoi(argv[1]);
	SocketStream s(host,port,chunk_size);

	const char needle[] = "<h1>";
	int needle_size = sizeof(needle)-1;

	char buf[chunk_size];

	s << "GET /~kkredo/file.html HTTP/1.0\r\n\r\n";

	int header_count = 0;
	int byte_count = 0;
	s >> buf;
	while (s.lastRecv()){
		char *end_of_buf = buf+s.lastRecv();
		auto i = std::search(buf, end_of_buf ,needle,needle+needle_size);
		while(i<end_of_buf) {
			++header_count;
			i = std::search(i+1, end_of_buf,needle,needle+needle_size);
		}
	}

	std::cout << "Number of <h1> tags: " << header_count << "\n";
	std::cout << "Number of bytes: " << byte_count << std::endl;

	return 0;
}

