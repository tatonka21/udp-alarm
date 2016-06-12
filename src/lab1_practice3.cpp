/*
 * PSUEDOCODE
 * begin
 *    identify//
 *    parse arguments//
 *    create containers for packets and osub vars//
 *    turn off leds:
 *       read portb
 *       bitmask
 *       write led
 *    listen//
 *    receive packet//
 *    print "RX" line feed//
 *    parse packet//
 *    compare limit to temp:
 *       if (temp> limit)//
 *          print "OVER" line feed//
 *          turn on led://
 *             read portb//
 *             bitmask//
 *             write led//
 *          send packet "TEMP_ALARM <sensor>"//
 *       if(temp <=limit)
 *          turn off led
 *             read portb
 *             bitmask
 *             write led
 *terminate
 * end
 */


#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <string.h>
#include <stdio.h>

#include "UDP_class.h"

using namespace std;



//====== MAIN ==========================================================

int cpp_do_ousb_command(string  *strx) //===========
{ FILE *fpipe ;
  if ( !(fpipe = (FILE*)popen(strx->c_str(),"r")))
   { cout << "pipe error" << endl ;
     exit(0) ;
   }
  char line[100] ;
  fgets( line, sizeof line, fpipe) ;
  pclose(fpipe) ;
  *strx = line ;        // caller can see whole ousb string in str.
  return( atoi(line)) ; // return int version of string.
}

int main(int argc, char *argv[])
{

//IDENTIFY
   if (argc == 1)  // no parameters.
     { cout << "3486075, s3486075@student.rmit.edu.au, Hayden Warmington"
            << endl ;
       return(0) ;
     }


//ARGUMENTS
   string IP1 = argv[1];
   int PORT1 = atoi(argv[2]);
   string IP2 = argv[3];
   int PORT2 = atoi(argv[4]);
   int limit = atoi(argv[5]);
   int led = atoi(argv[6]);

//CONTAINERS
   string sensor;
   int temp;
   string command;
   int portb;
   int pinb;

//OBJECTS
   Tudp_handler rx_udp( IP1, PORT1);
   Tudp_handler tx_udp( IP2, PORT2);
   stringstream stst;

//TURN OFF LEDS
   //READ PORTB
      command = "ousb -r io pinb";
      pinb = cpp_do_ousb_command(&command);

   //BITMASK
      portb = pinb &(~(1<<led));
   //WRITE LED
      stst << portb;
      command = "ousb -r io portb " + stst.str();
      cpp_do_ousb_command(&command);
      //CLEAR STST
      stst.clear();
      stst.str("");

//LISTEN
   rx_udp.wait_receive_udp ();
   cout << "RX" << endl;

//PARSE PACKET
   stst.str(rx_udp.rcv_str);
   stst >> sensor >> temp;
   //CLEAR STST
   stst.clear();
   stst.str("");

//COMPARE TEMP AND LIMIT
   //IF TEMP>LIMIT
      if (temp>limit) {
    	  //PRINT OVER
    	  cout << "OVER" << endl;
    	  //TURN ON LED
    	     //READ PINB
             command = "ousb -r io pinb";
             pinb = cpp_do_ousb_command(&command);
    	     //BITMASK
    	     portb = pinb|(1<<led);
    	     //WRITE PORTB
    	     stst << portb;
    	     command = "ousb -r io portb " + stst.str();
    	     cpp_do_ousb_command(&command);
    	     //CLEAR STST
    	     stst.clear();
    	     stst.str("");
    	     //SEND PACKET
    	     stst << "TEMP_ALARM " << sensor;
    	     tx_udp.send_str = stst.str();
    	     tx_udp.send_udp();
    	     //CLEAR STST
    	     stst.clear();
    	     stst.str("");

      }
      else if (temp<=limit) {
    	  //TURN OFF LEDS
    	     //READ PORTB
    	        command = "ousb -r io pinb";
    	        pinb = cpp_do_ousb_command(&command);

    	     //BITMASK
    	        portb = pinb &(~(1<<led));
    	     //WRITE LED
    	        stst << portb;
    	        command = "ousb -r io portb " + stst.str();
    	        cpp_do_ousb_command(&command);
    	        //CLEAR STST
    	        stst.clear();
    	        stst.str("");
      }


   return(0) ;
}
