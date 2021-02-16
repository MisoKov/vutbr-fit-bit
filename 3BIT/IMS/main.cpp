/**
 *	@file	main.cpp
 *	@author	Michal Koval, xkoval17 FIT
 *	@date	1.12.2020
 *	@brief	Main cpp file, from here experiments are called
 *	@note	Compiler: gcc 7.5
 */

#include "main.hpp"

using namespace std;


int main() {
    cout << "Low density 1 distance" << endl;
    CellularAutomata low_density_d1(100 ,1, 0.5, 0.5, 0.1, 0.12, 0.2, 8, 2, 18, 50, 2);
    low_density_d1.run();
    low_density_d1.clear();
    cout << "High density 1 distance" << endl;
    CellularAutomata high_density_d1(100 ,1, 0.5, 0.5, 0.1, 0.12, 0.7, 8, 2, 18, 50, 2);
    high_density_d1.run();
    high_density_d1.clear();
    cout << "Low density 2 distance" << endl;
    CellularAutomata low_density_d2(100 ,2, 0.5, 0.5, 0.1, 0.12, 0.2, 8, 2, 18, 50, 2);
    low_density_d2.run();
    low_density_d2.clear();
    cout << "Low density 2 distance" << endl;
    CellularAutomata high_density_d2(100 ,2, 0.5, 0.5, 0.1, 0.12, 0.7, 8, 2, 18, 50, 2);
    high_density_d2.run();
    high_density_d2.clear();
    cout << "Low pe" << endl;
    CellularAutomata low_pe(100 ,1, 0.2, 0.5, 0.1, 0.12, 0.46, 8, 2, 18, 50, 2);
    low_pe.run();
    low_pe.clear();
    cout << "High pe" << endl;
    CellularAutomata high_pe(100 ,1, 0.5, 0.5, 0.1, 0.12, 0.46, 8, 2, 18, 50, 2);
    high_pe.run();
    high_pe.clear();
    cout << "Low pi" << endl;
    CellularAutomata low_pi(100 ,1, 0.5, 0.2, 0.1, 0.12, 0.46, 8, 2, 18, 50, 2);
    low_pi.run();
    low_pi.clear();
    cout << "Low pi" << endl;
    CellularAutomata high_pi(100 ,1, 0.5, 0.5, 0.1, 0.12, 0.46, 8, 2, 18, 50, 2);
    high_pi.run();
    high_pi.clear();
    /// add more simulations, template: 
    /*
        cout << "name of simulation" << endl;
        CellularAutomata simName(timeLim, distance, pe, pi, pq, pr, density, ti, tq, tr, eint, iint);
        simName.run();
        simName.clear();
    */
    return 0;
}