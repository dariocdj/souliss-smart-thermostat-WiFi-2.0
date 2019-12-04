/**************************************************************************
    Souliss
    Copyright (C) 2016  Veseo
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    Modified by Dario Di Maio, original code from
       https://github.com/esp8266/Arduino
***************************************************************************/

#define DYNAMIC_CONNECTION_Init() \     
  SERIAL_OUT.println("start DYNAMIC_CONNECTION_Init"); \
  startWebServer(); \
  if (!ReadIPConfiguration()) \
  { \
    SetAccessPoint(); \
    SERIAL_OUT.println("display_print_splash_waiting_need_configuration"); \
  } \
  if (IsRuntimeGateway()) \
  { \
    SERIAL_OUT.println("display_print_splash_waiting_connection_gateway"); \
    SetAsGateway(myvNet_dhcp); \
    SetAddressingServer(); \
  } \
  else \
  { \
    U16 SoulissVNETAddress = Read_SoulissVNETAddress(); \
    U16 SoulissVNETGateway = Read_SoulissVNETGateway(); \    
    SERIAL_OUT.println("display_print_splash_waiting_connection_peer"); \
    SetAddress(SoulissVNETAddress, 0xFF00, SoulissVNETGateway); \
    SERIAL_OUT.println("Address set from Webconfig"); \  
  } \
  
  
#define DYNAMIC_CONNECTION_fast() \
  if (IsRuntimeGateway())  \
    FAST_GatewayComms(); \
  else \
    FAST_PeerComms(); \
  

#define DYNAMIC_CONNECTION_slow() \ 
    if (!IsRuntimeGateway())  \
      SLOW_PeerJoin(); \
   
