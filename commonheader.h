//commonheader.h
#ifndef COMMONHEADER
#define COMMONHEADER

const unsigned int packet_size = 255;
const unsigned int packet_buffer_size = packet_size - 6; //minus info fields
const unsigned int max_packet_per_msg = 5;

const unsigned char packet_k1 = 11;
const unsigned char packet_k2 = 113;
const unsigned char packet_k3 = 13;

struct sserialpacket
{
/*packet keys*/
	unsigned char k1 = packet_k1;
	unsigned char k2 = packet_k2;
	unsigned char k3 = packet_k3;

/*packet info*/

	unsigned char sum = 1; //packets/message
	unsigned char n_th = 0; //index of this packet
	unsigned char datasize = packet_buffer_size; //amount of data sent in this packet

/*data*/
	unsigned char buffer[packet_buffer_size];
};

#endif
