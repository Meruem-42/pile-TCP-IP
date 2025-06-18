#include "../include/tcpip.h"
#include <iomanip>

int main()
{
    int raw_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (raw_sock < 0) {
        perror("raw socket");
        return 1;
    }

    sockaddr_ll sll {};
    sll.sll_family = AF_PACKET;
    sll.sll_protocol = htons(ETH_P_ALL);
    sll.sll_ifindex = if_nametoindex("lo");
    if (bind(raw_sock, (sockaddr*)&sll, sizeof(sll)) < 0) {
        perror("bind");
        return 1;
    }

    // Optional: give the socket time to start listening
    sleep(1);

    // Send data
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    connect(sock, (sockaddr *)&server_addr, sizeof(server_addr));
    
    const char *msg = "hello";
    send(sock, msg, strlen(msg), 0);
    close(sock);

    // Receive
    char buffer[65536];
    std::cout << "Waiting for packet..." << std::endl;
    int bytes = recv(raw_sock, buffer, sizeof(buffer), 0);
    if (bytes < 0) {
        perror("recv");
        return 1;
    }

    std::cout << "Captured " << bytes << " bytes" << std::endl;

    for (int i = 0; i < bytes; ++i) {
    std::cout << std::hex << std::setw(2) << std::setfill('0') << (0xff & buffer[i]) << " ";
    if ((i + 1) % 16 == 0)
        std::cout << std::endl;
    }
    std::cout << std::dec << std::endl; // back to decimal output


    //PCAP LIB METHOD
    // pcap_t* handle = pcap_open_live("lo", BUFSIZ, 1, 1000, errbuf);

    // struct bpf_program fp;
    // pcap_compile(handle, &fp, "tcp port 12345", 0, PCAP_NETMASK_UNKNOWN);
    // pcap_setfilter(handle, &fp);

    // const u_char* packet;
    // struct pcap_pkthdr header;
    // packet = pcap_next(handle, &header);
}