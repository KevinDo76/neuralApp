#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>
#include <array>
#include <mutex>
class graphObj 
{
public:
	sf::Color lineColor;
	float (*graphFuncPointer)(float);
	virtual double computeValue(double i);

	
public:
	graphObj(sf::Color color, float (graphFunc)(float));
	~graphObj();
};

class graph
{
public:
	std::vector<graphObj> graphFunctions;
	float markerDistX;
	float markerDistY;
	int threadCount;

	//threadpool stuff
	std::vector<std::thread> threadsPool;
	std::vector<std::array<int, 3>>threadJobs;
	std::vector<std::array<float, 2>> sampleBuff;
	std::vector<std::mutex> mutexLocks;
	graphObj workingObj;
	//thread control variables
	bool threadStop = false;
public:
	graph();
	~graph();
	graph(const graph& g) = delete;

	void renderSelf(sf::RenderWindow& win);
	void insertGraphFunctions(const graphObj& obj);

private:
};

