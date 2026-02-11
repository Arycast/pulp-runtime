/**
 * The WiFiUDP class is used send and receive UDP messages over Wi-Fi.
 * 
 * https://docs.arduino.cc/language-reference/en/functions/wifi/udp/
 */
#ifndef WIFIUDP_H
#define WIFIUDP_H

#include <stdint.h>
#include <stream.h>

class WiFiUDP : public Stream {
private:
public:
    /**
     * method WiFiUDP
     *
     * Creates a named instance of the WiFi UDP class that can send and receive UDP 
     * messages.
     */
    WiFiUDP(void);
    
    /* Used as destructor as WiFiUDP is the constructor */
    ~WiFiUDP(void);

    /**
     * method begin
     * 
     * Initializes the WiFi UDP library and network settings. Starts WiFiUDP socket, 
     * listening at local port PORT.
     */
    uint8_t begin(int port);

    /**
     * method beginPocket
     * 
     * Starts a connection to write UDP data to the remote connection.
     */
    int beginPacket(const char *hostName, int port);
    int beginPacket(uint32_t hostIp, int port);
    
    /**
     * method endPacket
     * 
     * Called after writing UDP data to the remote connection. It finishes off the 
     * packet and send it.
     */
    int endPacket(void);

    /**
     * method write
     * 
     * Writes UDP data to the remote connection. Must be wrapped between beginPacket() 
     * and endPacket(). beginPacket() initializes the packet of data, it is not sent 
     * until endPacket() is called.
     */
    size_t write(uint8_t byte) override;
    size_t write(const uint8_t *buffer, size_t size);

    /**
     * method parsePacket
     * 
     * It starts processing the next available incoming packet, checks for the presence
     * of a UDP packet, and reports the size. parsePacket() must be called before 
     * reading the buffer with UDP.read().
     */
    int parsePacket(void);

    /**
     * method read
     * 
     * Reads UDP data from the specified buffer. If no arguments are given, it will 
     * return the next character in the buffer. This function can only be successfully 
     * called after WiFiUDP.parsePacket().
     */
    int read(void);
    int read(char *buffer, int len);

    /**
     * method stop
     * 
     * Disconnect from the server. Release any resource being used during the UDP 
     * session.
     */
    void stop(void);

    /**
     * method remoteIP
     * 
     * Gets the IP address of the remote connection. This function must be called after
     * WiFiUDP.parsePacket().
     */
    uint32_t remoteIP(void);

    /**
     * method remotePort
     * 
     * Gets the port of the remote UDP connection. This function must be called after
     * UDP.parsePacket().
     */
    uint16_t remotePort(void);

    /**
     * method available
     * 
     * Get the number of bytes (characters) available for reading from the buffer. This
     * is data that's already arrived. available() inherits from the Stream utility 
     * class.
     */
    int available(void) override;

    /**
     * method peek
     * 
     * Read a byte from the file without advancing to the next one. That is, successive
     * calls to peek() will return the same value, as will the next call to read(). 
     * This function inherited from the Stream class. 
     */
    int peek(void) override;

    /**
     * method flush
     * 
     * Discard any bytes that have been written to the client but not yet read. flush()
     * inherits from the Stream utility class.
     */
    void flush(void) override;

};

#endif