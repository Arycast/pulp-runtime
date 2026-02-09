/**
    * this file is implementation of Arduino WiFi Network API
    * https://docs.arduino.cc/language-reference/en/functions/wifi/wificlass/
*/

#ifndef wifi_network_h
#define wifi_network_h

#include "variables.h" 
#include "String.hpp"
#include "IPAddress.h" /* Support for 4-byte IP Address structures */

/**
    * WiFi connection status
    */

/**
WL_CONNECTED: assigned when connected to a WiFi network; 
WL_NO_SHIELD: assigned when no WiFi shield is present; 
WL_IDLE_STATUS: it is a temporary status assigned when WiFi.begin() is called and remains active until the number of attempts expires (resulting in WL_CONNECT_FAILED) or a connection is established (resulting in WL_CONNECTED); 
WL_NO_SSID_AVAIL: assigned when no SSID are available; 
WL_SCAN_COMPLETED: assigned when the scan networks is completed; 
WL_CONNECT_FAILED: assigned when the connection fails for all the attempts; 
WL_CONNECTION_LOST: assigned when the connection is lost; 
WL_DISCONNECTED: assigned when disconnected from a network;
    */

/**
    * class WiFiClass
    * inherits to IPAddress class for 4-byte IP Address structure
    */
class WiFiClass : public IPAddress {
public :
    /**
        * public class method
        */

    /**
        * method begin
        * initializes the WiFi library's network setting and provide the current status
        * @param ssid The name of the WiFi network
        * @param pass The password of the targeted WiFi network
        * @param keyIndex This identifies which key are going to use (WEP encrypted network can hold up 4 different keys)
        * @param key a hexadecimal string used as a security code for WEP encrypted networks
        * @return WL_CONNECTED (when connected to a network), WL_IDLE_STATUS (when not connected, but powered on)
        * https://docs.arduino.cc/language-reference/en/functions/wifi/wificlass/#wifibegin
        */
    int begin(const char* ssid);
    int begin(const char* ssid, const char* pass);
    int begin(const char* ssid, int keyIndex, const char* key);

    /**
        * method disconnect
        * disconnects thw WiFi shield from the current network
        * https://docs.arduino.cc/language-reference/en/functions/wifi/wificlass/#wifidisconnect
        */
    void disconnect(void);

    /**
        * method config
        * configure a static IP Address as well as change the DNS, Gateaway, and subnet addresses on the WiFi shield
        * manually set the network adress of the shield
        * calling this method before begin method will forces begin to configure the WiFi shield with adresses specified in config
        * the shield will initialize with begin method in the default DHCP mode. Once config called, it will change the addresses
        * @param ip IP Address of the device
        * @param dns The address for a DNS server
        * @param gateway The IP Address of the network gateway. Default to the device IP Address with 1 in the last octet (optional) 
        * @param subnet The subnet mask of the network. Default to 255.255.255.0 (optional)
        * https://docs.arduino.cc/language-reference/en/functions/wifi/wificlass/#wificonfig
        */
    void config(IPAddress ip);
    void config(IPAddress ip, IPAddress dns);
    void config(IPAddress ip, IPAddress dns, IPAddress gateaway);
    void config(IPAddress ip, IPAddress dns, IPAddress gateaway, IPAddress subnet);

    /**
        * method setDNS
        * configure the DNS server
        * @param dns_server1 The IP Address of the primary DNS server
        * @param dns_server2 The IP Address of the secondary DNS server
        * https://docs.arduino.cc/language-reference/en/functions/wifi/wificlass/#wifisetdns
        */
    void setDNS(IPAddress dns_server1);
    void setDNS(IPAddress dns_server1, IPAddress dns_server2);

    /**
        * method SSID
        * gets the SSID of the current network
        * @param wifiAccessPoint Specifies from which network to get the information
        * @return String containing name of network requested or network currently connected to
        * https://docs.arduino.cc/language-reference/en/functions/wifi/wificlass/#wifissid
        */
    // String SSID(void);
    // String SSID(int wifiAccessPoint);

    /**
        * method BSSID
        * gets the MAC Address of the router are currently connected to
        * @param bssid 6-byte array
        * @return a byte array containing the MAC Address of the router is currently connected to
        * https://docs.arduino.cc/language-reference/en/functions/wifi/wificlass/#wifibssid
        */
    byte BSSID(uint8_t* bssid);

    /**
        * method RSSI
        * gets the signal strength of the connection to the router
        * @param wifiAccessPoint Specifies from which network to get the information
        * @return The current RSSI in dBm
        * https://docs.arduino.cc/language-reference/en/functions/wifi/wificlass/#wifirssi
        */
    long RSSI(void);
    long RSSI(const char* wifiAccessPoint);

    /**
        * method encryptionType
        * gets the encryption type of the current network
        * @param wifiAccessPoint Specifies which network to get information from
        * @return Value represents the type of encryption (TKIP (WPA) = 2 WEP = 5 CCMP (WPA) = 4 NONE = 7 AUTO = 8)
        * https://docs.arduino.cc/language-reference/en/functions/wifi/wificlass/#wifiencryptiontype
        */
    byte encryptionType(void);
    byte encryptionType(const char* wifiAccessPoint);

    /**
        * method scanNetworks
        * scans for available WiFi networks and returns the discovered number
        * @return Number of discovered networks
        * https://docs.arduino.cc/language-reference/en/functions/wifi/wificlass/#wifiscannetworks
        */
    byte scanNetworks(void);

    /**
        * method status
        * gets the WiFi connection status
        * @return The connection status
        * https://docs.arduino.cc/language-reference/en/functions/wifi/wificlass/#wifistatus
        */
    int status(void);

    /**
        * method getSocket
        * gets the first socket available
        * @return The first socket available
        * https://docs.arduino.cc/language-reference/en/functions/wifi/wificlass/#wifigetsocket
        */
    int getSocket(void);

    /**
        * method macAddress
        * gets the MAC Address of WiFi shield
        * @param mac 6 byte array to hold the MAC Address
        * @return 6 bytes representing the WiFi shield's MAC Address
        * https://docs.arduino.cc/language-reference/en/functions/wifi/wificlass/#wifimacaddress
        */
    byte macAddress(const char* mac);
};

/**
    * Pre-instantiated object for global access to the WiFi
    */
extern WiFiClass WiFi;

#endif