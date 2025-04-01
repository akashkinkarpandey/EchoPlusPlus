#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <iostream>

/**
 * @class Message
 * @brief A class for handling message serialization and deserialization in a chat room application
 * 
 * This class implements a simple message protocol where each message consists of:
 * - A 4-byte header containing the length of the message body
 * - A message body with a maximum size of 512 bytes
 * 
 * The total message structure is:
 * [4-byte header][message body]
 * 
 * The header is encoded as a 4-digit number representing the body length.
 * This allows for messages up to 9999 bytes, but the actual maximum is limited to 512 bytes.
 */
class Message
{
public:
    /**
     * @brief Default constructor initializing body length to 0
     */
    Message() : bodyLength_(0) {}

    /**
     * @brief Maximum size of the message body in bytes
     */
    enum
    {
        maxBytes = 512
    };

    /**
     * @brief Size of the header in bytes
     */
    enum
    {
        header = 4
    };

    /**
     * @brief Constructor that creates a message from a string
     * @param message The string to be converted into a message
     * 
     * This constructor:
     * 1. Calculates the appropriate body length
     * 2. Encodes the header
     * 3. Copies the message body into the data buffer
     */
    Message(std::string message)
    {
        bodyLength_ = getNewBodyLength(message);
        encodeHeader();
        std::memcpy(data + header, message.c_str(), bodyLength_);
    }

    /**
     * @brief Calculates the appropriate body length for a given message
     * @param message The input message string
     * @return The calculated body length (capped at maxBytes)
     */
    size_t getNewBodyLength(std::string &message)
    {
        size_t newLength = message.size();
        if (newLength > maxBytes)
        {
            return maxBytes;
        }
        return newLength;
    }

    /**
     * @brief Prints the received message to the console
     * 
     * Extracts the message body and prints it with a prefix
     */
    void printMessage()
    {
        std::string message = getData();
        std::cout << "Message recieved: " << message << std::endl;
    }

    /**
     * @brief Gets the complete message including header and body
     * @return A string containing the complete message
     */
    std::string getData()
    {
        int length = header + bodyLength_;
        std::string result(data, length);
        return result;
    }

    /**
     * @brief Gets only the message body without the header
     * @return A string containing only the message body
     */
    std::string getBody()
    {
        std::string dataStr = getData();
        std::string result = dataStr.substr(header, bodyLength_);
        return result;
    }

    /**
     * @brief Encodes the message header with the current body length
     * 
     * The header is encoded as a 4-digit number padded with spaces
     */
    void encodeHeader()
    {
        char new_header[header + 1] = "";
        sprintf(new_header, "%4d", static_cast<int>(bodyLength_));
        memcpy(data, new_header, header);
    }

    /**
     * @brief Decodes the message header to get the body length
     * @return true if header is valid, false otherwise
     * 
     * Validates that the decoded length doesn't exceed maxBytes
     */
    bool decodeHeader()
    {
        char new_header[header + 1] = "";
        strncpy(new_header, data, header);
        new_header[header] = '\0';
        int headerValue = atoi(new_header);
        if (headerValue > maxBytes)
        {
            bodyLength_ = 0;
            return false;
        }
        bodyLength_ = headerValue;
        return true;
    }

    /**
     * @brief Gets the current body length
     * @return The length of the message body
     */
    size_t getBodyLength()
    {
        return bodyLength_;
    }

private:
    char data[header + maxBytes];  ///< Buffer storing the complete message (header + body)
    size_t bodyLength_;            ///< Current length of the message body
};

#endif MESSAGE_HPP