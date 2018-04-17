#include <iostream>
#include <queue>
#include <functional>
#include <mutex>
#include <vector>
#include "ThreadPool.h"

using namespace std;

ThreadPool::ThreadPool(size_t numThreads){
	running = true;
	threads.reserve(numThreads);
	numIdleThreads = numThreads;
	auto thread_loop = [&](size_t id) {
		while (running)
		{
			mu.lock();
			if (areJobsDone())
			{
				mainThread.notify_one();
			}
			if (!jobQueue.empty())
			{
				numIdleThreads--;
				auto job = jobQueue.front();
				jobQueue.pop();
				mu.unlock();
				job();
				mu.lock();
				numIdleThreads++;
				mu.unlock();
			}
			else
			{
				mu.unlock();
				unique_lock<mutex> mlock(mu);
				//condVar.wait(mlock, [&] { return !(jobQueue.empty() && running); });
				while (jobQueue.empty() && running)
				{
					condVar.wait(mlock);
				}
			}
		}
	};
	for (int i = 0; i < numThreads; i++)
	{
		threads.push_back(thread(thread_loop, i));
	}
}

ThreadPool::~ThreadPool()
{
	running = false; // the loop won't run anymore
	condVar.notify_all(); // notify all other threads so that they stop waiting
	for (thread& t : threads)
	{
		t.join(); // main thread will have to wait for all thread 
	}
}

void ThreadPool::addJob(job_function job)
{
	lock_guard<mutex> lck(mu);
	if (jobQueue.size() == 0)
		condVar.notify_one();
	jobQueue.push(job);
}

void ThreadPool::clearJobs()
{
	queue<job_function> empty;
	// may be deal with the size
	lock_guard<mutex> lock(mu);
	swap(jobQueue, empty);
}

bool ThreadPool::areJobsDone()
{
	return jobQueue.empty() && numIdleThreads == threads.size();
}

void ThreadPool::addJobs(vector<job_function> jobs)
{
	mu.lock();
	//cout << "Adding many jobs!" << endl;
	for(auto job : jobs)
		jobQueue.push(job);
	mu.unlock();
	condVar.notify_all();
}
	
void ThreadPool::waitForJobs()
{
	unique_lock<mutex> lck(mu);
	while (!this->areJobsDone())
		mainThread.wait(lck);
}
