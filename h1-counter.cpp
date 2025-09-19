#include <iostream>
#include <algorithm>
#include <cstring>
#include "SocketStream.h"

int count_headers(const	char *haystack_start,const char *haystack_end, const char *needle_start, const char *needle_end);
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
	s >> (buf+3);
	std::memset(buf, '#', 3);
	while (s.lastRecv()>0){


		char *end_of_chunk = buf+chunk_i;
		char *end_of_buf = buf+s.lastRecv()+3;

		header_count+=count_headers(buf,end_of_chunk,(char*)needle,(char*)needle+needle_size);
		header_count+=count_headers(end_of_chunk,end_of_buf,(char*)needle,(char*)needle+needle_size);
			
		chunk_bound-=s.lastRecv();

		std::memcpy(buf,end_of_buf-3,3);
		s >> (buf+3);
	}

	std::cout << "Number of <h1> tags: " << header_count << "\n";
	std::cout << "Number of bytes: " << byte_count << std::endl;

	return 0;
}

int count_headers(const char *haystack_start,const char *haystack_end, const char *needle_start, const char *needle_end){
	int result = 0;
	auto i = std::search(haystack_start, haystack_end ,needle_start,needle_end);
	while(i<haystack_end) {
		++result;
		i = std::search(i+1, haystack_end,needle_start,needle_end);
	}
	return result;
}
