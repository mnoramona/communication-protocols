#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *query_params, char **cookies, int cookies_count, char *token){
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // comenzile de tip GET
    if (query_params == NULL){
        sprintf(line, "GET %s HTTP/1.1", url);
    }
    else{
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    }

    compute_message(message, line);

    // se adauga host-ul
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // se adauga cookie-ul
    if(cookies != NULL){
        for(int i = 0; i < cookies_count; i++){
            sprintf(line, "Cookie: %s", cookies[i]);
            compute_message(message, line);
        }
    }

    if(token != NULL){
        // se adauga token-ul
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }
    
    // se adauga linia de final
    compute_message(message, "");
    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char **body_data, int body_data_fields_count, char **cookies, int cookies_count, char *token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    char *body_data_buffer = calloc(LINELEN, sizeof(char));

    // calculam lungimea totala a datelor din body
    int total_len = 0;
    for(int i = 0; i < body_data_fields_count; i++){
        total_len += strlen(body_data[i]);
    }

    // comenzile de tip POST
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);

    // se adauga content type
    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    // se adauga host-ul
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // se adauga lungimea totala a datelor din body
    sprintf(line, "Content-Length: %d", total_len);
    compute_message(message, line);

    if(cookies != NULL){
        // se adauga cookie-ul
        for(int i = 0; i < cookies_count; i++){
            sprintf(line, "Cookie: %s", cookies[i]);
            compute_message(message, line);
        }
    }

    if(token != NULL){
        // se adauga token-ul
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }

    // se adauga linia de final
    compute_message(message, "");
    
    memset(line, 0, LINELEN);

    // se adauga datele din body
    if(body_data != NULL){
        compute_message(message, body_data[0]);
    }

    free(line);
    return message;
}

char *compute_delete_request(char *host, char *url, char *query_params, char **cookies, int cookies_count, char *id, char *token){
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // comenzile de tip DELETE
    if (query_params != NULL){
        if(id != NULL){
            sprintf(line, "DELETE %s/%s?%s HTTP/1.1", url, id, query_params);
        }
        else{
            sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
        } 
    }

    compute_message(message, line);

    // se adauga host-ul
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if(cookies != NULL){
        // se adauga cookie-ul
        for(int i = 0; i < cookies_count; i++){
            sprintf(line, "Cookie: %s", cookies[i]);
            compute_message(message, line);
        }
    }

    if(token != NULL){
        // se adauga token-ul
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }

    // se adauga linia de final
    compute_message(message, "");
    return message;
}
