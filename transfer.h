#include "packet.h"

bool send_regular(int sockfd, struct sockaddr *sockInfo, PACKET *packet);
bool send_error(int sockfd, struct sockaddr *sockInfo, unsigned short error_code, char *error_message);
bool send_ack(int sockfd, struct sockaddr *sockInfo, unsigned short block_number);
bool send_data(int sockfd, struct sockaddr *sockInfo, u_int16_t block_number, char *data, size_t data_size);

bool send_file_to_client(int sockfd, struct sockaddr *client_addr, FILE *file);