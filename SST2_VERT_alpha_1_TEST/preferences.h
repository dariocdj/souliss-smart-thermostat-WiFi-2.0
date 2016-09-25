/**************************************************************************
  Copyright (C) 2016  mcbittech
  Wi-Fi Smart Thermostat 2.0 based on Souliss IoT Framework
    -wemos D1 mini
    -DHT22 Temperature & Humidity Sensor

  This example is only supported on ESP8266.
  Originally Developed by mcbittech
***************************************************************************/


#include <Arduino.h>

//LOCAL CRONO FUNCTION
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CRONO 1
#define CRONOLEARN 0

//CLOCK
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CLOCK 1
#define TIME_ZONE 1  // Central European Time
#define DAYLIGHTSAVINGTIME 0 // Ora legale

//SYSTEM AT BOOT
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SYSTEM_START_ON 1 if SST start at power on
//SYSTEM_START_ON 0 if SST start at power off.
#define SYSTEM_START_ON 1

//DISPLAY BRIGHT
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BRIGHT_MAX 100         
#define BRIGHT_MIN_DEFAULT 15  
#define BRIGHT_STEP_FADE_IN 20 
#define BRIGHT_STEP_FADE_OUT 1

//WIFI CONNECTION
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//0 Static Connection
//1 Dynamic Connection
#define DYNAMIC_CONNECTION 0

//DHCP
//1 Use DHCP IP
//0 Use Static IP Address
//0 Only for DYNAMIC_CONNECTION = 0
#define DHCP_OPTION 0

//STATIC IP
// use commas between number
#define STATIC_IP 192,168,1,51
#define STATIC_SUBNET 255,255,255,0
#define STATIC_IP_GW 192,168,1,1

// **** Define the WiFi name and password ****
//Only for DYNAMIC_CONNECTION = 0
#define WIFICONF_INSKETCH
#define WiFi_SSID               "WiFi_Flavio_PT"
#define WiFi_Password           "aeroxr6718"   

// Define the network configuration according to your router settingsuration according to your router settings
// and the other on the wireless oneless one
//Only for DYNAMIC_CONNECTION = 0
#define peer_address  0xAB21
#define myvNet_subnet 0xFF00
#define wifi_bridge_address    0xAB01 //gateway


//TOPICS PUBLISH&SUBSCRIBE
//Define if you want topics on display
#define ACTIVATETOPICSPAGE 1 
#define TOPICSPAGESNUMBER 2 // 1 if you have less than 3 topics subscribed, 2 if you have more than 3 topics subscribed


