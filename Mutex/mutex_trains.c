#include <stdio.h>   // printf
#include <pthread.h> // pthread
#include <unistd.h>  // sleep
#include <stdlib.h>  // srand
#include <time.h>    // time
#include <math.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;





char gare_t1D='A'; // Constante qui désigne les gare d'arrivée et de départ de chaque trains à l'instant t
char gare_t1A='B';
char gare_t2D='A';
char gare_t2A='B';
char gare_t3D='A';
char gare_t3A='B';

double duree_t1=0; // Durée d'un trajet du train 1
double duree_t2=0;
double duree_t3=0;
double vt1=0;
double vt2=0;
double vt3=0;


void train1_deplacement(int id, int tempsDepla_train1,char*train1){ // fonction représentant le déplacement du train 1 avec une zone critique permettant l'accès 


	time_t debut_t1,fin_t1; // Variables qui stockent les secondes écoulées 
	debut_t1 = time(NULL);

	for (int i = 0; i < 4; ++i)
	{

		
		
		//Si les trois trains accèdent à une même liaison ferrée, permet d'empêcher le dépassement

		if(((train1[i]==gare_t2D && train1[i+1]==gare_t2A)&&(train1[i]==gare_t3D && train1[i+1]==gare_t3A))){	

			// zone critique ne permettant l'accés qu'à un train, cela empêchera à des trains,
			// arrivés en premier sur la liaison de se faire dépasser, ou bien à 2 trains de se croiser
		    pthread_mutex_lock(&mutex);

			printf("Le train %d  s'engage sur la liaison : %c--%c\n", id,train1[i],train1[i+1]);
			sleep(tempsDepla_train1); // temps de déplacement du train
			printf("Le train %d arrive à la fin de la liaison : %c--%c\n", id,train1[i],train1[i+1]);
			gare_t1D=train1[i+1];		  // mise à jour de la gare pour le train1, utilisé pour les train2 et train3
			gare_t1A=train1[i+2];
			

      		pthread_mutex_unlock(&mutex);

     		pthread_mutex_lock(&mutex);
			printf("Le train %d a quitté la liaison %c--%c\n", id,train1[i],train1[i+1]);
     		pthread_mutex_unlock(&mutex);

		}


		// Si le train1 est sur la même liaison que train2 ou train3, empêche le dépassement

		else if ((train1[i]==gare_t2D && train1[i+1]==gare_t2A)||(train1[i]==gare_t3D && train1[i+1]==gare_t3A))
		{

     		pthread_mutex_lock(&mutex);
			
			printf("Le train %d  s'engage sur la liaison : %c--%c\n", id,train1[i],train1[i+1]);
			sleep(tempsDepla_train1); // temps de déplacement du train
			printf("Le train %d arrive a la fin de la liaison : %c--%c\n", id,train1[i],train1[i+1]);
			gare_t1D=train1[i+1];		  // mise à jour de la gare pour le train1, utilisé pour les train2 et train3
			gare_t1A=train1[i+2];

			pthread_mutex_unlock(&mutex);

     		pthread_mutex_lock(&mutex);
			printf("Le train %d a quitté la liaison %c--%c\n", id,train1[i],train1[i+1]);
			pthread_mutex_unlock(&mutex);
		}


		// Si le train1 arrive dans le sens contraire que le train2 ou train3 empêche le croisement 
		// en utisant le verrou écriture qui empêchera plusieurs trains d'accéder à la ressource qui est la liaison 

		else if ((train1[i]==gare_t2A && gare_t2D==train1[i+1])||(train1[i]==gare_t3A && gare_t3D==train1[i+1]))
		{

			// zone critique ne permettant l'accés qu'à un train, cela empêchera à des trains,
			// 2 trains de se croiser, ici un train ne pourra entrer sur la liaison que si 
			// l'autre est sorti

			pthread_mutex_lock(&mutex);
			
			printf("Le train %d  s'engage sur la liaison : %c--%c\n", id,train1[i],train1[i+1]);
			sleep(tempsDepla_train1); // temps de déplacement du train
			printf("Le train %d arrive a la fin de la liaison : %c--%c\n", id,train1[i],train1[i+1]);
			gare_t1D=train1[i+1];		  // mise à jour de la gare pour le train1, utilisé pour les train2 et train3
			gare_t1A=train1[i+2];
			printf("Le train %d a quitté la liaison %c--%c\n", id,train1[i],train1[i+1]);

			pthread_mutex_unlock(&mutex);

			
		}

		// Dans le cas où les trains circulent sur des liaisons différentes, les trains circulent librement
		// 
		else {

			
			pthread_mutex_lock(&mutex);
			
			printf("Le train %d s'engage sur la liaison : %c--%c\n", id,train1[i],train1[i+1]);
			sleep(tempsDepla_train1);
			printf("Le train %d arrive a la fin de la liaison : %c--%c\n", id,train1[i],train1[i+1]);
			printf("Le train %d a quitté la liaison %c--%c\n", id,train1[i],train1[i+1]);

			gare_t1D=train1[i+1];		  // mise à jour de la gare pour le train1, utilisé pour les train2 et train3
			gare_t1A=train1[i+2];

			pthread_mutex_unlock(&mutex);

		}

	}

	fin_t1 = time(NULL);
	duree_t1 += difftime(fin_t1,debut_t1); // on recueille le temps de chaque trajet complet effectué par le train1 pour les additionner 
	vt1+=difftime(fin_t1,debut_t1);										   // et obtenir le temps complet des 1000 trajets effectués par le train 1
	
  }



void train2_deplacement(int id,int tempsDepla_train2,char*train2){	// fonction représentant le déplacement du train 2


	time_t debut_t2,fin_t2;
	debut_t2 = time(NULL);


	for (int i = 0; i < 5; ++i)
	{

		//Si les trois trains accèdent à une même liaison ferrée, permet d'empêcher le dépassement
		if( (train2[i]==gare_t1D && train2[i+1]==gare_t1A) && (train2[i]==gare_t3D && train2[i+1]==gare_t3A))	{

			// zone critique ne permettant l'accés qu'à un train, cela empêchera à des trains,
			// arrivés en premier sur la liaison de se faire dépasser, ou bien à 2 trains de se croiser	
			pthread_mutex_lock(&mutex);
			
			printf("Le train %d  s'engage sur la liaison : %c--%c\n", id,train2[i],train2[i+1]);
			sleep(tempsDepla_train2); // temps de déplacement
			printf("Le train %d arrive a la fin de la liaison : %c--%c\n", id,train2[i],train2[i+1]);
			gare_t2D=train2[i+1];		  // mise à jour de la gare pour le train2, utilisé pour les train1 et train3
			gare_t2A=train2[i+2];

			pthread_mutex_unlock(&mutex);

			pthread_mutex_lock(&mutex);// zone critique permettant la sortie dans l'ordre d'arrivée
			printf("Le train %d a quitté la liaison %c--%c\n", id,train2[i],train2[i+1]);
			pthread_mutex_unlock(&mutex);


		}

		// Si le train2 est sur la même liaison que train3 ou train1, empêche le dépassement
		else if ( (train2[i]==gare_t1D && train2[i+1]==gare_t1A) || (train2[i]==gare_t3D && train2[i+1]==gare_t3A) ) {
			
			pthread_mutex_lock(&mutex);
			
			printf("Le train %d  s'engage sur la liaison : %c--%c\n", id,train2[i],train2[i+1]);
			sleep(tempsDepla_train2); // temps de déplacement du train
			printf("Le train %d arrive a la fin de la liaison : %c--%c\n", id,train2[i],train2[i+1]);
			gare_t2D=train2[i+1];		  // mise à jour de la gare pour le train2, utilisé pour les train1 et train3
			gare_t2A=train2[i+2];

			pthread_mutex_unlock(&mutex);


			pthread_mutex_lock(&mutex);// zone critique permettant la sortie dans l'ordre d'arrivée
			printf("Le train %d a quitté la liaison %c--%c\n", id,train2[i],train2[i+1]);
			pthread_mutex_unlock(&mutex);

		}

		// Si le train2 arrive dans le sens contraire que le train1 ou train3 empêche le croisement 
		// en utisant le verrou qui empêchera plusieurs trains d'accéder à la ressource qui est la liaison 

		else if ( (train2[i]==gare_t1A && gare_t1D==train2[i+1])||(train2[i]==gare_t3A && gare_t3D==train2[i+1])){
			
			// zone critique ne permettant l'accés qu'à un train, cela empêchera à des trains,
			// 2 trains de se croiser, ici un train ne pourra entrer sur la liaison que si 
			// l'autre est sorti	
			pthread_mutex_lock(&mutex);			
			
			printf("Le train %d  s'engage sur la liaison : %c--%c\n", id,train2[i],train2[i+1]);
			sleep(tempsDepla_train2); // temps de déplacement
			printf("Le train %d arrive a la fin de la liaison : %c--%c\n", id,train2[i],train2[i+1]);
			gare_t2D=train2[i+1];		  // mise à jour de la gare pour le train2, utilisé pour les train1 et train3
			gare_t2A=train2[i+2];
			printf("Le train %d a quitté la liaison %c--%c\n", id,train2[i],train2[i+1]);

			pthread_mutex_unlock(&mutex);

			

			
		}

		// Dans le cas où les trains circulent sur des liaisons différentes, les trains circulent librement
		// 
		else{

			pthread_mutex_lock(&mutex);
			
			printf("Le train %d  s'engage sur la liaison : %c--%c\n", id,train2[i],train2[i+1]);
			sleep(tempsDepla_train2);
			printf("Le train %d arrive a la fin de la liaison : %c--%c\n", id,train2[i],train2[i+1]);
			printf("Le train %d a quitté la liaison %c--%c\n", id,train2[i],train2[i+1]);
			gare_t2D=train2[i+1];		  // mise à jour de la gare pour le train2, utilisé pour les train1 et train3
			gare_t2A=train2[i+2];
			pthread_mutex_unlock(&mutex);

			
   		}

	}

	fin_t2 = time(NULL);
	duree_t2 += difftime(fin_t2,debut_t2); 
	vt2+=difftime(fin_t2,debut_t2);									   


}


void train3_deplacement(int id,int tempsDepla_train3,char*train3){	// fonction représentant le déplacement du train 2


	time_t debut_t3,fin_t3;
	debut_t3 = time(NULL);


	for (int i = 0; i < 5; ++i)
	{
		
		
		//Si les trois trains accèdent à une même liaison ferrée, permet d'empêcher le dépassement


		if( (train3[i]==gare_t1D && train3[i+1]==gare_t1A) && (train3[i]==gare_t2D && train3[i+1]==gare_t2A) )	{

			// zone critique ne permettant l'accés qu'à un train, cela empêchera à des trains,
			// arrivés en premier sur la liaison de se faire dépasser	

			pthread_mutex_lock(&mutex);				
			
			printf("Le train %d s'engage sur la liaison : %c--%c\n", id,train3[i],train3[i+1]);
			sleep(tempsDepla_train3); // temps de déplacement
			printf("Le train %d arrive a la fin de la liaison : %c--%c\n", id,train3[i],train3[i+1]);
			gare_t3D=train3[i+1];		  // mise à jour de la gare pour le train3, utilisé pour les train1 et train2
			gare_t3A=train3[i+2];

			pthread_mutex_unlock(&mutex);

			pthread_mutex_lock(&mutex);// zone critique permettant la sortie dans l'ordre d'arrivée
			printf("Le train %d a quitté la liaison %c--%c\n", id,train3[i],train3[i+1]);
			pthread_mutex_unlock(&mutex);
		}


		// Si le train3 est sur la même liaison que train1 ou train2, empêche le dépassement
		else if ( ((train3[i]==gare_t1D && train3[i+1]==gare_t1A)||(train3[i]==gare_t2D && train3[i+1]==gare_t2A)) ){
		
			pthread_mutex_lock(&mutex);

			printf("Le train %d s'engage sur la liaison : %c--%c\n", id,train3[i],train3[i+1]);
			sleep(tempsDepla_train3); // temps de déplacement
			printf("Le train %d arrive a la fin de la liaison : %c--%c\n", id,train3[i],train3[i+1]);
			gare_t3D=train3[i+1];		  // mise à jour de la gare pour le train3, utilisé pour les train1 et train2
			gare_t3A=train3[i+2];

			pthread_mutex_unlock(&mutex);

			pthread_mutex_lock(&mutex);// zone critique permettant la sortie dans l'ordre d'arrivée
			printf("Le train %d a quitté la liaison %c--%c\n", id,train3[i],train3[i+1]);
			pthread_mutex_unlock(&mutex);

		}



		// Si le train3 arrive dans le sens contraire que le train1 ou train2 empêche le croisement 
		// en utisant le verrou qui empêchera plusieurs trains d'accéder à la ressource qui est la liaison 
		else if ( ((train3[i]==gare_t1A && gare_t1D==train3[i+1])||(train3[i]==gare_t2A && gare_t2D==train3[i+1]))){
			
			// zone critique ne permettant l'accés qu'à un train, cela empêchera à des trains,
			// 2 trains de se croiser, ici un train ne pourra entrer sur la liaison que si 
			// l'autre est sorti

			pthread_mutex_lock(&mutex);

			printf("Le train %d s'engage sur la liaison : %c--%c\n", id,train3[i],train3[i+1]);			
			sleep(tempsDepla_train3); // temps de déplacement
			printf("Le train %d arrive a la fin de la liaison : %c--%c\n", id,train3[i],train3[i+1]);
			gare_t3D=train3[i+1];		  // mise à jour de la gare pour le train3, utilisé pour les train1 et train2
			gare_t3A=train3[i+2];
			printf("Le train %d a quitté la liaison %c--%c\n", id,train3[i],train3[i+1]);

			pthread_mutex_unlock(&mutex);

			
			
		}


		// Dans le cas où les trains circulent sur des liaisons différentes, les trains circulent librement
		// 

		else{

			pthread_mutex_lock(&mutex);

			printf("Le train %d s'engage sur la liaison : %c--%c\n", id,train3[i],train3[i+1]);
			sleep(tempsDepla_train3); // temps de déplacement
			printf("Le train %d arrive a la fin de la liaison : %c--%c\n", id,train3[i],train3[i+1]);
			printf("Le train %d a quitté la liaison %c--%c\n", id,train3[i],train3[i+1]);
			gare_t3D=train3[i+1];		  // mise à jour de la gare pour le train3, utilisé pour les train1 et train2
			gare_t3A=train3[i+2];

			pthread_mutex_unlock(&mutex);


		}

	}

	fin_t3 = time(NULL);
	duree_t3 += difftime(fin_t3,debut_t3); 
	vt3+=difftime(fin_t3,debut_t3);									   

  }




void* train(void *arg) {

int id = *((int*) arg);// permettra de donner dans notre cas un numéro à un train, est défini lors de la création des threads
int i;

char *train1="ABCBA"; // tableau représenatant le trajet de chaque train
char *train2="ABDCBA";
char *train3="ABDCEA";



	for (i = 1; i <=1000; i++) {

		srand(i); // initialisation de rand, permet de modifier sa graine
		int tempsDepla_train1=rand() % 3 +1; // temps du déplacement
		int tempsDepla_train2=rand() % 3 +1;
		int tempsDepla_train3=rand() % 3 +1;


		switch (id)  // permet d'exécuter la fonction du déplacement du train désigné par l'id
		{
		case 1:
		  train1_deplacement(id,tempsDepla_train1,train1);
		  break;
		case 2:
		  train2_deplacement(id,tempsDepla_train2,train2);
		  break;
		case 3:
		  train3_deplacement(id,tempsDepla_train3,train3);
		  break;
		
		
		}
		
		
	}


	return NULL;
}

int main() {

 	FILE* fichier=NULL; // ouverture d'un fichier dans lequel on écrit les résultats du banc de test
 	fichier=fopen("banc_de_test1.txt","w");

	int i, nb[3];// déclaration du tableau qui permettra de donner un numéro aux trains

	
    pthread_t tid[3];// tableau d'identifiants des threads

    double duree_t1_moy=0;
    double duree_t2_moy=0;
    double duree_t3_moy=0;
    double variance_t1=0;
    double variance_t2=0;
    double variance_t3=0;
    double ec_t1;
    double ec_t2;
    double ec_t3;


    


	for (i = 0; i < 3; i++) { //création des threads
		nb[i] = i+1; // initialisation du tableau dans la valeurs des cases sera passée en paramètre aux threads
    	pthread_create(&tid[i], NULL, train, (void*) &nb[i]);
    }
	

   for (i = 0; i < 3; i++) {
		pthread_join(tid[i], NULL); // permet d'attendre l'exécution de tous les threadss
    }

   
    puts("Terminus");

    duree_t1_moy = duree_t1/1000; // durée moyenne d'un trajet effectué par le train 1
    duree_t2_moy = duree_t2/1000;
    duree_t3_moy = duree_t3/1000;
    variance_t1=vt1/1000-duree_t1_moy*duree_t1_moy;
    ec_t1=sqrt(variance_t1);
    variance_t2=vt2/1000-duree_t2_moy*duree_t2_moy;
    ec_t2=sqrt(variance_t2);
    variance_t3=vt3/1000-duree_t3_moy*duree_t3_moy;
    ec_t3=sqrt(variance_t3);



    fprintf(fichier, "%f\n", duree_t1_moy);
	fprintf(fichier, "%f\n", duree_t2_moy);      
    fprintf(fichier, "%f\n", duree_t3_moy);


    fprintf(fichier, "la variance du temps moyen de déplacement du train 1 : %f\n", duree_t1_moy);
	fprintf(fichier, "la variance du temps moyen de déplacement du train 2 :%f\n", duree_t2_moy);      
    fprintf(fichier, "la variance du temps moyen de déplacement du train 3 :%f\n", duree_t3_moy);


    fprintf(fichier, "L'écart type du temps moyen de deplacement du train 1 : %f\n", ec_t1);
	fprintf(fichier, "L'écart type du temps moyen de deplacement du train 2 : %f\n", ec_t2);      
    fprintf(fichier, "L'écart type du temps moyen de deplacement du train 3 : %f\n", ec_t3);


	pthread_mutex_destroy(&mutex);
	fclose(fichier);

    return 0;
}
