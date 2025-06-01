#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>

// Read a 16-bit big-endian integer from buffer
uint16_t ReadBigEndian16(const char *buffer)
{
    return (static_cast<uint8_t>(buffer[0]) << 8) |
           (static_cast<uint8_t>(buffer[1]));
}

// Read a 64-bit big-endian integer from buffer
uint64_t ReadBigEndian64(const char *buffer)
{
    return (static_cast<uint64_t>(static_cast<uint8_t>(buffer[0])) << 56) |
           (static_cast<uint64_t>(static_cast<uint8_t>(buffer[1])) << 48) |
           (static_cast<uint64_t>(static_cast<uint8_t>(buffer[2])) << 40) |
           (static_cast<uint64_t>(static_cast<uint8_t>(buffer[3])) << 32) |
           (static_cast<uint64_t>(static_cast<uint8_t>(buffer[4])) << 24) |
           (static_cast<uint64_t>(static_cast<uint8_t>(buffer[5])) << 16) |
           (static_cast<uint64_t>(static_cast<uint8_t>(buffer[6])) << 8) |
           (static_cast<uint64_t>(static_cast<uint8_t>(buffer[7])));
}

// Function to parse the RNTuple anchor from a ROOT file
void ParseAnchor(const std::string &filePath = "RNTuple.root", std::streamoff offset = 1889)
{
    std::ifstream file(filePath, std::ios::binary); // Open file in binary mode
    if (!file)
    {
        std::cerr << "Error: Cannot open file " << filePath << "\n";
        return;
    }

    file.seekg(offset); // Move to the anchor offset
    if (!file)
    {
        std::cerr << "Error: Cannot seek to offset " << offset << "\n";
        return;
    }

    char buffer[8]; // Temporary buffer to hold bytes

    // Read 2 bytes each for version fields
    file.read(buffer, 2);
    uint16_t versionEpoch = ReadBigEndian16(buffer);

    file.read(buffer, 2);
    uint16_t versionMajor = ReadBigEndian16(buffer);

    file.read(buffer, 2);
    uint16_t versionMinor = ReadBigEndian16(buffer);

    file.read(buffer, 2);
    uint16_t versionPatch = ReadBigEndian16(buffer);

    // Read 8 bytes each for remaining 64-bit fields
    file.read(buffer, 8);
    uint64_t seekHeader = ReadBigEndian64(buffer);

    file.read(buffer, 8);
    uint64_t nbytesHeader = ReadBigEndian64(buffer);

    file.read(buffer, 8);
    uint64_t lenHeader = ReadBigEndian64(buffer);

    file.read(buffer, 8);
    uint64_t seekFooter = ReadBigEndian64(buffer);

    file.read(buffer, 8);
    uint64_t nbytesFooter = ReadBigEndian64(buffer);

    file.read(buffer, 8);
    uint64_t lenFooter = ReadBigEndian64(buffer);

    file.read(buffer, 8);
    uint64_t maxKeySize = ReadBigEndian64(buffer);

    file.read(buffer, 8);
    uint64_t checksum = ReadBigEndian64(buffer);
   

    file.close(); // Close the file after reading

    // Print the parsed anchor values
    std::cout << "RNTuple Anchor Parsed:\n";
    std::cout << "  Version:      " << versionEpoch << "." << versionMajor << "." << versionMinor << "." << versionPatch << "\n";
    std::cout << "  SeekHeader:   " << seekHeader << "\n";
    std::cout << "  NbytesHeader: " << nbytesHeader << "\n";
    std::cout << "  LenHeader:    " << lenHeader << "\n";
    std::cout << "  SeekFooter:   " << seekFooter << "\n";
    std::cout << "  NbytesFooter: " << nbytesFooter << "\n";
    std::cout << "  LenFooter:    " << lenFooter << "\n";
    std::cout << "  MaxKeySize:   " << maxKeySize << "\n";
    std::cout << "  Checksum:     " << checksum << "\n";
}
