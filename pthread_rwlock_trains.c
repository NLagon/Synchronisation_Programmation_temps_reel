#include <stdio.h>   // printf
#include <pthread.h> // pthread
#include <unistd.h>  // sleep
#include <stdlib.h>  // srand
#include <time.h>    // time

pthread_rwlock_t lock; //déclaration du verrou

char *train1="ABCBA"; // tableau représenatant le trajet de chaque train
char *train2="ABDCBA";
char *train3="ABDCEA";

void train1_deplacement(int id){ // fonction représentant le déplacement du train 1 avec une zone critique permettant l'accès 

	for (int i = 0; i < 4; ++i)
	{
		// zone critique ne permettant l'accés qu'à un train, cela empêchera à des trains,
		// arrivés en premier sur la liaison de se faire dépasser, ou bien à 2 trains de se croiser
		pthread_rwlock_wrlock(&lock);
		printf("Le train %d va s'engager sur la liaison : %c--%c\n", id,train1[i],train1[i+1]);
		sleep(rand() % 3); // temps du déplacement
		printf("Le train %d arrive a la fin de la liaison : %c--%c\n", id,train1[i],train1[i+1]);
		printf("Le train %d a quitté la liaison %c--%c\n", id,train1[i],train1[i+1]);
		pthread_rwlock_unlock(&lock);
   }
}


void train2_deplacement(int id){	// fonction représentant le déplacement du train 2

	for (int i = 0; i < 5; ++i)
	{
		// zone critique ne permettant l'accés qu'à un train, cela empêchera à des trains,
		// arrivés en premier sur la liaison de se faire dépasser, ou bien à 2 trains de se croiser
		pthread_rwlock_wrlock(&lock);
		printf("Le train %d va s'engager sur la liaison : %c--%c\n", id,train2[i],train2[i+1]);
		sleep(rand() % 3); // temps de déplacement
		printf("Le train %d arrive a la fin de la liaison : %c--%c\n", id,train2[i],train2[i+1]);
		printf("Le train %d a quitté la liaison %c--%c\n", id,train2[i],train2[i+1]);
		pthread_rwlock_unlock(&lock);
   }
}



void train3_deplacement(int id){	// fonction représentant le déplacement du train 2

	for (int i = 0; i < 5; ++i)
	{
		// zone critique ne permettant l'accés qu'à un train, cela empêchera à des trains,
		// arrivés en premier sur la liaison de se faire dépasser, ou bien à 2 trains de se croiser
		pthread_rwlock_wrlock(&lock);
		printf("Le train %d va s'engager sur la liaison : %c--%c\n", id,train3[i],train3[i+1]);
		sleep(rand() % 3); // temps de déplacement
		printf("Le train %d arrive a la fin de la liaison : %c--%c\n", id,train3[i],train3[i+1]);
		printf("Le train %d a quitté la liaison %c--%c\n", id,train3[i],train3[i+1]);
		pthread_rwlock_unlock(&lock);
   }
}



void* train(void *arg) {
	int id = *((int*) arg);// permettra de donner dans notre cas un numéro à un train, est défini lors de la création des threads
	int i;


	for (i = 1; i <=1000; i++) {

		srand(i); // initialisation de rand, permet de modifier sa graine


		switch (id)  // permet d'exécuter la fonction du déplacement du train désigné par l'id
		{
		case 1:
		  train1_deplacement(id);
		  break;
		case 2:
		  train2_deplacement(id);
		  break;
		case 3:
		  train3_deplacement(id);
		  break;
		
		
		}
		
		
}

/*void* ecrivain(void *arg) {
	int id = *((int*) arg);
	int i;

	for (i = 0; i < 5; i++) {
		printf("Ecrivain %d veut modifier la ressource\n", id);
		pthread_rwlock_wrlock(&lock);
		printf("Ecrivain %d modifie la ressource : %c\n", id, data);
		sleep(rand() % 2); // on simule un traitement long
		data++;
		printf("Ecrivain %d : maintenant la ressource est %c\n", id, data);
		pthread_rwlock_unlock(&lock);
		sleep(rand() % 3); // l'écrivain fait une pause
	}
	printf("Ecrivain %d : s'en va\n", id);
    */
	return NULL;
}

int main() {
	int i, nb[3];// déclaration du tableau qui permettra de donner un numéro aux trains
	
    pthread_t tid[3];// tableau d'identifiants des threads
    	

    pthread_rwlock_init(&lock, NULL);//initialisation du verrou

	for (i = 0; i < 3; i++) { //création des threads
		nb[i] = i+1; // initialisation du tableau dans la valeurs des cases sera passée en paramètre aux threads
    	pthread_create(&tid[i], NULL, train, (void*) &nb[i]);
    }
	//for (i = 0; i < 3; i++) {
	//	nb[i+5] = i;
	//	pthread_create(&tid[i+5], NULL, ecrivain, (void*) &nb[i+5]);
    //}

   for (i = 0; i < 3; i++) {
		pthread_join(tid[i], NULL); // permet d'attendre l'exécution de tous les threadss
    }
    puts("Consultation et modifications terminées");

	pthread_rwlock_destroy(&lock); // détruis le verrou
    return 0;
}
