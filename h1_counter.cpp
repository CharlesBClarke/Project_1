#include <cstring>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
int lookup_and_connect( const char *host, const char *service );

int match_header(const char *haystack, int buf_size, int index){
	const char neadle[]= {'<','h','1','>'};
	int neadle_len = 4;
	int match_count = 0;
	for(int j = 0; j<neadle_len; ++j){
		if(0<=index && index<buf_size){
			if (haystack[index] != neadle[j]) return -1;
			match_count+=1;
			index+=1;
		}
	}
	return match_count;
}

int main(int argc, char* argv[]) {
	int s;
	const char *host = "www.ecst.csuchico.edu";
	const char *port = "80";
	int max_packet_size = std::stoi(argv[1]);

	if ( ( s = lookup_and_connect( host, port ) ) < 0 ) {
		exit( 1 );
	}
	
	char buf[max_packet_size];

	char message[]= "GET /~kkredo/file.html HTTP/1.0\r\n\r\n";

	std::memcpy(buf,&message,sizeof(message));
	send(s, buf, sizeof(message),0);
	int trailing_match = 0;
	int header_count = 0;
	int chunk_boundry = max_packet_size;

	while (int size_of_recv = recv(s,buf, sizeof(buf),0)){
		for(int i = - 3; i< size_of_recv; ++i){
			if (chunk_boundry < 4) {if (chunk_boundry == 0) chunk_boundry = max_packet_size;}
			else if (i<0){
				int match_len = match_header(buf, size_of_recv, i);
				if (match_len+trailing_match == 4) header_count+=1;
			}
			else if (i< size_of_recv-3){
				if (match_header(buf, size_of_recv, i)==4) ++header_count;
			}
			else{
				int match_len = match_header(buf, size_of_recv, i);
				if (match_len!=0) trailing_match=match_len;
			}
			chunk_boundry--;
		}
	}

	std::cout << header_count << std::endl;

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
