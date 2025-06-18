#include "../include/tcpip.h"

int main()
{
    int raw_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (raw_sock < 0) {
        perror("raw socket");
        return 1;
    }

    // 2. Bind to loopback interface
    sockaddr_ll sll {};
    sll.sll_family = AF_PACKET;
    sll.sll_protocol = htons(ETH_P_ALL);
    sll.sll_ifindex = if_nametoindex("lo"); // or "eth0"
    bind(raw_sock, (sockaddr*)&sll, sizeof(sll));

    // 3. Send data using normal TCP socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    connect(sock, (sockaddr *)&server_addr, sizeof(server_addr));
    
    const char *msg = "hello";
    send(sock, msg, strlen(msg), 0);
    close(sock);

    // 4. Receive one packet
    char buffer[65536];
    int bytes = recv(raw_sock, buffer, sizeof(buffer), 0);
    std::cout << "Captured " << bytes << " bytes" << std::endl;

    // You can now parse the Ethernet/IP/TCP headers manually here

    close(raw_sock);


    //PCAP LIB METHOD
    // pcap_t* handle = pcap_open_live("lo", BUFSIZ, 1, 1000, errbuf);

    // struct bpf_program fp;
    // pcap_compile(handle, &fp, "tcp port 12345", 0, PCAP_NETMASK_UNKNOWN);
    // pcap_setfilter(handle, &fp);

    // const u_char* packet;
    // struct pcap_pkthdr header;
    // packet = pcap_next(handle, &header);
}