all:
	gcc -o tftp tftpserver.c helper.c packet.c transfer.c