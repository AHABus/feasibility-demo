Honours Project - FlightCore HAB Bus :: Feasibility Demonstrations
==================================================================

The feasibility demonstrations for the FlightCore HAB Bus project include the
four following pieces:

 * The radio transmission source code written as part of the FlightCore C
   library. It functions as a state machine tor transmit 8-bit bytes over an
   RTTY (8N2) link controlled by an AVR micro-controller and can be compiled
   using AVR-GCC.

 * The code added to the open source project Dl-Fldigi to extract the raw bytes
   received over RTTY and forward them to a UNIX virtual terminal (vtty) for
   another application to decode.
 
 * The systems diagram of the planned FlightCore flight software architecture,
   and the preliminary design of the packet protocol that will be implemented
   for radio communications with the ground station.
  
 * Pictures of the hardware that will be used for prototyping and development
   of the hardware side of the FlightCore flight controller and data bus.

