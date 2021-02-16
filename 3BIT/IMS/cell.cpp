/**
 *	@file	cell.cpp
 *	@author	Michal Koval, xkoval17 FIT
 *	@date	1.12.2020
 *	@brief	Implementation of Cell object representing one point in cellular automata lattice
 *	@note	Compiler: gcc 7.5
 */

#include "cell.hpp"

using namespace std;


Cell::Cell(int x, int y, States s)
{
    xPos = x;
    yPos = y;
    state = s;
    quarantineTimer = 0;
    infectionTimer = 0;
    recoveryTimer = 0;
}

void Cell::init(int x, int y, States s)
{
    xPos = x;
    yPos = y;
    state = s;
}

States Cell::getState()
{
    return state;
}

void Cell::setState(States s)
{
    state = s;
}

vector<pair<int, int>> Cell::getNeighborsPos(int distance, int xSize, int ySize)
{
    vector<pair<int, int>> neighs;
    
    if (distance == 0){
        return neighs;
    }

    // From left
    for (int x = -distance; x <= 0; x++)
    {
        // Check left boundary
        if (xPos + x >= 0){
            // From top
            for (int y = -distance; y <= 0; y++)
            {
                // Check top boundary
                if (yPos + y >= 0){
                    // Check if positions arent of the cell
                    if (!(y == 0 && x == 0)){
                        neighs.push_back(make_pair(xPos + x, yPos + y));
                    }
                }
            }
            // From bottom
            for (int y = distance; y > 0; y--)
            {
                // Check bottom boundary
                if (yPos + y < ySize){
                    neighs.push_back(make_pair(xPos + x, yPos + y));
                }
            }
        }
    }

    // From right
    for (int x = distance; x > 0; x--)
    {
        // Check right boundary
        if (xPos + x < xSize){
            // From top
            for (int y = -distance; y <= 0; y++)
            {
                // Check top boundary
                if (yPos + y >= 0){
                    neighs.push_back(make_pair(xPos + x, yPos + y));
                }
            }
            // From bottom
            for (int y = distance; y > 0; y--)
            {
                // Check bottom boundary
                if (yPos + y < ySize){
                    neighs.push_back(make_pair(xPos + x, yPos + y));
                }
            }
        }
    }
    
    return neighs;
}

pair<int, int> Cell::getCoordinates()
{
    return make_pair(xPos, yPos);
}

void Cell::setInfectionTimer(int time)
{
    infectionTimer = time;
}

int Cell::getInfectionTimer()
{
    return infectionTimer;
}

void Cell::setQuarantineTimer(int time)
{
    quarantineTimer = time;
}
int Cell::getQuarantineTimer()
{
    return quarantineTimer;
}
void Cell::setRecoveryTimer(int time)
{
    recoveryTimer = time;
}
int Cell::getRecoveryTimer()
{
    return recoveryTimer;
}