#include "packet.h"

int wait_packet(int sockfd, struct sockaddr *client_addr, unsigned short opcode, PACKET *packet)
{
	char packet_buffer[MAX_BUFFER];
	size_t num_of_bytes;
	socklen_t client_length = sizeof(client_addr);

	errno = 0;
	while (1)
	{
		printf("Waiting for message..");
		num_of_bytes = recvfrom(sockfd, packet_buffer, sizeof(packet_buffer), 0, client_addr, &client_length);
		printf("Received\n");

		if (errno != 0)
		{
			if (errno == EINTR)
			{
				printf("timeout \n");
			}
			else
			{
				printf("Error %d\n", errno);
			}
			return -1;
		}

		if (create_string_to_packet(packet_buffer, num_of_bytes, packet) == NULL)
		{
			return -1;
		}

		if (packet->opcode == opcode)
		{
			return num_of_bytes;
		}
		else if (packet->opcode == OPCODE_ERR)
		{
			return 0;
		}
	}
}

PACKET *create_string_to_packet(char *buffer, size_t buffer_size, PACKET *packet)
{
	char *offset;
	size_t n;
	if (buffer_size < 4 || packet == NULL)
	{
		return NULL;
	}

	bzero(packet, sizeof(packet));
	packet->opcode = get_network_to_host_short(buffer);
	offset = &buffer[2];

	if (packet->opcode == OPCODE_RRQ)
	{
		n = copy_n_src_dest(packet->read_request.filename, offset, MAX_FILENAME_LENGTH);
		offset = offset + n;
		copy_n_src_dest(packet->read_request.mode, offset, MAX_MODE_SIZE);
	}
	else if (packet->opcode == OPCODE_DATA)
	{
		packet->data.block_number = get_network_to_host_short(offset);
		offset = offset + sizeof(short);
		packet->data.data_size = buffer_size - (2 * sizeof(short));
		memcpy(packet->data.data, offset, buffer_size);
	}
	else if (packet->opcode == OPCODE_ACK)
	{
		packet->ack.block_number = get_network_to_host_short(offset);
	}
	else if (packet->opcode == OPCODE_ERR)
	{
		packet->error.error_code = get_network_to_host_short(offset);
	}
	else
	{
		return NULL;
	}

	return packet;
}

size_t create_packet_to_string(const PACKET *packet, char *buffer)
{
	if (packet == NULL || buffer == NULL)
	{
		return 0;
	}

	size_t n = 0;
	char *offset;
	*(short *)buffer = get_host_to_network_short(packet->opcode, NULL);
	offset = buffer + sizeof(short);

	if (packet->opcode == OPCODE_RRQ)
	{
		n = copy_n_src_dest(offset, packet->read_request.filename, MAX_FILENAME_LENGTH);
		offset = offset + n;
		n = n + copy_n_src_dest(offset, packet->read_request.mode, MAX_MODE_SIZE);
	}
	else if (packet->opcode == OPCODE_DATA)
	{
		*(short *)offset = get_host_to_network_short(packet->data.block_number, NULL);
		offset = offset + sizeof(short);
		memcpy(offset, packet->data.data, packet->data.data_size);
		n = n + sizeof(short) + packet->data.data_size;
	}
	else if (packet->opcode == OPCODE_ACK)
	{
		*(short *)offset = get_host_to_network_short(packet->ack.block_number, NULL);
		n = sizeof(ACK);
	}
	else if (packet->opcode == OPCODE_ERR)
	{
		*(short *)offset = get_host_to_network_short(packet->ack.block_number, NULL);
		offset = offset + sizeof(short);
		n = copy_n_src_dest(offset, packet->error.message, MAX_STRING_SIZE);
		n = n + sizeof(short);
	}
	else
	{
		return 0;
	}

	return (n + sizeof(short));
}

void print_error(PACKET *packet)
{
	if (packet->opcode != OPCODE_ERR)
	{
		return;
	}

	printf("Error received\t:");

	switch (packet->error.error_code)
	{
	case ERRCODE_UNDEFINED:
		printf("Undefined");
		break;

	case ERRCODE_FILE_NOT_FOUND:
		printf("File not found");
		break;

	case ERRCODE_FILE_ACCESS_DENIED:
		printf("File access denied");
		break;

	case ERRCODE_INVALID_OPERATION:
		printf("Invalid operation");
		break;

	case ERRCODE_UNKNOWN_BLOCK_ID:
		printf("Unknown block ID");
		break;

	default:
		printf("%hu", packet->error.error_code);
		break;
	}

	printf("\tError Message:\t%s\n", packet->error.message);
}
