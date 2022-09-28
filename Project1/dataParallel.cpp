//Peri Hassanzadeh
//ECE1570 High Performance Computing
//Date of Creation: 11/4/2021
//Last Update: 11/5/2021
//
//This is the data parallel version of Project 1, opening text documents, 
//filtering the words, counting the frequency, and summarizing the 
//top 10% of most frequent concepts. (2)
//The data parallel version of this project utilizes pthreads. 

//Import c++ libraries
#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <string>
#include <sys/time.h>
#include <string.h>
using namespace std;

//Global variable to hold the total number of threads created
#define NUM_THREADS 3
int wait = 0;

/**
* firstThread thread to deal with the first half of the filenames and filter the data
*/
void *firstThread(void *rank)
{
	cout << "Start of the first thread" << endl;
	//const char *file = (const char *)rank;

  //array to hold the names of first half of the files
	const char *files[39] = {
	"certdata/2013-01-08.txt", "certdata/2013-01-09.txt", "certdata/2013-01-10.txt", "certdata/2013-01-11.txt", "certdata/2013-01-25.txt", 
	"certdata/2013-01-28.txt", "certdata/2013-01-29.txt", "certdata/2013-02-01.txt", "certdata/2013-02-07.txt", "certdata/2013-02-14.txt",
	"certdata/2013-02-22.txt", "certdata/2013-03-05.txt", "certdata/2013-03-08.txt", "certdata/2013-03-11.txt", "certdata/2013-03-18.txt",
	"certdata/2013-03-20.txt", "certdata/2013-03-21.txt", "certdata/2013-04-02.txt", "certdata/2013-04-03.txt", "certdata/2013-04-05.txt",
	"certdata/2013-04-08.txt", "certdata/2013-04-10.txt", "certdata/2013-04-15.txt", "certdata/2013-04-19.txt", "certdata/2013-04-25.txt",
	"certdata/2013-04-26.txt", "certdata/2013-04-29.txt", "certdata/2013-04-30.txt", "certdata/2013-05-06.txt", "certdata/2013-05-14.txt",
	"certdata/2013-05-15.txt", "certdata/2013-05-17.txt", "certdata/2013-06-05.txt", "certdata/2013-06-07.txt", "certdata/2013-06-10.txt",
	"certdata/2013-06-11.txt", "certdata/2013-06-18.txt", "certdata/2013-06-26.txt", "certdata/2013-06-27.txt"
	};

  //inFile to read data from the given text files in certdata
  ifstream inFile;
  //outFile to print the results of the filtered words to results_datap1.txt
  ofstream outFile;
  outFile.open("results_datap1.txt");

  //String to the current line in the file
  string words;

  //array to hold all of the noise words
  const char *delwords[34] = {
    "a", "an", "the", "am", "is", "are", "was", "were", "being", "been", 
    "seem", "become", "became", "on", "for", "in", "to", "or", "of", "that", 
    "at", "of", "and", "this", "by", "it", "be", "can", "not", "from",
    "has", "like", "with", "as" };

  //For each of the 39 files which is the first half of all the files
  for(int i=0; i<39;i++)
  {
    //Open the corresponding text file
    inFile.open(files[i]);

    //Read each line of the text file until the end of the file and store it in the string words
    while (getline(inFile, words))
     {

      // remove symbols and numbers from the line
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
      while (token != NULL)
      {
        //Add the token to the list of words
        wordlist.push_back(token);
        token = strtok(NULL, " ");
      }
      
      //Find and remove the noise words
      for (int k=wordlist.size()-1; k>=0; k--) 
      {
        //Indicator to remove
        int removeme = 0;
        //Iterate the wordlist vector looking for each noise word
        for (int n=0;n<34;n++) 
        {
          if (!wordlist[k].compare(delwords[n])) 
          {
            removeme = 1;
          }
        }
        if (removeme == 1) // If the word is a noiseword, remove it from the vector
          wordlist.erase(wordlist.begin()+k);
      }

      //For each word in the wordlist vector, print it to a new line in the results_datap1.txt file
      for (int m=0;m<wordlist.size();m++)
        outFile << wordlist[m] << endl;
    }
    //Close the file that the words were being read from in order to open the next file
    inFile.close();
  }
  //Close the file that the words were being printed to "results_datap1.txt"
  outFile.close();

  //Indicate that the thread is finished
  cout << "End of first thread" << endl;

  //Increment the wait counter
  wait++;
  pthread_exit(NULL);
}

/**
* secondThread thread to deal with the second half of the filenames and filter the data
*/
void *secondThread(void *rank)
{
  //Indicate the second thread has started
	cout << "Start of second thread" << endl;

  //array to hold the names of the second half of the filenames
	const char *files[39] = { "certdata/2013-06-28.txt",
	"certdata/2013-07-12.txt", "certdata/2013-07-15.txt", "certdata/2013-07-26.txt", "certdata/2013-08-01.txt", "certdata/2013-08-02.txt",
	"certdata/2013-08-05.txt", "certdata/2013-08-15.txt", "certdata/2013-08-21.txt", "certdata/2013-08-23.txt", "certdata/2013-08-26.txt",
	"certdata/2013-08-30.txt", "certdata/2013-09-03.txt", "certdata/2013-09-04.txt", "certdata/2013-09-13.txt", "certdata/2013-09-23.txt",
	"certdata/2013-09-24.txt", "certdata/2013-10-01.txt", "certdata/2013-10-04.txt", "certdata/2013-10-10.txt", "certdata/2013-10-15.txt",
	"certdata/2013-10-16.txt", "certdata/2013-10-17.txt", "certdata/2013-10-18.txt", "certdata/2013-10-22.txt", "certdata/2013-10-25.txt",
	"certdata/2013-10-28.txt", "certdata/2013-10-30.txt", "certdata/2013-10-31.txt", "certdata/2013-11-04.txt", "certdata/2013-11-07.txt",
	"certdata/2013-11-14.txt", "certdata/2013-11-18.txt", "certdata/2013-11-22.txt", "certdata/2013-12-02.txt", "certdata/2013-12-03.txt",
	"certdata/2013-12-05.txt", "certdata/2013-12-12.txt", "certdata/2013-12-30.txt"
	};

  //inFile to read data from the given text files in certdata
  ifstream inFile;
  //outFile to print the results of the filtered words to results_datap2.txt
  ofstream outFile;
  outFile.open("results_datap2.txt");

  //String to the current line in the file
  string words;

  //array to hold all of the noise words
  const char *delwords[34] = {
    "a", "an", "the", "am", "is", "are", "was", "were", "being", "been", 
    "seem", "become", "became", "on", "for", "in", "to", "or", "of", "that", 
    "at", "of", "and", "this", "by", "it", "be", "can", "not", "from",
    "has", "like", "with", "as" };

  //For each of the 39 files which is the first half of all the files
  for(int i=0; i<39;i++){
    //Open the corresponding text file
    inFile.open(files[i]);

    //Read each line of the text file until the end of the file and store it in the string words
    while (getline(inFile, words)) {

      // remove symbols and numbers
      for (int j=0; j<words.size(); j++)
        if (!isalpha(words[j])) {
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
      while (token != NULL)
      {
        //Add the token to the list of words
        wordlist.push_back(token);
        token = strtok(NULL, " ");
      }
      
      //Find and remove the noise words
      for (int k=wordlist.size()-1; k>=0; k--) 
      {
        //Indicator to remove
        int removeme = 0;
        //Iterate the wordlist vector looking for each noise word
        for (int n=0;n<34;n++) 
        {
          if (!wordlist[k].compare(delwords[n])) 
          {
            removeme = 1;
          }
        }
        if (removeme == 1) // If the word is a noiseword, remove it from the vector
          wordlist.erase(wordlist.begin()+k);
      }

      //For each word in the wordlist vector, print it to a new line in the results_datap2.txt file
      for (int m=0;m<wordlist.size();m++)
        outFile << wordlist[m] << endl;
    }
    //Close the file that the words were being read from in order to open the next file
    inFile.close();
  }
  //Close the file that the words were being printed to "results_datap2.txt"
  outFile.close();

  //Indicate end of the second thread
  cout << "End of second thread" << endl;

  //Increment the wait counter
  wait++;
  pthread_exit(NULL);
}


/**
* merge thread to deal with combining the results from the first two threads each handling half of the text files
* This thread also finds the frequency use of each word (1)
*/
void *merge(void *rank)
{
  //Wait to start the merge until the first two halves of the text files have been filtered and sent to their corresponding text files
	while(wait!=2)
	{

	}

  //Indicate merge thread is starting
	cout << "Start of merge thread" << endl;

  //outFile to write final summary of frequency to and inFile to read in the words from the first two threads
	ofstream outFile;
	ifstream inFile;

  //word to hold the word on each line of the results from the previous two threads and stored in the results vector
	string word;
	vector<string> results;

  //Open the final summary document to be written to
	outFile.open("finalreport_datap.txt");
  //Open the first half of the filtered words from the first threads results
	inFile.open("results_datap1.txt");

  //Read until end of file, first threads results
	while(getline(inFile, word))
	{
    //Push each word to the results wordvector
		results.push_back(word);
	}

  //Close the first half of the words textfile
	inFile.close();

  //Open the second half of the filtered words from the second threads results
	inFile.open("results_datap2.txt");

  //Read until end of file, second threads results
	while(getline(inFile, word))
	{
    //Push each word to the results wordvector
		results.push_back(word);
	}

  //Create a map of a string and an integer value
  //The string holds the word and the integer is the frequency
	map<string, int> M;

  //Iterate through each word in the vector of all words
	for(int i=0; i<results.size(); i++)
	{
  	if(M.find(results[i]) == M.end())
  	{
      //If the word is only found once is end of map then the count is one
  	  M[results[i]] = 1;
  	}
  	else
  	{
      //Increment the count of the frequency in the map
  	  M[results[i]]++;
  	}
	}

  //Size is the total amount of words found
  int size = results.size();
  //tenp is ten percent of all the words found
  int tenp = ceil(size*.1);

  //Create and iterator to go through each word in the map
	for(auto& it:M)
	{
    //If the frequency of the word is more than 10 send it do the final report summary
  	if(it.second > 10)
  	  outFile << it.first << ' ' << it.second << '\n';
	}

  //Indicate end of thread
	cout << "End of merge thread" << endl;
	pthread_exit(NULL);
}

/**
 * Main function that deals with creating and joining the threads as well as determining time to complete program. (4)
 */
int main()
{
  //Struct to hold the variables in order to find completion time
	struct timeval start, end;
  //Get the time the program starts
	gettimeofday(&start, NULL);

	pthread_t ids[NUM_THREADS];

	//Creating the first two threads to deal with half of the text files each
	cout << "Creating the first thread that calls firstThread" << endl;
	pthread_create(&ids[0], NULL, firstThread, (void *)0);
	cout << "Creating the second thread that calls secondThread" << endl;
	pthread_create(&ids[1], NULL, secondThread, (void *)1);

	//Waiting for the first two threads to complete
	for(int i=0; i<2; i++)
	{
		pthread_join(ids[i], NULL);
	}

	//Creating a thread to merge the results of the previous two threads
	cout << "Creating a third thread to merge results" << endl;
	pthread_create(&ids[2], NULL, merge, (void *)2);
	//Complete the third thread
  pthread_join(ids[2], NULL);

  //Indicate the program is complete and time will now be calculated
	cout << "Process is complete" << endl;

  //Get the time the program finishes at
	gettimeofday(&end, NULL);

  //Calculate the time that was taken for the program to complete and print out the results (3)
 	int micro_end = end.tv_sec * 1000000 + end.tv_usec;
 	int micro_start = start.tv_sec * 1000000 + start.tv_usec;
  cout << "Total time for data parallel execution: " << (micro_end - micro_start) << " microseconds" << endl;
  
	return 0;
}

/** Sources - Project 1 Data Parallel Implementation
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
*/


/** Additional Explanation if Needed
* Data parallel is when the data as a whole is broken up among different threads and operations
* are preformed on them at the same time and then the main thread brings each part together in 
* the end once each thread has finished its part. For example in this part of the project,
* I broke up the files into two parts and gave each thread half of the files to operation on. 
* The threads each do the same thing to the data, but they run at the same time and once each
* is complete with their portion of the data, a main thread is able to join the resulting data
* into one summary file and I also have the main thread reporting on the frequency of the words
* as well. 
*/