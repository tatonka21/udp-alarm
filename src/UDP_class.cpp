/*=============  see UDP_class.h for a description ===========

Author Dr. Pj Radcliffe 2011,  released as GPL.
*/

#include "UDP_class.h"

//====== Constructor ========================================
//
//  PURPOSE - constructor, just save ip address and port.
//
//  Example: UDP_class my_udp("127.0.0.1", 55555) ; // only work with 127.0.0.1
//           UDP_class my_udp("", 55555) ; // rx only, accept packets from any IP.

Tudp_handler::Tudp_handler(string ip, int port)
{ udp_ip = ip ;
  udp_port = port ;
}



//===== send string ==========================================
//
// PURPOSE - send a text string to the destination.
//
// Example: my_udp.send_str = "Hello!\n" ;  my_udp.send_udp() ;


int Tudp_handler::send_udp()
{//--- inits
   error_message = "" ;
   struct sockaddr_in address ;
   int sock_descriptor ;
   int address_size ;

 //--- create UDP socket.
   sock_descriptor = socket(AF_INET, SOCK_DGRAM, 0) ;
   if (sock_descriptor == -1) 
     { error_message="Socket creation failed." ;
       return(-1) ;
     }
   address.sin_family = AF_INET ;
   address.sin_addr.s_addr = inet_addr(udp_ip.c_str()) ;
   address.sin_port = htons( udp_port) ;

 //--- send data packet.
   if (sendto(sock_descriptor, send_str.c_str(), send_str.length(), 0,
             (struct sockaddr *)&address, sizeof(address)) == -1
       )
         { error_message = "   Sending data to listener failed." ;
           return(-1) ;
	 }
   close( sock_descriptor) ;
   return(0) ;  // if got here sent OK.
}




//=========== Wait for an receive UDP packet ==========================
//
//  PURPOSE - wait until a UDP packet is received then put it
//            in a buffer.
//
//     ALSO - if the udp_ip address is blank then any IP address is accepted.
//            if it is not blank then only that source IP is accepted.
//
//  Example: my_udp.wait_receive_udp() ;  // data put in my_udp.rcv_str

int Tudp_handler::wait_receive_udp()
{//--- setup socket data description. 
   int  socket_descriptor ;
   struct sockaddr_in sin ;
   socklen_t sin_length ;

   sin.sin_family = AF_INET ;                  // IPv4 UDP
   in_addr_t in_address ;
   if ( udp_ip.empty())
        in_address = htonl(INADDR_ANY) ;
   else in_address = inet_addr( udp_ip.c_str());
   sin.sin_addr.s_addr = in_address  ;           // rx from any IP
   sin.sin_port = htons(udp_port) ;              // use provided port #

   if ((socket_descriptor = socket( AF_INET, SOCK_DGRAM, 0)) == -1)
     { error_message = "  Opening of socket failed" ;
       return(-1) ;
     }

   if ( (bind( socket_descriptor, (struct sockaddr *)&sin, sizeof(sin)))
        == -1 )
     { error_message = "   Binding socket failed" ;
       return(-1) ;
     }

 //--- wait for packet,  display and quit when receive.
   #define BUF_LEN 8000
   char buf [BUF_LEN+1] ;
   sin_length = sizeof( sin) ;
   recvfrom( socket_descriptor,  buf, sizeof( buf), 0, 
                                ( struct sockaddr *)&sin, &sin_length) ;
   rcv_str = buf ;
   close( socket_descriptor) ;
   return(0) ;
}




//================== get IP from URI ===================================
//
//  PURPOSE - convert a URI to an IP address (numbers and dots format).
//

int Tudp_handler::uri_to_ip(string& uri, string& ip)
{  struct hostent *server_host_name ;
   if ( (server_host_name = gethostbyname(udp_ip.c_str())) == 0)
        { error_message = "   Error resolving host name to ip address" ;
          return(-1) ;
        }
   char str[INET_ADDRSTRLEN] ;
   char **pptr = server_host_name->h_addr_list ;
   inet_ntop( AF_INET, *pptr, str, sizeof(str)) ;
   ip = str ;
}



#ifndef NO_SELF_TEST

//=============== main : only for self test ============================

#include <iostream>

int main()
{//--- setup class with ip and port.
 //    Can't have one class instance do tx and rx as must start
 //    listener first.  If listen starts and waits,  then send
 //    would never happen.

   Tudp_handler my_udp( "127.0.0.1", 55555) ; // only work with 127.0.0.1
 //Tudp_handler my_udp( "", 55555) ; // receive only,  accept from any IP.

 //--- test uri to ip conversion.
   string uri = "localhost" ;
   string ip ;
   my_udp.uri_to_ip( uri, ip) ;
   cout << endl << "   For URI " << uri << ",  found ip was " << ip << endl << endl ;
 
#ifdef SELF_TEST_TALK
 //--- start talker
   my_udp.send_str = "Hello from a UDP talker." ;
   if ( my_udp.send_udp() )
     {//--- got an error
        cout << "   Send error: " << my_udp.error_message << endl ;
        return(-1) ;
     }
   cout << "   Sent packet: " << my_udp.send_str << endl << endl ;
#endif


#ifdef SELF_TEST_LISTEN
 //--- start listener.
   cout << "   Waiting for UDP packet." << endl << endl ; 
   if (my_udp.wait_receive_udp () ) 
     {//--- got an error
        cout << "   Send error: " << my_udp.error_message << endl << endl ;
        return(-1) ;
     }
   cout << "   Received packet: " << my_udp.rcv_str << endl << endl ;

#endif

}

#endif

 
