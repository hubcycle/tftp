#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>

#define MAX_BUFFER 					550
#define MAX_FILENAME_LENGTH	256
#define MAX_PACKET_LENGTH		512
#define MAX_TRIES 					3
#define TIMEOUT 						5

#define MAX_STRING_SIZE 		512
#define MAX_MODE_SIZE 			8
#define MAX_DATA_SIZE 			512

#define OPCODE_RRQ 	1
#define OPCODE_WRQ 	2
#define OPCODE_DATA 3
#define OPCODE_ACK 	4
#define OPCODE_ERR 	5

#define ERRCODE_UNDEFINED 					0
#define ERRCODE_FILE_NOT_FOUND			1
#define ERRCODE_FILE_ACCESS_DENIED	2
#define ERRCODE_INVALID_OPERATION 	4
#define ERRCODE_UNKNOWN_BLOCK_ID 		5

int make_udpsocket_bind(int port);

short get_network_to_host_short(char *buffer);
short get_host_to_network_short(short host_short, char *buffer);
size_t copy_n_src_dest(char *dest, const char *src, size_t n);

#endif