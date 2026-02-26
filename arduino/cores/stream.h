/**
    * this file is implementation of Arduino Stream API
    * https://docs.arduino.cc/language-reference/en/functions/communication/stream/
*/

#ifndef stream_h
#define stream_h

#include <stdbool.h> /* provides boolean type support */
#include <stddef.h> /* provides standard definitions, like size_t */

#include "variables.h"
#include "String.hpp"

#include "Print.h" /* abstract base class for write, print, and println method */
#include "arduino_time.h" /* provides arduino time API (millis) */

/*
    * LookaheadMode
    * the mode used to look ahead in the stream for an integer
    */
enum LookaheadMode {
/*
    * SKIP ALL
    * all characters other than digits or a minus sign are ignored when scanning the stream for an integer
    * this is the default mode.    
    */
    SKIP_ALL,

/*
    * SKIP NONE
    * nothing is skipped, and the stream is not touched unless the first waiting character is valid
    */
    SKIP_NONE,

/*
    * only tabs, spaces, line feeds, and carriage returns are skipped.
    */
    SKIP_WHITESPACE
};

/*
    * class Stream
    * the base class for character and binary based streams
    * stream defines the reading and parsing functions
    * inherits from Print class
    */
class Stream : public Print {
protected :
    unsigned long _timeout; /* timeout duration for blocking read or peek operations */
    unsigned long def_timeout = 1000; /* default timeout value */

    int timedRead(); /* read operation with timeout */
    int timedPeek(); /* peek operation with timeout */
    int peekNextDigit(LookaheadMode lookahead, bool detectDecimal); /* helper for parseInt and parseFloat method */
public :
    /**
        * public class method
        */
    
    /**
        * method available (pure virtual function)
        * derived class must implement this method to provide their own logic
        * @return The number of bytes available for reading in the stream
        * https://docs.arduino.cc/language-reference/en/functions/communication/stream/streamAvailable/
        */
    virtual int available(void) = 0;

    /**
        * method read (pure virtual function)
        * reads a byte from the stream and advances to the next one
        * derived class must implement this method to provide their own logic
        * @return The next byte (or character), or -1 if none is available
        * https://docs.arduino.cc/language-reference/en/functions/communication/stream/streamRead/
        */
    virtual int read(void) = 0;

    /**
        * method flush (pure virtual function)
        * waits for the transmission of outgoing data to complete
        * this function ensures that all data in the output buffer has been sent before the program continues
        * derived class must implement this method to provide their own logic
        * https://docs.arduino.cc/language-reference/en/functions/communication/stream/streamFlush/
        */
    virtual void flush(void) = 0;

    /**
        * method find
        * reads data from the stream until the target is found
        * @param target The string to search for
        * @param length Length of the target
        * @return True if the target is found, false if the operation times out
        * https://docs.arduino.cc/language-reference/en/functions/communication/stream/streamFind/
        */
    bool find(char *target);
    bool find(char *target, size_t length);

    /**
        * method findUntil
        * reads data from the stream until the target string or the terminator string is found, or the operation times out
        * @param target The string to search for
        * @param terminator The terminator string that stop the search
        * @return True if the target string is found, false if the terminator is found first or the operation times out
        * https://docs.arduino.cc/language-reference/en/functions/communication/stream/streamFindUntil/
        */
    bool findUntil(char *target, char *terminator);

    /**
        * method peek (pure virtual function)
        * reads a byte from the stream without advancing to the next one
        * derived class must implement this method to provide their own logic
        * @return The next byte (or character), or -1 if none is available
        * https://docs.arduino.cc/language-reference/en/functions/communication/stream/streamPeek/
        */
    virtual int peek(void) = 0;

    /**
        * method readBytes
        * reads characters from a stream into a buffer
        * the function terminates if the specified length has been read, or it times out
        * @param buffer The buffer to store the bytes in
        * @param length The number of bytes to read
        * @return The number of bytes placed in the buffer, or 0 if no valid data was found
        * https://docs.arduino.cc/language-reference/en/functions/communication/stream/streamReadBytes/
        */
    size_t readBytes(char *buffer, size_t length);
    size_t readBytes(byte *buffer, size_t length);

    /**
        * method readBytesUntil
        * reads characters from a stream into a buffer
        * the function terminates if the terminator character is detected, the specified length has been read, or it times out
        * the terminator character is not included in the buffer
        * @param character The terminator character to search for
        * @param buffer The buffer to store the bytes in
        * @param length The maximum number of bytes to read
        * @return The number of bytes placed in the buffer, or 0 if the terminator was not found or no data was available
        * https://docs.arduino.cc/language-reference/en/functions/communication/stream/streamReadBytesUntil/
        */
    size_t readBytesUntil(char character, char *buffer, size_t length);
    size_t readBytesUntil(char character, byte *buffer, size_t length);

    /**
        * method readString
        * reads characters from a stream into a string
        * the function terminates if it times out
        * @return A string containing the characters read from the stream, or an empty string if no data is available
        * https://docs.arduino.cc/language-reference/en/functions/communication/stream/streamReadString/
        */
    String readString(void);

    /**
        * method readStringUntil
        * reads characters from a stream into a String
        * the function terminates if the terminator character is detected or it times out
        * the terminator character is not included in the returned String
        * @param terminator The character to search for
        * @return A string containing the characters read from the stream, up to (but not including) the terminator character.
        * If the operation times out before finding the terminator, it returns the characters read up to that point
        * (which may be an empty string if no data was available)
        * https://docs.arduino.cc/language-reference/en/functions/communication/stream/streamReadStringUntil/
        */
    String readStringUntil(char terminator);

    /**
        * method parseInt
        * returns the first valid (long) integer number from the current position
        * the function is terminated by the first character that is not a digit (except for the leading minus sign), or when it times out
        * @param lookahead The mode used to look ahead in the stream for an integer
        * @param ignore Character to skip in the search, used for example to skip thousands divider
        * @return The first valid integer number, or 0 if no valid number is found
        * https://docs.arduino.cc/language-reference/en/functions/communication/stream/streamParseInt/
        */
    long parseInt();
    long parseInt(LookaheadMode lookahead);
    long parseInt(LookaheadMode lookahead, char ignore);

    /**
        * method parseFloat
        * returns the first valid floating point number from the current position
        * the function is terminated by the first character that is not a floating point number, or when it times out
        * @param lookahead The mode used to look ahead in the stream for a floating point number
        * @param ignore Character to skip in the search, used for example to skip thousands dividers
        * @return The first valid floating point number, or 0.0 if no valid number is found
        * https://docs.arduino.cc/language-reference/en/functions/communication/stream/streamParseFloat/
        */
    float parseFloat();
    float parseFloat(LookaheadMode lookahead);
    float parseFloat(LookaheadMode lookahead, char ignore);

    /**
        * method setTimeout
        * sets the maximum milliseconds to wait for stream data
        * the default timeout value is 1000 milliseconds
        * @param time Timeout duration in milliseconds
        * https://docs.arduino.cc/language-reference/en/functions/communication/stream/streamSetTimeout/
        */
    void setTimeout(unsigned long time);

    /**
        * method getTimeout
        * returns the current timeout value, in milliseconds, set by setTimeout method
        * the default timeout value is 1000 milliseconds
        * @return The timeout value in milliseconds
        * https://docs.arduino.cc/language-reference/en/functions/communication/stream/streamGetTimeout/
        */
    unsigned long getTimeout(void);
};

#endif