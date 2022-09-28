//Peri Hassanzadeh
//ECE1570 High Performance Computing
//Date of Creation: 10/16/2021
//Last Update: 11/5/2021
//
//This is the serial version of Project 1, opening text documents, 
//filtering the words, counting the frequency, and summarizing the 
//top 10% of most frequent concepts.

//Import c++ libraries 
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <string>
#include <sys/time.h>
using namespace std;


/**
 * Prints the frequency of the top 10% most frequently used words and sends them to a final report.
 * This method was based off of an online resource that will be listed at the end of this document. (1)
 */
void printFreq()
{
  //Initialize variables 

  //wordVec to hold the words from the results.txt file
  vector<string> wordVec;

  //resultfile to read in the filtered words from a file
  ifstream resultfile;
  //finalreport to send the resulting frequency calculation to the final summary text file
  ofstream finalreport;

  //Open the subsequent files for input and output
  resultfile.open("results.txt");
  finalreport.open("finalreport.txt");

  //String to hold the words from results.txt file
  string words;

  //Until the end of the file, read in each line and store it in words variable
  while(getline(resultfile, words))
  {
    //Put the words from each line into a vector
    wordVec.push_back(words);
  }

  //Create a map of a string and an integer value
  //The string holds the word and the integer is the frequency 
  map<string, int> M;

  //Iterate through each word in the vector of all words
  for(int i=0; i<wordVec.size(); i++)
  {
    if(M.find(wordVec[i]) == M.end())
    {
      //If the word is only found once is end of map then the count is one
      M[wordVec[i]] = 1;
    }
    else
    {
      //Increment the count of the frequency in the map
      M[wordVec[i]]++;
    }
  }
  
  //Size is the total amount of words found
  int size = wordVec.size();
  //tenp is ten percent of all the words found
  int tenp = ceil(size*.1);

  //Create and iterator to go through each word in the map
  for(auto& it:M)
  {
    //If the frequency of the word is more than 10 send it do the final report summary
    if(it.second > 10)
      finalreport << it.first << ' ' << it.second << '\n';
  }
}


/**
 * Main function for the majority of the text analytics. 
 * This method was based off of an online resource that will be listed at the end of this document. (2)
 */
int main()
{
  //Struct to hold the variables in order to find completion time
  struct timeval start, end;
  //Get the time the program starts
  gettimeofday(&start, NULL);

  //inFile to read data from the given text files in certdata
  ifstream inFile;
  //outFile to print the results of the filtered words to results.txt
  ofstream outFile;
  outFile.open("results.txt");

  //wordVec to hold all of the filtered words to go into the results.txt file
  vector<string> wordVec;

  //words to hold each line of the text file 
  string words;
  
  //array to hold the names of all of the files
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

  //Iterate through each text file
  for(int i=0; i<78;i++){
    //Open the corresponding text file
    inFile.open(files[i]);

    //Read each line of the text file until the end of the file and store it in the string words
    while (getline(inFile, words)) {

      // remove symbols and numbers from the line
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
        if (removeme == 1) // If the 
          wordlist.erase(wordlist.begin()+k);
      }

      //For each word in the wordlist vector, print it to a new line in the results.txt file
      for (int m=0;m<wordlist.size();m++)
        outFile << wordlist[m] << endl;
    }
    //Close the file that the words were being read from in order to open the next file
    inFile.close();
  }
  //Close the file that the words were being printed to "results.txt"
  outFile.close();

  //Call the method to print the final summary and frequency of words
  printFreq();

  //Get the time the program finishes at
  gettimeofday(&end, NULL);

  //Calculate the time that was taken for the program to complete and print out the results (3)
  int micro_end = end.tv_sec * 1000000 + end.tv_usec;
  int micro_start = start.tv_sec * 1000000 + start.tv_usec;
  cout << "Total time for serial execution: " << (micro_end - micro_start) << " microseconds" << endl;
  return 0;
}


/** Sources - Project 1 Serial Implementation
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
*/