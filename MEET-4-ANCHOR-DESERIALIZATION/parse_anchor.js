const fs = require("fs")
// the conversion to Big Endian is already implicitly handled by Node.js’s Buffer methods
function readUInt16BE(buffer, offset) {
  return buffer.readUInt16BE(offset)
}

function readUInt64BE(buffer, offset) {
  // read two 32-bit parts and combine into BigInt
  const high = buffer.readUInt32BE(offset)
  const low = buffer.readUInt32BE(offset + 4)
  return (BigInt(high) << 32n) + BigInt(low)
}

function parseAnchor(filename = "RNTuple.root", anchorOffset = 1889) {
    //store the file
  const fileBuffer = fs.readFileSync(filename)
  //check if file or offset is correct
  if (fileBuffer.length < anchorOffset + 72) {
    console.error("File too small or offset incorrect")
    return
  }

  // Slice the anchor buffer (72 bytes total)
  const anchorBuffer = fileBuffer.slice(anchorOffset, anchorOffset + 72)

  // Read fields  (unlike c++ here we are manually writing offset)
  const versionEpoch = readUInt16BE(anchorBuffer, 0)
  const versionMajor = readUInt16BE(anchorBuffer, 2)
  const versionMinor = readUInt16BE(anchorBuffer, 4)
  const versionPatch = readUInt16BE(anchorBuffer, 6)

  const seekHeader = readUInt64BE(anchorBuffer, 8)
  const nbytesHeader = readUInt64BE(anchorBuffer, 16)
  const lenHeader = readUInt64BE(anchorBuffer, 24)
  const seekFooter = readUInt64BE(anchorBuffer, 32)
  const nbytesFooter = readUInt64BE(anchorBuffer, 40)
  const lenFooter = readUInt64BE(anchorBuffer, 48)
  const maxKeySize = readUInt64BE(anchorBuffer, 56)
  const checksum = readUInt64BE(anchorBuffer, 64)

  console.log("Reading RNTuple anchor at offset", anchorOffset, "...\n")

  console.log("Anchor Version:")
  console.log(`  Epoch:  ${versionEpoch}`)
  console.log(`  Major:  ${versionMajor}`)
  console.log(`  Minor:  ${versionMinor}`)
  console.log(`  Patch:  ${versionPatch}\n`)

  console.log("Header:")
  console.log(`  Seek:   ${seekHeader}`)
  console.log(`  Nbytes: ${nbytesHeader}`)
  console.log(`  Length: ${lenHeader}\n`)

  console.log("Footer:")
  console.log(`  Seek:   ${seekFooter}`)
  console.log(`  Nbytes: ${nbytesFooter}`)
  console.log(`  Length: ${lenFooter}\n`)

  console.log(`Max Key Size: ${maxKeySize}`)

  // Print checksum as hex, padded to 16 digits (64 bits)
  const checksumHex = "0x" + checksum.toString(16).padStart(16, "0")
  console.log("Checksum:    ", checksumHex)
}

// Run parser
parseAnchor()
