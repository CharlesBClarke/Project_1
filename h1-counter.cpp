#include <cassert>
#include <iostream>
#include <algorithm>
#include <cstring>
#include "SocketStream.h"

int count_headers(const	char *haystack_start,const char *haystack_end, const char *needle_start, const char *needle_end);

int main(int argc, char* argv[]) {
	const char *host = "www.ecst.csuchico.edu";
	const char *port = "80";
	int chunk_size = std::stoi(argv[1]);
	SocketStream s(host,port,chunk_size-3);

	const char needle[] = "<h1>";
	int needle_size = sizeof(needle)-1;

	char buf[chunk_size];

	s << "GET /~kkredo/file.html HTTP/1.0\r\n\r\n";

	int header_count = 0;
	int byte_count = 0;
	int chunk_bound = chunk_size;

	std::memset(buf, '#', 3);
	s >> (buf+3);
	while (s.lastRecv()>0){
		byte_count += s.lastRecv();
		char *end_of_buf = buf+s.lastRecv()+3;
		char *end_of_chunk = end_of_buf;

		if (chunk_bound<=s.lastRecv()){
			end_of_chunk = buf+chunk_bound;
			chunk_bound = chunk_size-(s.lastRecv()-chunk_bound);
		}else{
			chunk_bound-=s.lastRecv();
		}

		header_count+=count_headers(buf,end_of_chunk,(char*)needle,(char*)needle+needle_size);
		header_count+=count_headers(end_of_chunk,end_of_buf,(char*)needle,(char*)needle+needle_size);

		std::memcpy(buf,end_of_buf-3,3);
		s >> (buf+3);
	}

	std::cout << "Number of <h1> tags: " << header_count << "\n";
	std::cout << "Number of bytes: " << byte_count << std::endl;

	return 0;
}

int count_headers(const char* h_start, const char* h_end, const char* n_start, const char* n_end){
    if (n_start == n_end) return 0;
    int result = 0;
    const char* i = std::search(h_start, h_end, n_start, n_end);
    while (i != h_end) {
	++result;
	i = std::search(i + (n_end - n_start), h_end, n_start, n_end);
    }
    return result;
}
