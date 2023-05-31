Am inceput aceasta tema pe baza laboratorului 9. In afara de client.c si requests.c/.h restul au ramas nemodificate.
# In requests:
- am adaugat o functie separata pentru request-ul de tip delete
- am adaugat si token-ul la parametrii

# In client:
    Am inceput prin a-mi face viata mai usoara si sa imi declar separat fiecare cale mi-am declarat si port-ul si ip-ul, si niste dimensiuni ajutatoare
    Am adaugat functii auxiliare precum read_command sau show_menu
    Toata implementarea se face intr-un while(1) care merge pana la aparitia comenzii "exit"
    Am folosit cate un if pentru fiecare comanda si am apelat functiile corespunzatoare
    Am adaugat si un if pentru cazul in care se introduce o comanda gresita
    Pentru fiecare comanda am facut un request si am afisat raspunsul primit de la server
    Pentru comenzile "register", "login", "enter_library" am verificat prezenta unui cookie si am afisat un mesaj corespunzator
    Pentru comenzile "add_book", "get_books", "delete_book" am verificat prezenta unui token si am afisat un mesaj corespunzator
* A fost lupta mare cand m-am prins pe ultima suta de metrii ca si token-ul trebuie tratat ca json si nu ca string
* Apropo de aceasta greseala, greu mi-a fost sa ma prind cand am "voie" sa folosesc strstr si cand nu, adica string vs json. La un moment dat cautam si "error" tot ca pe json
    Parsarea raspunsurilor am facut-o folosind biblioteca parser.c/.h. 
    Parsarea token-ului si a cartilor pentru comenzile "get_books" si "get_book" am facut-o pornind de la acel "\r\n\r\n" si am cautat in continuare dupa "token" sau "books"
    Pentru fiecare cerinta am folosit functiile din requests.c/.h, helper.c/.h si parser.c/.h si am incercat sa fiu cat de cat pe placul checker-ului
    La final am eliberat memoria alocata pentru raspunsuri si pentru request-uri
    Pe tot parcursul implementarii m-am folosit de comentarii, multe repetitive, dar care m-au ajutat sa inteleg mai bine ce fac si sa nu ma pierd in cod.
    Nu am apucat sa ma folosesc de postman, am rulat mult manual si putin cu checker-ul.
* Avand in vedere ca m-am apucat in ziua deadline-ului teoretic(21 mai), dintre care 1 o zi am zacut bolnava la pat, eu zic ca a mers destul de bine. Din lipsa de timp nu am reusit sa mai structurez codul, sa mai scap de portiuni care se repeta, dar am incercat sa il fac cat mai usor de citit si de inteles.
