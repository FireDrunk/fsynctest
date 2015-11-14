#define CONCURRENCY 8

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <thread>
#include <sys/time.h> 
#include <math.h>

using namespace std;

long int getTime()
{
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return tp.tv_sec * 1000 + tp.tv_usec / 1000; 
}

void writeAsync(std::string line, FILE *fileDescriptor)
{
	stringstream ss;
	ss << "[" << getTime() << "]" << "[ASYNC] - " << line << endl;
	fprintf(fileDescriptor, ss.str().c_str());
	ss.str(string());
}

void writeSync(std::string line, FILE *fileDescriptor)
{
	stringstream ss;
	ss << "[" << getTime() << "]" << "[SYNC] - " << line << endl;
	fprintf(fileDescriptor, ss.str().c_str());
	fflush(fileDescriptor);
	ss.str(string());
}

int main(int argc, const char* argv[] ) 
{
	stringstream ss;
	printf("Starting...\n");

	FILE *files[CONCURRENCY];

	for (int x = 0; x < CONCURRENCY; x++)
	{
		// Filename generator
		ss << "tmp/test_" << x << ".txt";

		// Filename
		string filename(ss.str());
		ss.str(string());

		// Open File and add handle to Array
		files[x] = fopen(filename.c_str(), "w+");

		// Write start header
		writeAsync("Started!", files[x]);
	}

	// TODO: Stuff

	// While Time
	// Async Write Time to file a async
	// Sync Write log of action to other file sync
	//
	int max = CONCURRENCY * 128;

	printf("Writing async <-> sync.\n");
	printf("Max: [%d]\n", max);

	for (int x=0; x < max; x++)
	{
		stringstream ss;

		int file_nr = x % CONCURRENCY;
		int log_file_nr = (x+1) % CONCURRENCY;
		int nr_of_writes = rand() % 100 + 1;

		for (int x=0; x <= nr_of_writes; x++)
		{
			printf("[%d/%d] Writing ASYNC in file: [%d]\n", x,nr_of_writes,file_nr);
			writeAsync("*", files[file_nr]);
		}

		printf("[%d/%d] Writing SYNC in file: [%d]\n", x,max,log_file_nr);
		ss << "Written to File: #[" << file_nr << "]";
		writeSync(ss.str(), files[log_file_nr]);

		ss.str(string());

		//this_thread::sleep_for(chrono::milliseconds(200));
	}

	for (int x=0; x < CONCURRENCY; x++)
	{
		writeSync("Synced!", files[x]);
	}

	// Pre exit
	for (int x=0; x < CONCURRENCY; x++)
	{
		// Write Finish
		writeAsync("Finished!", files[x]);

		// Close File handle
		fclose(files[x]);
	}

	// Exit code
	return 0;
}
