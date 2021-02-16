//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     MICHAL KOVAL <xkoval17@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author MICHAL KOVAL
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include <iostream>
#include "gtest/gtest.h"
#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech 
//    matic.
//============================================================================//

class MatrixTest : public ::testing::Test
{
protected:
	Matrix matrix;
	
	Matrix mat1x1(){
		Matrix matrix = Matrix(1,1);
		matrix.set(std::vector<std::vector<double>> {{42}});
		return matrix;	
	}
	Matrix mat2x4(){
		Matrix matrix = Matrix(2,4);
		matrix.set(std::vector<std::vector<double>> {	{1,2,3,4},
														{-1,-2,-3,-4}	});
		return matrix;
		
	}
	Matrix mat4x3(){
		Matrix matrix = Matrix(4,3);
		matrix.set(std::vector<std::vector<double>> {	{1,2,3},
														{-1,-2,-3},
														{4,5,6},
														{-4,-5,-6}	});	
		return matrix;
	}
	Matrix mat5x5(){
		Matrix matrix = Matrix(5,5);
		matrix.set(std::vector<std::vector<double>> {	{1,2,3,4,5},
														{-1,-2,-3,-4,-5},
														{0,3,5,7,8},
														{6,7,8,9,0},
														{0,1,9,2,8}	});	
		return matrix;
	}
};
TEST_F (MatrixTest, Construction)
{
	// prazdna matica 1x1 {{0}}
	EXPECT_NO_THROW(Matrix());

	// matica RxC
	EXPECT_NO_THROW(Matrix(1,1));
	EXPECT_NO_THROW(Matrix(4,2));
	EXPECT_NO_THROW(Matrix(2,4));
	EXPECT_NO_THROW(Matrix(42,42));
	EXPECT_NO_THROW(Matrix(42, 3));
	EXPECT_NO_THROW(Matrix(3,42));
	EXPECT_NO_THROW(Matrix(420,420));
	EXPECT_NO_THROW(Matrix(4222,4222));
	
    // zaporne rozmery
	EXPECT_ANY_THROW(Matrix(-1,-1));
	EXPECT_ANY_THROW(Matrix(-3, 3));
	EXPECT_ANY_THROW(Matrix(5, -5));
	// nulove rozmery
	EXPECT_ANY_THROW(Matrix(0,0));
	EXPECT_ANY_THROW(Matrix(0,42));
	EXPECT_ANY_THROW(Matrix(42,0));		
}
TEST_F(MatrixTest, Set1Value_1x1)
{
	Matrix matrix = mat1x1();
	// v rozsahu matice
	EXPECT_TRUE(matrix.set(0,0,1));
	EXPECT_DOUBLE_EQ(1,matrix.get(0,0));
	EXPECT_TRUE(matrix.set(0,0,-1));
	EXPECT_DOUBLE_EQ(-1,matrix.get(0,0));
	EXPECT_TRUE(matrix.set(0,0,42.24));
	EXPECT_DOUBLE_EQ(42.24,matrix.get(0,0));
	EXPECT_TRUE(matrix.set(0,0,-21.12));
	EXPECT_DOUBLE_EQ(-21.12,matrix.get(0,0));
	EXPECT_TRUE(matrix.set(0,0,9012903901));
	EXPECT_DOUBLE_EQ(9012903901,matrix.get(0,0));
	EXPECT_TRUE(matrix.set(0,0,-9012903901));
	EXPECT_DOUBLE_EQ(-9012903901,matrix.get(0,0));
	
	// mimo rozsah matice
	EXPECT_FALSE(matrix.set(0,1,1));
	EXPECT_FALSE(matrix.set(1,0,1));
	EXPECT_FALSE(matrix.set(1,1,1));
	EXPECT_FALSE(matrix.set(3,7,1));
}
TEST_F(MatrixTest, Set1Value_2x4)
{
	Matrix matrix = mat2x4();
	// v rozsahu matice
	EXPECT_TRUE(matrix.set(0,0,1));
	EXPECT_DOUBLE_EQ(1,matrix.get(0,0));
	EXPECT_TRUE(matrix.set(0,1,-1));
	EXPECT_DOUBLE_EQ(-1,matrix.get(0,1));
	EXPECT_TRUE(matrix.set(0,3,42.24));
	EXPECT_DOUBLE_EQ(42.24,matrix.get(0,3));
	EXPECT_TRUE(matrix.set(1,0,-21.12));
	EXPECT_DOUBLE_EQ(-21.12,matrix.get(1,0));
	EXPECT_TRUE(matrix.set(1,2,9012903901));
	EXPECT_DOUBLE_EQ(9012903901,matrix.get(1,2));
	EXPECT_TRUE(matrix.set(1,3,-9012903901));
	EXPECT_DOUBLE_EQ(-9012903901,matrix.get(1,3));
	
	// mimo rozsah matice
	EXPECT_FALSE(matrix.set(2,0,1));
	EXPECT_FALSE(matrix.set(2,4,1));
	EXPECT_FALSE(matrix.set(0,4,1));
	EXPECT_FALSE(matrix.set(3,7,1));
}
TEST_F(MatrixTest, Set1Value_4x3)
{
	Matrix matrix = mat4x3();
	// v rozsahu matice
	EXPECT_TRUE(matrix.set(0,0,1));
	EXPECT_DOUBLE_EQ(1,matrix.get(0,0));
	EXPECT_TRUE(matrix.set(0,2,-1));
	EXPECT_DOUBLE_EQ(-1,matrix.get(0,2));
	EXPECT_TRUE(matrix.set(1,1,42.24));
	EXPECT_DOUBLE_EQ(42.24,matrix.get(1,1));
	EXPECT_TRUE(matrix.set(2,0,-21.12));
	EXPECT_DOUBLE_EQ(-21.12,matrix.get(2,0));
	EXPECT_TRUE(matrix.set(3,0,9012903901));
	EXPECT_DOUBLE_EQ(9012903901,matrix.get(3,0));
	EXPECT_TRUE(matrix.set(3,2,-9012903901));
	EXPECT_DOUBLE_EQ(-9012903901,matrix.get(3,2));
	
	// mimo rozsah matice
	EXPECT_FALSE(matrix.set(0,3,1));
	EXPECT_FALSE(matrix.set(4,0,1));
	EXPECT_FALSE(matrix.set(4,3,1));
	EXPECT_FALSE(matrix.set(13,7,1));
}
TEST_F(MatrixTest, Set1Value_5x5)
{
	Matrix matrix = mat5x5();
	// v rozsahu matice
	EXPECT_TRUE(matrix.set(0,0,1));
	EXPECT_DOUBLE_EQ(1,matrix.get(0,0));
	EXPECT_TRUE(matrix.set(1,1,-1));
	EXPECT_DOUBLE_EQ(-1,matrix.get(1,1));
	EXPECT_TRUE(matrix.set(2,2,42.24));
	EXPECT_DOUBLE_EQ(42.24,matrix.get(2,2));
	EXPECT_TRUE(matrix.set(3,3,-21.12));
	EXPECT_DOUBLE_EQ(-21.12,matrix.get(3,3));
	EXPECT_TRUE(matrix.set(4,4,9012903901));
	EXPECT_DOUBLE_EQ(9012903901,matrix.get(4,4));
	EXPECT_TRUE(matrix.set(2,3,-9012903901));
	EXPECT_DOUBLE_EQ(-9012903901,matrix.get(2,3));
	
	// mimo rozsah matice
	EXPECT_FALSE(matrix.set(0,5,1));
	EXPECT_FALSE(matrix.set(5,0,1));
	EXPECT_FALSE(matrix.set(5,5,1));
	EXPECT_FALSE(matrix.set(13,7,1));
}
TEST_F(MatrixTest, SetMatrix1x1)
{
	Matrix matrix = mat1x1();
	EXPECT_TRUE(matrix.set(std::vector<std::vector<double>> {{0}}));
	EXPECT_TRUE(matrix.set(std::vector<std::vector<double>> {{-1}}));
	EXPECT_TRUE(matrix.set(std::vector<std::vector<double>> {{42.24}}));
	EXPECT_TRUE(matrix.set(std::vector<std::vector<double>> {{-42.24}}));

	
	// mimo rozsah matice
	EXPECT_FALSE(matrix.set(std::vector<std::vector<double>> {{0},{0}}));
	EXPECT_FALSE(matrix.set(std::vector<std::vector<double>> {{1,2}}));
	EXPECT_FALSE(matrix.set(std::vector<std::vector<double>> {{3},{4},{5}}));
	EXPECT_FALSE(matrix.set(std::vector<std::vector<double>> {{6,7,8}}));
	
}
TEST_F(MatrixTest, SetMatrix2x4)
{
	Matrix matrix = mat2x4();
	EXPECT_TRUE(matrix.set(std::vector<std::vector<double>> {{0,0,0,0},
															 {1,1,1,1}}));
	EXPECT_TRUE(matrix.set(std::vector<std::vector<double>> {{-1,-1,-1,2},
															 {-3,-3,-3,4}}));
	EXPECT_TRUE(matrix.set(std::vector<std::vector<double>> {{1.1,2.2,3.3,4.4},
															 {1.1,2.2,3.3,4.4}}));
	EXPECT_TRUE(matrix.set(std::vector<std::vector<double>> {{-1.1,2.2,-3.3,4.4},
															 {-1.1,-2.2,-3.3,-4.4}}));
	
	// mimo rozsah matice
	EXPECT_FALSE(matrix.set(std::vector<std::vector<double>> {{0}}));
	EXPECT_FALSE(matrix.set(std::vector<std::vector<double>> {{1},{1}}));
	EXPECT_FALSE(matrix.set(std::vector<std::vector<double>> {{1,2,3,4}}));
	EXPECT_FALSE(matrix.set(std::vector<std::vector<double>> {{0},{1,2,3,4}}));
}
TEST_F(MatrixTest, SetMatrix4x3)
{
	Matrix matrix = mat4x3();
	EXPECT_TRUE(matrix.set(std::vector<std::vector<double>> {{0,0,0},
															 {1,1,1},
															 {2,2,2},
															 {3,3,3}}));
	EXPECT_TRUE(matrix.set(std::vector<std::vector<double>> {{-1,-1,-1},
															 {-2,-2,-2},
															 {-3,-3,-3},
															 {-4,-4,-4}}));
	EXPECT_TRUE(matrix.set(std::vector<std::vector<double>> {{1.1,1.2,1.3},
															 {2.1,2.2,2.3},
															 {3.1,3.2,3.3},
															 {4.1,4.2,4.3}}));
	EXPECT_TRUE(matrix.set(std::vector<std::vector<double>> {{-1.1,-1.2,1.3},
															 {2.1,2.2,-2.3},
															 {-3.1,3.2,3.3},
															 {-4.1,4.2,-4.3}}));
	
	// mimo rozsah matice
	EXPECT_FALSE(matrix.set(std::vector<std::vector<double>> {{0}}));
	EXPECT_FALSE(matrix.set(std::vector<std::vector<double>> {{1,1,1},
															 {2,2,2},
															 {3,3,3}}));
	EXPECT_FALSE(matrix.set(std::vector<std::vector<double>> {{1},{2},{3},{4}}));
	EXPECT_FALSE(matrix.set(std::vector<std::vector<double>> {{2,1},
															  {4,2,4},
															  {5,2,1},
															  {5},
															  {5,3,1,3,5}}));
}
TEST_F(MatrixTest, SetMatrix5x5)
{
	Matrix matrix = mat5x5();
	EXPECT_TRUE(matrix.set(std::vector<std::vector<double>> {{0,0,0,0,0},
															 {1,1,1,1,1},
															 {2,2,2,2,2},
															 {3,3,3,3,3},
															 {4,4,4,4,4}}));
	EXPECT_TRUE(matrix.set(std::vector<std::vector<double>> {{-10,-10,-10,-10,-10},
															 {-1,-1,-1,-1,-1},
															 {-2,-2,-2,-2,-2},
															 {-3,-3,-3,-3,-3},
															 {-4,-4,-4,-4,-4}}));
	EXPECT_TRUE(matrix.set(std::vector<std::vector<double>> {{0.1,0.2,0.3,0.4,0.4},
															 {1.1,1.2,1.3,1.4,1.5},
															 {2.1,2.2,2.3,2.4,2.5},
															 {3.1,3.2,3.3,3.4,3.5},
															 {4.1,4.2,4.3,4.4,4.5}}));
	EXPECT_TRUE(matrix.set(std::vector<std::vector<double>> {{-0.1,0.2,0.3,-0.4,0.4},
															 {1.1,-1.2,1.3,1.4,1.5},
															 {2.1,-2.2,2.3,2.4,2.5},
															 {3.1,3.2,3.3,-3.4,3.5},
															 {4.1,-4.2,4.3,4.4,4.5}}));
	
	// mimo rozsah matice
	EXPECT_FALSE(matrix.set(std::vector<std::vector<double>> {{0}}));
	EXPECT_FALSE(matrix.set(std::vector<std::vector<double>> {{1},{2},{3},{4},{5}}));
	EXPECT_FALSE(matrix.set(std::vector<std::vector<double>> {{1,2,3},{4,5,6},{7,8,9},{0,1,2},{3,4,5},{6,7,8}}));
	EXPECT_FALSE(matrix.set(std::vector<std::vector<double>> {{1,2,3,4,5,6},{2,3},{4,5},{6,7},{8,9}}));
}
TEST_F(MatrixTest, Get1x1)
{
	Matrix matrix = mat1x1();
	// ziskanie hodnoty v rozsahu
	EXPECT_NO_THROW(matrix.get(0,0));
	EXPECT_DOUBLE_EQ(42,matrix.get(0,0));
	
	matrix.set(0,0,-5.4321);
	EXPECT_DOUBLE_EQ(-5.4321,matrix.get(0,0));
	matrix.set(std::vector<std::vector<double>> {{-1}});
	EXPECT_DOUBLE_EQ(-1,matrix.get(0,0));
	
	// ziskanie hodnoty mimo rozsahu
	EXPECT_ANY_THROW(matrix.get(1,0));
	EXPECT_ANY_THROW(matrix.get(0,1));
	EXPECT_ANY_THROW(matrix.get(1,1));
	EXPECT_ANY_THROW(matrix.get(-10,10));
}
TEST_F(MatrixTest, Get2x4)
{
	Matrix matrix = mat2x4();
	// ziskanie hodnoty v rozsahu
	EXPECT_NO_THROW(matrix.get(0,0));
	EXPECT_DOUBLE_EQ(1,matrix.get(0,0));
	EXPECT_NO_THROW(matrix.get(1,2));
	EXPECT_DOUBLE_EQ(-3,matrix.get(1,2));
	
	matrix.set(0,0,-5.4321);
	EXPECT_DOUBLE_EQ(-5.4321,matrix.get(0,0));
	matrix.set(std::vector<std::vector<double>> {{0,0,0,0},
												 {1,1,1,1}});
	EXPECT_DOUBLE_EQ(0,matrix.get(0,3));
	EXPECT_DOUBLE_EQ(1,matrix.get(1,0));
	
	// ziskanie hodnoty mimo rozsahu
	EXPECT_ANY_THROW(matrix.get(2,0));
	EXPECT_ANY_THROW(matrix.get(0,4));
	EXPECT_ANY_THROW(matrix.get(2,4));
	EXPECT_ANY_THROW(matrix.get(-10,10));
}
TEST_F(MatrixTest, Get4x3)
{
	Matrix matrix = mat4x3();
	// ziskanie hodnoty v rozsahu
	EXPECT_NO_THROW(matrix.get(0,0));
	EXPECT_DOUBLE_EQ(1,matrix.get(0,0));
	EXPECT_NO_THROW(matrix.get(3,2));
	EXPECT_DOUBLE_EQ(-6,matrix.get(3,2));
	
	matrix.set(0,0,-5.4321);
	EXPECT_DOUBLE_EQ(-5.4321,matrix.get(0,0));
	matrix.set(std::vector<std::vector<double>> {{0,0,0},
												 {1,1,1},
												 {2,2,2},
												 {3,3,3}});
	EXPECT_DOUBLE_EQ(0,matrix.get(0,0));
	EXPECT_DOUBLE_EQ(2,matrix.get(2,1));
	
	// ziskanie hodnoty mimo rozsahu
	EXPECT_ANY_THROW(matrix.get(4,0));
	EXPECT_ANY_THROW(matrix.get(0,3));
	EXPECT_ANY_THROW(matrix.get(4,3));
	EXPECT_ANY_THROW(matrix.get(-10,10));
}
TEST_F(MatrixTest, Get5x5)
{
	Matrix matrix = mat5x5();
	// ziskanie hodnoty v rozsahu
	EXPECT_NO_THROW(matrix.get(0,0));
	EXPECT_DOUBLE_EQ(1,matrix.get(0,0));
	EXPECT_NO_THROW(matrix.get(2,4));
	EXPECT_DOUBLE_EQ(8,matrix.get(2,4));
	
	matrix.set(0,0,-5.4321);
	EXPECT_DOUBLE_EQ(-5.4321,matrix.get(0,0));
	matrix.set(std::vector<std::vector<double>> {{-10,-10,-10,-10,-10},
												 {-1,-1,-1,-1,-1},
												 {-2,-2,-2,-2,-2},
												 {-3,-3,-3,-3,-3},
												 {-4,-4,-4,-4,-4}});
	EXPECT_DOUBLE_EQ(-10,matrix.get(0,0));
	EXPECT_DOUBLE_EQ(-2,matrix.get(2,2));
	EXPECT_DOUBLE_EQ(-3,matrix.get(3,4));
	
	// ziskanie hodnoty mimo rozsahu
	EXPECT_ANY_THROW(matrix.get(5,0));
	EXPECT_ANY_THROW(matrix.get(0,5));
	EXPECT_ANY_THROW(matrix.get(5,5));
	EXPECT_ANY_THROW(matrix.get(-10,10));
}
TEST_F(MatrixTest, Equal)
{
	Matrix matrix1x1 = mat1x1();
	Matrix matrix2x4 = mat2x4();
	Matrix matrix4x3 = mat4x3();
	Matrix matrix5x5 = mat5x5();
	
	// rovnake matice 
	matrix = mat1x1();
	EXPECT_TRUE(matrix.operator==(matrix1x1));
	matrix = mat2x4();
	EXPECT_TRUE(matrix.operator==(matrix2x4));
	matrix = mat4x3();
	EXPECT_TRUE(matrix.operator==(matrix4x3));
	matrix = mat5x5();
	EXPECT_TRUE(matrix.operator==(matrix5x5));
	
	// rozne rozmery matic
	matrix = mat1x1();
	EXPECT_ANY_THROW(matrix.operator==(matrix5x5));
	EXPECT_ANY_THROW(matrix.operator==(matrix2x4));
	EXPECT_ANY_THROW(matrix.operator==(matrix4x3));
	matrix = mat2x4();
	EXPECT_ANY_THROW(matrix.operator==(matrix4x3));
	EXPECT_ANY_THROW(matrix.operator==(matrix1x1));
	EXPECT_ANY_THROW(matrix.operator==(matrix5x5));
	
	// nerovnajuce sa matice
	matrix = mat1x1();
	matrix.set(0,0,0);
	EXPECT_FALSE(matrix.operator==(matrix1x1));
	matrix = mat5x5();
	matrix.set(4,4,66);
	EXPECT_FALSE(matrix.operator==(matrix5x5));
	matrix = mat2x4();
	matrix.set(std::vector<std::vector<double>> {{-1,-1,-1,2},
												 {-3,-3,-3,4}});
	EXPECT_FALSE(matrix.operator==(matrix2x4));
	matrix = mat4x3();
	matrix.set(std::vector<std::vector<double>> {{0,0,0},
												 {1,1,1},
												 {2,2,2},
												 {3,3,3}});
	EXPECT_FALSE(matrix.operator==(matrix4x3));
}
TEST_F(MatrixTest, Add)
{
	Matrix pom_matrix;
	std::vector<std::vector<double>> vysledok;
	
	// spravny vysledok
	pom_matrix = mat5x5();
	matrix = mat5x5();
	EXPECT_NO_THROW((matrix = matrix.operator+(pom_matrix)));
	pom_matrix.set(std::vector<std::vector<double>> {{2,4,6,8,10},
													 {-2,-4,-6,-8,-10},
													 {0,6,10,14,16},
													 {12,14,16,18,0},
													 {0,2,18,4,16}});
	EXPECT_TRUE(matrix.operator==(pom_matrix));
	
	pom_matrix = Matrix(2,4);
	pom_matrix.set(std::vector<std::vector<double>> {{2.05,-4.4,6.41,-8},
													 {2.898,4,-6.3,-8}});
	matrix = mat2x4();
	EXPECT_NO_THROW((matrix = matrix.operator+(pom_matrix)));
	vysledok = {{3.05,-2.4,9.41,-4},
				{1.898,2,-9.3,-12}};								
	for (int x = 0; x < 2; x++){
		for(int y = 0; y < 4; y++){
			EXPECT_DOUBLE_EQ(matrix.get(x,y), vysledok[x][y]);
		}
	}
	
	// rozne rozmery
	matrix = mat5x5();
	pom_matrix = mat1x1();
	EXPECT_ANY_THROW(matrix.operator+(pom_matrix));
	matrix = mat2x4();
	pom_matrix = mat4x3();
	EXPECT_ANY_THROW(matrix.operator+(pom_matrix));
}
TEST_F(MatrixTest, MultMatrix)
{
	Matrix pom_matrix;
	std::vector<std::vector<double>> vysledok;
	// vhodne rozmery
	
	// integers
	matrix = mat2x4();
	pom_matrix = mat4x3();
	EXPECT_NO_THROW(matrix = matrix.operator*(pom_matrix));
	vysledok = {{-5,-7,-9},
				{5,7,9}};
	for (int x = 0; x < 2; x++){
		for(int y = 0; y < 3; y++){
			EXPECT_DOUBLE_EQ(matrix.get(x,y), vysledok[x][y]);
		}
	}
	
	// floating point numbers
	matrix = Matrix(2,3);
	matrix.set(std::vector<std::vector<double>> {{-5.5424,-7.76543,-9.97645345},
												{5.634,7.234,9.666}});
	pom_matrix = Matrix(3,5);
	pom_matrix.set(std::vector<std::vector<double>> {{34.542,7.834,-15.5424,-7.676543,-69.97645345},
													{1.0000234,999.9,-5.54,7.1234,9.776666},
													{123.456789,0.000010001,5.45,-7.21,-0.000310078}});
	EXPECT_NO_THROW(matrix = matrix.operator*(pom_matrix));	
	vysledok = {{-1430.8721,-7808.07272,74.79101,59.16044,311.92057},
				{1395.17712,7277.41345,-74.9625416,-61.41083,-323.52593}};
	for (int x = 0; x < 2; x++){
		for(int y = 0; y < 5; y++){
			EXPECT_NEAR(matrix.get(x,y), vysledok[x][y], 0.00001);
		}
	}
												
	// nevhodne rozmery matic
	matrix = mat5x5();
	EXPECT_ANY_THROW(matrix.operator*(mat1x1()));
	EXPECT_ANY_THROW(matrix.operator*(mat4x3()));
	EXPECT_ANY_THROW(matrix.operator*(mat2x4()));
}
TEST_F(MatrixTest, MultConst)
{
	std::vector<std::vector<double>> vysledok;
	// integer
	matrix = mat2x4();
	EXPECT_NO_THROW(matrix = matrix.operator*(3));
	vysledok = {{3,6,9,12},
				{-3,-6,-9,-12}};
	for (int x = 0; x < 2; x++){
		for (int y = 0; y < 4; y++){
			EXPECT_DOUBLE_EQ(matrix.get(x,y), vysledok[x][y]);
		}
	}
	// floating point number
	EXPECT_NO_THROW(matrix = matrix.operator*(0.1234));
	vysledok = {{0.3702,0.7404,1.1106,1.4808},
				{-0.3702,-0.7404,-1.1106,-1.4808}};
	for (int x = 0; x < 2; x++){
		for (int y = 0; y < 4; y++){
			EXPECT_DOUBLE_EQ(matrix.get(x,y), vysledok[x][y]);
		}
	}
}
TEST_F(MatrixTest, SolveEquation)
{
	std::vector<double> vysledok;
	// 1x1
	EXPECT_NO_THROW(mat1x1().solveEquation({1}));
	vysledok = {1.0/42};
	EXPECT_TRUE((mat1x1().solveEquation({1}) == vysledok));
	// 2x2
	matrix = Matrix(2,2);
	matrix.set(std::vector<std::vector<double>> {{1,0},
												 {4,5}});
	EXPECT_NO_THROW(matrix.solveEquation({1,2}));
	vysledok = {1,(-2.0/5)};
	EXPECT_TRUE((matrix.solveEquation({1,2}) == vysledok));
	// 3x3
	matrix = Matrix(3,3);
	matrix.set(std::vector<std::vector<double>> {{1,2,0},
												 {4,5,6},
												 {7,8,9}});
	EXPECT_NO_THROW(matrix.solveEquation({1,2,3}));
	vysledok = {(-1.0/3),(2.0/3),0};
	EXPECT_TRUE((matrix.solveEquation({1,2,3}) == vysledok));
	// singularna matica
	matrix = Matrix(2,2);
	matrix.set(std::vector<std::vector<double>> {{2,1},
												 {4,2}});;
	EXPECT_ANY_THROW(matrix.solveEquation({1,2}));
	EXPECT_ANY_THROW(mat5x5().solveEquation({1,2,3,4,5}));											 
	// nespravna velkost vektoru b
	EXPECT_ANY_THROW(Matrix(5,5).solveEquation({1,2,3}));
	EXPECT_ANY_THROW(mat1x1().solveEquation({1,2,3}));
	// nestvorcova matica
	EXPECT_ANY_THROW(mat2x4().solveEquation({2,2,2,2}));
	EXPECT_ANY_THROW(mat4x3().solveEquation({2,2,2}));
}
/*** Konec souboru white_box_tests.cpp ***/

