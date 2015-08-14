#include "AISprite.h"
#include "Scripting.h"
#include "TopdownMap.h"
#include "PlatformerMap.h"
#include "Environment2DMapHelper.h"
//==================================================================================================================================
//==================================================================================================================================
AISprite::AISprite(D3D* d3d)
:	Sprite(d3d)
,	m_run_behavior(false)
,	m_run_behavior_default(false)
,	m_default_behavior("")
,	m_behavior_name("")
,	tree(0)
,	m_TargetSprite(0)
,	m_ran_main(false)
{
	mSteeringForce = new AISteeringForce(this);
}
//==================================================================================================================================
AISprite::AISprite(const AISprite& o) : Sprite(o)
{
}
//==================================================================================================================================
AISprite::~AISprite()
{
}
//==================================================================================================================================
void AISprite::SetRunBehavior(bool set)
{
	m_run_behavior = set;
}
//==================================================================================================================================
void AISprite::SetRunBehaviorDefault(bool set)
{
	m_run_behavior_default = set;
}
//==================================================================================================================================
void AISprite::SetBehaviorName(string name)
{
	m_behavior_name = name;
}
//==================================================================================================================================
void AISprite::CalculateSteeringForce()
{
	// Only AI will need to have this since a human can do all of these things.
	if (!m_player)
	{
		Steering() = true;
		
		ZShadeSandboxMath::XMMath3 force = mSteeringForce->CalculateForce();
		Force().x = force.x;
		Force().y = force.y;
	}
}
//==================================================================================================================================
void AISprite::SetWanderingRadius(float value)
{
	mSteeringForce->WanderingRadius() = value;
}
//==================================================================================================================================
void AISprite::SetWanderingDistance(float value)
{
	mSteeringForce->WanderingDistance() = value;
}
//==================================================================================================================================
void AISprite::SetWanderingJitter(float value)
{
	mSteeringForce->WanderingJitter() = value;
}
//==================================================================================================================================
void AISprite::SetMinDetectionBoxLength(float value)
{
	mSteeringForce->MinDetectionBoxLength() = value;
}
//==================================================================================================================================
void AISprite::SetWaypointSeekDist(float value)
{
	mSteeringForce->WaypointSeekDist() = value;
}
//==================================================================================================================================
void AISprite::SetMaxFleeDistance(float value)
{
	mSteeringForce->MaxFleeDistance() = value;
}
//==================================================================================================================================
void AISprite::SetWeightSeek(float value)
{
	mSteeringForce->WeightSeek() = value;
}
//==================================================================================================================================
void AISprite::SetWeightFlee(float value)
{
	mSteeringForce->WeightFlee() = value;
}
//==================================================================================================================================
void AISprite::SetWeightArrive(float value)
{
	mSteeringForce->WeightArrive() = value;
}
//==================================================================================================================================
void AISprite::SetWeightSpriteAvoidance(float value)
{
	mSteeringForce->WeightSpriteAvoidance() = value;
}
//==================================================================================================================================
void AISprite::SetWeightSeparation(float value)
{
	mSteeringForce->WeightSeparation() = value;
}
//==================================================================================================================================
void AISprite::SetWeightAlignment(float value)
{
	mSteeringForce->WeightAlignment() = value;
}
//==================================================================================================================================
void AISprite::SetWeightCohesion(float value)
{
	mSteeringForce->WeightCohesion() = value;
}
//==================================================================================================================================
void AISprite::SetWeightWander(float value)
{
	mSteeringForce->WeightWander() = value;
}
//==================================================================================================================================
void AISprite::SetWeightFollowPath(float value)
{
	mSteeringForce->WeightFollowPath() = value;
}
//==================================================================================================================================
void AISprite::SetWeightPursueTarget(float value)
{
	mSteeringForce->WeightPursueTarget() = value;
}
//==================================================================================================================================
void AISprite::SetWeightEvadeTarget(float value)
{
	mSteeringForce->WeightEvadeTarget() = value;
}
//==================================================================================================================================
void AISprite::SetWeightInterpose(float value)
{
	mSteeringForce->WeightInterpose() = value;
}
//==================================================================================================================================
void AISprite::SetWeightHide(float value)
{
	mSteeringForce->WeightHide() = value;
}
//==================================================================================================================================
void AISprite::SetWeightOffsetPursuit(float value)
{
	mSteeringForce->WeightOffsetPursuit() = value;
}
//==================================================================================================================================
void AISprite::SetCanSeek(bool value)
{
	mSteeringForce->CanSeek() = value;
}
//==================================================================================================================================
void AISprite::SetCanFlee(bool value)
{
	mSteeringForce->CanFlee() = value;
}
//==================================================================================================================================
void AISprite::SetCanArrive(bool value)
{
	mSteeringForce->CanArrive() = value;
}
//==================================================================================================================================
void AISprite::SetCanAvoidSprites(bool value)
{
	mSteeringForce->CanAvoidSprites() = value;
}
//==================================================================================================================================
void AISprite::SetCanFlockSeparation(bool value)
{
	mSteeringForce->CanFlockSeparation() = value;
}
//==================================================================================================================================
void AISprite::SetCanFlockAlignment(bool value)
{
	mSteeringForce->CanFlockAlignment() = value;
}
//==================================================================================================================================
void AISprite::SetCanFlockCohesion(bool value)
{
	mSteeringForce->CanFlockCohesion() = value;
}
//==================================================================================================================================
void AISprite::SetCanWander(bool value)
{
	mSteeringForce->CanWander() = value;
}
//==================================================================================================================================
void AISprite::SetCanFollowPath(bool value)
{
	mSteeringForce->CanFollowPath() = value;
}
//==================================================================================================================================
void AISprite::SetCanPursueTarget(bool value)
{
	mSteeringForce->CanPursueTarget() = value;
}
//==================================================================================================================================
void AISprite::SetCanEvadeTarget(bool value)
{
	mSteeringForce->CanEvadeTarget() = value;
}
//==================================================================================================================================
void AISprite::SetCanInterpose(bool value)
{
	mSteeringForce->CanInterpose() = value;
}
//==================================================================================================================================
void AISprite::SetCanHide(bool value)
{
	mSteeringForce->CanHide() = value;
}
//==================================================================================================================================
void AISprite::SetCanOffsetPursuit(bool value)
{
	mSteeringForce->CanOffsetPursuit() = value;
}
//==================================================================================================================================
void AISprite::SetWaypointSystem(ZShadeSandboxAI::WaypointSystem* waypoints)
{
	mSteeringForce->WaypointSystem() = waypoints;
}
//==================================================================================================================================
void AISprite::SetEvader(AISprite* sprite)
{
	mSteeringForce->Evader() = sprite;
}
//==================================================================================================================================
void AISprite::SetPursuer(AISprite* sprite)
{
	mSteeringForce->Pursuer() = sprite;
}
//==================================================================================================================================
void AISprite::SetHideTarget(AISprite* sprite)
{
	mSteeringForce->Target() = sprite;
}
//==================================================================================================================================
void AISprite::SetSpriteA(AISprite* sprite)
{
	mSteeringForce->SpriteA() = sprite;
}
//==================================================================================================================================
void AISprite::SetSpriteB(AISprite* sprite)
{
	mSteeringForce->SpriteB() = sprite;
}
//==================================================================================================================================
void AISprite::SetLeader(AISprite* sprite)
{
	mSteeringForce->Leader() = sprite;
}
//==================================================================================================================================
void AISprite::SetLeaderOffset(ZShadeSandboxMath::XMMath3 point)
{
	mSteeringForce->LeaderOffset() = point;
}
//==================================================================================================================================
void AISprite::SetTargetPoint(ZShadeSandboxMath::XMMath3 point)
{
	mSteeringForce->TargetPoint() = point;
}
//==================================================================================================================================
void AISprite::SetTargetDecelerationType(int type)
{
	if (type == 3)
	{
		mSteeringForce->TargetDecelerationType() = EDeceleration::Type::eSlow;
	}
	else if (type == 2)
	{
		mSteeringForce->TargetDecelerationType() = EDeceleration::Type::eNormal;
	}
	else if (type == 1)
	{
		mSteeringForce->TargetDecelerationType() = EDeceleration::Type::eFast;
	}
}
//==================================================================================================================================
int AISprite::TargetDecelerationType() const
{
	switch (mSteeringForce->TargetDecelerationType())
	{
		case EDeceleration::Type::eSlow: return 3;
		case EDeceleration::Type::eNormal: return 2;
		case EDeceleration::Type::eFast: return 1;
		default: return 1;
	}
}
//==================================================================================================================================
void AISprite::CreateWaypointSystem()
{
	mSteeringForce->WaypointSystem() = new ZShadeSandboxAI::WaypointSystem();
}
//==================================================================================================================================
int AISprite::WaypointSystemSize()
{
	if (mSteeringForce->WaypointSystem())
		return mSteeringForce->WaypointSystem()->Size();
	else
		return 0;
}
//==================================================================================================================================
bool AISprite::WaypointSystemEmpty()
{
	if (mSteeringForce->WaypointSystem())
		return mSteeringForce->WaypointSystem()->Empty();
	else
		return true;
}
//==================================================================================================================================
void AISprite::ClearWaypoints()
{
	if (mSteeringForce->WaypointSystem())
		mSteeringForce->WaypointSystem()->Clear();
}
//==================================================================================================================================
void AISprite::RemoveWaypoint(ZShadeSandboxMath::XMMath3 waypoint)
{
	if (mSteeringForce->WaypointSystem())
		mSteeringForce->WaypointSystem()->RemoveWaypoint(waypoint);
}
//==================================================================================================================================
void AISprite::AddWaypoint(ZShadeSandboxMath::XMMath3 waypoint)
{
	if (mSteeringForce->WaypointSystem())
		mSteeringForce->WaypointSystem()->AddWaypoint(waypoint);
}
//==================================================================================================================================
float AISprite::CurrentWaypointX()
{
	ZShadeSandboxMath::XMMath3 waypoint(-99999999, -99999999, -99999999);
	if (mSteeringForce->WaypointSystem())
		mSteeringForce->WaypointSystem()->CurrentWaypoint(waypoint);
	return waypoint.x;
}
//==================================================================================================================================
float AISprite::CurrentWaypointY()
{
	ZShadeSandboxMath::XMMath3 waypoint(-99999999, -99999999, -99999999);
	if (mSteeringForce->WaypointSystem())
		mSteeringForce->WaypointSystem()->CurrentWaypoint(waypoint);
	return waypoint.y;
}
//==================================================================================================================================
float AISprite::CurrentWaypointZ()
{
	ZShadeSandboxMath::XMMath3 waypoint(-99999999, -99999999, -99999999);
	if (mSteeringForce->WaypointSystem())
		mSteeringForce->WaypointSystem()->CurrentWaypoint(waypoint);
	return waypoint.z;
}
//==================================================================================================================================
void AISprite::SetNextWaypoint()
{
	if (mSteeringForce->WaypointSystem())
		mSteeringForce->WaypointSystem()->SetNextWaypoint();
}
//==================================================================================================================================
bool AISprite::WaypointSystemFinished()
{
	if (mSteeringForce->WaypointSystem())
		return mSteeringForce->WaypointSystem()->Finished();
	else
		return false;
}
//==================================================================================================================================
void AISprite::SetWaypointSystemCanLoop(bool value)
{
	if (mSteeringForce->WaypointSystem())
		mSteeringForce->WaypointSystem()->CanLoop() = value;
}
//==================================================================================================================================
bool AISprite::WaypointSystemCanLoop() const
{
	if (mSteeringForce->WaypointSystem())
		return mSteeringForce->WaypointSystem()->CanLoop();
	else
		return false;
}
//==================================================================================================================================
int AISprite::CurrentWaypointIndex() const
{
	if (mSteeringForce->WaypointSystem())
		return mSteeringForce->WaypointSystem()->CurrentWaypointIndex();
	else
		return -99999999;
}
//==================================================================================================================================
void AISprite::TagNearestSpritesInMap()
{
	std::vector<AISprite*> sprites;
	
	TopdownMap* topdownMap = Environment2DMapHelper::ActiveTopdownMap();
	if (topdownMap != 0)
	{
		// Get the list of sprites in the map
		sprites = topdownMap->Sprites();
	}
	
	PlatformerMap* platformerMap = Environment2DMapHelper::ActivePlatformerMap();
	if (platformerMap != 0)
	{
		// Get the list of sprites in the map
		sprites = platformerMap->Sprites();
	}
	
	ZShadeSandboxMath::XMMath3 myPosition(TopLeftPosition().x, TopLeftPosition().y, TopLeftPosition().z);
	
	// The detection box length is proportional to the agent's velocity
	float boxLengthX = MinDetectionBoxLength() + (Speed().x / MaxSpeed().x) * MinDetectionBoxLength();
	float boxLengthY = MinDetectionBoxLength() + (Speed().y / MaxSpeed().y) * MinDetectionBoxLength();

	auto iter = sprites.begin();
	
	for (; iter != sprites.end(); ++iter)
	{
		(*iter)->UnTag();
		
		ZShadeSandboxMath::XMMath3 sprPosition((*iter)->TopLeftPosition().x, (*iter)->TopLeftPosition().y, (*iter)->TopLeftPosition().z);
		
		ZShadeSandboxMath::XMMath3 to = sprPosition - myPosition;
		
		// The bounding radius of the other sprite is taken into account by adding it to the range
		float range = boxLengthX + (*iter)->BoundingRadius();
		
		if (((*iter) != this) && (to.LengthSquared() < range * range))
		{
			(*iter)->Tag();
		}
	}
}
//==================================================================================================================================
int AISprite::FindNearestSpriteIDInMap()
{
	std::vector<AISprite*> sprites;
	
	TopdownMap* topdownMap = Environment2DMapHelper::ActiveTopdownMap();
	if (topdownMap != 0)
	{
		// Get the list of sprites in the map
		sprites = topdownMap->Sprites();
	}
	
	PlatformerMap* platformerMap = Environment2DMapHelper::ActivePlatformerMap();
	if (platformerMap != 0)
	{
		// Get the list of sprites in the map
		sprites = platformerMap->Sprites();
	}
	
	int nearestID = -1;
	
	ZShadeSandboxMath::XMMath3 myPosition(TopLeftPosition().x, TopLeftPosition().y, TopLeftPosition().z);
	
	// The detection box length is proportional to the agent's velocity
	float boxLengthX = MinDetectionBoxLength() + (Speed().x / MaxSpeed().x) * MinDetectionBoxLength();
	float boxLengthY = MinDetectionBoxLength() + (Speed().y / MaxSpeed().y) * MinDetectionBoxLength();

	auto iter = sprites.begin();
	
	for (; iter != sprites.end(); ++iter)
	{
		ZShadeSandboxMath::XMMath3 sprPosition((*iter)->TopLeftPosition().x, (*iter)->TopLeftPosition().y, (*iter)->TopLeftPosition().z);
		
		ZShadeSandboxMath::XMMath3 to = sprPosition - myPosition;
		
		// The bounding radius of the other sprite is taken into account by adding it to the range
		float range = boxLengthX + (*iter)->BoundingRadius();
		
		if (((*iter) != this) && (to.LengthSquared() < range * range))
		{
			nearestID = (*iter)->ID();
			break;
		}
	}
	
	return nearestID;
}
//==================================================================================================================================
void AISprite::AddAllNearestSpriteIDsToCollection()
{
	if (mNearestSpritesID.size() > 0)
	{
		mNearestSpritesID.clear();
	}
	
	std::vector<AISprite*> sprites;
	
	TopdownMap* topdownMap = Environment2DMapHelper::ActiveTopdownMap();
	if (topdownMap != 0)
	{
		// Get the list of sprites in the map
		sprites = topdownMap->Sprites();
	}
	
	PlatformerMap* platformerMap = Environment2DMapHelper::ActivePlatformerMap();
	if (platformerMap != 0)
	{
		// Get the list of sprites in the map
		sprites = platformerMap->Sprites();
	}
	
	ZShadeSandboxMath::XMMath3 myPosition(TopLeftPosition().x, TopLeftPosition().y, TopLeftPosition().z);
	
	// The detection box length is proportional to the agent's velocity
	float boxLengthX = MinDetectionBoxLength() + (Speed().x / MaxSpeed().x) * MinDetectionBoxLength();
	float boxLengthY = MinDetectionBoxLength() + (Speed().y / MaxSpeed().y) * MinDetectionBoxLength();
	
	auto iter = sprites.begin();
	
	for (; iter != sprites.end(); ++iter)
	{
		ZShadeSandboxMath::XMMath3 sprPosition((*iter)->TopLeftPosition().x, (*iter)->TopLeftPosition().y, (*iter)->TopLeftPosition().z);
		
		ZShadeSandboxMath::XMMath3 to = sprPosition - myPosition;
		
		// The bounding radius of the other sprite is taken into account by adding it to the range
		float range = boxLengthX + (*iter)->BoundingRadius();
		
		if (((*iter) != this) && (to.LengthSquared() < range * range))
		{
			mNearestSpritesID.push_back((*iter)->ID());
		}
	}
}
//==================================================================================================================================
int AISprite::AmountOfSpritesInRange()
{
	return mNearestSpritesID.size();
}
//==================================================================================================================================
int AISprite::FindNearestSpriteIDInCollection(int i)
{
	if (i > 0 && i < mNearestSpritesID.size() - 1)
		return mNearestSpritesID[i];
	else
		return -1;
}
//==================================================================================================================================
bool AISprite::WithinAttackDistance(int spriteID)
{
	AISprite* spr = 0;
	
	TopdownMap* topdownMap = Environment2DMapHelper::ActiveTopdownMap();
	if (topdownMap != 0)
	{
		spr = topdownMap->GetSpriteByID(spriteID);
	}
	
	PlatformerMap* platformerMap = Environment2DMapHelper::ActivePlatformerMap();
	if (platformerMap != 0)
	{
		spr = topdownMap->GetSpriteByID(spriteID);
	}
	
	// No sprite to attack
	if (spr == 0) return false;
	
	return WithinAttackDistance(spr);
}
//==================================================================================================================================
bool AISprite::WithinAttackDistance(AISprite* sprite)
{
	ZShadeSandboxMath::XMMath3 myPosition(TopLeftPosition().x, TopLeftPosition().y, TopLeftPosition().z);
	ZShadeSandboxMath::XMMath3 sprPosition(sprite->TopLeftPosition().x, sprite->TopLeftPosition().y, sprite->TopLeftPosition().z);
	
	float distSqr = fMinimumAttackDistance * fMinimumAttackDistance;
	if (myPosition.DistanceSquared(sprPosition) < distSqr)
	{
		return true;
	}
	
	return false;
}
//==================================================================================================================================
AISprite* AISprite::Clone(GameDirectory2D* gd)
{
	string filename = BaseTextureFilename();
	int w = Width();
	int h = Height();
	bool is_player = IsPlayer();
	bool is_platformer_sprite = m_platformerSprite;
	int vision = Vision();
	int depth = Depth();
	int hard = IsHard();
	int nohit = IsNoHit();
	int hp = HP();
	int mana = Mana();
	int gold = Gold();
	int strength = Strength();
	float speedX = SpeedX();
	float speedY = SpeedY();
	int defense = Defense();
	int exp = Experience();
	int level = Level();
	int touch = IsTouch();
	int background = IsBackgroundSprite();
	int entity = IsEntitySprite();
	int invisible = IsInvisibleSprite();
	int teleport = CanTeleport();
	int teleport_map_x = TeleportX();
	int teleport_map_y = TeleportY();
	int default_seq = DefaultSequence();
	int anim_speed = AnimationSpeed();
	int id = ID();
	int max_hp = HPMax();
	string name_str = Name();
	string animation_str = AnimationProfileName();
	string script_str = ScriptName();
	string teleportMap_str = TeleportMapName();
	string inventoryMsg_str = InventoryMessage();
	bool alwaysSeenByPlayer = AlwaysSeenByPlayer();
	bool seenByPlayer = SeenByPlayer();
	bool inFowCircle = InFowCircle();
	
	// Clone the AI stuff
	float minimumAttackDistance = fMinimumAttackDistance;
	float wanderingRadius = mSteeringForce->WanderingRadius();
	float wanderingDistance = mSteeringForce->WanderingDistance();
	float wanderingJitter = mSteeringForce->WanderingJitter();
	float minDetectionBoxLength = mSteeringForce->MinDetectionBoxLength();
	float waypointSeekDist = mSteeringForce->WaypointSeekDist();
	float maxFleeDistance = mSteeringForce->MaxFleeDistance();
	float weightSeek = mSteeringForce->WeightSeek();
	float weightFlee = mSteeringForce->WeightFlee();
	float weightArrive = mSteeringForce->WeightArrive();
	float weightSpriteAvoidance = mSteeringForce->WeightSpriteAvoidance();
	float weightSeparation = mSteeringForce->WeightSeparation();
	float weightAlignment = mSteeringForce->WeightAlignment();
	float weightCohesion = mSteeringForce->WeightCohesion();
	float weightWander = mSteeringForce->WeightWander();
	float weightFollowPath = mSteeringForce->WeightFollowPath();
	float weightPursueTarget = mSteeringForce->WeightPursueTarget();
	float weightEvadeTarget = mSteeringForce->WeightEvadeTarget();
	float weightInterpose = mSteeringForce->WeightInterpose();
	float weightHide = mSteeringForce->WeightHide();
	float weightOffsetPursuit = mSteeringForce->WeightOffsetPursuit();
	bool canSeek = mSteeringForce->CanSeek();
	bool canFlee = mSteeringForce->CanFlee();
	bool canArrive = mSteeringForce->CanArrive();
	bool canAvoidSprites = mSteeringForce->CanAvoidSprites();
	bool canFlockSeparation = mSteeringForce->CanFlockSeparation();
	bool canFlockAlignment = mSteeringForce->CanFlockAlignment();
	bool canFlockCohesion = mSteeringForce->CanFlockCohesion();
	bool canWander = mSteeringForce->CanWander();
	bool canFollowPath = mSteeringForce->CanFollowPath();
	bool canPursueTarget = mSteeringForce->CanPursueTarget();
	bool canEvadeTarget = mSteeringForce->CanEvadeTarget();
	bool canInterpose = mSteeringForce->CanInterpose();
	bool canHide = mSteeringForce->CanHide();
	bool canOffsetPursuit = mSteeringForce->CanOffsetPursuit();
	
	ZShadeSandboxAI::WaypointSystem* waypointSystem = 0;
	if (mSteeringForce->WaypointSystem())
	{
		waypointSystem = mSteeringForce->WaypointSystem()->Clone();
	}
	
	AISprite* evader = 0;
	AISprite* pursuer = 0;
	AISprite* target = 0;
	AISprite* spriteA = 0;
	AISprite* spriteB = 0;
	AISprite* leader = 0;
	
	// Make sure a stack overflow error does not happen
	if (mSteeringForce->Evader()) evader = mSteeringForce->Evader()->Clone(gd);
	if (mSteeringForce->Pursuer()) pursuer = mSteeringForce->Pursuer()->Clone(gd);
	if (mSteeringForce->Target()) target = mSteeringForce->Target()->Clone(gd);
	if (mSteeringForce->SpriteA()) spriteA = mSteeringForce->SpriteA()->Clone(gd);
	if (mSteeringForce->SpriteB()) spriteB = mSteeringForce->SpriteB()->Clone(gd);
	if (mSteeringForce->Leader()) leader = mSteeringForce->Leader()->Clone(gd);
	
	ZShadeSandboxMath::XMMath3 leaderOffset = mSteeringForce->LeaderOffset();
	ZShadeSandboxMath::XMMath3 targetPoint = mSteeringForce->TargetPoint();
	EDeceleration::Type targetDecelerationType = mSteeringForce->TargetDecelerationType();
	ZShadeSandboxMath::XMMath3 wanderTarget = mSteeringForce->WanderTarget();
	
	string path;
	if (filename == "player_cover.png")
	{
		path = gd->m_required_textures;
	}
	else
	{
		path = gd->m_sprites_path;
	}
	
	//Add the sprite back in
	AISprite* spr = new AISprite(m_D3DSystem);
	spr->Initialize(path, filename, XMFLOAT3(TopLeftPosition().x, TopLeftPosition().y, 0), w, h, EPhysicsType());
	spr->IsDisplaySprite() = false;
	
	//Need to update all attributes of the sprite
	spr->PlatformerSprite() = is_platformer_sprite;
	spr->IsPlayer() = is_player;
	spr->ID() = id;
	spr->Vision() = vision;
	spr->Depth() = depth;
	spr->IsHard() = hard;
	spr->IsNoHit() = nohit;
	spr->AnimationProfileName() = animation_str;
	spr->DefaultSequence() = default_seq;
	spr->AnimationSpeed() = anim_speed;
	spr->HP() = hp;
	spr->Mana() = mana;
	spr->Gold() = gold;
	spr->Strength() = strength;
	spr->SpeedX() = speedX;
	spr->SpeedY() = speedY;
	spr->Defense() = defense;
	spr->Experience() = exp;
	spr->Level() = level;
	spr->ScriptName() = script_str;
	spr->IsTouch() = touch;
	spr->IsBackgroundSprite() = background;
	spr->IsEntitySprite() = entity;
	spr->IsInvisibleSprite() = invisible;
	spr->CanTeleport() = teleport;
	spr->TeleportMapName() = teleportMap_str;
	spr->TeleportX() = teleport_map_x;
	spr->TeleportY() = teleport_map_y;
	spr->Name() = name_str;
	spr->HPMax() = max_hp;
	spr->InventoryMessage() = inventoryMsg_str;
	spr->AlwaysSeenByPlayer() = alwaysSeenByPlayer;
	spr->SeenByPlayer() = seenByPlayer;
	spr->InFowCircle() = inFowCircle;

	// Clone the AI stuff
	spr->MinimumAttackDistance() = minimumAttackDistance;
	spr->SetWanderingRadius(wanderingRadius);
	spr->SetWanderingDistance(wanderingDistance);
	spr->SetWanderingJitter(wanderingJitter);
	spr->SetMinDetectionBoxLength(minDetectionBoxLength);
	spr->SetWaypointSeekDist(waypointSeekDist);
	spr->SetMaxFleeDistance(maxFleeDistance);
	spr->SetWeightSeek(weightSeek);
	spr->SetWeightFlee(weightFlee);
	spr->SetWeightArrive(weightArrive);
	spr->SetWeightSpriteAvoidance(weightSpriteAvoidance);
	spr->SetWeightSeparation(weightSeparation);
	spr->SetWeightAlignment(weightAlignment);
	spr->SetWeightCohesion(weightCohesion);
	spr->SetWeightWander(weightWander);
	spr->SetWeightFollowPath(weightFollowPath);
	spr->SetWeightPursueTarget(weightPursueTarget);
	spr->SetWeightEvadeTarget(weightEvadeTarget);
	spr->SetWeightInterpose(weightInterpose);
	spr->SetWeightHide(weightHide);
	spr->SetWeightOffsetPursuit(weightOffsetPursuit);
	spr->SetCanSeek(canSeek);
	spr->SetCanFlee(canFlee);
	spr->SetCanArrive(canArrive);
	spr->SetCanAvoidSprites(canAvoidSprites);
	spr->SetCanFlockSeparation(canFlockSeparation);
	spr->SetCanFlockAlignment(canFlockAlignment);
	spr->SetCanFlockCohesion(canFlockCohesion);
	spr->SetCanWander(canWander);
	spr->SetCanFollowPath(canFollowPath);
	spr->SetCanPursueTarget(canPursueTarget);
	spr->SetCanEvadeTarget(canEvadeTarget);
	spr->SetCanInterpose(canInterpose);
	spr->SetCanHide(canHide);
	spr->SetCanOffsetPursuit(canOffsetPursuit);
	spr->SetWaypointSystem(waypointSystem);
	spr->SetEvader(evader);
	spr->SetPursuer(pursuer);
	spr->SetHideTarget(target);
	spr->SetSpriteA(spriteA);
	spr->SetSpriteB(spriteB);
	spr->SetLeader(leader);
	spr->SetLeaderOffset(leaderOffset);
	spr->SetTargetPoint(targetPoint);
	spr->SetTargetDecelerationType(targetDecelerationType);
	spr->SetWanderTarget(wanderTarget);
	
	return spr;
}
//==================================================================================================================================
AISprite* AISprite::Clone(GameDirectory2D* gd, float x, float y)
{
	string filename = BaseTextureFilename();
	int w = Width();
	int h = Height();
	bool is_player = IsPlayer();
	bool is_platformer_sprite = m_platformerSprite;
	int vision = Vision();
	int depth = Depth();
	int hard = IsHard();
	int nohit = IsNoHit();
	int hp = HP();
	int mana = Mana();
	int gold = Gold();
	int strength = Strength();
	float speedX = SpeedX();
	float speedY = SpeedY();
	int defense = Defense();
	int exp = Experience();
	int level = Level();
	int touch = IsTouch();
	int background = IsBackgroundSprite();
	int entity = IsEntitySprite();
	int invisible = IsInvisibleSprite();
	int teleport = CanTeleport();
	int teleport_map_x = TeleportX();
	int teleport_map_y = TeleportY();
	int default_seq = DefaultSequence();
	int anim_speed = AnimationSpeed();
	int id = ID();
	int max_hp = HPMax();
	//int is_static = IsStatic();
	string name_str = Name();
	string animation_str = AnimationProfileName();
	string script_str = ScriptName();
	string teleportMap_str = TeleportMapName();
	string inventoryMsg_str = InventoryMessage();
	bool alwaysSeenByPlayer = AlwaysSeenByPlayer();
	bool seenByPlayer = SeenByPlayer();
	bool inFowCircle = InFowCircle();
	
	// Clone the AI stuff
	float minimumAttackDistance = fMinimumAttackDistance;
	float wanderingRadius = mSteeringForce->WanderingRadius();
	float wanderingDistance = mSteeringForce->WanderingDistance();
	float wanderingJitter = mSteeringForce->WanderingJitter();
	float minDetectionBoxLength = mSteeringForce->MinDetectionBoxLength();
	float waypointSeekDist = mSteeringForce->WaypointSeekDist();
	float maxFleeDistance = mSteeringForce->MaxFleeDistance();
	float weightSeek = mSteeringForce->WeightSeek();
	float weightFlee = mSteeringForce->WeightFlee();
	float weightArrive = mSteeringForce->WeightArrive();
	float weightSpriteAvoidance = mSteeringForce->WeightSpriteAvoidance();
	float weightSeparation = mSteeringForce->WeightSeparation();
	float weightAlignment = mSteeringForce->WeightAlignment();
	float weightCohesion = mSteeringForce->WeightCohesion();
	float weightWander = mSteeringForce->WeightWander();
	float weightFollowPath = mSteeringForce->WeightFollowPath();
	float weightPursueTarget = mSteeringForce->WeightPursueTarget();
	float weightEvadeTarget = mSteeringForce->WeightEvadeTarget();
	float weightInterpose = mSteeringForce->WeightInterpose();
	float weightHide = mSteeringForce->WeightHide();
	float weightOffsetPursuit = mSteeringForce->WeightOffsetPursuit();
	bool canSeek = mSteeringForce->CanSeek();
	bool canFlee = mSteeringForce->CanFlee();
	bool canArrive = mSteeringForce->CanArrive();
	bool canAvoidSprites = mSteeringForce->CanAvoidSprites();
	bool canFlockSeparation = mSteeringForce->CanFlockSeparation();
	bool canFlockAlignment = mSteeringForce->CanFlockAlignment();
	bool canFlockCohesion = mSteeringForce->CanFlockCohesion();
	bool canWander = mSteeringForce->CanWander();
	bool canFollowPath = mSteeringForce->CanFollowPath();
	bool canPursueTarget = mSteeringForce->CanPursueTarget();
	bool canEvadeTarget = mSteeringForce->CanEvadeTarget();
	bool canInterpose = mSteeringForce->CanInterpose();
	bool canHide = mSteeringForce->CanHide();
	bool canOffsetPursuit = mSteeringForce->CanOffsetPursuit();
	
	ZShadeSandboxAI::WaypointSystem* waypointSystem = 0;
	if (mSteeringForce->WaypointSystem())
	{
		waypointSystem = mSteeringForce->WaypointSystem()->Clone();
	}
	
	AISprite* evader = 0;
	AISprite* pursuer = 0;
	AISprite* target = 0;
	AISprite* spriteA = 0;
	AISprite* spriteB = 0;
	AISprite* leader = 0;
	
	// Make sure a stack overflow error does not happen
	if (mSteeringForce->Evader()) evader = mSteeringForce->Evader()->Clone(gd);
	if (mSteeringForce->Pursuer()) pursuer = mSteeringForce->Pursuer()->Clone(gd);
	if (mSteeringForce->Target()) target = mSteeringForce->Target()->Clone(gd);
	if (mSteeringForce->SpriteA()) spriteA = mSteeringForce->SpriteA()->Clone(gd);
	if (mSteeringForce->SpriteB()) spriteB = mSteeringForce->SpriteB()->Clone(gd);
	if (mSteeringForce->Leader()) leader = mSteeringForce->Leader()->Clone(gd);
	
	ZShadeSandboxMath::XMMath3 leaderOffset = mSteeringForce->LeaderOffset();
	ZShadeSandboxMath::XMMath3 targetPoint = mSteeringForce->TargetPoint();
	EDeceleration::Type targetDecelerationType = mSteeringForce->TargetDecelerationType();
	ZShadeSandboxMath::XMMath3 wanderTarget = mSteeringForce->WanderTarget();
	
	string path;
	if (filename == "player_cover.png")
	{
		path = gd->m_required_textures;
	}
	else
	{
		path = gd->m_sprites_path;
	}
	
	//Add the sprite back in
	AISprite* spr = new AISprite(m_D3DSystem);
	spr->Initialize(path, filename, XMFLOAT3(x, y, 0), w, h, EPhysicsType());
	spr->IsDisplaySprite() = false;
	
	//Need to update all attributes of the sprite
	spr->PlatformerSprite() = is_platformer_sprite;
	spr->IsPlayer() = is_player;
	spr->ID() = id;
	spr->Vision() = vision;
	spr->Depth() = depth;
	spr->IsHard() = hard;
	spr->IsNoHit() = nohit;
	spr->AnimationProfileName() = animation_str;
	spr->DefaultSequence() = default_seq;
	spr->AnimationSpeed() = anim_speed;
	spr->HP() = hp;
	spr->Mana() = mana;
	spr->Gold() = gold;
	spr->Strength() = strength;
	spr->SpeedX() = speedX;
	spr->SpeedY() = speedY;
	spr->Defense() = defense;
	spr->Experience() = exp;
	spr->Level() = level;
	spr->ScriptName() = script_str;
	spr->IsTouch() = touch;
	spr->IsBackgroundSprite() = background;
	spr->IsEntitySprite() = entity;
	spr->IsInvisibleSprite() = invisible;
	spr->CanTeleport() = teleport;
	spr->TeleportMapName() = teleportMap_str;
	spr->TeleportX() = teleport_map_x;
	spr->TeleportY() = teleport_map_y;
	spr->Name() = name_str;
	spr->HPMax() = max_hp;
	spr->InventoryMessage() = inventoryMsg_str;
	spr->AlwaysSeenByPlayer() = alwaysSeenByPlayer;
	spr->SeenByPlayer() = seenByPlayer;
	spr->InFowCircle() = inFowCircle;

	// Clone the AI stuff
	spr->MinimumAttackDistance() = minimumAttackDistance;
	spr->SetWanderingRadius(wanderingRadius);
	spr->SetWanderingDistance(wanderingDistance);
	spr->SetWanderingJitter(wanderingJitter);
	spr->SetMinDetectionBoxLength(minDetectionBoxLength);
	spr->SetWaypointSeekDist(waypointSeekDist);
	spr->SetMaxFleeDistance(maxFleeDistance);
	spr->SetWeightSeek(weightSeek);
	spr->SetWeightFlee(weightFlee);
	spr->SetWeightArrive(weightArrive);
	spr->SetWeightSpriteAvoidance(weightSpriteAvoidance);
	spr->SetWeightSeparation(weightSeparation);
	spr->SetWeightAlignment(weightAlignment);
	spr->SetWeightCohesion(weightCohesion);
	spr->SetWeightWander(weightWander);
	spr->SetWeightFollowPath(weightFollowPath);
	spr->SetWeightPursueTarget(weightPursueTarget);
	spr->SetWeightEvadeTarget(weightEvadeTarget);
	spr->SetWeightInterpose(weightInterpose);
	spr->SetWeightHide(weightHide);
	spr->SetWeightOffsetPursuit(weightOffsetPursuit);
	spr->SetCanSeek(canSeek);
	spr->SetCanFlee(canFlee);
	spr->SetCanArrive(canArrive);
	spr->SetCanAvoidSprites(canAvoidSprites);
	spr->SetCanFlockSeparation(canFlockSeparation);
	spr->SetCanFlockAlignment(canFlockAlignment);
	spr->SetCanFlockCohesion(canFlockCohesion);
	spr->SetCanWander(canWander);
	spr->SetCanFollowPath(canFollowPath);
	spr->SetCanPursueTarget(canPursueTarget);
	spr->SetCanEvadeTarget(canEvadeTarget);
	spr->SetCanInterpose(canInterpose);
	spr->SetCanHide(canHide);
	spr->SetCanOffsetPursuit(canOffsetPursuit);
	spr->SetWaypointSystem(waypointSystem);
	spr->SetEvader(evader);
	spr->SetPursuer(pursuer);
	spr->SetHideTarget(target);
	spr->SetSpriteA(spriteA);
	spr->SetSpriteB(spriteB);
	spr->SetLeader(leader);
	spr->SetLeaderOffset(leaderOffset);
	spr->SetTargetPoint(targetPoint);
	spr->SetTargetDecelerationType(targetDecelerationType);
	spr->SetWanderTarget(wanderTarget);
	
	return spr;
}
//==================================================================================================================================
void AISprite::CloneMe(GameDirectory2D* gd, float x, float y, int w, int h)
{
	string filename = BaseTextureFilename();
	
	//float hbox_x;
	//float hbox_y;
	//int hbox_w;
	//int hbox_h;
	//GetHardbox(hbox_x, hbox_y, hbox_w, hbox_h);
	
	bool is_player = IsPlayer();
	bool is_platformer_sprite = m_platformerSprite;
	int vision = Vision();
	int depth = Depth();
	int hard = IsHard();
	int nohit = IsNoHit();
	int hp = HP();
	int mana = Mana();
	int gold = Gold();
	int strength = Strength();
	float speedX = SpeedX();
	float speedY = SpeedY();
	int defense = Defense();
	int exp = Experience();
	int level = Level();
	int touch = IsTouch();
	int background = IsBackgroundSprite();
	int entity = IsEntitySprite();
	int invisible = IsInvisibleSprite();
	int teleport = CanTeleport();
	int teleport_map_x = TeleportX();
	int teleport_map_y = TeleportY();
	int default_seq = DefaultSequence();
	int anim_speed = AnimationSpeed();
	int id = ID();
	int max_hp = HPMax();
	//int is_static = IsStatic();
	string name_str = Name();
	string animation_str = AnimationProfileName();
	string script_str = ScriptName();
	string teleportMap_str = TeleportMapName();
	string inventoryMsg_str = InventoryMessage();
	bool alwaysSeenByPlayer = AlwaysSeenByPlayer();
	bool seenByPlayer = SeenByPlayer();
	bool inFowCircle = InFowCircle();
	
	// Clone the AI stuff
	float minimumAttackDistance = fMinimumAttackDistance;
	float wanderingRadius = mSteeringForce->WanderingRadius();
	float wanderingDistance = mSteeringForce->WanderingDistance();
	float wanderingJitter = mSteeringForce->WanderingJitter();
	float minDetectionBoxLength = mSteeringForce->MinDetectionBoxLength();
	float waypointSeekDist = mSteeringForce->WaypointSeekDist();
	float maxFleeDistance = mSteeringForce->MaxFleeDistance();
	float weightSeek = mSteeringForce->WeightSeek();
	float weightFlee = mSteeringForce->WeightFlee();
	float weightArrive = mSteeringForce->WeightArrive();
	float weightSpriteAvoidance = mSteeringForce->WeightSpriteAvoidance();
	float weightSeparation = mSteeringForce->WeightSeparation();
	float weightAlignment = mSteeringForce->WeightAlignment();
	float weightCohesion = mSteeringForce->WeightCohesion();
	float weightWander = mSteeringForce->WeightWander();
	float weightFollowPath = mSteeringForce->WeightFollowPath();
	float weightPursueTarget = mSteeringForce->WeightPursueTarget();
	float weightEvadeTarget = mSteeringForce->WeightEvadeTarget();
	float weightInterpose = mSteeringForce->WeightInterpose();
	float weightHide = mSteeringForce->WeightHide();
	float weightOffsetPursuit = mSteeringForce->WeightOffsetPursuit();
	bool canSeek = mSteeringForce->CanSeek();
	bool canFlee = mSteeringForce->CanFlee();
	bool canArrive = mSteeringForce->CanArrive();
	bool canAvoidSprites = mSteeringForce->CanAvoidSprites();
	bool canFlockSeparation = mSteeringForce->CanFlockSeparation();
	bool canFlockAlignment = mSteeringForce->CanFlockAlignment();
	bool canFlockCohesion = mSteeringForce->CanFlockCohesion();
	bool canWander = mSteeringForce->CanWander();
	bool canFollowPath = mSteeringForce->CanFollowPath();
	bool canPursueTarget = mSteeringForce->CanPursueTarget();
	bool canEvadeTarget = mSteeringForce->CanEvadeTarget();
	bool canInterpose = mSteeringForce->CanInterpose();
	bool canHide = mSteeringForce->CanHide();
	bool canOffsetPursuit = mSteeringForce->CanOffsetPursuit();
	
	ZShadeSandboxAI::WaypointSystem* waypointSystem = 0;
	if (mSteeringForce->WaypointSystem())
	{
		waypointSystem = mSteeringForce->WaypointSystem()->Clone();
	}
	
	AISprite* evader = 0;
	AISprite* pursuer = 0;
	AISprite* target = 0;
	AISprite* spriteA = 0;
	AISprite* spriteB = 0;
	AISprite* leader = 0;
	
	// Make sure a stack overflow error does not happen
	if (mSteeringForce->Evader()) evader = mSteeringForce->Evader()->Clone(gd);
	if (mSteeringForce->Pursuer()) pursuer = mSteeringForce->Pursuer()->Clone(gd);
	if (mSteeringForce->Target()) target = mSteeringForce->Target()->Clone(gd);
	if (mSteeringForce->SpriteA()) spriteA = mSteeringForce->SpriteA()->Clone(gd);
	if (mSteeringForce->SpriteB()) spriteB = mSteeringForce->SpriteB()->Clone(gd);
	if (mSteeringForce->Leader()) leader = mSteeringForce->Leader()->Clone(gd);
	
	ZShadeSandboxMath::XMMath3 leaderOffset = mSteeringForce->LeaderOffset();
	ZShadeSandboxMath::XMMath3 targetPoint = mSteeringForce->TargetPoint();
	EDeceleration::Type targetDecelerationType = mSteeringForce->TargetDecelerationType();
	ZShadeSandboxMath::XMMath3 wanderTarget = mSteeringForce->WanderTarget();
	
	string path;
	if (filename == "player_cover.png")
	{
		path = gd->m_required_textures;
	}
	else
	{
		path = gd->m_sprites_path;
	}
	
	//Add the sprite back in
	Initialize(path, filename, XMFLOAT3(x, y, 0), w, h, EPhysicsType());

	IsDisplaySprite() = false;
	
	//Need to update all attributes of the sprite
	PlatformerSprite() = is_platformer_sprite;
	IsPlayer() = is_player;
	ID() = id;
	Vision() = vision;
	Depth() = depth;
	IsHard() = hard;
	IsNoHit() = nohit;
	AnimationProfileName() = animation_str;
	DefaultSequence() = default_seq;
	AnimationSpeed() = anim_speed;
	HP() = hp;
	Mana() = mana;
	Gold() = gold;
	Strength() = strength;
	SpeedX() = speedX;
	SpeedY() = speedY;
	Defense() = defense;
	Experience() = exp;
	Level() = level;
	ScriptName() = script_str;
	IsTouch() = touch;
	IsBackgroundSprite() = background;
	IsEntitySprite() = entity;
	IsInvisibleSprite() = invisible;
	CanTeleport() = teleport;
	TeleportMapName() = teleportMap_str;
	TeleportX() = teleport_map_x;
	TeleportY() = teleport_map_y;
	Name() = name_str;
	HPMax() = max_hp;
	InventoryMessage() = inventoryMsg_str;
	AlwaysSeenByPlayer() = alwaysSeenByPlayer;
	SeenByPlayer() = seenByPlayer;
	InFowCircle() = inFowCircle;

	// Clone the AI stuff
	MinimumAttackDistance() = minimumAttackDistance;
	SetWanderingRadius(wanderingRadius);
	SetWanderingDistance(wanderingDistance);
	SetWanderingJitter(wanderingJitter);
	SetMinDetectionBoxLength(minDetectionBoxLength);
	SetWaypointSeekDist(waypointSeekDist);
	SetMaxFleeDistance(maxFleeDistance);
	SetWeightSeek(weightSeek);
	SetWeightFlee(weightFlee);
	SetWeightArrive(weightArrive);
	SetWeightSpriteAvoidance(weightSpriteAvoidance);
	SetWeightSeparation(weightSeparation);
	SetWeightAlignment(weightAlignment);
	SetWeightCohesion(weightCohesion);
	SetWeightWander(weightWander);
	SetWeightFollowPath(weightFollowPath);
	SetWeightPursueTarget(weightPursueTarget);
	SetWeightEvadeTarget(weightEvadeTarget);
	SetWeightInterpose(weightInterpose);
	SetWeightHide(weightHide);
	SetWeightOffsetPursuit(weightOffsetPursuit);
	SetCanSeek(canSeek);
	SetCanFlee(canFlee);
	SetCanArrive(canArrive);
	SetCanAvoidSprites(canAvoidSprites);
	SetCanFlockSeparation(canFlockSeparation);
	SetCanFlockAlignment(canFlockAlignment);
	SetCanFlockCohesion(canFlockCohesion);
	SetCanWander(canWander);
	SetCanFollowPath(canFollowPath);
	SetCanPursueTarget(canPursueTarget);
	SetCanEvadeTarget(canEvadeTarget);
	SetCanInterpose(canInterpose);
	SetCanHide(canHide);
	SetCanOffsetPursuit(canOffsetPursuit);
	SetWaypointSystem(waypointSystem);
	SetEvader(evader);
	SetPursuer(pursuer);
	SetHideTarget(target);
	SetSpriteA(spriteA);
	SetSpriteB(spriteB);
	SetLeader(leader);
	SetLeaderOffset(leaderOffset);
	SetTargetPoint(targetPoint);
	SetTargetDecelerationType(targetDecelerationType);
	SetWanderTarget(wanderTarget);
}
//==================================================================================================================================
void AISprite::updateBehavior()
{
	if (tree == 0) return;
	
	//Think about what we are doing
	Think();
	
	if (m_run_behavior)
	{
		tree->RunBehavior(m_behavior_name);
		tree->TickActiveBehavior();
	}
	else
	{
		if (m_run_behavior_default)
		{
			//Run default behavior
			tree->RunBehavior(m_default_behavior);
			tree->TickActiveBehavior();
		}
	}
}
//==================================================================================================================================
void AISprite::AddDefaultBehavior(string p_default_behavior_name)
{
	m_default_behavior = p_default_behavior_name;
}
//==================================================================================================================================
void AISprite::AddBehaviorSequence(string p_behavior_name, string p_sequence_name, string p_action_func_name, string p_condition_func_name)
{
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		if (m_behaviors[i]->name == p_behavior_name)
		{
			SEQUENCE_INFO* si = new SEQUENCE_INFO();
			si->name = p_sequence_name;
			si->action_func_name = p_action_func_name;
			si->condition_func_name = p_condition_func_name;
			m_behaviors[i]->sequence_info.push_back( si );
		}
	}
}
//==================================================================================================================================
void AISprite::RegisterBehavior(string p_behavior_name)
{
	BEHAVIOR_INFO* bi = new BEHAVIOR_INFO();

	bi->name = p_behavior_name;

	m_behaviors.push_back( bi );
}
//==================================================================================================================================
void AISprite::CreateTree()
{
	tree = new BehaviorTree();

	for (int i = 0; i < m_behaviors.size(); i++)
	{
		Behavior* aiBehavior = new Behavior(m_behaviors[i]->name);
		{
			PrioritySelector* selector = new PrioritySelector();
			{
				for (int j = 0; j < m_behaviors[i]->sequence_info.size(); j++)
				{
					Sequence* sequence = new Sequence();
					{
						Node* sequence_node = new TaskNode();
						{
							sequence_node->SetSequenceLevel(1);
							Decorator* sequence_decorator = new Decorator();
							{
								LuaTask* action = new LuaTask();
								LuaDelegate* condition = new LuaDelegate(this);
								action->SetLuaState(Scripting::LuaState());
								condition->SetLuaState(Scripting::LuaState());
								string path = Scripting::GetGameDirectory()->m_scripts_path;
								path.append("\\");
								path.append(m_script_name);
								action->AddLuaAction(this, m_behaviors[i]->sequence_info[j]->name, path, m_behaviors[i]->sequence_info[j]->action_func_name);
								condition->AddFunction(path, m_behaviors[i]->sequence_info[j]->condition_func_name);
								sequence_decorator->SetUseLua(true);
								sequence_decorator->AddDecoratedComponent(action);
								sequence_decorator->AddCondition(condition);
							}
							sequence_node->AddDecorator( sequence_decorator );
						}
						sequence->AddChildTask( sequence_node );
					}
					selector->AddSequence( sequence );
				}
			}
			aiBehavior->AddSelector( selector );
		}
		tree->InsertBehavior( aiBehavior );
	}
}
//==================================================================================================================================
void AISprite::Main()
{
	if (!m_ran_main)
	{
		m_script->Main(1, this);

		m_ran_main = true;
	}
}
//==================================================================================================================================
void AISprite::MoveScript()
{
	// I can move
	if (m_moving)
	{
		m_script->Move(1, this);
	}
}
//==================================================================================================================================
void AISprite::Think()
{
	// I can think about what to do
	if (m_thinking)
	{
		m_script->Think(1, this);
	}
}
//==================================================================================================================================
void AISprite::Talk()
{
	// I decided to talk
	if (m_talking)
	{
		m_script->Talk(1, this);
	}
}
//==================================================================================================================================
void AISprite::Die()
{
	// I was hurt badly and am dying
	if (m_dying)
	{
		m_script->Die(1, this);
	}
}
//==================================================================================================================================
void AISprite::Hit()
{
	// I was hit
	if (m_hit)
	{
		m_script->Hit(1, this);
	}
}
//==================================================================================================================================
void AISprite::Touch()
{
	// I was touched
	if (m_touched)
	{
		m_script->Touch(1, this);
	}
}
//==================================================================================================================================
void AISprite::Attack()
{
	// I will be attacking a sprite
	if (m_attacked)
	{
		if (m_TargetSprite != NULL)
		{
			m_script->Attack(2, this, m_TargetSprite);
		}
	}
}
//==================================================================================================================================
void AISprite::Target()
{
	// I will be targeting a sprite
	if (m_targeted)
	{
		if (m_TargetSprite != NULL)
		{
			m_script->Target(2, this, m_TargetSprite);
		}
	}
}
//==================================================================================================================================