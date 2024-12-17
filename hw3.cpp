/*
 Name: Nathan Hubbell
 Email: nchubbell@crimson.ua.edu
 Course Section: Fall 2024 CS 201
 Homework #: 3
*/
// To Compile: g++ -std=c++20 HW3Sample.cpp
// To Run: ./a.out dbfile1.txt query.txt

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <unordered_map>
#include <vector>
#include <chrono>

using namespace std;

int main(int argc, char *argv[]) {
	auto totalStart = std::chrono::high_resolution_clock::now(); // begin total time
	if (argc != 3) {
	   cout << "Usage: " << argv[0] << " <db file> <query file>" << endl;
	   exit(-1);
	}

	string line, name;
	regex delim("/");
	ifstream dbfile(argv[1]);
	if (!dbfile.is_open()) {
	   cout << "Unable to open file: " << argv[1] << endl;
	   exit(-1);
	}
	// create unordered maps
	unordered_map<string, vector<string>> mtoA; // every movie and its actors
	unordered_map<string, vector<string>> atoM; // every actor and ther movies

	cout << "***Reading db file " << argv[1] << "***" << endl;
	auto Readstart = std::chrono::high_resolution_clock::now(); // start reading file time
	while (getline(dbfile, line)) {
	   // parse each line for tokens delimited by "/"
	   auto begin = sregex_token_iterator(line.begin(), line.end(), delim, -1);
	   auto end = sregex_token_iterator();
		auto movie = *begin; // saves the movie name
	   ++begin;

	   for (sregex_token_iterator word = begin; word != end; ++word) { // loops through all actors for one movie
		// inputs the movies and actors into each map
		mtoA[movie].push_back(*word); 
		atoM[*word].push_back(movie);
	   }
	}
	dbfile.close();
	auto Readend = std::chrono::high_resolution_clock::now(); // stop reading time
	cout << "***Done reading db file " << argv[1] << "***" << endl;

	ifstream queryfile(argv[2]);
	if (!queryfile.is_open()) {
	   cout << "Unable to open file: " << argv[2] << endl;
	   exit(-1);
	}

	cout << "***Reading query file " << argv[2] << "***" << endl;
	auto Searchstart = std::chrono::high_resolution_clock::now(); // start searching time
	while (getline(queryfile, name)) {
		if (name.back() != ')' && isalpha(name.back()) == 0) name.pop_back(); // gets rid of miscarraige character
	    cout << name << " ";
		if (name.find(')') != string::npos) { // determines if name is a movie or an actor
			if (mtoA.find(name) == mtoA.end()) { // checks if the movie exists
				std::cout << "Movie not found!" << std::endl; // movie doesn't eist
			} else {
				std::cout << "Actors: " << std::endl;
				// loop that prints out all the actors associated with that movie
				for (const auto& actor : mtoA.find(name)->second) {  
					std::cout << "\t" << actor << std::endl;
				}
			}
		} else {
			if (atoM.find(name) == atoM.end()) { // checks if actor exists
				std::cout << "Actor not found!" << std::endl; // actor doesn't exist
			} else {
				std::cout << "Movies: " << std::endl;
				// loop that prints out all movies the actor was in
				for (const auto& actor : atoM.find(name)->second) {  
					std::cout << "\t" << actor << std::endl;
				}
			}
		}
		cout << endl;
	}
	queryfile.close();
	auto Searchend = std::chrono::high_resolution_clock::now(); // stop search time
	cout << "***Done reading query file " << argv[2] << "***" << endl;

	// these statements calulate the total times to read, search, and do both
	std::chrono::duration<double> Readelapsed = Readend - Readstart; // total time to read
	std::chrono::duration<double> Searchelapsed = Searchend - Searchstart; // total time to search
	auto totalEnd = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> total = totalEnd - totalStart; // total time for program

	cout << "Time to read dbfile is " << Readelapsed.count() << " seconds" << endl; // outputs time to read into database
	cout << "Time to search with query is " << Searchelapsed.count() << " seconds" << endl; // outputs time to search database
	cout << "Total time is  " << total.count() << " seconds" << endl; // outputs total time of the program
	return 0;
}