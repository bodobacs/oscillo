//commonheader.h
#ifndef COMMONHEADER
#define COMMONHEADER

struct sserialpacketheader;

const unsigned char FLAG_TRIGGERED = 0x01;

const int packet_size = 255;
const int max_packet_per_msg = 3;

const unsigned char packet_k1 = 11;
const unsigned char packet_k2 = 113;
const unsigned char packet_k3 = 13;

const unsigned char bit_continous = 1;
const unsigned char bit_triggered = 3;

struct sserialpacketheader{
/*packet keys*/
	unsigned char k1 = packet_k1;
	unsigned char k2 = packet_k2;
	unsigned char k3 = packet_k3;

/*packet info*/
	unsigned char n_th = 0; //index of this packet
	unsigned char last_command = 0; //index of this packet
	unsigned char last_command_data = 0; //index of this packet
	unsigned char flags;
	unsigned char trigged_H;
	unsigned char trigged_L;//position of
};

const int packet_buffer_size = packet_size - sizeof(sserialpacketheader); // - bytes for info fields

struct sserialpacket
{
	struct sserialpacketheader h;
/*data*/
	unsigned char buffer[packet_buffer_size];
};

const unsigned char c_toggle_triggered = 1;
const unsigned char c_set_trig_level = 2;

struct sscommandpacket
{
	unsigned char command;
	unsigned char data;
};


#endif
