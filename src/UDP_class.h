/*===================== UDP_class =======================================

  PURPOSE - provide a basic class packaging of UDP send and receive.


  USAGE   - include the cpp file in the compile line:
                  g++ -o z UDP_class.cpp your_code.cpp

          - your code file your_code.cpp must include the line:
                  #include UDP_class.h

          - declaring the class will save the IP address and port
            but not whether send or receive is required.

          - to send or receive use the functions send_str and rcv_str.

          - note that rcv_str hangs up until a packet is received.

          - ensure the correct SELF_TEST #define below is uncommented.


   ALSO   - see http://www.yolinux.com/TUTORIALS/Sockets.html for details
            of each socket function used.

          - see UDP_class_usage* for an example of how to use the class.

Author Dr. Pj Radcliffe 2011,  released as GPL.

*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <string>

using namespace std ; // Use standard C++ names for functions.

//--- Have just one #define uncommented.
//    Choose NO_SELF_TEST if adding this class to your own code.

#define NO_SELF_TEST
//#define SELF_TEST_TALK
//#define SELF_TEST_LISTEN


class Tudp_handler
{ public:
    Tudp_handler(string ip, int port) ; // constructor, just saves ip and port.
    int send_udp() ;                    // send, 0=no error.
    int wait_receive_udp() ;            // wait to rx, 0=no error.
    int uri_to_ip(string& uri, string& ip) ; // convert URI name to ip address.


    //--- it is unusual to expose data in the public area but this way
    //    the user can use these strings to form the send string, and
    //    manipulate the received string.

    string error_message ;    // has error description or is blank.
    string send_str ;         // contents will be sent when send_up() called.
    string rcv_str ;          // after wait_receive() result put here.

  protected:
    string udp_ip ;
    int    udp_port ;
} ;
