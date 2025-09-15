#include <cstddef>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int lookup_and_connect( const char *host, const char *service );

bool match_header(const char *haystack, int buf_size, int index){
	const char neadle[]= {'<','h','1','>'};
	int neadle_len = 4;
	for(int j = 0; j<neadle_len; ++j){
		if (haystack[index] != neadle[j]) return false;
		index+=1;
	}
	return true;
}

int main(int argc, char* argv[]) {
	int s;
	const char *host = "www.ecst.csuchico.edu";
	const char *port = "80";


	int max_packet_size = std::stoi(argv[1]);
	const char needle[] = "<h1>";

	if ( ( s = lookup_and_connect( host, port ) ) < 0 ) {
		exit( 1 );
	}
	
	char buf[max_packet_size];

	char message[]= "GET /~kkredo/file.html HTTP/1.0\r\n\r\n";
	int to_send = sizeof(message);
	int size_of_sent=0;
	char *write_hearst = message;
	do {
		size_of_sent=send(s, write_hearst+=size_of_sent, to_send,0);
		to_send-=size_of_sent;
	}while(to_send != 0 && size_of_sent!=0);
	int trailing_match = 0;
	int header_count = 0;
	int chunk_boundry = max_packet_size;
	int byte_count = 0;

	while (int size_of_recv = recv(s,buf, sizeof(buf),0)){
		int current_chunk=size_of_recv;
		byte_count+=size_of_recv;
		int to_get=max_packet_size-size_of_recv;
		char *write_head = buf;
		while (to_get>=0 && size_of_recv) {
			size_of_recv= recv(s,write_head+=size_of_recv,to_get,0);
			to_get-=size_of_recv;
			current_chunk+=size_of_recv;
		}
		char *read_head = buf;
		while(true){
			auto i = std::search(read_head, buf+current_chunk,needle,needle+4);
			if (i<buf+current_chunk){
				++i;
				++header_count;
			}else{
				break;
			}
			read_head=i;
		}
	}

	std::cout << "number of <h1> tags: " << header_count << std::endl;
	std::cout << "number of bytes: " << byte_count << std::endl;

	close( s );

	return 0;
}

int lookup_and_connect( const char *host, const char *service ) {
	struct addrinfo hints{};
	struct addrinfo *rp, *result;
	int s;

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	if ( ( s = getaddrinfo( host, service, &hints, &result ) ) != 0 ) {
		fprintf( stderr, "stream-talk-client: getaddrinfo: %s\n", gai_strerror( s ) );
		return -1;
	}

	for ( rp = result; rp != NULL; rp = rp->ai_next ) {
		if ( ( s = socket( rp->ai_family, rp->ai_socktype, rp->ai_protocol ) ) == -1 ) {
			continue;
		}

		if ( connect( s, rp->ai_addr, rp->ai_addrlen ) != -1 ) {
			break;
		}

		close( s );
	}
	if ( rp == NULL ) {
		perror( "stream-talk-client: connect" );
		return -1;
	}
	freeaddrinfo( result );

	return s;
}
