//==================================================================================================================================
// ContactStats.h
//
//==================================================================================================================================
// History
//
// -Created on 1/29/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __CONTACTSTATS_H
#define __CONTACTSTATS_H
//==================================================================================================================================
//==================================================================================================================================
struct ContactStats
{
	// How many bodies are colliding with one another
	int collidingBodies;
	
	// How many bounding boxes are being displayed
	int boundingBoxes;
	
	// How many steps did it take to find a contact
	int speculativeContacts;
	
	// How many speculative contacts were discarded
	int speculativeDiscards;
	
	// How many iterations to correct a collision
	int penetrationCorrections;
	
	// How many contacts are made
	int contacts;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__CONTACTSTATS_H