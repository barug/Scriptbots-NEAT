/*
 Copyright 2001 The University of Texas at Austin

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include "NEAT/innovation.h"
#include <iostream>

using namespace NEAT;

Innovation::Innovation(int nin,int nout,double num1,double num2,int newid,double oldinnov) {
	innovation_type=NEWNODE;
	node_in_id=nin;
	node_out_id=nout;
	innovation_num1=num1;
	innovation_num2=num2;
	newnode_id=newid;
	old_innov_num=oldinnov;

	//Unused parameters set to zero
	new_weight=0;
	new_traitnum=0;
	recur_flag=false;
}

Innovation::Innovation(int nin,int nout,double num1,double w,int t) {
	innovation_type=NEWLINK;
	node_in_id=nin;
	node_out_id=nout;
	innovation_num1=num1;
	new_weight=w;
	new_traitnum=t;

	//Unused parameters set to zero
	innovation_num2=0;
	newnode_id=0;
	recur_flag=false;
	old_innov_num=0;
}

Innovation::Innovation(int nin,int nout,double num1,double w,int t,bool recur) {
	innovation_type=NEWLINK;
	node_in_id=nin;
	node_out_id=nout;
	innovation_num1=num1;
	new_weight=w;
	new_traitnum=t;

	//Unused parameters set to zero
	innovation_num2=0;
	newnode_id=0;
	recur_flag=recur;
    old_innov_num=0;
}

Innovation::Innovation(std::ifstream &inFile)
{
	std::string wordBuff;

	inFile >> wordBuff;
	if (wordBuff != "innovationBegin")
		throw std::runtime_error("bad format : innovationBegin");

	int intBuff;

	inFile >> intBuff;

	innovation_type = (innovtype) intBuff;  //Either NEWNODE or NEWLINK

	inFile >> node_in_id;     //Two nodes specify where the innovation took place
	inFile >> node_out_id;

	inFile >> innovation_num1;  //The number assigned to the innovation
	inFile >> innovation_num2;  // If this is a new node innovation, then there are 2 innovations (links) added for the new node

	inFile >> new_weight;   //  If a link is added, this is its weight
	inFile >> new_traitnum; // If a link is added, this is its connected trait

	inFile >> newnode_id;  // If a new node was created, this is its node_id

	inFile >> old_innov_num;  // If a new node was created, this is the innovnum of the gene's link it is being stuck inside

	inFile >> recur_flag;

	std::cout << innovation_type << " ";  //Either NEWNODE or NEWLINK

	std::cout << node_in_id << " ";     //Two nodes specify where the innovation took place
	std::cout << node_out_id << " ";

	std::cout << innovation_num1 << " ";  //The number assigned to the innovation
	std::cout << innovation_num2 << " ";  // If this is a new node innovation, then there are 2 innovations (links) added for the new node

	std::cout << new_weight << " ";   //  If a link is added, this is its weight
	std::cout << new_traitnum << " "; // If a link is added, this is its connected trait

	std::cout << newnode_id << " ";  // If a new node was created, this is its node_id

	std::cout << old_innov_num << " ";  // If a new node was created, this is the innovnum of the gene's link it is being stuck inside

	std::cout << recur_flag << " ";


	inFile >> wordBuff;
	if (wordBuff != "innovationEnd")
		throw std::runtime_error("bad format : innovationEnd");
}

void Innovation::printToFile(std::ofstream &outfile)
{
	outfile << "innovationBegin" << std::endl;

	outfile << innovation_type << " ";  //Either NEWNODE or NEWLINK

	outfile << node_in_id << " ";     //Two nodes specify where the innovation took place
	outfile << node_out_id << " ";

	outfile << innovation_num1 << " ";  //The number assigned to the innovation
	outfile << innovation_num2 << " ";  // If this is a new node innovation, then there are 2 innovations (links) added for the new node

	outfile << new_weight << " ";   //  If a link is added, this is its weight
	outfile << new_traitnum << " "; // If a link is added, this is its connected trait

	outfile << newnode_id << " ";  // If a new node was created, this is its node_id

	outfile << old_innov_num << " ";  // If a new node was created, this is the innovnum of the gene's link it is being stuck inside

	outfile << recur_flag << " ";


	outfile << "innovationEnd" << std::endl;
}