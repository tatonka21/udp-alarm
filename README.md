# UDP Alarm Specification
Graphical user interface for the Open-USB-IO platform

AUTHOR OF SPEC: PJ RADCLIFFE

PURPOSE     This file is a proforma for the UDP Alarm lab exam.  It defines
            the assignment - there is no other documentation.

BACKGROUND  Networked devices are extensively used in engineering systems.
            The following system represents a 3 stage steam turbine and
            generator.  Each of the 4 items has its own temperature sensor
            that is connected to a private IP network.

                            SYSTEM BLOCK DIAGRAM

                   .     .     .
                   |\    |\    |\         .-----------.
                   | \   | \   | \    .---'           '---.
            .------'  \--'  \--'  \---'                   |--.
            '------.  /--   /--.  /---.                   |--'
                   | /   | /   | /    '---.           .---'
                   |'    |'    |'         '-----------'
                   t1    d1    t3             g1
                   .     .     .              .
                   .---------------------------  <-private LAN
                        |
                  .-----+------------.    .--------------------.
                  | your embedded PC +====+ Open-USB-IO board  |
                  '-----+------------'    |   drives displays. |
                        |                 '--------------------'
                  .-----+-----------------.
                  | control room computer |
                  '-----------------------'


            Each sensor sends out a packet approximately every 10 seconds
            that contains the temperature in degrees centigrade as a text
            string.  This packet is sent to your embedded PC using UDP
            where a program you write takes action.

            Your program runs on this embedded PC connected to an Open-USB-IO
            board.  If any temperature sensor goes over limit then the
            appropriate LED is turned on and a UDP alarm packet is sent to
            a control room computer.


SPECIFIC REQUIREMENTS 

         * You must use the Eclipse IDE to develop and run all programs.
           Your code must use UDP_class.cpp and UDP_class.h but not other
           files.

         * Your code will run on one PC, the real sensors and control room
           computer will be simulated using the udp_talker and udp_listener
           projects already in Eclipse.

                                SOFTWARE BLOCK DIAGRAM

                                    .-------------.-------.
                                    | Open-USB-IO >  LEDs |
                                    '------+------'-------'
                                           |
                                           ^
                                           |
             .----------------.     .------+--------.     .---------------.
             |                |     |               |     |               |
             | UDP talker     | UDP | Code written  | UDP | UDP listener  |
             | program sends  +->---+ for this lab  +->---+ display the   |
             | packets you    |     |               |     | UDP you       |
             | define         |     |               |     | generate.     |
             |                |     '---------------'     |               |
             '----------------'                           '---------------'
   TYPICAL IP:Port                     -->-127.0.0.1:55555   -->-127.0.0.2:44444


         * Your program must handle just one sensor.

         * The program will be started from the command line as follows-
           ./your_code own_ip own_port control_name  control_port temp_limit ousb_LED
           where
                own_ip   = IP address of this computer.
                own_port = port number to listen on.
                   note the temperature sensor is setup to send to the above.
                control_name = IP of control room computer.
                control_port = port on control room computer.
                temp_limit = degrees C that temperature must not exceed.
                             If temperature goes above this then declare an alarm.
                ousb_LED   = LED number 0-7 to control on alarm panel.

           Example ./your_code 127.0.0.1 55555 127.0.0.2 44444 120 0
           (If you are using Eclipse do not put ./your_code in the parameter list.)

         * The parameters can be assumed to all exist and have no errors.
           You do not need to check for errors.

         * On startup your program must turn off the Open-USB-IO LED
           for this sensor.
           Multiple copies of your code could be running and each can control
           one LED on the Open-USB-IO board.  Each active program must not
           interfere with the other LEDs.
           After adjusting the LEDs the program can start listening for packets.

         * A sensor will send a data packet formatted as follows-
             <2 letters name><one space><3 digits of temperature as deg. C>
           Example: the t3 sensor packet reading 10 degC would be "t3 010".

         * When the packet is received the program must print to the command
           line the word RX then a line feed (note the case).
           It must then compare the temperature to the limit set on program start up.

           If over limit then the LED  must be turned on, if under or on limit
           the LED must be turned off.  No other LEDS can be altered.

           If over limit your program must then print to the command line 
           the word OVER then a line feed (again note the case).

           If the temperature is over limit then a UDP packet must be sent to
           the control room computer.  The format is-
             <TEMP_ALARM><one space><2 letter sensor name>
           Continuing the example above "TEMP_ALARM t3"

         * If your program is started with no parameters then it must self
           identify as shown at the start of the code below and then terminate.
           You must change the id to your own id.

         * Your code should not loop forever (as the real code would).  It should
           listen for a packet,  act on the packet,  and then terminate.

         * In the lab exam submit the .cpp file,  not the binary.
           Copy the source code into /home/user/Documents and rename it-
             lab1_1234567.cpp
           where you should replace 1234567 with your own student number.
           Next hit the submit button on the desktop and answer the questions.
