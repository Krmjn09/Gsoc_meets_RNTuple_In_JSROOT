#include <iostream>
#include <fstream>
#include <cstdint>
#include <iomanip>

// Reads a 16 bit number and arranges them in big -endian order

// uint64_t: fixed-width unsigned integer type
uint16_t ReadU16BE(std::ifstream &stream)
{
    uint8_t bytes[2];
    stream.read(reinterpret_cast<char *>(bytes), 2);
    return (bytes[0] << 8) | bytes[1];
}

uint64_t ReadU64BE(std::ifstream &stream)
{
    uint8_t bytes[8];
    stream.read(reinterpret_cast<char *>(bytes), 8);
    uint64_t val = 0;
    for (int i = 0; i < 8; ++i)
    {
        val = (val << 8) | bytes[i];
    }
    return val;
}

// deserializes the anchor of RnTUPLE.ROOT
// anchorOffset : KIND OF SPOT WHERE ANCHOR LIVES
void parse_anchor(const std::string &filename = "RNTuple.root", std::streamoff anchorOffset = 1895)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    file.seekg(anchorOffset);
    if (!file)
    {
        std::cerr << "Error: Could not seek to offset " << anchorOffset << "\n";
        return;
    }

    std::cout << "Reading RNTuple anchor at offset " << anchorOffset << "...\n\n";

    uint16_t versionEpoch = ReadU16BE(file);
    uint16_t versionMajor = ReadU16BE(file);
    uint16_t versionMinor = ReadU16BE(file);
    uint16_t versionPatch = ReadU16BE(file);

    uint64_t seekHeader = ReadU64BE(file);
    uint64_t nbytesHeader = ReadU64BE(file);
    uint64_t lenHeader = ReadU64BE(file);
    uint64_t seekFooter = ReadU64BE(file);
    uint64_t nbytesFooter = ReadU64BE(file);
    uint64_t lenFooter = ReadU64BE(file);
    uint64_t maxKeySize = ReadU64BE(file);
    uint64_t checksum = ReadU64BE(file);

    std::cout << "Anchor Version:\n";
    std::cout << "  Epoch:  " << versionEpoch << "\n";
    std::cout << "  Major:  " << versionMajor << "\n";
    std::cout << "  Minor:  " << versionMinor << "\n";
    std::cout << "  Patch:  " << versionPatch << "\n\n";

    std::cout << "Header:\n";
    std::cout << "  Seek:   " << seekHeader << "\n";
    std::cout << "  Nbytes: " << nbytesHeader << "\n";
    std::cout << "  Length: " << lenHeader << "\n\n";

    std::cout << "Footer:\n";
    std::cout << "  Seek:   " << seekFooter << "\n";
    std::cout << "  Nbytes: " << nbytesFooter << "\n";
    std::cout << "  Length: " << lenFooter << "\n\n";

    std::cout << "Max Key Size: " << maxKeySize << "\n";
    std::cout << "Checksum:     0x" << std::hex << std::setw(16) << std::setfill('0') << checksum << std::dec << "\n";

    file.close();
}
