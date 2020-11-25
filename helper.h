#include "packet.h"

char *create_data_packet(int block_num, char *data)
{
	char *packet;
	char s_block_num[3];
	sprintf(s_block_num, "%d", block_num);
	packet = (char *)malloc(strlen(data) + 4);
	bzero(packet, strlen(data) + 4);
	strcpy(packet, "03");
	strcat(packet, s_block_num);
	strcat(packet, data);
	return packet;
}

int make_udpsocket_bind(int port);

