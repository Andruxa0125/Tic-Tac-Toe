#include <iostream>
#include <queue>
#include <functional>
#include <mutex>
#include <vector>

using namespace std;

typedef function<void(void)> job_function;

class ThreadPool
{
private:
	queue<job_function> jobQueue;
	std::mutex mutex;
	vector<thread> threads;
	bool running;
public:
	virtual ~ThreadPool();
	ThreadPool(size_t size = 5);
	void addJob(job_function job);
	void clearJobs();
};

ThreadPool::ThreadPool(size_t numThreads)
{
	running = true;
	threads.reserve(numThreads);
	//for (int i = 0; i < numThreads; i++)
	//	threads.push_back(thread);
}

ThreadPool::~ThreadPool()
{
	running = false; // the loop won't run anymore
	for (thread& t : threads)
	{
		t.join(); // main thread will have to wait for all thread 
	}
}

void ThreadPool::addJob(job_function job)
{
	mutex.lock();
	jobQueue.push(job);
	mutex.unlock();
}

void ThreadPool::clearJobs()
{
	queue<job_function> empty;
	// may be deal with the size
	mutex.lock();
	swap(jobQueue, empty);
	mutex.unlock();
}