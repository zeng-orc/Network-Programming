#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>

#include "dns_protocol.h"

const int SIZE = 256;
const int val = 49152;  // 1100 0000 0000 0000

void decompress(char* msg, int msg_length, int* length) {
  int i = msg_length, new_line = *length;

  while (msg[i] != '\0') {
    u_int ptr;
    memcpy(&ptr, msg + i, 2);
    ptr = ntohs(ptr);
    if (val <= ptr) {
      int offset = ptr - val;
      *length = new_line;
      decompress(msg, offset, &new_line);
      *length += 2;
      return;
    } else {
      int mark = msg[i++];
      i += mark;
      new_line += (mark + 1);
    }
  }
  *length = new_line + 1;
}

int get_ip_len(int qname_len) {
  return (qname_len - 1 < 1) ? 1 : (qname_len - 1);
}

int main(int argc, char* argv[]) {
  char service_port[SIZE] = "53";
  if (argc >= 2) strncpy(service_port, argv[1], strlen(argv[1]));

  struct addrinfo hints = {};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo* result = NULL;
  int rc = 0;
  if ((rc = getaddrinfo(NULL, service_port, &hints, &result)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
    return 1;
  }

  int sockfd = -1;
  for (struct addrinfo* p = result; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("server: bind");
      continue;
    }
    break;
  }

  freeaddrinfo(result);

  char send_message[2 * SIZE], recv_message[4 * SIZE];

  struct sockaddr_storage cli_addr;
  unsigned int cli_addr_len = sizeof(cli_addr);

  int received = -1;
  while ((received = recvfrom(sockfd, &recv_message, sizeof(recv_message), 0,
                              (struct sockaddr*)&cli_addr,
                              (socklen_t*)&cli_addr_len)) > 0) {
    if (received < 0) {
      perror("server: recvfrom");
      return 1;
    }

    char qname[SIZE];
    int qname_len = 0, offset = 0;
    DNS_HEADER header, response_header;
    QUESTION question;

    memset(&header, 0, sizeof(header));
    memcpy(&header, recv_message, sizeof(DNS_HEADER));

    offset = sizeof(DNS_HEADER);

    memset(qname, 0, SIZE);
    decompress(recv_message, offset, &qname_len);
    memcpy(qname, recv_message + offset, sizeof(QUESTION));

    if (ntohs(question.qclass) != 1) {
      printf("Type=%d\n", ntohs(question.qtype));
      fprintf(stderr,
              "A packet of type A is expected, another type is received.\n");
      return 1;
    }

    if (ntohs(header.q_count) > 1) {
      fprintf(stderr, "Questions count is greater than 1\n");
      return 1;
    }

    response_header.id = header.id;
    response_header.rcode = response_header.aa = response_header.tc = 0;
    response_header.qr = 1;
    response_header.rd = 1;
    response_header.opcode = response_header.z = response_header.ra = 0;
    response_header.q_count = htons(1);
    response_header.ans_count = htons(1);
    response_header.auth_count = htons(0);
    response_header.add_count = htons(0);

    offset = sizeof(response_header);
    memcpy(&send_message, &response_header, sizeof(response_header));

    memcpy(send_message + offset, &qname, qname_len);
    offset += qname_len;
    memcpy(send_message + offset, &question, sizeof(QUESTION));
    offset += sizeof(QUESTION);

    R_DATA rr;
    char name[SIZE];
    uint32_t rdata;

    strncpy(name, qname, qname_len);
    rr.type = htons(1);
    rr._class = htons(1);
    rr.ttl = htons(10);
    rr.data_len = htons(4);
    rdata = htonl(get_ip_len(qname_len));

    memcpy(send_message + offset, name, qname_len);
    offset += qname_len;
    memcpy(send_message + offset, &rr, sizeof(R_DATA));
    offset += sizeof(R_DATA);
    memcpy(send_message + offset, &rdata, sizeof(rdata));
    offset += sizeof(rdata);

    int sent = sendto(sockfd, &send_message, offset, 0,
                      (struct sockaddr*)&cli_addr, cli_addr_len);

    if (sent < 0) {
      perror("server: sendto");
      return 1;
    }

    memset(recv_message, 0, sizeof(recv_message));
    memset(send_message, 0, sizeof(send_message));
  }

  close(sockfd);

  return 0;
}
