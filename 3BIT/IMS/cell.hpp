/**
 *	@file	cell.hpp
 *	@author	Michal Koval, xkoval17 FIT
 *	@date	1.12.2020
 *	@brief	Header file of Cell object
 *	@note	Compiler: gcc 7.5
 */

#ifndef CELL_HPP
#define CELL_HPP

#include <cstdlib>
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

typedef enum {
    NON,
    SUSCEPTIBLE,
    EXPOSED,
    INFECTED,
    QUARANTINED,
    RECOVERED
} States;


class Cell
{
  int xPos;
  int yPos;
  States state;
  int infectionTimer;
  int quarantineTimer;
  int recoveryTimer;
  

public:
  Cell(int x, int y, States s);
  void init(int x, int y, States s);
  States getState();
  void setState(States s);
  void setInfectionTimer(int time);
  int getInfectionTimer();
  void setQuarantineTimer(int time);
  int getQuarantineTimer();
  void setRecoveryTimer(int time);
  int getRecoveryTimer();
  pair<int, int> getCoordinates();
  vector<pair<int, int>> getNeighborsPos(int distance, int xSize, int ySize); 
};

#endif //CELL_HPP