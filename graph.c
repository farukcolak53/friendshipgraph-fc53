//Ahmet Faruk Çolak

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#define bigNumber 7777

struct Person{
	char personName[15];
	int counter; //counts the number of friends
	struct Person *friends[100];
	struct Person *nextPerson;
};
typedef struct Person Person;
  
FILE *fp;

int addPersonV(Person **hdr, char name[15]){ //adds persons who are vertices
	
	Person *newPerson;
	Person *last = *hdr;

	// create node to insert and assign values to its fields
	newPerson = malloc(sizeof(Person));
	strcpy(newPerson->personName,name);
	newPerson->counter = 0;
	newPerson->nextPerson = NULL;
	
	//If LL is empty
	if (*hdr == NULL){	
		*hdr = newPerson;
		return 1;
	}
	else{
  		//Else traverse till the last node
   		while (last->nextPerson != NULL) 
       		last = last->nextPerson; 
   
    	//Change the next of last node
		last->nextPerson = newPerson; 
    	return 1;
	}	
}

int addFriend(Person **hdr, char vertex[15], char friendName[15]){
	
	Person *current = *hdr;
	Person *friendd;
		
	//Loop to find correct vertex
	while(current != NULL && (strcmp(current->personName, vertex) != 0)){
		current = current->nextPerson;
	}
	
	friendd = malloc(sizeof(Person));
	strcpy(friendd->personName, friendName);
	current->friends[current->counter] = friendd;
	current->counter++; //increments the number of friends by one
}

int minDistance(int distance[], int pathCheck[]){
	
	int min = INT_MAX;
	int indexOfMin;
	int i;
	for (i = 0; i < 10; i++){
   		if ((pathCheck[i] == 0) && (distance[i] <= min)){
	 		min = distance[i]; 
			indexOfMin = i;
		}
	}
   return indexOfMin;
}
   
int closenessCentrality(int source, Person *iter){
	
	int distance[10];     
    int pathCheck[10]; //If shortest path found from source to i,and it consists vertex i, pathCheck[i] will be 1
    int i;
	for (i = 0; i < 10; i++){ //Initializes the arrays
		distance[i] = INT_MAX;
		pathCheck[i] = 0;
	}
	
    distance[source] = 0; //Distance of source to itself is 0 
    
	for (i = 0; i < (10-1); i++){
       	int u = minDistance(distance, pathCheck);
       	int k;
       	Person *temp = iter;
       	for(k = 0; k < u; k++){ //Finds current vertical vertex(as in the matrix representation)
       		temp = temp->nextPerson; 
		}
		pathCheck[u] = 1;
       	int v;
    	for (v = 0; v < 10; v++){
    		Person *temp2 = iter;
    		for(k = 0; k < v; k++){//Finds current horizontal vertex(as in the matrix representation)
				temp2 = temp2->nextPerson;
			}
			int x = 0;
			for(k = 0; k < temp->counter; k++){ //Checks if there is a friendship
	 			if(strcmp(temp->friends[k]->personName, temp2->personName) == 0){
	 				x = 1;
	 				break;
	 			}
			}
    		if (((distance[u] + x) < distance[v]) && x && (distance[u] != INT_MAX)){
				distance[v] = distance[u] + x;
        	}
     	}
 	} 
 	
	int score = 0;
   	for (i = 0; i < 10; i++){ //Finds the closeness centrality score
   		score = score + distance[i];
	} 

   	return score;
}

void betweennessPaths(int n, int startPoint, Person *iter){   
    
	int weight[10][10];
	int distance[10];
	int predecessor[10]; //predecessor of each node
    int visitation[10];
	int counter, minDist, nextPoint;
	int i, j, k, m;
    
    for(i = 0; i < n; i++){
        Person *temp = iter;
       	for(k = 0; k < i; k++){
       		temp = temp->nextPerson;
		}
    	for(j = 0; j < n; j++){
    		
    		Person *temp2 = iter;
    		for(m = 0; m < j; m++){
				temp2 = temp2->nextPerson;
			}
			int x = 0;
			int a;
			for(a = 0; a < temp->counter; a++){
	 			if(strcmp(temp->friends[a]->personName, temp2->personName) == 0){
	 				x = 1;
	 				break;
	 			}
			}
        	if(x == 0)
                weight[i][j] = bigNumber;
            else
                weight[i][j] = x;
		}
	}
              
    //Initializations
    for(i = 0; i < n; i++){
        distance[i] = weight[startPoint][i];
        predecessor[i] = startPoint;
        visitation[i] = 0;
    }
    
    distance[startPoint] = 0;
    visitation[startPoint] = 1;
    counter = 1;
    
    while(counter < n-1){
        minDist = bigNumber;
        
        //nextPoint will be the closest point
        for(i = 0; i < n; i++){
        	if(distance[i] < minDist && !visitation[i]){
                minDist = distance[i];
                nextPoint = i;
            }
		}
        //Checks whether there is a closer path           
        visitation[nextPoint] = 1;
        for(i = 0; i < n; i++){
        	if(!visitation[i]){
                if((minDist + weight[nextPoint][i]) < distance[i]){
                    distance[i] = minDist + weight[nextPoint][i];
                    predecessor[i] = nextPoint;
                }
			}
		}         
    	counter++;
    }
    
    int sum = 0 ;
    int a;
    Person *temp1 = iter;
    Person *temp2 = iter;
    for(a = 0; a < startPoint; a++){
    	temp1 = temp1->nextPerson;
  	}	
    //Prints paths
    for(i = startPoint; i < n; i++){
        if(i != startPoint){
        	temp2 = iter;

            for(a = 0; a < i ;a++){
            	temp2 = temp2->nextPerson;
			}	
        	sum += distance[i];
            printf("%-10s%-10s",temp1->personName,temp2->personName);
            printf("%-20s", "");
            printf("%s",temp2->personName);

            j = i;
            do{
              	temp2 = iter;
            	j = predecessor[j];

                for(a = 0; a < j; a++){
            		temp2 = temp2->nextPerson;
				}	

			 	printf("<-%s",temp2->personName);
            }while(j != startPoint);
            printf("\n");
    	}
    }
}
                
int main(){
	
	setlocale(LC_ALL, "Turkish");  //türkçe karakter
    
    Person *header = NULL;
    char name[10];
    char vertex[10];
	
    fp = fopen("input.txt", "r");
    while(!feof(fp)){
    	fscanf(fp,"%s",name); //reads name by name;
 		int i, j;	
 		
    	//subtracts ; and , from string
    	char ch = name[strlen(name)-1];
    	if(ch == ';'){ //From ; we understand that it is a vertex
    		for(i = 0; name[i] != '\0'; ++i){
	        	while (name[i] == ';' || name[i] == ','){
    	        	for(j = i; name[j] != '\0'; ++j){
 	              		name[j] = name[j+1];
    	        	}
        	    	name[j] = '\0';
        		}
    		}
    		strcpy(vertex, name);
    		addPersonV(&header, vertex); //adds vertices
		}
		else{
			for(i = 0; name[i] != '\0'; ++i){
	        	while (name[i] == ';' || name[i] == ','){
    	        	for(j = i; name[j] != '\0'; ++j){
 	               		name[j] = name[j+1];
    	        	}
        	    	name[j] = '\0';
        		}
    		}
			addFriend(&header, vertex, name);
		}
	}
	fclose(fp);

 	Person *iter = header;
 	printf("%-10s", "");
	while(iter != NULL){
		printf("%-10s", iter->personName);
	 	iter = iter->nextPerson;
	}
	
	iter = header; // iter is the current vertex
	while(iter != NULL){
		printf("\n%-10s", iter->personName);
		Person *check = header; //check is the current person(in all persons)
		while(check != NULL){
			int i;
			int checkBit = 0;
	 		for(i = 0; i < iter->counter; i++){
	 			if(strcmp(iter->friends[i]->personName, check->personName) == 0){
	 				checkBit = 1; //When a friendship is found, breaks the loop and prints 1, continues with next person
	 				break;
	 			}
			}
			printf("%-10d", checkBit);	
	 		check = check->nextPerson;
		}
		iter = iter->nextPerson;
	}

	printf("\n--------------------------------------------------------------------------------------------------------");
	printf("\n--------------------------------------------------------------------------------------------------------");
	
	//Degree Centrality//
	printf("\n      DEGREE CENTRALITY\n");
	printf("      -----------------\n");
	printf("\n%-10s%-10s%-20s", "Person", "Score", "Standardized Score");
	iter = header;
	while(iter != NULL){
		printf("\n%-10s", iter->personName);
		printf("%-10d", iter->counter); 
		printf("%-10.2f", ((float)(iter->counter)/(float)(10 - 1)));
		iter = iter->nextPerson;
	}
	
	printf("\n--------------------------------------------------------------------------------------------------------");
	printf("\n--------------------------------------------------------------------------------------------------------");
	
	//Closeness Centrality//
	printf("\n      CLOSENESS CENTRALITY\n");
	printf("      --------------------\n");
	printf("\n%-10s%-10s%-20s", "Person", "Score", "Standardized Score");
	
	iter = header;
	int i;
	for(i = 0; i < 10; i++){
		int score = closenessCentrality(i, iter);
		
		Person *temp = iter;
		int x;
   		for(x = 0; x < i; x++){
    		temp = temp->nextPerson;
   		}
		printf("\n%-10s%-10.2f%-10.2f",temp->personName, (float)(1.0 / score), (float)((10 - 1.0) / score));  	
	}
	
	printf("\n--------------------------------------------------------------------------------------------------------");
	printf("\n--------------------------------------------------------------------------------------------------------");
	
	//Betweenness Centrality//
	printf("\n      BETWEENNESS CENTRALITY\n");
	printf("      ----------------------\n");
	printf("%-10s%-10s%-20s%-10s\n", "Source", "Target", "Intermedia Nodes", "Path");
	iter = header;
	for(i = 0; i < 10; i++){
    	betweennessPaths(10, i, iter);			
	}
	return 1;
}

//https://www.programiz.com/c-programming/examples/remove-characters-string
//https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
//https://www.thecrazyprogrammer.com/2014/03/dijkstra-algorithm-for-finding-shortest-path-of-a-graph.html
