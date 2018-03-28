Nume: Mihalache Florin-Razvan
Grupa: 313 CC
Timpul efectiv de munca: 6 zile

Tema 1

In cadrul temei, m-am gandit sa iau totul la modul simplist: mai intai citesc
prima linie din fisierul de intrare in main si cu switch-case pe primul caracter
din prima linie decid ce algoritm de aplica. La ultimele 3 task-uri (inclusiv 
bonusul) am luat si cuanta din prima linie in switch-case.

Task 1 - First Come First Served
In acest task am construit 3 liste (ready, running si waiting) - principiu 
aplicat si la celelalte task-uri din tema. Am facut ca in running sa ajunga
primul element din coada folosind functii specifice cozii (adaugare / 
scoatere din coada) la (multiple) add, repetand aceeasi procedura in cazul
comenzilor wait, event si tick (din running in waiting, din waiting in ready).

Am facut o functie auxiliara de tip void pentru tick pe care o apelez in toti
algoritmii construiti ca functii void - in aceasta functie scade durata de 
viata al procesului si daca aceasta devine 0 se goleste running.

La comanda show am apelat o functie de afisare in fisier a procesului din
running (cu nume si durata de viata), functie pe care o apelez in cadrul
tuturor task-urilor din tema.

La event folosesc o functia auxiliara (numita search) ce cauta un element
in lista dupa numele procesului, pentru a selecta procesul dorit din waiting
pentru a-l baga in running. Procedeul in esenta lui se repeta la fiecare
algoritm.

La multiple add am sters de pe linia de pe care era ma [procese] primele
3 caractere, adica "ma ", apoi am parsat procesele in felul urmator:
am construit o functie ce taie primele 3 cuvinte in functie de spatiu
dintr-un sir de caractere (cut_first_n_words), iar in aceasta functie 
am apelat o functie auxiliara de mine construita pentru a combate un
bug (no_strcpy), in care sterg primele k caractere - ea functioneaza
ca un strcpy, scopul ei fiind de a inlocui strcpy, din cauza caruia
am erori pe valgrind si vmchecker. Notabila e observatia ca nu folosesc
deloc strcpy si in schimb folosesc sprintf, cu scopul de a scapa de erorile
din valgrind si vmchecker, provocate de strcpy. 
Sa revenim la multiple add. Cu ajutorul functiei cut_first_n_words am taiat 
primele 3 cuvinte (care reprezentau datele despre un proces), functie apelata
dupa ce adaugam fiecare proces luat din sirul de caractere cu sscanf in ready.
A intervenit la final insa o problema, dupa ce am terminat de adaugat in ready
la multiple add. In mod normal trebuia sa am p elemente, insa in realitate aveam
3 * p elemente, ultimele 2*p fiind adaugate aiurea, asa am am procedat in asa fel
incat sa elimin ultimele 2*p elemente din ready, folosind o structura repetitiva
si functia auxiliara remove_last, care sterge ultimul element din coada. Acelasi
procedeu e folosit si la urmatoarele task-uri, la multiple add, asa ca "manarim"
bug-ul :)

Task 2 - Shortest Job First

In acest task, folosesc o functie auxiliara ce returneaza nodul ce are
durata de viata cea mai mica (search_minimum), pentru a vedea procesul
din ready ce are durata minima de viata, cu scopul de a-l baga in running,
stergand dupa aceea procesul din ready. Asta caracterizeaza in principiu
acest task, prin care se diferentiaza de primul. In principiu, algoritmii
difera prin modul cum iei din procesele din ready si cum le adaugi si le rulezi
in running.

Task 3 - Round Robin

Acest task ar fi ca o continuare a primului task, diferentiindu-se prin faptul
ca exista o cuanta prestabilita de timp (pe care o iau de pe prima linie cu
sscanf). Folosesc o variabila de contorizare a timpului petrecut in running 
(count) care e incrementata la fiecare tick al running-ului. Daca contorul
devine egal cu cuanta de timp, procesul din running e mutat in ready (la 
sfarsitul cozii ready mai precis) si in ready e bagat primul proces din ready
si e reinitializat contorul pentru cuanta de timp.

Task 4 - Planificare cu prioritati

Acest task se aseamana cu task-ul 3, diferenta fiind ca din ready nu mai
selectez primul proces, ci procesul cu prioritatea maxima. In acest caz,
am folosit o functie auxiliara (search_maximum), ce returneaza procesul
cu prioritatea maxima, pe care il bag in running si ulterior stergand
procesul respectiv din ready. Ca la RR, exista un contor pentru cuanta, 
pentru a vedea cat sta in running, iar cand contorul atinge cuanta de 
timp prestabilita, procesul din running ajunge in ready si contorul
este reinitializat cu 0.

Task 5 - Planificare cu prioritati - bonus

Acest task porneste de la task-ul 4, functionand in esenta pe baza aceluiasi
principiu, insa diferenta este ca daca un proces trece de doua ori din running
in ready prioritatea acestuia scade, iar daca aceste trece de doua ori din running
in waiting prioritatea acestuia creste. In acest caz, folosesc doua cozi (exit_ready
si exit_waiting, in care copiez procesele ce au trecut din running in ready,
respectiv din running in waiting) si o functie ce numara aparitiile unui proces
in functie de numele acestuia, pentru a vedea de cate ori apare in cele doua cozi.
Daca apare de doua ori in fiecare coada, prioritatea acestuia creste / scade (depinde
de caz) si se elimina aparitiile acestuia in coada respectiva, pentru a reevalua
de cate ori a trecut din running in ready / waiting.