#ifndef wifi_client_h
#define wifi_client_h

#include <stdbool.h>
#include <stddef.h>

#include "variables.h"
#include "String.hpp"
#include "IPAddress.h"
#include "stream.h"

class WiFiClient : public IPAddress, public Stream
{
public :
    WiFiClient(void);
    ~WiFiClient(void);

    // override virtual method (from Stream class)
    int available(void) override;
    int read(void) override;
    void flush(void) override;
    int peek(void) override;

    bool connected(void);
    bool connect(IPAddress ip, int port);
    bool connect(String URL, int port);
    size_t write(byte data) override;
    /*byte write(byte data);*/ /* cannot overload function solely based on return type */
    size_t write(char data);
    size_t write(const byte *data, size_t length) override;
    size_t write(const char *data, size_t length);
    /*byte write(char data);*/
    byte print(char data);
    byte print(byte data);
    byte print(int data);
    byte print(long data);
    byte print(String data);
    byte print(char data, int BASE); //DEC for decimal (base 10), OCT for octal (base 8), HEX for hexadecimal (base 16).
    byte print(byte data, int BASE);
    byte print(int data, int BASE);
    byte print(long data, int BASE);
    byte print(String data, int BASE);
    byte println(void);
    byte println(char data);
    byte println(byte data);
    byte println(int data);
    byte println(long data);
    byte println(String data);
    byte println(char data, int BASE); //DEC for decimal (base 10), OCT for octal (base 8), HEX for hexadecimal (base 16).
    byte println(byte data, int BASE);
    byte println(int data, int BASE);
    byte println(long data, int BASE);
    byte println(String data, int BASE);
    void stop(void);
};

extern WiFiClient client;

#endif