//===============================================================================================================================
// PolygonEditorHelper
//
//===============================================================================================================================
// History
//
//	-Created on 8/12/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __POLYGONEDITORHELPER_H
#define __POLYGONEDITORHELPER_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

//===============================================================================================================================
//===============================================================================================================================
/*	(This is in 3D)
	
	Need to:
	
	Figure out where the base class that holds the list of polygons for the editor should be.
	It is going in Map3D class
	
	1) Load a polygon from xml file
	2) Save a polygon to xml file
	3) Add a point to currently editing polygon
	4) Remove a point from currently editing polygon
	5) Set rendering primitive type for currently editing polygon
	6) Function to rebuild the currently editing polygon
	7) Toggle culling of the polygon to make it visible or not
	8) Function to move the selected vertex from the currently editing polygon
	9) Function to select a polygon based on x,y mouse points
	10) Function to select a vertex in currently editing polygon based on x,y mouse points
	11) Scale (resize) the currently editing polygon
	12) Rotate the currently editing polygon
	13) Transform (move) the entire currently editing polygon to a new position
	14) Create a Convex Hull of the currently editing polygon
	15) Assign material to currently editing polygon based on name of material
*/
class PolygonEditorHelper
{
public:
	
	PolygonEditorHelper();
	
	
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__POLYGONEDITORHELPER_H