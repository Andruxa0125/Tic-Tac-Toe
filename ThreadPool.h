#pragma once
#include <iostream>
#include <queue>
#include <functional>
#include <mutex>
#include <vector>
#include <condition_variable>

using namespace std;

typedef function<void(void)> job_function;

class ThreadPool
{
private:
	queue<job_function> jobQueue;
	mutex mu;
	vector<thread> threads;
	bool running;
	condition_variable condVar;
	condition_variable mainThread;
	int numIdleThreads;
public:
	virtual ~ThreadPool();
	ThreadPool(size_t size);
	void addJob(job_function job);
	void clearJobs();
	bool areJobsDone();
	void addJobs(vector<job_function> jobs);
	void waitForJobs();
};



