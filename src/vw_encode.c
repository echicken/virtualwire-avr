#include <stdlib.h>
#include "vw_crc16.h"
#include "vw_4to6.h"
#include "vw_encode.h"

uint8_t* vw_encode(uint8_t* data, uint8_t data_length)
{
  uint8_t  i;
  uint8_t  index         = 0;
  uint8_t  packet_length = data_length + 3; // Packet length is 1 byte, FCS is 2 bytes
  uint8_t* buffer        = malloc(packet_length + 9); // 8 byte preamble and null terminated
  uint16_t crc           = 0xffff;

  // The first 240 bits are alternating the training preamble, alternating 1
  // and 0. This is used to set the autogain correctly on the receiver.
  for(i = 0; i <= 23; i++)
    buffer[index++] = 0x2a;

  // Followed by 12 bits sync vector / start symbol so we know where the
  // packet begins and we haven't just received noise.
  buffer[index++] = 0x3a;
  buffer[index++] = 0x2c;

  // Encode the packet length. Each byte of the packet length and data are
  // encoded as two 6-bit symbols, high nibble then low nibble.
  crc = _crc_ccitt_update(crc, packet_length);
  vw_4to6_encode(packet_length, buffer, &index);

  // Encode the data in the same way. The byte order is big endian.
  for (i = 0; i < data_length; i++) {
    crc = _crc_ccitt_update(crc, data[i]);
    vw_4to6_encode(data[i], buffer, &index);
  }

  // Encode the ones complement of the CRC in little endian byte order.
  crc = ~crc;
  vw_4to6_encode(crc & 0xff, buffer, &index);
  vw_4to6_encode(crc >> 8, buffer, &index);

  // Null terminated the buffer, so we know when to stop transmitting.
  buffer[index] = 0;

  return buffer;
}
