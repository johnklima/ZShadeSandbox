//==================================================================================================================================
// WaypointSystem.h
//
//==================================================================================================================================
// History
//
// -Created on 8/3/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __WAYPOINTSYSTEM_H
#define __WAYPOINTSYSTEM_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "ZMath.h"
#include "XMMath.h"

//==================================================================================================================================
//==================================================================================================================================
namespace ZShadeSandboxAI {
class WaypointSystem
{
public:
	
	WaypointSystem();
	int Size();
	bool Empty();
	void Clear();
	void RemoveWaypoint(ZShadeSandboxMath::XMMath3 waypoint);
	void AddWaypoint(ZShadeSandboxMath::XMMath3 waypoint);
	bool CurrentWaypoint(ZShadeSandboxMath::XMMath3& waypoint);
	void SetNextWaypoint();
	bool Finished();
	bool& CanLoop() { return bCanLoop; }
	bool CanLoop() const { return bCanLoop; }
	std::vector<ZShadeSandboxMath::XMMath3> GetWaypoints() const { return mWaypoints; }
	int& CurrentWaypointIndex() { return iCurrentWaypointIndex; }
	int CurrentWaypointIndex() const { return iCurrentWaypointIndex; }
	WaypointSystem* Clone();
	
private:
	
	bool bCanLoop;
	int iCurrentWaypointIndex;
	std::vector<ZShadeSandboxMath::XMMath3> mWaypoints;
};
}
//==================================================================================================================================
//==================================================================================================================================
#endif//__WAYPOINTSYSTEM_H