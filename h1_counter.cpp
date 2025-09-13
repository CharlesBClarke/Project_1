#include <cstring>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
int lookup_and_connect( const char *host, const char *service );

int main( ) {
	int s;
	const char *host = "www.ecst.csuchico.edu";
	const char *port = "80";

	if ( ( s = lookup_and_connect( host, port ) ) < 0 ) {
		exit( 1 );
	}
	
	// Ugly maigic num for now
	char buf[8172];

	char message[]= "GET /~kkredo/file.html HTTP/1.0\r\n\r\n";

	std::memcpy(buf,&message,sizeof(message));
	send(s, buf, sizeof(message),0);
	std::string responce;

	while (int size_of_recv = recv(s,buf, sizeof(buf),0)){
		std::string temp(buf, size_of_recv);
		responce.append(temp);
	}

	std::cout << responce << std::endl;

	/* Modify the program so it
	 *
	 * 1) connects to www.ecst.csuchico.edu on port 80 (mostly done above)
	 * 2) sends "GET /~kkredo/file.html HTTP/1.0\r\n\r\n" to the server
	 * 3) receives all the data sent by the server (HINT: "orderly shutdown" in recv(2))
	 * 4) prints the total number of bytes received
	 *
	 * */

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
