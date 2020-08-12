#include "socket_wrapper.h"
#include <json-c/json.h>

void json_object_print(json_object * jobj, int dep) {
    printf("{\n");    
    
    int val_type;
    json_object_object_foreach(jobj, key, val) {
        for(int i = 0; i < dep; i++)
                    printf("\t");
        val_type = json_object_get_type(val);
        switch(val_type) {
            case json_type_object:
                printf("<%s> : ", key);
                json_object_print(val, dep + 1);
                break;
            case json_type_int:
                printf("%s : %d\n", key, json_object_get_int(val));
                break;
            case json_type_string:
                printf("%s : %s\n", key, json_object_get_string(val));
                break;
            case json_type_double:
                printf("%s : %.2lf\n", key, json_object_get_double(val));
                break;
        }
    }
    for(int i = 0; i < dep - 1; i++)
        printf("\t");
    printf("}\n");
}



int main(int argc, char * argv[]) {
    int listen_fd, conn_fd;

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_len;
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

    while(1) {
        clnt_len = sizeof(clnt_addr);

        //클라이언트 접속 요청 수락
        conn_fd = Accept(listen_fd, (struct sockaddr *)&clnt_addr, &clnt_len);
        printf("Fuzzer's connection is accepted!\n");

        fd_set fd_status;
        struct timeval timeout;
        int result;

        FD_ZERO(&fd_status);
        FD_SET(conn_fd, &fd_status);
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;
        result = select(conn_fd + 1, &fd_status, 0, 0, &timeout);

        if(result == -1 || result == 0) {
            error_handling("error!");
            //break;
        }
        if((recv_file(conn_fd, "./recv.json")) == -1) {
            puts("Fuzzer might be dead!");
            close(conn_fd);
            break;
        };

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
        json_object_object_add(jobj, "ip", json_object_new_string(inet_ntoa(clnt_addr.sin_addr)));
        printf("heap address : %p\n", jobj);
        puts("---------------------current stats---------------------");
        json_object_print(jobj, 1);
        puts("-------------------------------------------------------\n\n");


        //free
        json_object_put(jobj);
        free(buf);
        fclose(fp);
            
    }


    return 0;
}
