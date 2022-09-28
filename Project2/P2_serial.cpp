//Peri Hassanzadeh
//ECE1570 High Performance Computing
//Date of Creation: 11/28/21
//Last Update: 12/1/2021
//
//This is the serial version of Project 2

//Import c++ libraries
#include <limits.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <string>
#include <sys/time.h>
#include <stdlib.h>
#define V 50
using namespace std;

int minDistance(int dist[], bool sptSet[]) {
   int min = INT_MAX, min_index;
   for (int v = 0; v < V; v++)
   if (sptSet[v] == false && dist[v] <= min)
      min = dist[v], min_index = v;
   return min_index;
}

int printSolution(int dist[], int n, int matrixNum) {
   printf("Vertex Distance from Source for Matrix \n");
   cout << "Matrix Number "<<matrixNum << endl;
   for (int i = 0; i < V; i++)
      printf("%d \t %d\n", i, dist[i]);
}

void dijkstra(int graph[V][V], int src, int matrixNumber) {
   int dist[V];
   bool sptSet[V];
   for (int i = 0; i < V; i++)
      dist[i] = INT_MAX, sptSet[i] = false;
      dist[src] = 0;
   for (int count = 0; count < V - 1; count++) {
      int u = minDistance(dist, sptSet);
      sptSet[u] = true;
      for (int v = 0; v < V; v++)
         if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) dist[v] = dist[u] + graph[u][v];
   }
   printSolution(dist, V, matrixNumber);
}

int main() {

   //Struct to hold the variables in order to find completion time
   struct timeval start, end;
   //Get the time the program starts
   gettimeofday(&start, NULL);

   //Create matrix
   int graph[100][100];

   //Put random values in the matrix
   for(int i=0; i<100; i++)
   {
      for(int j=0; j<100; j++)
      {
         int randomnum = rand() % 2;
         graph[i][j] = randomnum;
         //cout << graph[i][j] << endl;
      }
   }


   //Split up the matrix
   int firstGraph[V][V];
   for(int k=0; k<V; k++)
   {
      for(int p=0; p<V; p++)
      {
         firstGraph[k][p] = graph[k][p];
      }
   }

   int secondGraph[V][V];
   for(int a=0; a<V; a++)
   {
      for(int b=0; b<V; b++)
      {
         secondGraph[a][b] = graph[a][b+50];
      }
   }

   int thirdGraph[V][V];
   for(int c=0; c<V; c++)
   {
      for(int d=0; d<V; d++)
      {
         thirdGraph[c][d] = graph[c+50][d];
      }
   }

   int fourthGraph[V][V];
   for(int e=0; e<V; e++)
   {
      for(int f=0; f<V; f++)
      {
         fourthGraph[e][f] = graph[e+50][f+50];
      }
   }

   //Find shortest path of each matrix
   dijkstra(firstGraph, 0, 1);
   dijkstra(secondGraph, 0, 2);
   dijkstra(thirdGraph, 0, 3);
   dijkstra(fourthGraph, 0, 4);
   
   //Get the time the program finishes at
   gettimeofday(&end, NULL);

   //Calculate the time that was taken for the program to complete and print out the results (3)
   int micro_end = end.tv_sec * 1000000 + end.tv_usec;
   int micro_start = start.tv_sec * 1000000 + start.tv_usec;
   cout << "Total time for serial execution: " << (micro_end - micro_start) << " microseconds" << endl;
   return 0;
}

/** Sources - Project 1 Serial Implementation
* (1) Dijkstra's Algorithm
* - https://www.tutorialspoint.com/c-cplusplus-program-for-dijkstra-s-shortest-path-algorithm
* -- I used this resource to implement a dijkstra's algorithm as the main part of my serial version.
* -- This is used to find the shortest path for each point in the array.
*
* (2) Binary Matrix
* -- https://www.daniweb.com/programming/software-development/threads/232485/random-binary-matrix
* -- For filling a binary matrix with random numbers
*/