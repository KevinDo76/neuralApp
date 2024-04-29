#include "graph.h"
#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <thread>
#include <chrono>

#define INVIS sf::Color(0,0,0,0)

void worker(bool& threadStop, int selfIndex, std::vector<std::array<int,3>>& threadJobs, graphObj& workingObj, std::vector<std::array<float, 2>>& sampleBuff, std::vector<std::mutex>& mutexLocks) {
	std::cout << "Worker thread started, id: " << selfIndex << "\n";
	while (!threadStop) {
		mutexLocks[selfIndex].lock();
		if (threadJobs[selfIndex][0]) {

			for (int i = threadJobs[selfIndex][1]; i < threadJobs[selfIndex][1] + threadJobs[selfIndex][2]; i++) {
				sampleBuff[i][1] = workingObj.computeValue(sampleBuff[i][0]);
			}
			threadJobs[selfIndex][0] = 0;
			mutexLocks[selfIndex].unlock();
		}
		else {
			mutexLocks[selfIndex].unlock();
		}
	}
	std::cout << "Worker thread ended, id: " << selfIndex << "\n";
}


graph::graph() : threadCount(6), workingObj(sf::Color::Red, nullptr), markerDistX(1), markerDistY(1) {
	std::vector<std::mutex> temp(this->threadCount);
	this->mutexLocks.swap(temp);
	for (int i = 0; i < threadCount; i++) {
		threadJobs.push_back({ 0, 0, 0 });
		mutexLocks[i].lock();
		this->threadsPool.push_back(std::thread(worker, std::ref(this->threadStop), i, std::ref(this->threadJobs), std::ref(this->workingObj), std::ref(this->sampleBuff), std::ref(this->mutexLocks)));
		//job avaliable, start index, work count
	}
	std::cout << "Thread pool initialized\n";
}

graph::~graph() {
	std::cout << "Stopping thread pool\n";
	this->threadStop = true;
	for (int i = 0; i < this->threadsPool.size(); i++) {
		mutexLocks[i].unlock();
		threadsPool[i].join();
	}
}

void graph::insertGraphFunctions(const graphObj& obj) {
	this->graphFunctions.emplace_back(obj);
}

void graph::renderSelf(sf::RenderWindow& win) {
	sf::View windowView = win.getView();
	sf::Vector2f lowBound = sf::Vector2f(windowView.getCenter().x - windowView.getSize().x/2.f, windowView.getCenter().y - windowView.getSize().y / 2.f);
	sf::Vector2f highBound = sf::Vector2f(windowView.getCenter().x + windowView.getSize().x / 2.f, windowView.getCenter().y + windowView.getSize().y / 2.f);
	sf::Vector2f viewPortSize = highBound - lowBound;

	int sampleResolution = std::min(std::max(50,(int)windowView.getSize().x * 18),(int)win.getSize().x * 2);
	//int sampleResolution = 1000;
	//int sampleResolution = win.getSize().x * 2;

	sf::VertexArray XAxis(sf::Lines, 2);
	sf::VertexArray YAxis(sf::Lines, 2);
	sf::VertexArray Marker(sf::Lines, 2);

	//AXIS RENDERING

	XAxis[0].position = sf::Vector2f(lowBound.x, 0);
	XAxis[1].position = sf::Vector2f(highBound.x, 0);

	YAxis[0].position = sf::Vector2f(0, lowBound.y);
	YAxis[1].position = sf::Vector2f(0, highBound.y);

	//GENERATING AXIS UNIT MARKER

	if (!((highBound.y - int(lowBound.y / this->markerDistY) * this->markerDistY) / this->markerDistY > 400)) {
		for (float i = int(lowBound.y / markerDistY) * markerDistY; i < highBound.y; i += markerDistY) {
			Marker[0].position = sf::Vector2f(5 * 0.04f, i);
			Marker[1].position = sf::Vector2f(5 * -0.04f, i);
			win.draw(Marker);
		}
	}

	if (!((highBound.x - int(lowBound.x / this->markerDistX) * this->markerDistX) / this->markerDistX > 400)) {
		for (float i = int(lowBound.x / this->markerDistX) * this->markerDistX; i < highBound.x; i += this->markerDistX) {
			Marker[0].position = sf::Vector2f(i, 5 * 0.04f);
			Marker[1].position = sf::Vector2f(i, 5 * -0.04f);
			win.draw(Marker);
		}
	}

	//CALLING WIN.DRAW

	Marker[0].position = sf::Vector2f(viewPortSize.x / 2.f + lowBound.x, viewPortSize.y / 2 * 0.015 + (lowBound.y + viewPortSize.y/2));
	Marker[1].position = sf::Vector2f(viewPortSize.x / 2.f + lowBound.x, viewPortSize.y / 2 * -0.015 + (lowBound.y + viewPortSize.y/2));
	win.draw(Marker);

	Marker[0].position = sf::Vector2f(viewPortSize.y / 2 * 0.015 + (lowBound.x + viewPortSize.x / 2), viewPortSize.y / 2.f + lowBound.y);
	Marker[1].position = sf::Vector2f(viewPortSize.y / 2 * -0.015 + (lowBound.x + viewPortSize.x / 2), viewPortSize.y / 2.f + lowBound.y);
	win.draw(Marker);

	win.draw(XAxis);
	win.draw(YAxis);

	// LINE GRAPH RENDERING

	for (int graphInstanceIndex = 0; graphInstanceIndex < this->graphFunctions.size(); graphInstanceIndex++) {

		sampleBuff.clear();
		std::vector<std::thread> threads;
		//initalizing sampleBuffer
		//using double to fix zoom problem
		for (double i = lowBound.x; i <= highBound.x; i += (highBound.x - lowBound.x) / sampleResolution) {
			sampleBuff.push_back({ (float)i, 0 });
			if (sampleBuff.size() > sampleResolution * 2) {
				break;
			}
		}

		int samplePerThread = (int)sampleBuff.size() / this->threadCount;
		
		int startIndex = 0;
		this->workingObj = this->graphFunctions[graphInstanceIndex];
		for (int i = 0; i < this->threadCount; i++) {
			int additional = 0;
			if (i == this->threadCount - 1) {
				additional = (int)sampleBuff.size() - (threadCount * samplePerThread);
			}
			threadJobs[i][2] = samplePerThread + additional;
			threadJobs[i][1] = startIndex;
			threadJobs[i][0] = 1;
			this->mutexLocks[i].unlock();
			startIndex += samplePerThread;
		}

		for (int i = 0; i < this->threadCount; i++) {
			while (this->threadJobs[i][0]) {}

			this->mutexLocks[i].lock();
		}


		//std::cout << samplePerThread << " " << (float)sampleBuff.size() / (float)samplePerThread << "\n";



		sf::VertexArray graphLine(sf::LinesStrip, (sampleBuff.size() - 1) * 2);

		int c = 0;
		for (int i = 0; i < sampleBuff.size() - 1; i++) {
			float p1 = sampleBuff[i][1];
			float p2 = sampleBuff[i + 1][1];
			if (i > sampleResolution * 2 - 1) { break; }
			
			graphLine[c].position = sf::Vector2f(sampleBuff[i][0], -p1);
			graphLine[c].color = this->graphFunctions[graphInstanceIndex].lineColor;
			c++;
			graphLine[c].position = sf::Vector2f(sampleBuff[i + 1][0], -p2);
			graphLine[c].color = this->graphFunctions[graphInstanceIndex].lineColor;
			c++;

			if (std::fabs(p1 - p2) > 50) { graphLine[c - 2].color = INVIS; graphLine[c - 1].color = INVIS; }
		}

		win.draw(graphLine);
	}
};

//graph obj

graphObj::graphObj(sf::Color color, float (*graphFunc)(float)) : graphFuncPointer(graphFunc), lineColor(color) {}

graphObj::~graphObj() {}

double graphObj::computeValue(double i) {
	return this->graphFuncPointer(i);
}