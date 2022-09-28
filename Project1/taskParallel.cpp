//Peri Hassanzadeh
//ECE1570 High Performance Computing
//Date of Creation: 11/4/2021
//Last Update: 11/5/2021
//
//This is the task parallel version of Project 1, opening text documents, 
//filtering the words, counting the frequency, and summarizing the 
//top 10% of most frequent concepts.
//The task parallel version of this project utilizes pthreads and mutexes. 

//Import c++ libraries
#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <string>
#include <sys/time.h>
#include <string.h>
#include <sched.h>
using namespace std;

//Global variable to hold the total number of threads created
#define NUM_THREADS 4

//Creating Global Variables
string formatedLine;
vector<string> wordsFromString, filteredWords;
ifstream inFile;
ofstream wordsOutFile, finalOutFile;

//Create multiple pthread mutexes to lock and unlock within the threads
pthread_mutex_t wordsFromStringVectorMutex, filteredWordsVectorMutex, wordsOutFileMutex, finalOutFileMutex;

//Create the flags to indicate when threads can start using data
int readDataFlag, formatFlag, freqFileFlag, listWordsFlag;

//array to hold the names of first half of the files
const char *files[78] = {
"certdata/2013-01-08.txt", "certdata/2013-01-09.txt", "certdata/2013-01-10.txt", "certdata/2013-01-11.txt", "certdata/2013-01-25.txt", 
"certdata/2013-01-28.txt", "certdata/2013-01-29.txt", "certdata/2013-02-01.txt", "certdata/2013-02-07.txt", "certdata/2013-02-14.txt",
"certdata/2013-02-22.txt", "certdata/2013-03-05.txt", "certdata/2013-03-08.txt", "certdata/2013-03-11.txt", "certdata/2013-03-18.txt",
"certdata/2013-03-20.txt", "certdata/2013-03-21.txt", "certdata/2013-04-02.txt", "certdata/2013-04-03.txt", "certdata/2013-04-05.txt",
"certdata/2013-04-08.txt", "certdata/2013-04-10.txt", "certdata/2013-04-15.txt", "certdata/2013-04-19.txt", "certdata/2013-04-25.txt",
"certdata/2013-04-26.txt", "certdata/2013-04-29.txt", "certdata/2013-04-30.txt", "certdata/2013-05-06.txt", "certdata/2013-05-14.txt",
"certdata/2013-05-15.txt", "certdata/2013-05-17.txt", "certdata/2013-06-05.txt", "certdata/2013-06-07.txt", "certdata/2013-06-10.txt",
"certdata/2013-06-11.txt", "certdata/2013-06-18.txt", "certdata/2013-06-26.txt", "certdata/2013-06-27.txt", "certdata/2013-06-28.txt",
"certdata/2013-07-12.txt", "certdata/2013-07-15.txt", "certdata/2013-07-26.txt", "certdata/2013-08-01.txt", "certdata/2013-08-02.txt",
"certdata/2013-08-05.txt", "certdata/2013-08-15.txt", "certdata/2013-08-21.txt", "certdata/2013-08-23.txt", "certdata/2013-08-26.txt",
"certdata/2013-08-30.txt", "certdata/2013-09-03.txt", "certdata/2013-09-04.txt", "certdata/2013-09-13.txt", "certdata/2013-09-23.txt",
"certdata/2013-09-24.txt", "certdata/2013-10-01.txt", "certdata/2013-10-04.txt", "certdata/2013-10-10.txt", "certdata/2013-10-15.txt",
"certdata/2013-10-16.txt", "certdata/2013-10-17.txt", "certdata/2013-10-18.txt", "certdata/2013-10-22.txt", "certdata/2013-10-25.txt",
"certdata/2013-10-28.txt", "certdata/2013-10-30.txt", "certdata/2013-10-31.txt", "certdata/2013-11-04.txt", "certdata/2013-11-07.txt",
"certdata/2013-11-14.txt", "certdata/2013-11-18.txt", "certdata/2013-11-22.txt", "certdata/2013-12-02.txt", "certdata/2013-12-03.txt",
"certdata/2013-12-05.txt", "certdata/2013-12-12.txt", "certdata/2013-12-30.txt"
};

//array to hold all of the noise words
const char *delwords[34] = {
"a", "an", "the", "am", "is", "are", "was", "were", "being", "been", 
"seem", "become", "became", "on", "for", "in", "to", "or", "of", "that", 
"at", "of", "and", "this", "by", "it", "be", "can", "not", "from",
"has", "like", "with", "as" };


/**
* readInDataThread thread deals with reading in the text from the files and filtering out the unnecessary characters and whitespace.
* The filtered data is sent to a temporary text file to be used later on in another thread.
*/
void *readInDataThread(void *rank)
{
	//Indicate the start of the thread to read in data from given text files
	cout << "Start of readInDataThread" << endl;

	//Lock the wordsFromStringVectorMutex 
	cout << "Locking wordsFromStringVectorMutex" <<endl;
	pthread_mutex_lock(&wordsFromStringVectorMutex);

	//For all the 78 files
	for(int i=0; i<78; i++)
	{
		//Open the corresponding file
    	inFile.open(files[i]);
    	
    	//String to hold the line from the text file
    	string words;

    	//Until the end of the file read in each line
    	while (getline(inFile, words)) 
    	{

	      // remove symbols and numbers from line
	      for (int j=0; j<words.size(); j++)
	        if (!isalpha(words[j])) 
	        {
	      		words[j] = ' ';
	        }

	      //Make sure each letter is lowercase in the string words
	      for (int i=0; i<words.size(); i++)
	        words[i] = tolower(words[i]);

	      // make into a list of words
	      vector<string> wordlist;
	      char charwords [words.length()+1];
	      strcpy(charwords, words.c_str());
	      //Break up the string by whitespace to know a new word is found
	      char *token = strtok(charwords, " ");
	      
	      while (token != NULL){
	      	//Add the token to the list of words
	      	wordsFromString.push_back(token);
	      	token = strtok(NULL, " ");
	      }
    	}
    	//Close the current textfile so the next one can be opened
    	inFile.close();

    	//If all 77 files have been read in 
    	if(i==77)
    	{
    		//Change the readDataFlag so the next thread knows the data is no longer being processed and can now be used
    		readDataFlag =1;

    		//Unlock the wordsFromStringVectorMutex so the vector of words can now be used by the other threads
    		cout << "Unlocking wordsFromStringVectorMutex" << endl;
    		pthread_mutex_unlock(&wordsFromStringVectorMutex);
    	}
	}
	//Indicate end of the thread
	cout << "End of readInDataThread" << readDataFlag<<endl;
	pthread_exit(NULL);
}


/**
* formatStringThread filters out the noise words from the previous temp text file that was used in the readInDataThread.
* The newly filtered data is set to a vector called filteredWords.
*/
void *formatStringThread(void *rank)
{
	while(readDataFlag != 1)
	{
		//Waits until files are read in
	}

	//Indicate that the formatStringThread is starting
	cout << "Start of formatStringThread" << endl;

	//Lock the wordsFromStringVectorMutex and filteredWordsVectorMutex
	cout << "Locking wordsFromStringVectorMutex and filteredWordsVectorMutex" << endl; 
	pthread_mutex_lock(&wordsFromStringVectorMutex);
	pthread_mutex_lock(&filteredWordsVectorMutex);

	//Find and remove the noise words
	for (int k=wordsFromString.size()-1; k>=0; k--) 
	{
		//Indicator to remove
        int removeme = 0;
        for (int n=0;n<34;n++) 
        {
      		if (!wordsFromString[k].compare(delwords[n])) 
      		{
      			//Indicate is a noiseword
        		removeme = 1;
      		}
        }
        if (removeme == 1) // If the word is a noiseword, remove it from the vector
     		wordsFromString.erase(wordsFromString.begin()+k);
	}

	//Set a new vector for all of the filtered words to be used by another thread
    filteredWords = wordsFromString;
    
    //Indicate words have been formated and change status of flag
    formatFlag =1;

    //Unlock the wordsFromStringVectorMutex and filteredWordsVectorMutex
    cout << "Unlocking wordsFromStringVectorMutex and filteredWordsVectorMutex" << endl;
    pthread_mutex_unlock(&wordsFromStringVectorMutex);
    pthread_mutex_unlock(&filteredWordsVectorMutex);

    //Indicate end of thread
    cout << "End of formatStringThread" << endl;
	pthread_exit(NULL);
}


/**
* listWordsStringtoVectThread takes the complete filtered words and moves them to the temporary text file of all the words.
* This file will be later used to determine the frequency of all the words as well as find the top 10% used words
*/
void *listWordsStringtoVectThread(void *rank)
{
	while(formatFlag == 0)
	{

	}
	cout << "Start of listWordsStringtoVectThread" << endl;
	cout << "Locking filteredWordsVectorMutex and wordsOutFileMutex" << endl;
	pthread_mutex_lock(&filteredWordsVectorMutex);
	pthread_mutex_lock(&wordsOutFileMutex);

	wordsOutFile.open("results_taskP.txt");
	// make into a list of words
	for (int m=0;m<filteredWords.size();m++)
    	wordsOutFile << filteredWords[m] << endl;

    listWordsFlag = 1;
    cout << "Unlocking filteredWordsVectorMutex and wordsOutFileMutex" << endl;
  	pthread_mutex_unlock(&filteredWordsVectorMutex);
	pthread_mutex_unlock(&wordsOutFileMutex);

	wordsOutFile.close();
	cout << "End of listWordsStringtoVectThread" << endl;
	pthread_exit(NULL);
}


/**
* frequencyToFileThread finds the frequency of all the words after being filterd
* The final results of the frequency are sent to a summary textfile called finalReport_taskP.txt
*/
void *frequencyToFileThread(void *rank)
{
	while(listWordsFlag ==0)
	{

	}
  cout << "Start of frequencyToFileThread" << endl;
  vector<string> wordVec;
  string wordss;
  ifstream wordsOutFileIn;

  cout << "Locking wordsOutFileMutex and finalOutFileMutex" << endl;
  pthread_mutex_lock(&wordsOutFileMutex);
  pthread_mutex_lock(&finalOutFileMutex);

  wordsOutFileIn.open("results_taskP.txt");
  finalOutFile.open("finalReport_taskP.txt");

  while(getline(wordsOutFileIn, wordss))
  {
    wordVec.push_back(wordss);
  }

  map<string, int> M;

  for(int i=0; i<wordVec.size(); i++)
  {
    if(M.find(wordVec[i]) == M.end())
    {
      M[wordVec[i]] = 1;
    }
    else
    {
      M[wordVec[i]]++;
    }
  }
  
  int size = wordVec.size();
  int tenp = ceil(size*.1);

  for(auto& it:M)
  {
    if(it.second > 10)
      finalOutFile << it.first << ' ' << it.second << '\n';
  }
  freqFileFlag = 1;
  cout << "Unlocking wordsOutFileMutex and finalOutFileMutex" << endl;
  pthread_mutex_unlock(&wordsOutFileMutex);
  pthread_mutex_unlock(&finalOutFileMutex);

  wordsOutFileIn.close();
  finalOutFile.close();

  cout<< "End of frequencyToFileThread" << endl;

	pthread_exit(NULL);
}


/**
* Main method - determines time taken for program to run as well as handling creation of threads and mutexes. 
* Data is initialized here and threads are joined and summary report of time taken is reported here as well. 
*/
int main()
{
	//Struct to hold the variables in order to find completion time
	struct timeval start, end;
	//Get the time the program starts
	gettimeofday(&start, NULL);

	pthread_t ids[NUM_THREADS];
	cout << "Start of main program" << endl;
	pthread_mutex_init(&wordsFromStringVectorMutex, NULL);
	pthread_mutex_init(&filteredWordsVectorMutex, NULL);
	pthread_mutex_init(&wordsOutFileMutex, NULL);
	pthread_mutex_init(&finalOutFileMutex, NULL);

	//Initialize all flags to 0
	readDataFlag=0;
	formatFlag=0;
	freqFileFlag=0;
	listWordsFlag=0;

	//Creating the first two threads
	cout << "Creating the first thread that calls readInDataThread" << endl;
	pthread_create(&ids[0], NULL, readInDataThread, (void *)0);
	cout << "Creating the second thread that calls formatStringThread" << endl;
	pthread_create(&ids[1], NULL, formatStringThread, (void *)1);
	cout << "Creating the third thread that calls listWordsStringtoVectThread" << endl;
	pthread_create(&ids[2], NULL, listWordsStringtoVectThread, (void *)2);


	cout << "Joining all threads" << endl;
	//Waiting for the first two threads to complete
	for(int i=0; i<3; i++)
	{
		pthread_join(ids[i], NULL);
	}

	cout << "Creating the fourth thread that calls frequencyToFileThread" << endl;
	pthread_create(&ids[3], NULL, frequencyToFileThread, (void *)3);
	pthread_join(ids[3], NULL);

	//Close both files
	finalOutFile.close();
	wordsOutFile.close();

	//Indicate the program has complete
	cout << "End of main program" << endl;
	
	//Get the time the program finishes at
	gettimeofday(&end, NULL);

	//Calculate the time that was taken for the program to complete and print out the results (3)
 	int micro_end = end.tv_sec * 1000000 + end.tv_usec;
 	int micro_start = start.tv_sec * 1000000 + start.tv_usec;
  	cout << "Total time for task parallel execution: " << (micro_end - micro_start) << " microseconds" << endl;
  
	return 0;
}

/** Sources - Project 1 Task Parallel Implementation
* (1) Geeks for Geeks
* - https://www.geeksforgeeks.org/program-to-find-frequency-of-each-element-in-a-vector-using-map-in-c/
* -- I used this resource to implement a way to find the frequency of each word in the vector. This example
* -- uses integers instead of strings, but I was able to utilize it to change the map variable to string.
*
* (2) Canvas Project 1 Discussion Board
* -- I was able to utilize the code posted on the Project 1 discussion board thanks to Dr. Bigriggs post.
* -- The majority of the text analytic process in the main function is from the post. 
*
* (3) Canvas Lecture Notes
* - Bsharedmem.ppt Slide 7
* -- I was able to utilize timing for my program by using the lecture notes as an example.
* -- A struct is used and then the start and end times are referenced and used for calculations later on. 
* 
* (4) TutorialsPoint
* - https://www.tutorialspoint.com/cplusplus/cpp_multithreading.htm
* -- Used as a reference for the syntax of threads and how to create/join
*
* (5) Oracle Mutexes
* - https://docs.oracle.com/cd/E19455-01/806-5257/sync-12/index.html
* -- Used as a reference for the syntax of mutexes and how to use regarding locking and unlocking
*/