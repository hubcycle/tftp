#include "transfer.h"

bool send_regular(int sockfd, struct sockaddr *sockInfo, PACKET *packet)
{
	size_t n;
	char buffer[MAX_BUFFER];
	n = create_packet_to_string(packet, buffer);

	return (sendto(sockfd, buffer, n, 0, (struct sockaddr *)sockInfo, sizeof(struct sockaddr)) >= 0);
}

bool send_error(int sockfd, struct sockaddr *sockInfo, unsigned short error_code, char *error_message)
{
	PACKET packet;

	packet.opcode = OPCODE_ERR;
	packet.error.error_code = error_code;
	strncpy(packet.error.message, error_message, MAX_STRING_SIZE);

	return send_regular(sockfd, sockInfo, &packet);
}

bool send_ack(int sockfd, struct sockaddr *sockInfo, unsigned short block_number)
{
	PACKET packet;

	packet.opcode = OPCODE_ACK;
	packet.ack.block_number = block_number;

	return send_regular(sockfd, sockInfo, &packet);
}

bool send_data(int sockfd, struct sockaddr *sockInfo, u_int16_t block_number, char *data, size_t data_size)
{
	PACKET packet;

	packet.opcode = OPCODE_DATA;
	packet.data.block_number = block_number;
	packet.data.data_size = data_size;
	memcpy(packet.data.data, data, data_size);

	return send_regular(sockfd, sockInfo, &packet);
}

bool send_file_to_client(int sockfd, struct sockaddr *client_addr, FILE *file)
{
	unsigned short block_number = 1;
	char data_buffer[MAX_DATA_SIZE];
	size_t num_of_bytes = MAX_DATA_SIZE;
	int result;
	PACKET packet;
	unsigned int count = 0;

	do
	{
		num_of_bytes = fread(data_buffer, sizeof(char), sizeof(data_buffer), file);
		printf("Sending data block :\t %hu\t", block_number);
		if (send_data(sockfd, client_addr, block_number, data_buffer, num_of_bytes) == false)
		{
			return false;
		}
		printf("Sent\n");

		result = wait_packet(sockfd, client_addr, OPCODE_ACK, &packet);

		if (result == -1)
		{
			if (count > MAX_TRIES)
			{
				send_error(sockfd, client_addr, ERRCODE_INVALID_OPERATION, "Too many timeouts");
				return false;
			}
			count++;
		}
		else
		{
			// verify for error
			if (packet.opcode == OPCODE_ERR)
			{
				print_error(&packet);
				return false;
			}

			if (packet.ack.block_number != block_number)
			{
				send_error(sockfd, client_addr, ERRCODE_UNKNOWN_BLOCK_ID, "Wrong Block Number");
				return false;
			}

			block_number++;
			count = 0;
		}

	} while (num_of_bytes == MAX_DATA_SIZE);

	return true;
}