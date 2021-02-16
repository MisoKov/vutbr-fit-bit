/**
 *	@file	pca.hpp
 *	@author	Michal Koval, xkoval17 FIT
 *	@date	1.12.2020
 *	@brief	Implementation of probabilistic/stochastic cellular automata implementing COVID-19 model
 *	@note	Compiler: gcc 7.5
 */

#include "pca.hpp"

using namespace std;

CellularAutomata::CellularAutomata(size_t timeLim, int distance, float pe, float pi, float pq, float pr,
                                  float density, int ti, int tq, int tr, int eint, int iint)
{
    // initializing CA
    this->neighborhoodDistance = distance;
    this->timeLim = timeLim;
    recoverProp = pr;
    recoveryTime = tr;
    infectTime = ti;
    infectProp = pi;
    quarantProp = pq;
    quarantTime = tq;
    exposureProp = pe;
    initialExposed = eint;
    initialInfected = iint;
    xSize = X_SIZE;
    ySize = Y_SIZE;
    currentTime = 0;
    // initializing landscape - grid of cells
    for (size_t x = 0; x < xSize; x++)
    {
        for (size_t y = 0; y < ySize; y++)
        {
            landscape.push_back(new Cell(x, y, NON));
        }
        
    }


    // choosing residences 
    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<> landDistrib(0, xSize*ySize - 1);
 
    if (density > 0){
        residenceCount = density*(xSize*ySize);
    } else {
        residenceCount = RESIDENCES_COUNT;
    }
    vector<int> residences;
    for (int i = 0; i < residenceCount ; )
    {
        //Use `distrib` to transform the random unsigned int generated by gen into an int in [0, xSize*ySize - 1]
        int landInd = landDistrib(gen);
        if (landscape[landInd]->getState() == NON){
            landscape[landInd]->setState(SUSCEPTIBLE);
            residences.push_back(landInd);
            i++;
        }
    }


    // choosing initial infected
    uniform_int_distribution<> resDistrib(0, residences.size());
    for (int i = 0; i < initialInfected ; )
    {
        int resInd = resDistrib(gen);
        if (landscape[residences[resInd]]->getState() == SUSCEPTIBLE){
            landscape[residences[resInd]]->setState(INFECTED);
            landscape[residences[resInd]]->setQuarantineTimer(quarantTime);
            landscape[residences[resInd]]->setRecoveryTimer(recoveryTime);
            i++;
        }
    }
    // choosing initial exposed
    for (int i = 0; i < initialExposed ; )
    {
        int resInd = resDistrib(gen);
        if (landscape[residences[resInd]]->getState() == SUSCEPTIBLE){
            landscape[residences[resInd]]->setState(EXPOSED);
            landscape[residences[resInd]]->setInfectionTimer(infectTime);
            i++;
        }
    }
}

void CellularAutomata::printCellsInfo()
{
    for (size_t x = 0; x < xSize; x++)
    {
        for (size_t y = 0; y < ySize; y++)
        {
            Cell c = *landscape[getOrderFromCoordinates(x, y)];
            cout << "x: " << x << " y: "<<y<<" o: "<< getOrderFromCoordinates(x, y) << " state: " << c.getState()<<endl;
        }
        
    }
}

void CellularAutomata::clear()
{
    for (size_t x = 0; x < xSize; x++)
    {
        for (size_t y = 0; y < ySize; y++)
        {
            delete(landscape[getOrderFromCoordinates(x, y)]); 
        }
        
    }
}

bool CellularAutomata::runCycle()
{
    bool saturation = true;
    vector<Cell*> next_landscape(xSize*ySize);
    for(size_t x = 0; x < xSize; x++)
    {
        for(size_t y = 0; y < ySize; y++)
        {   
            Cell cell = *landscape[getOrderFromCoordinates(x, y)];
            if (cell.getState() == NON){
                next_landscape[getOrderFromCoordinates(x, y)] = new Cell(x, y, NON);
                continue;
            }
            vector<Cell> neighbors = getNeighbors(cell);
            next_landscape[getOrderFromCoordinates(x, y)] = getNextStateForCell(cell, neighbors);
            if (saturation == true){
                if (next_landscape[getOrderFromCoordinates(x, y)]->getState() != 
                    landscape[getOrderFromCoordinates(x, y)]->getState()){
                        saturation = false;
                    }
            }
        }
    }

    for(size_t x = 0; x < xSize; x++)
    {
        for(size_t y = 0; y < ySize; y++)
        {   
            delete(landscape[getOrderFromCoordinates(x, y)]);
        }
    }

    landscape = next_landscape;
    return saturation;
}

void CellularAutomata::run()
{
    if (timeLim > 0){
        int imax = 0;
        for (currentTime = 0; currentTime < timeLim; currentTime++)
        {
            runCycle();
            int i = getInfected();
            if (i > imax){
                imax = i;
            }
            if (currentTime % 10 == 0){
                printStats();
            }
        }
        cout << "End: " << endl;
        cout << "imax : " << imax << "imax %: " << (float)imax/(float)(residenceCount) << endl;
    } else {
        bool saturation = false;
        int time = 0;
        int imax = 0;
        while (saturation != true)
        {
            saturation = runCycle();
            time++;
            int i = getInfected();
            if (i > imax){
                imax = i;
            }

            if (currentTime % 10 == 0){
                printStats();
            }
            
            // HARD END
            if (time == 150){
                cout << "End: " << endl;
                cout << "imax : " << imax << "imax %: " << (float)imax/(float)(residenceCount) << endl;
                return;
            }
            
        }
        cout << "End: " << endl;
        cout << "imax : " << imax << "imax %: " << (float)imax/(float)(residenceCount) << endl;
    }
}


Cell CellularAutomata::getCell(int x, int y)
{
    return *landscape[getOrderFromCoordinates(x, y)];
}

pair<int,int> CellularAutomata::getCoordinatesFromOrder(int order)
{
    int x = order / xSize;
    int y = order - x * xSize;
    return make_pair(x, y); 
}

void CellularAutomata::printLandscape()
{
    for (size_t x = 0; x < xSize; x++)
    {
        for (size_t y = 0; y < ySize; y++)
        {
            States s = landscape[getOrderFromCoordinates(x,y)]->getState();
                if (s == EXPOSED){
                    cout << "\033[1;32m\u25A0\033[0m";
                } else if (s == SUSCEPTIBLE) {
                    cout << "\033[1;34m\u25A0\033[0m";
                } else if (s == QUARANTINED){
                    cout << "\033[1;36m\u25A0\033[0m";
                } else if (s == RECOVERED){
                    cout << "\033[1;33m\u25A0\033[0m";
                } else if (s == INFECTED){
                    cout << "\033[1;31m\u25A0\033[0m";
                } else if (s == NON){
                    cout << "\033[1;30m\u25A0\033[0m";
                } else {
                    cout << "problem";
                }
        }
        cout << endl;
    }
}


int CellularAutomata::getInfected()
{
    int counter = 0;
    for (size_t x = 0; x < xSize; x++)
    {
        for (size_t y = 0; y < ySize; y++)
        {
            States s = landscape[getOrderFromCoordinates(x,y)]->getState();
                if (s == INFECTED){
                    counter++;
                }
        }
    }
    return counter;
}

void CellularAutomata::printStats()
{
    int e = 0;
    int n = 0;
    int su = 0;
    int r = 0;
    int q = 0;
    int i = 0;
    int un = 0;
    for (size_t x = 0; x < xSize; x++)
    {
        for (size_t y = 0; y < ySize; y++)
        {
            States s = landscape[getOrderFromCoordinates(x,y)]->getState();
                if (s == EXPOSED){
                    e++;
                } else if (s == SUSCEPTIBLE) {
                    su++;
                } else if (s == QUARANTINED){
                    q++;
                } else if (s == RECOVERED){
                    r++;
                } else if (s == INFECTED){
                    i++;
                } else if (s == NON) {
                    n++;
                } else {
                    un++;
                }
        }
    }
    cout << "t: " << currentTime << "  |  ";
    cout << "S: " << su << " , " << (float)su/(float)residenceCount << "%  |  ";
    cout << "E: " << e << " , " << (float)e/(float)residenceCount << "%  |  ";
    cout << "I: " << i << " , " << (float)i/(float)residenceCount << "%  |  ";
    cout << "Q: " << q << " , " << (float)q/(float)residenceCount << "%  |  ";
    cout << "R: " << r << " , " << (float)r/(float)residenceCount << "%  |  " << endl;
    //printLandscape();
}

int CellularAutomata::getOrderFromCoordinates(int x, int y)
{
    return (x*xSize) + (y);
}

vector<Cell> CellularAutomata::getNeighbors(Cell cell)
{
    vector<pair<int, int>> neighCoords = cell.getNeighborsPos(neighborhoodDistance, xSize, ySize);
    vector<Cell> neighCells;
    for (size_t i = 0; i < neighCoords.size(); i++)
    {
        Cell c = getCell(neighCoords[i].first, neighCoords[i].second);
        if (c.getState() != NON){
            neighCells.push_back(c);
        }
    }
    return neighCells;
} 


int CellularAutomata::getNumExposedInfected(vector<Cell> neighbors)
{
    int n = 0;
    for (Cell c: neighbors)
    {
        if (c.getState() == INFECTED or c.getState() == EXPOSED){
            n++;
        }
    }
    return n;
}

Cell* CellularAutomata::getNextStateForCell(Cell c, vector<Cell> neighbors)
{
    pair<int, int> coords = c.getCoordinates();
    Cell* next = new Cell(coords.first, coords.second, NON);
    States state = c.getState();
    if (state ==  SUSCEPTIBLE){
        int dangerousNeighbours = getNumExposedInfected(neighbors);
        float currentProb;
        if (dangerousNeighbours){
            currentProb =  1 - pow(1-exposureProp, dangerousNeighbours);
        } else { 
            currentProb = 0;
        }
        if (getBoolFromProbability(currentProb)){

            next->setState(EXPOSED);
            next->setInfectionTimer(infectTime);
            return next;
        }
        next->setState(SUSCEPTIBLE);
        return next;
    } else if (state == EXPOSED){
        int nextTime;
        int currentTime = c.getInfectionTimer();
        if (currentTime <= 0){
            if (getBoolFromProbability(infectProp)){
                next->setState(INFECTED);
                next->setQuarantineTimer(quarantTime);
                next->setRecoveryTimer(recoveryTime);
                return next;
            }
        }
        nextTime = currentTime - 1;
        next->setState(EXPOSED);
        next->setInfectionTimer(nextTime);
        return next;
    } else if (state == INFECTED){
        int currentQTime = c.getQuarantineTimer();
        int currentRTime = c.getRecoveryTimer();
        int nextQTime = currentQTime - 1;
        int nextRTime = currentRTime - 1;
        // Quarantine
        if (currentQTime <= 0){
            if (getBoolFromProbability(quarantProp)){
                next->setState(QUARANTINED);
                next->setRecoveryTimer(nextRTime);
                return next;
            }
        }
        // Recovery
        if (currentRTime > 0){
            next->setState(INFECTED);
            next->setQuarantineTimer(nextQTime);
            next->setRecoveryTimer(nextRTime);
            return next;
        } else {
             if (getBoolFromProbability(recoverProp)){
                next->setState(RECOVERED);
                return next;
            } else {
                next->setState(INFECTED);
                next->setQuarantineTimer(nextQTime);
                next->setRecoveryTimer(nextRTime);
                return next;
            }
        }
    } else if (state == QUARANTINED){
        int currentRTime = c.getRecoveryTimer();
        int nextRTime = currentRTime - 1;
        if (currentRTime > 0){
            next->setState(QUARANTINED);
            next->setRecoveryTimer(nextRTime);
            return next;
        } else {
             if (getBoolFromProbability(recoverProp)){
                next->setState(RECOVERED);
                return next;
            } else {
                next->setState(QUARANTINED);
                next->setRecoveryTimer(nextRTime);
                return next;
            }
        }
    } else {
        // RECOVERED
        next->setState(RECOVERED);
        return next;
    }
}

bool getBoolFromProbability(float probability)
{
    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    bernoulli_distribution bd(probability); // Bernoulli distribution with parameter probability
    return bd(gen);
}
