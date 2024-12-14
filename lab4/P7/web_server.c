#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100
#define PORT 8080

void error_handling(const char *message);
void handle_request(int clnt_sock);
void send_data(FILE *fp, const char *content_type, const char *filename);
void execute_cgi(int clnt_sock, const char *method, const char *query);

int main() {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    printf("HTTP Server is running on port %d...\n", PORT);

    while (1) {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1) {
            perror("accept() error");
            continue;
        }
        handle_request(clnt_sock);
        close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}

void handle_request(int clnt_sock) {
    char req_line[SMALL_BUF];
    char method[10], path[SMALL_BUF], protocol[10];
    FILE *clnt_read = fdopen(clnt_sock, "r");
    FILE *clnt_write = fdopen(dup(clnt_sock), "w");

    if (fgets(req_line, SMALL_BUF, clnt_read) == NULL) {
        fclose(clnt_read);
        fclose(clnt_write);
        return;
    }

    // Parse the request line
    sscanf(req_line, "%s %s %s", method, path, protocol);

    if (strcmp(method, "GET") == 0) {
        if (strcmp(path, "/") == 0)
            strcpy(path, "/index.html");
        send_data(clnt_write, "text/html", path + 1); // Remove leading '/'
    } else if (strcmp(method, "POST") == 0) {
        char content[BUF_SIZE] = {0};
        char *query;

        // Skip headers
        while (fgets(req_line, SMALL_BUF, clnt_read) && strcmp(req_line, "\r\n") != 0);

        // Read POST body
        fread(content, 1, BUF_SIZE, clnt_read);
        query = content;

        execute_cgi(clnt_sock, method, query);
    } else {
        fprintf(clnt_write, "HTTP/1.1 405 Method Not Allowed\r\n\r\n");
    }

    fclose(clnt_read);
    fclose(clnt_write);
}

void send_data(FILE *fp, const char *content_type, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(fp, "HTTP/1.1 404 Not Found\r\n\r\n");
        fprintf(fp, "<html><body><h1>404 Not Found</h1></body></html>");
        fclose(fp);
        return;
    }

    fprintf(fp, "HTTP/1.1 200 OK\r\n");
    fprintf(fp, "Content-Type: %s\r\n\r\n", content_type);

    char buf[BUF_SIZE];
    while (fgets(buf, BUF_SIZE, file) != NULL)
        fputs(buf, fp);

    fclose(file);
    fflush(fp);
}

void execute_cgi(int clnt_sock, const char *method, const char *query) {
    FILE *fp = fdopen(dup(clnt_sock), "w");

    fprintf(fp, "HTTP/1.1 200 OK\r\n");
    fprintf(fp, "Content-Type: text/html\r\n\r\n");

    fprintf(fp, "<html><body>");
    fprintf(fp, "<h1>CGI Program Output</h1>");
    fprintf(fp, "<p>Method: %s</p>", method);
    fprintf(fp, "<p>Query: %s</p>", query);
    fprintf(fp, "</body></html>");

    fclose(fp);
}

void error_handling(const char *message) {
    perror(message);
    exit(1);
}
