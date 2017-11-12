#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
	int runtime; // durata procesului
	int priority; // prioritatea procesului
	char process_name[22]; // numele procesului
	struct node * next; // pointerul la urmatorul nod din lista
} Node;

Node *queue_quantum (Node *list, char *data, int run_time, int proc_priority) // functia de adaugare in coada
{
	Node *new, *aux;
	new = (Node*)malloc(sizeof(Node));
	new->runtime = run_time; // durata procesului
	new->priority = proc_priority; // prioritatea procesului
	sprintf(new->process_name, "%s", data); // numele procesului
	new->next = NULL; // elementul ce va fi adaugat la coada va avea legatura la NULL, pentru a incheia coada
	if (list == NULL) // daca coada e NULL, se initializeaza coada
		return new;
	aux = list; // altfel se adauga elemente la coada
	while(aux->next != NULL)
		aux = aux->next; // ne ducem la ultimul nod din coada
	aux->next = new; // legam ultimul nod din coada la ceea ce adaugam la coada
	return list;
}

Node *dequeue (Node *list) // functia de stergere a primului nod din lista
{
	Node *new;
	new = list; // nodul pe care il stergem
	if (list == NULL) // daca nodul e NULL, e returnat NULL
		return NULL;
	list = new->next; // legarea primului nod de al doilea nod
	free(new); // stergerea nodului
	return list;
}

void print_process (Node *list, FILE * file) // afisarea in fisier al procesului din running
{
	if (list == NULL) {
		fprintf(file, "\n"); // daca running e gol, se afiseaza linie goala
		return;
	}
	fprintf(file, "%s %d\n", list->process_name, list->runtime); // afisam fara while caci in running e un singur proces
}

int number_of_words (char *buffer) // numarul de cuvinte dintr-un sir de caractere
{
	int n = 1, k = strlen(buffer), i;
	for (i = 0; i < k; i++)
	{
		if (buffer[i] == ' ') // enumeram numarul de cuvinte (minim 1) dintr-un sir folosindu-ne de numarul de spatii
			n++;
	}
	return n; // e returnat numarul de cuvinte din sir
}

void no_strcpy (char *buffer, int k)
{
	int n = strlen(buffer);
	int i;
	for (i = 0; i < n - k; i++)
		buffer[i] = buffer[i + k];
	buffer[strlen(buffer) - k + 1] = '\0';
}

void cut_first_n_words(char *buffer, int n) // taiem primele n cuvinte dintr-un sir de caractere
{
	char *copy = malloc(110 * sizeof(char));
	sprintf(copy,"%s",buffer); // copia buffer-ului ce va fi folosita pentru spargerea cu strtok
	char *token;
	int i = 0, k = 0; // i e numarul de cuvinte, iar k e numarul de caractere din primele n cuvinte + spatii
	token = strtok(copy, " "); // spargem pe cuvinte
	while ((i < n) && (token != NULL)) // continuarea spargerii pe cuvinte
	{		
		i++; // cresterea numarului de cuvinte
		k = k + 1 + strlen(token); // numarul de caractere
		token = strtok(NULL, " ");
	}
	no_strcpy(buffer, k); // taiem primele k caractere din buffer (adica primele n cuvinte)
	free(copy); // eliberam memoria
}

int number_of_elements (Node *list) //how to fix a fucking bug :X aka numarul de elemente dintr-o lista
{
	int n = 0; // numarul de noduri
	while (list != NULL)
	{
		n++; // incrementarea valorii
		list = list->next; // trecerea la urmatorul nod
	}
	return n;
}

Node *tick (Node *list) // comanda tick
{
	if (list == NULL)
		return list;
	list->runtime--; // scaderea duratei de viata
	if (list->runtime == 0)
		list = dequeue(list); // daca durata e 0, se goleste lista
	return list;
}

Node *remove_last (Node *list) // se sterge ultimul element din coada (pentru bug-ul de la multiple add)
{
	Node *aux, *temp;
	aux = list;
	while (aux->next->next != NULL) // avansam pana la penultimul element
		aux = aux->next; 
	temp = aux->next; // ultimul element
	aux->next = NULL; // legarea penultimului element la NULL
	free(temp); // stergerea ultimului element
	return list;
}

Node *search (Node *list, char *buffer) // cautarea unui nod in functie de nume
{
	Node *new = NULL; // nodul cu numele cautat - daca nu se gaseste, nodul ramane NULL
	while (list != NULL) // parcurgerea listei
	{
		if (strcmp(list->process_name, buffer) == 0) // daca e gasit numele dat in lista
		{
			new = queue_quantum(new, list->process_name, list->runtime, list->priority); // nodul cu numele pe care l-am cautat in lista
			return new;
		}
		else
			list = list->next; // daca nu s-a gasit, se cauta in continuare in lista
	}
	return new;
}

int minimum_value (Node *list) // timpul de viata cel mai mic al unui proces
{
	if (list == NULL)
		return -1;
	int min = list->runtime; // initializarea minimului
	Node *aux = list;
	while (aux != NULL)
	{
		if (aux->runtime <= min) // cautarea valorii minime de timp de viata al unui proces din lista
			min = aux->runtime; // minimul cautat
		aux = aux->next; // cautarea in continuare a valorii minime de timp de viata
	}
	return min;
}

Node *remove_element_runtime (Node *list) // stergerea nodului cu durata de viata cea mai mica
{
	int min = minimum_value(list);
	if (list == NULL)
		return NULL;
	Node *aux;
	if (list->runtime == min) // daca nodul cautat e capul listei
		{
			aux = list;
			list = list->next; // avansam la al doilea element al listei
			free(aux); // stergerea capului listei
			return list;
		}
	Node *temp = list;
	while (temp->next != NULL)
	{
		if (temp->next->runtime == min) // daca nodul (care nu e cap) are durata de viata cea mai mica
		{
			aux = temp->next; // nodul ce va fi sters
			temp->next = temp->next->next; // nodul precedent celui ce va fi sters va fi legat cu cel succesor acestuia
			free(aux); // stergerea nodului cautat
			return list;
		}
		temp = temp->next; // avansarea in lista
	}
	return list;
}

int maximum_value (Node *list) // prioritatea maxima din lista
{
	if (list == NULL)
		return -1;
	int max = list->priority; // initializarea maximului
	Node *aux = list;
	while (aux != NULL) // parcurgerea cozii pentru aflarea maximului
	{
		if (aux->priority > max) // determinarea maximului
			max = aux->priority; // prioritatea max
		aux = aux->next; // parcurgerea in continuare a cozii
	}
	return max;
}

Node *remove_element_priority(Node *list) // stergerea primului nod cu prioritatea maxima 
{
	int max = maximum_value(list); // prioritatea maxima din coada
	if (list == NULL)
		return NULL;
	Node *aux;
	if (list->priority == max) // daca capul are prioritatea maxima
		{
			aux = list;
			list = list->next; // trecem la al doilea element
			free(aux); // stergem capul listei
			return list;
		}
	Node *temp = list;
	while (temp->next != NULL)
	{
		if (temp->next->priority == max) // daca nodul din lista (numai capul nu) are prioritatea maxima
		{
			aux = temp->next; // nodul ce se va sterge
			temp->next = temp->next->next; // legarea nodului precedent celui sters cu cel succesor celui sters
			free(aux); // se sterge nodul
			return list;
		}
		temp = temp->next; // cautarea in coada a nodului cu prioritatea maxima
	}
	return list;
}

Node *remove_element (Node *list, char *buffer) // stergerea primului nod cu numele dat ca argument al functiei (buffer)
{
	if (list == NULL)
		return NULL;
	Node *aux;
	if (strcmp(list->process_name, buffer) == 0) // daca capul listei are numele cautat
		{
			aux = list;
			list = list->next; // se trece la urmatorul nod (care devine capul listei)
			free(aux); // stergerea nodului cu numele cautat
			return list;
		}
	Node *temp = list;
	while (temp->next != NULL) // daca nodul (care nu e cap) are numele cautat
	{
		if (strcmp(temp->next->process_name, buffer) == 0) // se cauta in lista
		{
			aux = temp->next; // nodul ce va fi sters
			temp->next = temp->next->next; // legarea nodului precedent cu cel succesor
			free(aux); // stergerea nodului cautat
			return list;
		}
		temp = temp->next; // cautarea nodului cu numele dat in lista in continuare daca nu s-a gasit
	}
	return list;
}

Node *search_minimum (Node *list) // cautarea nodului cu durata minima de viata
{
	int min = minimum_value(list); // durata minima de viata
	Node *new = NULL;
	while (list != NULL)
	{
		if (list->runtime == min) // daca e gasit nodul cu durata minima de viata
			{
				new = queue_quantum(new, list->process_name, list->runtime, list->priority); // nodul cu durata minima de viata
				return new;
			}
		else
			list = list->next; // avansarea in lista
	}
	return new;
}

Node *search_maximum (Node *list) // cautarea nodului cu priotatea maxima 
{
	int max = maximum_value(list); // prioritatea maxima
	Node *new = NULL;
	while (list != NULL)
	{
		if (list->priority == max) // daca e gasit nodul cu prioritatea maxima
			{
				new = queue_quantum(new, list->process_name, list->runtime, list->priority); // nodul cu prioritatea maxima
				return new;
			}
		else
			list = list->next; // avansarea in lista
	}
	return new;
}

void FCFS (FILE *input, FILE *output) // algoritmul First Come First Served
{
	Node * ready = NULL; // coada pentru starea Ready
	Node * running = NULL; // coada pentru starea Running
	Node * waiting = NULL; // coada pentru starea Waiting
	char *buffer = (char *)malloc(101*sizeof(char));
	while (fgets(buffer, 100, input) != NULL)
	{
		if (buffer[0] == 'a') // comanda add
		{	
			int runtime, priority; // durata de viata si prioritatea
			char name[21]; // numele procesului
			sscanf(buffer, "%*s %s %d %d", name, &runtime, &priority); // scoaterea celor de mai de sus din linia citita la add
			ready = queue_quantum(ready, name, runtime, priority); // adaugarea procesului in ready
			if (running == NULL) // daca running e nul, automat primul proces din coada ready e bagat in running
			{
				running = queue_quantum(running, ready->process_name, ready->runtime, ready->priority); // adaugarea din ready in running
				ready = dequeue(ready); // scoatem procesul care a ajuns in running din ready
			}
			else
				running = tick(running); // altfel running face tick
		}
		else if (buffer[0] == 's') // comanda show
			print_process(running, output); // afisarea procesului din running
		else if (buffer[0] == 't') // comanda tick
			{
				running = tick(running); // tick
				if (running == NULL) // daca running e gol, se ia primul proces din ready (asta daca ready nu e gol)
				{
					if (ready == NULL) // daca ready e gol, sarim peste tick
						continue;
					else // altfel daca nu e gol ready are loc tick
					{
						running = queue_quantum(running, ready->process_name, ready->runtime, ready->priority); // adaugarea din
						// ready in running
						ready = dequeue(ready); // stergerea din ready 
					}
				}
			}
		else if (buffer[0] == 'w') // comanda wait
		{
			if (running != NULL) // daca running e gol, se adauga in waiting
			{	
				waiting = queue_quantum(waiting, running->process_name, running->runtime, running->priority);
				// adaugarea din waiting in running
				running = dequeue(running);
				// stergerea procesului din running
			}
			if (ready != NULL) // daca ready nu e NULL, se ia primul proces din ready si se adauga in running
			{
				running = queue_quantum(running, ready->process_name, ready->runtime, ready->priority);
				// adaugarea din ready in running
				ready = dequeue(ready);
				// stergerea procesului adaugat din ready in running 
			}
			else continue;
		}
		else if (buffer[0] == 'e') // comanda event
		{
			char name[21];
			sscanf(buffer, "%*s %s", name); // luam numele procesului din linia citita
			if (waiting != NULL)
			{
				Node *list = search(waiting, name); // cautam numele procesului in lista de waiting - lista de tranzitie
				ready = queue_quantum(ready, list->process_name, list->runtime, list->priority); 
				// adaugam procesul din waiting in ready
				waiting = remove_element(waiting, name); // stergem din waiting procesul adaugat in ready
				list = dequeue(list); // golim lista noastra de tranzitie in care salvam nodul cautat din waiting
			}
			if (ready != NULL)
				running = tick(running);
			if (running == NULL) //tick
			{
				running = queue_quantum(running, ready->process_name, ready->runtime, ready->priority);
				// se adauga primul proces din ready in running
				ready = dequeue(ready); // stergem procesul din ready adaugat in running
			}
		}
		else if ((buffer[0] == 'm') && (buffer[1] == 'a'))
		{
			buffer = buffer + 3; // stergerea primelor 3 caractere - "ma "
			int runtime, priority; 
			char name[21];
			int k = number_of_words(buffer); // numarul de cuvinte din linia citita
			int i = 0; // contor de cuvinte
			while (i < k)
			{
				i++;
				sscanf(buffer, "%s %d %d", name, &runtime, &priority); // parsarea procesului din linia citita
				ready = queue_quantum(ready, name, runtime, priority); // adaugarea in coada ready
				cut_first_n_words(buffer, 3); // avansarea in linia citita prin 'stergerea' procesului parsat in ready
			}

			k = k / 3; // cum sa rezolvi un bug: in coada ready ar fi trebuit sa am p elemente, insa lista are 3 * p 
			// elemente, ultimele 2 * p fiind puse aiurea, asa ca stergem ultimele 2 * p elemente din lista
			// k e numarul de elemente care ar trebui sa fie in ready
			int n = number_of_elements(ready); // numarul de elemente care se afla de fapt in ready inainte de 
			// "manarirea"  bug-ului
			if (n != k) // how to fuck a bug in the ass
			{
				int j = 0;
				k = k * 2;
				while (j < k)
				{
					ready = remove_last(ready); // stergerea elementelor redundante
					j++;
				}
			}
			if (running == NULL) // tick
			{
				running = queue_quantum(running, ready->process_name, ready->runtime, ready->priority); 
				// se adauga primul proces din ready in running
				ready = dequeue(ready); // stergem procesul din ready adaugat in running
			}
			else
				running = tick(running); // tick
		}
		else continue;
}}

void SJF (FILE *input, FILE *output) // Shortest Job First
{
	Node * ready = NULL;
	Node * running = NULL;
	Node * waiting = NULL;
	char *buffer = (char *)malloc(101*sizeof(char));
	while (fgets(buffer, 100, input))
	{
		if (buffer[0] == 'a')
		{	
			int runtime, priority; 
			char name[21];
			sscanf(buffer, "%*s %s %d %d", name, &runtime, &priority);
			ready = queue_quantum(ready, name, runtime, priority); 
			if (running == NULL)
			{
				Node *list = search_minimum(ready); // nodul cu durata de viata cea mai mica din ready
				running = queue_quantum(running, list->process_name, list->runtime, list->priority);
				// adaugarea procesului cu durata minima din ready in running
				ready = remove_element_runtime(ready); 
				// stergerea procesului cu durata minima din ready
				list = dequeue(list);
				// golim lista de tranzitie folosita mai sus
			}
			else
				running = tick(running); // tick
			if (running == NULL) // daca running e null (dupa tick), se adauga procesul cu durata minima 
			// de viata din ready in running
			{
				Node *list = search_minimum(ready); // nodul cu durata de viata cea mai mica din ready
				running = queue_quantum(running, list->process_name, list->runtime, list->priority);
				// adaugarea procesului cu durata minima din ready in running
				ready = remove_element_runtime(ready); 
				// stergerea procesului cu durata minima din ready
				list = dequeue(list);
				// golim lista de tranzitie folosita mai sus
			}
		}
		else if (buffer[0] == 's') // show
			print_process(running, output); // afisarea in fisier al procesului din running
		else if (buffer[0] == 'w') // wait
		{
			if (running != NULL)
			{	
				waiting = queue_quantum(waiting, running->process_name, running->runtime, running->priority);
				running = dequeue(running);
			}
			if (ready != NULL)
			{
				Node *list = search_minimum(ready);
				running = queue_quantum(running, list->process_name, list->runtime, list->priority);
				ready = remove_element_runtime(ready); 
				list = dequeue(list);
			}
			else continue;
		}
		else if (buffer[0] == 't')
		{
			running = tick(running);
			if (running == NULL)
			{
				if (ready == NULL)
					continue;
				else
				{
					Node *list = search_minimum(ready);
					running = queue_quantum(running, list->process_name, list->runtime, list->priority);
					ready = remove_element_runtime(ready); 
					list = dequeue(list);
				}
			}
		}
		else if (buffer[0] == 'e')
		{
			char name[21];
			sscanf(buffer, "%*s %s", name);
			if (waiting != NULL)
			{
				Node *list = search(waiting, name);
				ready = queue_quantum(ready, list->process_name, list->runtime, list->priority);
				waiting = remove_element(waiting, name);
				list = dequeue(list);
			}
			if (ready != NULL)
				running = tick(running);
			if (running == NULL)
			{
				Node *list = search_minimum(ready);
				running = queue_quantum(running, list->process_name, list->runtime, list->priority);
				ready = remove_element_runtime(ready); 
				list = dequeue(list);
			} 
		}
		else if ((buffer[0] == 'm') && (buffer[1] == 'a'))
		{
			buffer = buffer + 3;
			int runtime, priority; 
			char name[21];
			int k = number_of_words(buffer);
			int i = 0;
			while (i < k)
			{
				i++;
				sscanf(buffer, "%s %d %d", name, &runtime, &priority);
				ready = queue_quantum(ready, name, runtime, priority);
				cut_first_n_words(buffer, 3);
			} 
			k = k / 3;
			int n = number_of_elements(ready);
			if (n != k) // how to fuck a bug in the ass
			{
				int j = 0;
				k = k * 2;
				while (j < k)
				{
					ready = remove_last(ready);
					j++;
				}
			}
			if (running == NULL)
			{
				Node *list = search_minimum(ready);
				running = queue_quantum(running, list->process_name, list->runtime, list->priority);
				ready = remove_element_runtime(ready); 
				list = dequeue(list);
			}
			else
				running = tick(running);
		}
		else continue;
	}
}

void RR (FILE *input, FILE *output, int quantum) // Round Robin
{	
	Node * ready = NULL;
	Node * running = NULL;
	Node * waiting = NULL;
	char *buffer = (char *)malloc(105 * sizeof(char));
	int count = 0; // contor de cuanta
	sscanf(buffer, "%*d %d", &quantum); // luam cuanta de pe prima linie a fisierului
	while (fgets(buffer, 100, input) != NULL)
	{
		if (buffer[0] == 'a')
		{	
			int runtime, priority; 
			char name[21];
			sscanf(buffer, "%*s %s %d %d", name, &runtime, &priority);
			ready = queue_quantum(ready, name, runtime, priority); 
			if (running == NULL)
			{
				running = queue_quantum(running, ready->process_name, ready->runtime, ready->priority); 
				// luam primul proces din ready si il bagam in running
				ready = dequeue(ready);
				// eliminam procesul din ready bagat in running
				count = 0;
				// contorul e initializat
			}
			else
			{
				running = tick(running); // tick
				count++; // incrementarea contorului de cuanta
			}
			if (count == quantum) // daca contorul a atins cuanta
			{
				ready = queue_quantum(ready, running->process_name, running->runtime, running->priority); 
				// se ia adauga in ready procesul din running
				running = dequeue(running);
				// golim running-ul
				running = queue_quantum(running, ready->process_name, ready->runtime, ready->priority); 
				// adaugam primul proces din ready in running
				ready = dequeue(ready);
				// eliminam procesul rin ready adaugat in running
				count = 0;
				// reinitializam contorul de cuanta
			}
		}
		else if (buffer[0] == 's')
			print_process(running, output);
		else if (buffer[0] == 't')
			{
				
				running = tick(running); // tick
				count++; // la fiecare tick creste cuanta
				if ((count == quantum) && (running != NULL))
				{
					ready = queue_quantum(ready, running->process_name, running->runtime, running->priority);
					running = dequeue(running);
					running = queue_quantum(running, ready->process_name, ready->runtime, ready->priority); 
					ready = dequeue(ready);
					count = 0;
				}
				if (running == NULL)
				{
					if (ready == NULL)
						continue;
					else
					{
						running = queue_quantum(running, ready->process_name, ready->runtime, ready->priority);
						ready = dequeue(ready);
						count = 0;
					}
				}
			}
		else if (buffer[0] == 'w')
		{
			if (running != NULL)
			{	
				waiting = queue_quantum(waiting, running->process_name, running->runtime, running->priority);
				running = dequeue(running);
			}
			if (ready != NULL)
			{
				running = queue_quantum(running, ready->process_name, ready->runtime, ready->priority);
				ready = dequeue(ready);
				count = 0;
			}
			else continue;
		}
		else if (buffer[0] == 'e')
		{
			char name[21];
			sscanf(buffer, "%*s %s", name);
			if (waiting != NULL)
			{
				Node *list = search(waiting, name);
				ready = queue_quantum(ready, list->process_name, list->runtime, list->priority);
				waiting = remove_element(waiting, name);
				list = dequeue(list);
			}
			if (ready != NULL)
				{
					running = tick(running);
					count++;
				}
			if (count == quantum)
			{
				ready = queue_quantum(ready, running->process_name, running->runtime, running->priority);
				running = dequeue(running);
				running = queue_quantum(running, ready->process_name, ready->runtime, ready->priority); 
				ready = dequeue(ready);
				count = 0;
			}
			if (running == NULL)
			{
				running = queue_quantum(running, ready->process_name, ready->runtime, ready->priority);
				ready = dequeue(ready);
				count = 0;
			}
		}
		else if ((buffer[0] == 'm') && (buffer[1] == 'a'))
		{
			buffer = buffer + 3;
			int runtime, priority; 
			char name[21];
			int k = number_of_words(buffer);
			int i = 0;
			while (i < k)
			{
				i++;
				sscanf(buffer, "%s %d %d", name, &runtime, &priority);
				ready = queue_quantum(ready, name, runtime, priority);
				cut_first_n_words(buffer, 3);
			} 
			k = k / 3;
			int n = number_of_elements(ready);
			if (n != k) // how to fuck a bug in the ass
			{
				int j = 0;
				k = k * 2;
				while (j < k)
				{
					ready = remove_last(ready);
					j++;
				}
			}
			if (running == NULL)
			{
				running = queue_quantum(running, ready->process_name, ready->runtime, ready->priority); 
				ready = dequeue(ready);
				count = 0;
			}

			else
			{
				running = tick(running);
				count++;
			}
			if (running == NULL)
			{
				running = queue_quantum(running, ready->process_name, ready->runtime, ready->priority); 
				ready = dequeue(ready);
				count = 0;
			}
			if ((count == quantum) && (running != NULL))
			{
				ready = queue_quantum(ready, running->process_name, running->runtime, running->priority);
				running = dequeue(running);
				running = queue_quantum(running, ready->process_name, ready->runtime, ready->priority); 
				ready = dequeue(ready);
				count = 0;
			}
		}
		else continue;

}}

void PP (FILE *input, FILE *output, int quantum) // Planificarea cu prioritati
{	
	Node * ready = NULL;
	Node * running = NULL;
	Node * waiting = NULL;
	char *buffer = (char *) malloc (101 * sizeof(char));
	int count = 0; // contorul de cuanta
	while (fgets(buffer, 100, input) != NULL)
	{
		if (buffer[0] == 'a')
		{	
			int runtime, priority; 
			char name[21];
			sscanf(buffer, "%*s %s %d %d", name, &runtime, &priority);
			ready = queue_quantum(ready, name, runtime, priority); 
			if (running == NULL)
			{
				running = search_maximum(ready); // se cauta elementul cu prioritatea maxima
				// si se baga in running
				ready = remove_element_priority(ready);
				// se sterge din ready procesul cu prioritate maxima
				count = 0; // initializarea contorului de cuanta
			}
			else
			{
				running = tick(running); // tick
				count++; // creste cuanta
			}
			if (count == quantum) // daca contorul atinge cuanta procesul din running se duce in ready
				// si in running vine alt proces cu prioritate maxima
			{
				ready = queue_quantum(ready, running->process_name, running->runtime, running->priority); 
				// bagam in ready procesul din running
				running = dequeue(running);
				// se goleste running-ul
				running = search_maximum(ready);
				// in running va ajunge alt proces cu prioritate maxima 
				ready = remove_element_priority(ready);
				// din ready se sterge procesul cu prioritatea maxima
				count = 0; // reinitializarea contorului
			}
		}
		else if (buffer[0] == 's')
			print_process(running, output);
		else if (buffer[0] == 't')
		{
				running = tick(running); // tick
				count++; // crestere contorul 
				if ((count == quantum) && (running != NULL)) // daca contorul a atins cuanta si daca running nu e gol
				{
					ready = queue_quantum(ready, running->process_name, running->runtime, running->priority);
					running = dequeue(running);
					running = search_maximum(ready);
					ready = remove_element_priority(ready);
					count = 0;
				}
				if (running == NULL)
				{
					if (ready == NULL)
						continue;
					else
					{
						running = search_maximum(ready);
						ready = remove_element_priority(ready);
						count = 0;
					}
				}
		}
		else if (buffer[0] == 'w') // wait
		{
			if (running != NULL)
			{	
				waiting = queue_quantum(waiting, running->process_name, running->runtime, running->priority);
				running = dequeue(running);
			}
			if (ready != NULL)
			{
				running = search_maximum(ready);
				ready = remove_element_priority(ready);
				count = 0;
			}
			else continue;
		}
		else if (buffer[0] == 'e')
		{
			char name[21];
			sscanf(buffer, "%*s %s", name);
			if (waiting != NULL)
			{
				Node *list = search(waiting, name);
				ready = queue_quantum(ready, list->process_name, list->runtime, list->priority);
				waiting = remove_element(waiting, name);
				list = dequeue(list);
			}
			if (ready != NULL)
			{
				running = tick(running);
				count++;
			}
			if (maximum_value(ready) > maximum_value(running)) // daca prioritatea maxima din ready e mai
			// mare decat cea din running, atunci in running va ajunge procesul din ready cu prioritatea
			// maxima si procesul aflat inainte in running va ajunge in ready
			{
				ready = queue_quantum(ready, running->process_name, running->runtime, running->priority);
				running = dequeue(running);
				running = search_maximum(ready);
				ready = remove_element_priority(ready);
				count = 0;
			}
			if (count == quantum)
			{
				ready = queue_quantum(ready, running->process_name, running->runtime, running->priority);
				running = dequeue(running);
				running = search_maximum(ready);
				ready = remove_element_priority(ready);
				count = 0;
			}
			if (running == NULL)
			{
				running = search_maximum(ready);
				ready = remove_element_priority(ready);
				count = 0;
			}
		}
		else if ((buffer[0] == 'm') && (buffer[1] == 'a'))
		{
			buffer = buffer + 3;
			int runtime, priority; 
			char name[21];
			int k = number_of_words(buffer);
			int i = 0;
			while (i < k)
			{
				i++;
				sscanf(buffer, "%s %d %d", name, &runtime, &priority);
				ready = queue_quantum(ready, name, runtime, priority);
				cut_first_n_words(buffer, 3);
			} 
			k = k / 3;
			int n = number_of_elements(ready);
			if (n != k) // how to fuck a bug in the ass
			{
				int j = 0;
				k = k * 2;
				while (j < k)
				{
					ready = remove_last(ready);
					j++;
				}
			}
			if (running == NULL)
			{
				running = search_maximum(ready);
				ready = remove_element_priority(ready);
				count = 0;
			}
			else
			{
				running = tick(running);
				count++;
			}
			if (running == NULL)
			{
				running = search_maximum(ready);
				ready = remove_element_priority(ready);
				count = 0;
			}
			if ((count == quantum) && (running != NULL))
			{
				ready = queue_quantum(ready, running->process_name, running->runtime, running->priority);
				running = dequeue(running);
				running = search_maximum(ready);
				ready = remove_element_priority(ready);
				count = 0;
			}
		}
		else continue;
}}

int apparitions_list (Node *list, char *buffer) // numarul de aparitii al unui string in lista
{
	if (list == NULL)
		return 0;
	int nr = 0; // numarul de aparitii
	while (list != NULL)
	{
		if (strcmp(list->process_name, buffer) == 0) // daca apare
			nr++; // se incrementeaza
		list = list->next; // cautarea in continuare in lista
	}
	return nr;
}

void Bonus (FILE *input, FILE *output, int quantum) // Planificarea cu prioritati - bonus
{	
	Node * ready = NULL;
	Node * running = NULL;
	Node * waiting = NULL;
	Node * exit_waiting = NULL; // coada proceselor ce intra din running in waiting
	Node * exit_ready = NULL; // coada proceselor ce intra din running in ready
	// ambele cozi sunt folosite pentru a vedea de cate ori intra in ready sau waiting
	char *buffer = (char *) malloc (101 * sizeof(char));
	int count = 0; // contorul de cuanta
	while (fgets(buffer, 100, input) != NULL)
	{
		if (buffer[0] == 'a')
		{	
			int runtime, priority; 
			char name[21];
			sscanf(buffer, "%*s %s %d %d", name, &runtime, &priority);
			ready = queue_quantum(ready, name, runtime, priority); 
			if (running == NULL)
			{
				running = search_maximum(ready);
				ready = remove_element_priority(ready);
				count = 0;
			}
			else
			{
				running = tick(running); // tick
				count++; // creste contorul de cuanta
			}
			if (count == quantum)
			{
				exit_ready = queue_quantum(exit_ready, running->process_name, running->runtime, running->priority);
				// daca contorul atinge cuanta se va tine cont, astfel in exit_ready se va copia procesul din running
				// iar cand se va verifica daca procesul respectiv apare de 2 ori in exit_ready (se verifica dupa
				// nume), aparitiile acestuia in exit_ready vor fi eliminate, iar daca ajunge iar in ready se va
				// repeta procesul
				int n = apparitions_list(exit_ready, running->process_name); // de cate ori apare procesul din running
				// in exit_ready
				if (n == 2) // daca apare de 2 ori
				{
					running->priority--; // scade prioritatea acestuia
					int p = 0;
					while (p < n) // se vor elimina aparitiile acestuia din exit_ready
						{
							exit_ready = remove_element(exit_ready, running->process_name);
							// stergerea aparitiilor procesului din running in exit_ready
							p++;
						}
				}
				ready = queue_quantum(ready, running->process_name, running->runtime, running->priority); 
				// adaugarea procesului din running in ready
				running = dequeue(running);
				// golirea running-ului
				running = search_maximum(ready);
				// in running trece procesul cu prioritatea maxima 
				ready = remove_element_priority(ready);
				// in ready se sterge procesul ce a trecut in running
				count = 0; // reinitializarea contorului
			}
		}
		else if (buffer[0] == 's')
			print_process(running, output);
		else if (buffer[0] == 't')
		{
				running = tick(running); // tick
				count++;
				if ((count == quantum) && (running != NULL)) // se repeta procedeul de mai sus
				{
					exit_ready = queue_quantum(exit_ready, running->process_name, running->runtime, running->priority);
					int n = apparitions_list(exit_ready, running->process_name);
					if (n == 2) 
					{
						running->priority--;
						int p = 0;
						while (p < n)
						{
							exit_ready = remove_element(exit_ready, running->process_name);
							p++;
						}
					}
					ready = queue_quantum(ready, running->process_name, running->runtime, running->priority); 
					running = dequeue(running);
					running = search_maximum(ready);
					ready = remove_element_priority(ready);
					count = 0;
				}
				if (running == NULL)
				{
					if (ready == NULL)
						continue;
					else
					{
						running = search_maximum(ready);
						ready = remove_element_priority(ready);
						count = 0;
					}
				}
		}
		else if (buffer[0] == 'w') // wait
		{
			if (running != NULL) // daca running-ul nu e gol, procesul din running trece in waiting
			{	
				exit_waiting = queue_quantum(exit_waiting, running->process_name, running->runtime, running->priority);
				// daca contorul atinge cuanta se va tine cont, astfel in exit_waiting se va copia procesul din running
				// iar cand se va verifica daca procesul respectiv apare de 2 ori in exit_waiting (se verifica dupa
				// nume), aparitiile acestuia in exit_waiting vor fi eliminate, iar daca ajunge iar in waiting se va
				// repeta procesul
				int n = apparitions_list(exit_waiting, running->process_name); // de cate ori apare procesul din running
				// in exit_waiting
				if (n == 2)
				{
					running->priority++; // creste prioritatea procesului
					int p = 0;
					while (p < n) // se vor elimina aparitiile acestuia din exit_ready
					{
						exit_waiting = remove_element(exit_waiting, running->process_name);
						// stergerea aparitiilor procesului din running in exit_waiting
						p++;
					}
				}
				waiting = queue_quantum(waiting, running->process_name, running->runtime, running->priority); 
				// se adauga procesul din running in waiting
				running = dequeue(running);
				// se sterge procesul din running
			}
			if (ready != NULL)
			{
				running = search_maximum(ready);
				ready = remove_element_priority(ready);
				count = 0;
			}
			else continue;
		}
		else if (buffer[0] == 'e')
		{
			char name[21];
			sscanf(buffer, "%*s %s", name);
			if (waiting != NULL)
			{
				Node *list = search(waiting, name);
				ready = queue_quantum(ready, list->process_name, list->runtime, list->priority);
				waiting = remove_element(waiting, name);
				list = dequeue(list);
			}
			if (ready != NULL)
			{
				running = tick(running);
				count++;
			}
			if (maximum_value(ready) > maximum_value(running)) // daca prioritatea maxima din ready e mai mare
			// decat prioritatea maxima din running, atunci in running va ajunge procesul din ready cu prioritatea
			// cea mai mare
			{
				exit_ready = queue_quantum(exit_ready, running->process_name, running->runtime, running->priority);
				// se repeta procedeul cu exit_ready de mai sus
				int n = apparitions_list(exit_ready, running->process_name);
				if (n == 2)
				{
					running->priority--; // scaderea prioritatii

					int p = 0;
					while (p < n)
					{
						exit_ready = remove_element(exit_ready, running->process_name);
						p++;
					}
				}
				ready = queue_quantum(ready, running->process_name, running->runtime, running->priority); 
				running = dequeue(running);
				running = search_maximum(ready);
				ready = remove_element_priority(ready);
				count = 0;
			}
			if (count == quantum) // daca contorul a atins cuanta se repeta procedeul de mai sus cu exit_ready
			{
				exit_ready = queue_quantum(exit_ready, running->process_name, running->runtime, running->priority);
				int n = apparitions_list(exit_ready, running->process_name);
				if (n == 2)
				{
					running->priority--;
					int p = 0;
					while (p < n)
					{
						exit_ready = remove_element(exit_ready, running->process_name);
						p++;
					}
				}
				ready = queue_quantum(ready, running->process_name, running->runtime, running->priority); 
				running = dequeue(running);
				running = search_maximum(ready);
				ready = remove_element_priority(ready);
				count = 0;
			}
			if (running == NULL)
			{
				running = search_maximum(ready);
				ready = remove_element_priority(ready);
				count = 0;
			}
		}
		else if ((buffer[0] == 'm') && (buffer[1] == 'a'))
		{
			buffer = buffer + 3;
			int runtime, priority; 
			char name[21];
			int k = number_of_words(buffer);
			int i = 0;
			while (i < k)
			{
				i++;
				sscanf(buffer, "%s %d %d", name, &runtime, &priority);
				ready = queue_quantum(ready, name, runtime, priority);
				cut_first_n_words(buffer, 3);
			} 
			k = k / 3;
			int n = number_of_elements(ready);
			if (n != k) // how to fuck a bug in the ass
			{
				int j = 0;
				k = k * 2;
				while (j < k)
				{
					ready = remove_last(ready);
					j++;
				}
			}
			if (running == NULL)
			{
				running = search_maximum(ready);
				ready = remove_element_priority(ready);
				count = 0;
			}
			else
			{
				running = tick(running);
				count++;
			}
			if (running == NULL)
			{
				running = search_maximum(ready);
				ready = remove_element_priority(ready);
				count = 0;
			}
			if ((count == quantum) && (running != NULL))
			{
				exit_ready = queue_quantum(exit_ready, running->process_name, running->runtime, running->priority);
				int n = apparitions_list(exit_ready, running->process_name); // de cate ori apare numele procesului din running
				if (n == 2)
				{
					running->priority--; // scaderea prioritatii
					int p = 0;
					while (p < n)
					{
						exit_ready = remove_element(exit_ready, running->process_name);
						p++;
					}
				}
				ready = queue_quantum(ready, running->process_name, running->runtime, running->priority); 
				running = dequeue(running);
				running = search_maximum(ready);
				ready = remove_element_priority(ready);
				count = 0;
			}
		}
		else continue;
}}

int main (int argc, char **argv)
{	
	FILE *process_in = fopen(argv[1], "r"); // fisierul de intrare
	FILE *process_out = fopen(argv[2], "w+"); // fisierul de iesire
	char buffer[105]; // liniile din fisierul de intrare
	int quantum; // cuanta de timp
	fgets(buffer, 100, process_in); // citirea primei linii din fisierul de input, pentru a vedea ce algoritm aplicam
	switch(buffer[0]) // vedem dupa primul caracter din prima linie din fisier ce algoritm se aplica
	{
		case '1':
			FCFS(process_in, process_out);
			break;
		case '2':
			SJF(process_in, process_out);
			break;
		case '3':
			sscanf(buffer, "%*d %d %*d", &quantum); // selectam cuanta de timp
			RR(process_in, process_out, quantum);
			break;
		case '4':
			sscanf(buffer, "%*d %d %*d", &quantum); // selectam cuanta de timp
			PP(process_in, process_out, quantum);
			break; 
		case '5': 
			sscanf(buffer, "%*d %d %*d", &quantum); // selectam cuanta de timp
			Bonus(process_in, process_out, quantum);
			break;
		default:
			break;
	}
	fclose(process_in);
	fclose(process_out);
	return 0;
}