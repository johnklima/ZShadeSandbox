//==================================================================================================================================
// BSPPVSGenerator
//
//==================================================================================================================================
// History
//
// -Created on 6/28/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __BSPPVSGENERATOR_H
#define __BSPPVSGENERATOR_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "Vertex.h"
#include "XMMath.h"
#include "BSPNode.h"
#include "BSPPortal.h"
#include "BSPPlane.h"
#include "BSPParameters.h"

//==================================================================================================================================
//==================================================================================================================================
namespace ZShadeSandboxBSP {
class BSPPVSGenerator
{
public:
	
	BSPPVSGenerator();
	
	// Sets up the source and destination portals
	long CalculatePVS();

private:

	// Makes the current generator portal the new destination portal and rebuilds the anti-penumbra (clip plane)
	void CalculatePVS(long sourceLeaf, ZShadeSandboxBSP::BSPPortal* srcPortal, ZShadeSandboxBSP::BSPPortal* targetPortal, long targetLeaf, BYTE* leafPVS);

	// Builds the anti-penumbra (array of clip planes) between the source and destination portal and clips the generator to the planes and returns it
	ZShadeSandboxBSP::BSPPortal* ClipToAntiPenumbra(ZShadeSandboxBSP::BSPPortal* srcPortal, ZShadeSandboxBSP::BSPPortal* targetPortal, ZShadeSandboxBSP::BSPPortal* generatorPortal);

	// Constructs a plane object from the given portal
	ZShadeSandboxBSP::BSPPlane GetPortalPlane(ZShadeSandboxBSP::BSPPortal* portal);

	/**
	 *	Allocates a temporary array to hold the CURRENT leafs PVS data.This array is large enough to hold 1 bit for each leaf.
	 *	When we find that a Generator portal is visible (therefore so is the leaf on the opposing side) we need to record that
	 *	the Leaf on the opposing side of the Generator portal is visible from the source leaf. We pass this function the temporary
	 *	array and the Leaf that is visible.This function sets the correct Bit to 1 in the Temporary Visibility array.
	 */
	void SetPVSBit(BYTE*& visArray, long destLeaf);

	/**
	 *	Adds this data to the master PVSDATA array and compresses the data as it is added (using zero run length encoding).
	 *	When the 'CalculatePVS' function returns control back to the main program the 'PVSData' array will hold the PVS for
	 *	every leaf compressed and each Leaf will also have in its PVSIndex field an offset into this array that describes
	 *	the location of the start of its PVSdata. By adding each leafs visiblity info to the array one at a time means that
	 *	we save on memory overhead.Instead of having to calculate the whole PVS information in one array and then Compress it into
	 *	another array (therefore needing twice the size of the PVS data during the compression because of the need for two array),
	 *	we will just calulate one leafs visibility info at a time and then add that to the PVSData array. The buffer that was used
	 *	to hold the uncompressed Visibility info for the leaf can then be cleared and re used for the next leaf and so on.
	 */
	long CompressLeafSet(BYTE* visArray, long writePos);
};
}
//==================================================================================================================================
//==================================================================================================================================
#endif//__BSPPVSGENERATOR_H