/**
    * this file is implementation of Arduino WiFi Client API
    * https://docs.arduino.cc/language-reference/en/functions/wifi/client/
*/

#ifndef wifi_client_h
#define wifi_client_h

#include <stdbool.h> /* provides boolean type support */
#include <stddef.h> /* provides standard definitions */

#include "variables.h" 
#include "String.hpp"
#include "IPAddress.h" /* Support for 4-byte IP Address structures */
#include "stream.h" /* base class for character and binary based streams */

/**
    * class WiFiClient
    * inherits from IPAddress and Stream class
    */
class WiFiClient : public IPAddress, public Stream
{
private:
    size_t printFloat(double number, int digits) override { return 0; }
    size_t printNumber(unsigned long val, int format) { return 0; }

public :
    /**
        * public class method
        */

    WiFiClient(void);
    ~WiFiClient(void);

    /** 
        * method available, read, and flush
        * override class from stream class because WiFi client has its own implementation
        */
    
    /**
        * method available
        * returns the amount of data that has been written to the client by the server is connected to (bytes available for reading)
        * @return The number of bytes available
        * https://docs.arduino.cc/language-reference/en/functions/wifi/client/#clientavailable
        */
    int available(void) override;

    /**
        * method read
        * read the next byte received from the server client is connected to
        * @return The next byte (or -1 if none is available)
        * https://docs.arduino.cc/language-reference/en/functions/wifi/client/#clientread
        */
    int read(void) override;

    /**
        * method flush
        * discards any bytes that have been written to the client but not yet read
        * https://docs.arduino.cc/language-reference/en/functions/wifi/client/#clientflush
        */
    void flush(void) override;

    /**
        * method peek
        * standard don't have peek method, but declare (and define) it here
        * so that this class not became virtual class
        */
    int peek(void) override;

    /**
        * method connected
        * whether or not the client is connected
        * note that a client is considered connected if the connection has been closed but there is still unread data
        * @return True if the client is connected, false if not
        * https://docs.arduino.cc/language-reference/en/functions/wifi/client/#clientconnected 
        */
    bool connected(void);

    /**
        * method connect
        * connect to the IP address and port specified in the constructor
        * connect() also supports DNS lookups when using a domain name
        * @param ip The IP Address that the client will connect to
        * @param port The port that the client will connect to
        * @param URL The domain name that the client will connected to
        * @return True if the connection succeeds, false if not
        */
    bool connect(IPAddress ip, int port);
    bool connect(String URL, int port);

    /**
        * method write
        * write data to the server the client is connected to
        * @param data The byte or char to write
        * @return The number of characters written
        * https://docs.arduino.cc/language-reference/en/functions/wifi/client/#clientwrite
        */
    size_t write(byte data) override;
    /*byte write(byte data);*/ /* cannot overload function solely based on return type */
    size_t write(char data);
    /*byte write(char data);*/
    size_t write(const byte *data, size_t length);
    size_t write(const char *data) override { return 0; }

    /**
        * method print
        * print data to the server that a client is connected to
        * @param data The data to print
        * @param BASE The base in which to print numbers
        * @return The number of bytes written, though reading that number is optional. Rather than byte, it is size_t to prevent return-conflicts as write-return in Print class is size_t. 
        * https://docs.arduino.cc/language-reference/en/functions/wifi/client/#clientprint
        */
    size_t print(char data);
    size_t print(const char *val) override { return 0; }
    size_t print(byte data);
    size_t print(int data);
    size_t print(unsigned int val) override { return 0; }
    size_t print(long data);
    size_t print(String data);
    size_t print(const String &s) override { return 0; }
    size_t print(char data, int BASE);
    size_t print(byte data, int BASE);
    size_t print(int data, int BASE);
    size_t print(unsigned int val, int BASE) override { return 0; }
    size_t print(long data, int BASE);
    size_t print(unsigned long val, int BASE) override { return 0; }
    size_t print(double val, int BASE) override { return 0; }
    size_t print(String data, int BASE);

    /**
        * method println
        * print data, followed by a carriage return and newline, to the server a client is connected to
        * @param data The data to print
        * @param BASE The base in which to print numbers
        * @return The number of bytes written, though reading that number is optional
        * https://docs.arduino.cc/language-reference/en/functions/wifi/client/#clientprint
        */
    size_t println(void);
    size_t println(char data);
    size_t println(const char *val) override { return 0; }
    size_t println(byte data);
    size_t println(int data);
    size_t println(unsigned int val) override { return 0; }
    size_t println(long data);
    size_t println(String data);
    size_t println(char data, int BASE);
    size_t println(byte data, int BASE);
    size_t println(int data, int BASE);
    size_t println(unsigned int val, int BASE) override { return 0; }
    size_t println(long data, int BASE);
    size_t println(unsigned long val, int BASE) override { return 0; }
    size_t println(double val, int BASE) override { return 0; }
    size_t println(String data, int BASE);

    void stop(void);
};

/**
    * Pre-instantiated object for global access to the WiFiClient
    */
extern WiFiClient client;

#endif