/**
 *	@file	pca.hpp
 *	@author	Michal Koval, xkoval17 FIT
 *	@date	1.12.2020
 *	@brief	Header file representing probability/stochastic cellular automata
 *	@note	Compiler: gcc 7.5
 */

#ifndef PCA_HPP
#define PCA_HPP

#include "cell.hpp"
#include <utility>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <random>

#define X_SIZE 200
#define Y_SIZE 200
#define RESIDENCES_COUNT 25000
#define INFECTION_TIME 8

using namespace std;

class CellularAutomata
{
  vector<Cell*> landscape;
  size_t xSize;
  size_t ySize;
  int neighborhoodDistance;
  size_t timeLim;
  float exposureProp;
  float infectProp;
  float quarantProp;
  float recoverProp;
  float popDensity;
  int infectTime;
  int quarantTime;
  int recoveryTime;
  int initialExposed;
  int initialInfected;
  int residenceCount;
  size_t currentTime;

public:
  CellularAutomata(size_t timeLim, int distance, float pe, float pi, float pq, float pr, 
                  float density, int ti, int tq, int tr, int eint, int iint);
  pair<int, int> getCoordinatesFromOrder(int order);
  int getOrderFromCoordinates(int x, int y);
  Cell getCell(int x, int y);
  vector<Cell> getNeighbors(Cell cell);
  void printLandscape();
  void printStats();
  int getNumExposedInfected(vector<Cell> neighbors);
  Cell* getNextStateForCell(Cell c, vector<Cell> neighbors);
  bool runCycle();
  void run();
  void clear();
  void printCellsInfo();
  int getInfected();
};

bool getBoolFromProbability(float probability);

#endif //PCA_HPP