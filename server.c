#include "socket_wrapper.h"
#include <json-c/json.h>

int main(int argc, char * argv[]) {
    int listen_fd, conn_fd;

    struct sockaddr_in serv_addr;
    
    if(argc != 2) {
        printf("Usage : %s <Port>\n", argv[0]);
        exit(1);
    }

    listen_fd = Socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    Bind(listen_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

    Listen(listen_fd, 5);

    printf("listening to port %s ..\n", argv[1]);

    //클라이언트 접속 요청 수락
    conn_fd = Accept(listen_fd, NULL, NULL);
    printf("Fuzzer's connection is accepted!\n");

    fd_set fd_status;
    struct timeval timeout;
    int result;

    while(1) {
        FD_ZERO(&fd_status);
        FD_SET(conn_fd, &fd_status);
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        result = select(conn_fd + 1, &fd_status, 0, 0, &timeout);

        if(result == -1 || result == 0) {
            error_handling("Fuzzer might be dead!");
            //break;
        }
        else {
            if((recv_file(conn_fd, "./recv.json")) == -1) {
                error_handling("recv_file() error");
            };
        }

        // JSON parsing
        FILE * fp = fopen("./recv.json", "r");
        if(fp == NULL) {
            puts("fopen() error");
            exit(1);
        }
        int size;
        char * buf;

        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        buf = (char *) malloc(size + 1);
        if(buf == NULL) {
            puts("malloc() error");
            exit(1);
        }
        memset(buf, 0, sizeof(buf));
        
        fread(buf, size, 1, fp);
        

        json_object * jobj = json_tokener_parse(buf);
        puts("---------------------current stats---------------------");
        printf("%s\n", json_object_to_json_string(jobj));
        puts("-------------------------------------------------------\n\n");


        //free
        //json_object_object_delete(jobj);

        free(buf);
        fclose(fp);
        
    }


    return 0;
}
