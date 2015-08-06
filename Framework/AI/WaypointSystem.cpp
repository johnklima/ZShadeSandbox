#include "WaypointSystem.h"
using ZShadeSandboxAI::WaypointSystem;
//==================================================================================================================================
//==================================================================================================================================
WaypointSystem::WaypointSystem()
:	bCanLoop(false)
,	iCurrentWaypointIndex(0)
,	mCurrentWaypoint(ZShadeSandboxMath::XMMath3(0, 0, 0))
{
}
//==================================================================================================================================
void WaypointSystem::AddWaypoint(ZShadeSandboxMath::XMMath3 waypoint)
{
	mWaypoints.push_back(waypoint);
}
//==================================================================================================================================
bool WaypointSystem::CurrentWaypoint(ZShadeSandboxMath::XMMath3& waypoint)
{
	int counter = 0;
	auto waypoint_iter = mWaypoints.begin();
	
	while (waypoint_iter != mWaypoints.end())
	{
		if (counter == iCurrentWaypointIndex)
		{
			waypoint = (*waypoint_iter);
			
			return true;
		}
		
		++counter;
		++waypoint_iter;
	}
	
	return false;
}
//==================================================================================================================================
void WaypointSystem::Clear()
{
	mWaypoints.clear();
}
//==================================================================================================================================
bool WaypointSystem::Empty()
{
	return (mWaypoints.size() == 0);
}
//==================================================================================================================================
int WaypointSystem::Size()
{
	return mWaypoints.size();
}
//==================================================================================================================================
void WaypointSystem::RemoveWaypoint(ZShadeSandboxMath::XMMath3 waypoint)
{
	ZShadeSandboxMath::XMMath3 point;
	auto current = mWaypoints.begin();
	while (current != mWaypoints.end())
	{
		point = (*current);
		if (point == waypoint)
		{
			current = mWaypoints.erase(current);
		}
		else
		{
			++current;
		}
	}
	
	// If the index is greater than what the list contains then it needs to be decreased
	// since we just removed a waypoint from the list and we do not want to read an out
	// of bounds waypoint
	if (iCurrentWaypointIndex >= mWaypoints.size())
	{
		--iCurrentWaypointIndex;
	}
}
//==================================================================================================================================
void WaypointSystem::SetNextWaypoint()
{
	// If we are setting the next waypoint and we are already on the last waypoint then reset the back to the beginning.
	// We also only reset back to the beginning if this waypoint system can continuously loop through the waypoints.
	if (iCurrentWaypointIndex >= mWaypoints.size() && bCanLoop)
	{
		iCurrentWaypointIndex = 0;
	}
	// Otherwise continue on to the next waypoint
	else
	{
		// Only continue to the next waypoint as long as there are waypoints left to continue on to
		if (iCurrentWaypointIndex < mWaypoints.size())
			iCurrentWaypointIndex++;
	}
}
//==================================================================================================================================
bool WaypointSystem::Finished()
{
	// We are done traversing the waypoints if we are at the last waypoint and there is no looping
	return (!bCanLoop && iCurrentWaypointIndex == mWaypoints.size() - 1);
}
//==================================================================================================================================