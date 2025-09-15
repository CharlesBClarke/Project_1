#include <cstddef>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int lookup_and_connect( const char *host, const char *service );

int main(int argc, char* argv[]) {
	int s;
	const char *host = "www.ecst.csuchico.edu";
	const char *port = "80";
	if ((s = lookup_and_connect(host, port)) < 0) exit(1);

	int chunk_size = std::stoi(argv[1]);
	const char needle[] = "<h1>";
	int needle_size = sizeof(needle)-1;

	char buf[chunk_size];

	char message[]= "GET /~kkredo/file.html HTTP/1.0\r\n\r\n";
	int to_send = sizeof(message)-1;

	char *write_head = message;
	int size_of_sent = send(s, write_head, to_send,0);
	while(to_send != 0 && size_of_sent!=0){
		size_of_sent = send(s, write_head+=size_of_sent, to_send,0);
		to_send-=size_of_sent;
	}

	int header_count = 0;
	int byte_count = 0;
	while (int size_of_recv = recv(s,buf, chunk_size,0)){
		byte_count+=size_of_recv;
		write_head = buf;

		int to_get=chunk_size;
		while (to_get>=0 && size_of_recv) {
			size_of_recv = recv(s,write_head+=size_of_recv,to_get-=size_of_recv,0);
			byte_count += size_of_recv;
		}

		auto i = std::search(buf, write_head,needle,needle+needle_size);
		while(i<write_head) {
			++header_count;
			i = std::search(i+1, write_head,needle,needle+needle_size);
		}
	}

	std::cout << "Number of <h1> tags: " << header_count << std::endl;
	std::cout << "Number of bytes: " << byte_count << std::endl;

	close(s);

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
