# [IMS - Modelling and Simulation](https://www.fit.vut.cz/study/course/13998/)
## Epidemiologické modely pomocí celulárních automatů - COVID-19 probabilictic cellular automaton model
**Obtained points**:<br>
20/20

Model of COVID-19 using probabilictic cellular automaton. 
* [Source paper](https://github.com/MisoKov/fit-ims/blob/main/source%20research/2006.11270v1.pdf)
* [Documentation](https://github.com/MisoKov/fit-ims/blob/main/doc/xkoval17.pdf)

### Compile:
	make compile
### Run:
	make run
### Parameters:
Parameters of simulation need to be changed in source files `pca.hpp` to change constants. <br>
New simulations with their own parameters need to be added to `main()` in `main.cpp`.
### Visualization:
To turn on terminal ascii art visualization uncomment `line 291` in `pca.cpp`.

![cell grid](https://github.com/MisoKov/fit-ims/blob/main/doc/cell_grid.PNG)


