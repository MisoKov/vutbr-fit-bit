//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     MICHAL KOVAL <xkoval17@stud.fit.vutbr.cz>
// $Date:       $2018-02-24
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author MICHAL KOVAL
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

class NonEmptyTree : public ::testing::Test
{
protected:
	BinaryTree *tree = new BinaryTree();
	int values[13] = {2,6,7,34,33,56,8,77,3,80,66,50,0};
	virtual void SetUp(){
		for (int i = 0; i < 13; i++){
			tree->InsertNode(values[i]);
		}
	}
	virtual void TearDown(){
		delete tree;
	}
};

class EmptyTree : public ::testing::Test
{
protected:
	BinaryTree *tree = new BinaryTree();
	int values[14] = {0,1,2,3,4,5,6,11,22,33,44,55,66,77};
};

class Axioms : public ::testing::Test
{
protected:
	BinaryTree *tree = new BinaryTree();
	int values[7] = {1,15,30,45,50,80,123};
	virtual void SetUp(){
		for (int i = 0; i < 7; i++){
			tree->InsertNode(values[i]);
		}
	}
	virtual void TearDown(){
		delete tree;
	}
};
TEST_F (NonEmptyTree, InsertNode)
{
	std::pair<bool, Node_t *> pair;
	Node_t * node;
	
	// existujuca hodnota 3
	pair = tree->InsertNode(3);
	node = pair.second;
	ASSERT_TRUE(node != NULL);
	EXPECT_FALSE(pair.first);
	// kontrola stavov
	EXPECT_EQ(node->key, 3);
	EXPECT_EQ(node->color, RED);
	// potomkovia su listy
	EXPECT_TRUE(node->pLeft->pLeft == NULL && node->pLeft->pRight == NULL);
	EXPECT_TRUE(node->pRight->pLeft == NULL && node->pRight->pRight == NULL);
	// kontrola parenta
	EXPECT_EQ(node->pParent->key, 2);
	EXPECT_EQ(node->pParent->color, BLACK);
	
	// pridanie novej hodnoty
	
	// key 5
	pair = tree->InsertNode(5);
	ASSERT_TRUE(pair.second != NULL);
	ASSERT_TRUE(pair.first);
	// kontrola stavov
	EXPECT_EQ(pair.second->key, 5);
	EXPECT_EQ(pair.second->color, BinaryTree::RED);
	// potomkovia su listy
	EXPECT_TRUE(pair.second->pLeft->pLeft == NULL && pair.second->pLeft->pRight == NULL);
	EXPECT_TRUE(pair.second->pRight->pLeft == NULL && pair.second->pRight->pRight == NULL);
	// parent
	EXPECT_EQ(pair.second->pParent->key, 3);
	EXPECT_EQ(pair.second->pParent->color, BinaryTree::BLACK);
	EXPECT_EQ(pair.second->pParent, node);
	
	// key 9
	pair = tree->InsertNode(9);
	ASSERT_TRUE(pair.second != NULL);
	ASSERT_TRUE(pair.first);
	// kontrola stavov
	EXPECT_EQ(pair.second->key, 9);
	EXPECT_EQ(pair.second->color, BinaryTree::RED);
	// potomkovia su listy
	EXPECT_TRUE(pair.second->pLeft->pLeft == NULL && pair.second->pLeft->pRight == NULL);
	EXPECT_TRUE(pair.second->pRight->pLeft == NULL && pair.second->pRight->pRight == NULL);
	node = tree->FindNode(8);
	EXPECT_EQ(pair.second->pParent->key, 8);
	EXPECT_EQ(pair.second->pParent->color, BinaryTree::BLACK);
	EXPECT_EQ(pair.second->pParent, node);
}

TEST_F (NonEmptyTree, DeleteNode)
{
	// neexistujuci key
	EXPECT_FALSE(tree->DeleteNode(123));
	EXPECT_FALSE(tree->DeleteNode(-1));
	EXPECT_FALSE(tree->DeleteNode(70));
	// existujuci key
	for (int i = 0; i < 13; i++){
		EXPECT_TRUE(tree->DeleteNode(values[i]));
	}
	
}

TEST_F (NonEmptyTree, FindNode)
{
	// neexistujuci key
	EXPECT_TRUE(tree->FindNode(100) == NULL);
	EXPECT_TRUE(tree->FindNode(-10) == NULL);
	EXPECT_TRUE(tree->FindNode(71) == NULL);
	
	// existujuci key
	Node_t * node;
	for (int i : values){
		node = tree->FindNode(i);
		ASSERT_TRUE(node != NULL);
		EXPECT_EQ(node->key, i);
	}
}
TEST_F (EmptyTree, InsertNode)
{
	std::pair<bool, Node_t *> pair;
	Node_t * node;
	// vlozenie uzlu s key 10
	pair = tree->InsertNode(10);
	node = pair.second;
	ASSERT_TRUE(pair.first);
	ASSERT_TRUE(node != NULL);
	// kontrola stavov
	EXPECT_EQ(node->key, 10);
	EXPECT_EQ(node->color, BLACK);
	// parent neexistuje, je to prvy prvok v strome
	EXPECT_TRUE(node->pParent == NULL);
	// potomkovia su listy
	EXPECT_TRUE(node->pLeft->pLeft == NULL && node->pLeft->pRight == NULL);
	EXPECT_TRUE(node->pRight->pLeft == NULL && node->pRight->pRight == NULL);
	
	// vlozenie tej istej hodnoty
	pair = tree->InsertNode(10);
	ASSERT_FALSE(pair.first);
	ASSERT_FALSE(node == NULL);
	
	// vlozenie uzlu s key 5
	pair = tree->InsertNode(5);
	node = pair.second;
	ASSERT_TRUE(pair.first);
	ASSERT_TRUE(node != NULL);
	// kontrola stavov
	EXPECT_EQ(node->key, 5);
	EXPECT_EQ(node->color, RED);
	// parent stavy
	EXPECT_TRUE(node->pParent != NULL);
	EXPECT_EQ(node->pParent->key, 10);
	EXPECT_EQ(node->pParent->color, BLACK);
	EXPECT_TRUE(node->pParent->pParent == NULL);
	// potomkovia su listy
	EXPECT_TRUE(node->pLeft->pLeft == NULL && node->pLeft->pRight == NULL);
	EXPECT_TRUE(node->pRight->pLeft == NULL && node->pRight->pRight == NULL);
	
	// vlozenie uzlu s key 5
	pair = tree->InsertNode(1);
	node = pair.second;
	ASSERT_TRUE(pair.first);
	ASSERT_TRUE(node != NULL);
	// kontrola stavov
	EXPECT_EQ(node->key, 1);
	EXPECT_EQ(node->color, RED);
	// parent stavy
	EXPECT_TRUE(node->pParent != NULL);
	EXPECT_EQ(node->pParent->key, 5);
	EXPECT_EQ(node->pParent->color, BLACK);
	EXPECT_TRUE(node->pParent->pParent == NULL);
	// potomkovia su listy
	EXPECT_TRUE(node->pLeft->pLeft == NULL && node->pLeft->pRight == NULL);
	EXPECT_TRUE(node->pRight->pLeft == NULL && node->pRight->pRight == NULL);
	
}

TEST_F (EmptyTree, DeleteNode)
{
	EXPECT_FALSE(tree->DeleteNode(123));
	EXPECT_FALSE(tree->DeleteNode(-1));
	EXPECT_FALSE(tree->DeleteNode(70));
	for (int i : values){
		EXPECT_FALSE(tree->DeleteNode(i));
	}
}

TEST_F (EmptyTree, FindNode)
{
	EXPECT_TRUE(tree->FindNode(100) == NULL);
	EXPECT_TRUE(tree->FindNode(-10) == NULL);
	EXPECT_TRUE(tree->FindNode(71) == NULL);
	Node_t * node;
	for (int i : values){
		node = tree->FindNode(i);
		EXPECT_TRUE(node == NULL);
	}
}
TEST_F (Axioms, Axiom1)
{
	std::vector<Node_t *> listy;
	tree->GetLeafNodes(listy);
	
	for (Node_t * list : listy){
		EXPECT_EQ(list->color, BLACK);
	}
}

TEST_F (Axioms, Axiom2)
{
	std::vector<Node_t *> uzly;
	tree->GetNonLeafNodes(uzly);
	
	for (Node_t * uzol : uzly){
		if (uzol->color == RED){
			EXPECT_TRUE(uzol->pLeft->color == uzol->pRight->color == BLACK);
		}
	}
}

TEST_F (Axioms, Axiom3)
{
	std::vector<Node_t *> listy;
	tree->GetLeafNodes(listy);
	unsigned int black_count;
	unsigned int check;
	bool first_run = true;
	
	for (Node_t * list : listy){
		black_count = -1;
		while (list != NULL){
			if (list->color == BLACK){
				black_count++;
			}
			list = list->pParent;
		}
		if (first_run){
			check = black_count;
			first_run = false;
		} else {
			ASSERT_EQ(check, black_count);
			check = black_count;
		}
	}
}
/*** Konec souboru black_box_tests.cpp ***/
