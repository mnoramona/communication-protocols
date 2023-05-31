#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <string.h>

#include "parson/parson.c"

#include "helpers.h"
#include "requests.h"

// Datele de conectare: HOST: 34.254.242.81, PORT: 8080
    int port = 8080;
    char *ip = (char *) "34.254.242.81";

// Rute de acces: 

    // POST /api/v1/tema/auth/register
    char* register_root = (char*) "/api/v1/tema/auth/register";

    // POST /api/v1/tema/auth/login
    char* login_root = (char*) "/api/v1/tema/auth/login";

    // GET /api/v1/tema/library/access
    char* access_root = (char*) "/api/v1/tema/library/access";

    // GET /api/v1/tema/library/books
    char* books_root = (char*) "/api/v1/tema/library/books";

    // GET /api/v1/tema/library/books/:bookId
    char* book_root = (char*) "/api/v1/tema/library/books/";

    // POST /api/v1/tema/library/books
    char* add_book_root = (char*) "/api/v1/tema/library/books";

    // DELETE /api/v1/tema/library/books/:bookId
    char* delete_book_root = (char*) "/api/v1/tema/library/books/";

    // GET /api/v1/tema/auth/logout
    char* logout_root = (char*) "/api/v1/tema/auth/logout";

#define COM 100
#define BUF 256

// functie de citire a comenzilor din consola
char *read_command() {
    char *command = (char *)malloc(COM * sizeof(char));
    fgets(command, COM, stdin);
    return command;
}

// functie de afisare a meniului
void show_menu(){
    printf("Alege o comanda din urmatoarele optiuni:\n");
    printf("1. Inregistrare cont: \"register\"\n");
    printf("2. Autentificare cont: \"login\"\n");
    printf("3. Cerere de acces in biblioteca: \"enter_library\"\n");
    printf("4. Cerere pentru vizualizarea tuturor cartilor: \"get_books\"\n");
    printf("5. Cerere pentru vizualizarea informatiilor despre o carte: \"get_book\"\n");
    printf("6. Cerere pentru adaugarea unei carti: \"add_book\"\n");
    printf("7. Cerere pentru stergerea unei carti: \"delete_book\"\n");
    printf("8. Deconectare: \"logout\"\n");
    printf("9. Iesire din aplicatie: \"exit\"\n");
}

int main(int argc, char *argv[]) {
    char *token = NULL;
    char *cookie = NULL;

    char *title = (char *)malloc(BUF * sizeof(char));
    char *author = (char *)malloc(BUF * sizeof(char));
    char *genre = (char *)malloc(BUF * sizeof(char));
    char *publisher = (char *)malloc(BUF * sizeof(char));
    char *page_count = (char *)malloc(BUF * sizeof(char));

    show_menu();

    while (1){
        char *message = (char *)malloc(BUFLEN * sizeof(char));
        char *response = (char *)malloc(BUFLEN * sizeof(char));
        int sockfd;
        char *command = read_command();
        if (!strcmp(command, "register\n")){
            // se deschide conexiunea
            sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

            // se verifica daca exista deja un cookie
            if (cookie != NULL) {
                printf("Sunteti deja autentificat!\n");
                continue;
            } else {
                // se citesc datele de inregistrare
                char *username = (char *)malloc(BUF * sizeof(char));
                char *password = (char *)malloc(BUF * sizeof(char));
                printf("username=");
                fgets(username, BUF, stdin);
                printf("password=");
                fgets(password, BUF, stdin);

                // se creeaza json-ul
                JSON_Value *root_value = json_value_init_object();
                JSON_Object *root_object = json_value_get_object(root_value);
                json_object_set_string(root_object, "username", username);
                json_object_set_string(root_object, "password", password);

                // se transforma json-ul in string
                char *json_string = json_serialize_to_string_pretty(root_value);

                // afisez json-ul
                printf("%s\n", json_string);

                // creez un array de char pointers
                char *body_data[1] = {json_string};
                char *cookies[1] = {cookie};

                // se creeaza mesajul
                message = compute_post_request(ip, register_root, "application/json", body_data, 1, cookies, 0, NULL);

                // se afiseaza mesajul
                printf("%s\n", message);

                // se trimite mesajul la server
                send_to_server(sockfd, message);

                // se primeste raspunsul de la server
                response = receive_from_server(sockfd);

                // se afiseaza raspunsul
                printf("%s\n", response);

                // se verifica daca inregistrarea a fost realizata cu succes
                if (strstr(response, "error") != NULL) {
                    printf("Inregistrarea nu a fost realizata cu succes!\n");
                } else {
                    printf("Inregistrarea a fost realizata cu succes!\n");
                }
            }
            
            // se inchide conexiunea
            close_connection(sockfd);
        } else if (!strcmp(command, "login\n")){
            // se deschide conexiunea
            sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);
            
            // se verifica daca exista deja un cookie
            if (cookie != NULL) {
                printf("Sunteti deja autentificat!\n");
                continue;
            } else {
                // se citesc datele de autentificare
                char *username = (char *)malloc(BUF * sizeof(char));
                char *password = (char *)malloc(BUF * sizeof(char));
                printf("username=");
                fgets(username, BUF, stdin);
                printf("password=");
                fgets(password, BUF, stdin);

                // se creeaza json-ul
                JSON_Value *root_value = json_value_init_object();
                JSON_Object *root_object = json_value_get_object(root_value);
                json_object_set_string(root_object, "username", username);
                json_object_set_string(root_object, "password", password);

                // se transforma json-ul in string
                char *json_string = json_serialize_to_string_pretty(root_value);

                // afisez json-ul
                printf("%s\n", json_string);

                // creez un array de char pointers
                char *body_data[1] = {json_string};
                char *cookies[1] = {NULL};

                // se creeaza mesajul
                message = compute_post_request(ip, login_root, "application/json", body_data, 1, cookies, 0, NULL);

                // se afiseaza mesajul
                printf("%s\n", message);

                // se trimite mesajul la server
                send_to_server(sockfd, message);

                // se primeste raspunsul de la server
                response = receive_from_server(sockfd);

                // se afiseaza raspunsul
                printf("%s\n", response);

                // se verifica daca autentificarea a fost realizata cu succes
                if(strstr(response, "error") != NULL) {
                    printf("Autentificarea nu a fost realizata cu succes!\n");
                } else {
                    printf("Autentificarea a fost realizata cu succes!\n");
                    // se extrage cookie-ul
                    char *cookieStart = strstr(response, "Cookie");
                    char *pathStart = strstr(response, "Path");

                    if (cookieStart != NULL && pathStart != NULL) {
                        int cookieStartIndex = cookieStart - response + 8;
                        int cookieLength = pathStart - cookieStart - 10;

                        cookie = (char *)malloc(cookieLength * sizeof(char));

                        strncpy(cookie, response + cookieStartIndex, cookieLength);
                        cookie[cookieLength] = '\0';
                    } else {
                    cookie[0] = '\0';  // Setează șirul rezultat ca fiind gol
                    }
                }   
            }

        } else if (!strcmp(command, "enter_library\n")){
           // daca nu sunt autentificat
            if (cookie == NULL) {
                printf("Nu sunteti autentificat!\n");
                continue;
            } else {
                // se deschide conexiunea
                sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);
    
                // creez un array de char pointers
                char *cookies[1] = {cookie};
    
                // se creeaza mesajul
                message = compute_get_request(ip, access_root, NULL, cookies, 1, NULL);
    
                // se afiseaza mesajul
                printf("%s\n", message);
    
                // se trimite mesajul la server
                send_to_server(sockfd, message);
    
                // se primeste raspunsul de la server
                response = receive_from_server(sockfd);
    
                // se afiseaza raspunsul
                printf("%s\n", response);
    
                // se verifica daca accesul a fost realizat cu succes
                if (strstr(response, "error") == NULL) {
                     printf("Acces realizat cu succes!\n");

                    // se extrage token-ul
                    char *start = strstr(response, "\r\n\r\n");
                    start += 4;

                    token = (char *)malloc(500 * sizeof(char));
                    JSON_Value *root_value = json_parse_string(start);
                    JSON_Object *json = json_value_get_object(root_value);
                    char *altjson = json_object_get_string(json, "token");
                    memcpy(token, altjson, strlen(altjson) + 1);

                } else {
                    printf("Accesul nu a fost realizat cu succes!\n");
                }
            }
        } else if (!strcmp(command, "get_books\n")){
            // se verifica daca exista token-ul de acces 
            if (token == NULL) {
                printf("Nu aveti acces la biblioteca!\n");
                continue;
            } else {
                
                // se deschide conexiunea
                sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

                // creez un array de char pointers
                char *cookies[1] = {cookie};
    
                // se creeaza mesajul
                message = compute_get_request(ip, books_root, NULL, cookies, 1, token);
    
                // se afiseaza mesajul
                printf("%s\n", message);
    
                // se trimite mesajul la server
                send_to_server(sockfd, message);
    
                // se primeste raspunsul de la server
                response = receive_from_server(sockfd);
    
                // se afiseaza raspunsul
                printf("%s\n", response);
    
                // se verifica daca accesul a fost realizat cu succes
                if (strstr(response, "error") == NULL) {
                    printf("Acces realizat cu succes!\n");
                    
                    char* start = strstr(response, "\r\n\r\n");
                    start += 4;
                    JSON_Value *root_value = json_parse_string(start);
                    JSON_Array *books = json_value_get_array(root_value);
                    int books_count = json_array_get_count(books);
                    
                    // afisare carti dupa id si title
                    for (int i = 0; i < books_count; i++) {
                        JSON_Object *book = json_array_get_object(books, i);
                        int id = json_object_get_number(book, "id");
                        char *title = json_object_get_string(book, "title");
                        printf("%d %s\n", id, title);
                    }

                } else {
                    printf("Accesul nu a fost realizat cu succes!\n");
                }
            }

        } else if (!strcmp(command, "get_book\n")){
            // se verifica daca exista token-ul de acces 
            if (token == NULL) {
                printf("Nu aveti acces la biblioteca!\n");
                continue;
            } else {
                // se deschide conexiunea
                sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

                // se citeste id-ul cartii
                char *id = (char *)malloc(BUF * sizeof(char));
                printf("id=");
                fgets(id, BUF, stdin);
                
                // parsez id-ul cu "/"
                char *id_aux = (char *)malloc(BUF * sizeof(char));

                // adaug in id_aux book_root
                memcpy(id_aux, books_root, strlen(books_root));

                // adaug in id_aux "/"
                strcat(id_aux, "/");
                strcat(id_aux, id);

                // elimin \n
                id_aux[strlen(id_aux) - 1] = '\0';

                // afisez id-ul
                printf("%s\n", id_aux);

                // creez un array de char pointers
                char *cookies[1] = {cookie};
    
                // se creeaza mesajul
                message = compute_get_request(ip, id_aux, NULL, cookies, 0, token);
    
                // se afiseaza mesajul
                printf("%s\n", message);
    
                // se trimite mesajul la server
                send_to_server(sockfd, message);
    
                // se primeste raspunsul de la server
                response = receive_from_server(sockfd);
    
                // se afiseaza raspunsul
                printf("%s\n", response);
    
                // se verifica daca accesul a fost realizat cu succes
                if (strstr(response, "error") == NULL) {
                    printf("Acces realizat cu succes!\n");
                    char* start = strstr(response, "\r\n\r\n");
                    start += 4;
                    JSON_Value *root_value = json_parse_string(start);
                    JSON_Object *books = json_value_get_object(root_value);

                    // afisare carti
                    printf("title=%s\n", json_object_get_string(books, "title"));
                    printf("author=%s\n", json_object_get_string(books, "author"));
                    printf("genre=%s\n", json_object_get_string(books, "genre"));
                    printf("publisher=%s\n", json_object_get_string(books, "publisher"));
                    printf("page_count=%.0f\n", json_object_get_number(books, "page_count"));


                } else {
                     printf("Accesul nu a fost realizat cu succes!\n");
                }
            }
        } else if (!strcmp(command, "add_book\n")){
            //daca nu am token-ul de acces
            if(token == NULL){
                printf("Nu aveti acces la biblioteca!\n");
                continue;
            } else {
                // se deschide conexiunea
                sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

                // se citesc datele cartii
                printf("title=");
                fgets(title, BUF, stdin);
                printf("author=");
                fgets(author, BUF, stdin);
                printf("genre=");
                fgets(genre, BUF, stdin);
                printf("publisher=");
                fgets(publisher, BUF, stdin);
                printf("page_count=");
                fgets(page_count, BUF, stdin);

                title[strcspn(title, "\n")] = '\0';
                author[strcspn(author, "\n")] = '\0';
                genre[strcspn(genre, "\n")] = '\0';
                publisher[strcspn(publisher, "\n")] = '\0';
                page_count[strcspn(page_count, "\n")] = '\0';

                // formez json-ul
                JSON_Value *root_value = json_value_init_object();
                JSON_Object *root_object = json_value_get_object(root_value);
                json_object_set_string(root_object, "title", title);
                json_object_set_string(root_object, "author", author);
                json_object_set_string(root_object, "genre", genre);
                json_object_set_string(root_object, "publisher", publisher);
                json_object_set_string(root_object, "page_count", page_count);

                char *json_string = json_serialize_to_string_pretty(root_value);

                // creez un array de char pointers
                char *cookies[1] = {cookie};
                char *body[1] = {json_string};
    
                // se creeaza mesajul
                message = compute_post_request(ip, books_root, "application/json", body, 1, cookies, 0, token);
    
                // se afiseaza mesajul
                printf("%s\n", message);
    
                // se trimite mesajul la server
                send_to_server(sockfd, message);
    
                // se primeste raspunsul de la server
                response = receive_from_server(sockfd);
    
                // se afiseaza raspunsul
                printf("%s\n", response);
    
                // se verifica daca adagarea a fost realizata cu succes
                if (strstr(response, "error") == NULL) {
                     printf("Adaugarea a fost realizata cu succes!\n");
                } else {
                     printf("Adaugarea nu a fost realizata cu succes!\n");
                }

                // se inchide conexiunea
                close_connection(sockfd);
            }
        } else if (!strcmp(command, "delete_book\n")){
            // daca nu sunt autentificat
            if (token == NULL) {
                printf("Nu aveti acces la biblioteca!\n");
                continue;
            } else {
                // se deschide conexiunea
                sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

                // se citeste id-ul cartii
                char *id = (char *)malloc(BUF * sizeof(char));
                printf("id=");
                fgets(id, BUF, stdin);

                // parsez id-ul cu "/"
                char *id_aux = (char *)malloc(BUF * sizeof(char));

                // adaug in id_aux book_root
                memcpy(id_aux, books_root, strlen(books_root));

                memcpy(id_aux, "/", 1);
                strcat(id_aux, id);
    
                // creez un array de char pointers
                char *cookies[1] = {cookie};
    
                // se creeaza mesajul
                message = compute_delete_request(ip, delete_book_root, NULL, cookies, 1, NULL, token);
    
                // se afiseaza mesajul
                printf("%s\n", message);
    
                // se trimite mesajul la server
                send_to_server(sockfd, message);
    
                // se primeste raspunsul de la server
                response = receive_from_server(sockfd);
    
                // se afiseaza raspunsul
                printf("%s\n", response);
    
                // se verifica daca stergerea a fost realizata cu succes
                if (strstr(response, "error") != NULL) {
                    printf("Stergere realizata cu succes!\n");
                } else {
                    printf("Stergerea nu a fost realizata cu succes!\n");
                }
            }

        } else if (!strcmp(command, "logout\n")){
            // daca nu sunt autentificat
            if (cookie == NULL) {
                printf("Nu sunteti autentificat!\n");
                continue;
            } else {
                // se deschide conexiunea
                sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);
    
                // creez un array de char pointers
                char *cookies[1] = {cookie};
    
                // se creeaza mesajul
                message = compute_get_request(ip, logout_root, NULL, cookies, 1, NULL);
    
                // se afiseaza mesajul
                printf("%s\n", message);
    
                // se trimite mesajul la server
                send_to_server(sockfd, message);
    
                // se primeste raspunsul de la server
                response = receive_from_server(sockfd);
    
                // se afiseaza raspunsul
                printf("%s\n", response);
    
                // se verifica daca delogarea a fost realizata cu succes
                if (strstr(response, "error") == 0) {
                     printf("Delogare realizata cu succes!\n");
                     // se sterge cookie-ul
                     cookie = NULL;
                     token = NULL;
                } else {
                     printf("Delogarea nu a fost realizata cu succes!\n");
                }
            }
        } else if (!strcmp(command, "exit\n")){
            break;
        } else {
            printf("Comanda invalida!\n");
        }
        free(message);
        free(response);
    }
    return 0;
}
