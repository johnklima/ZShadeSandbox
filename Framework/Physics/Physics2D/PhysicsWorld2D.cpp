#include "PhysicsWorld2D.h"
#include "TopdownMap.h"
#include "PlatformerMap.h"
#include "Convert.h"
#include "Forces.h"
#include "PhysicsBody2D.h"
//==================================================================================================================================
//==================================================================================================================================
float Forces::GRAVITATION_EARTH = 9.798f;
float Forces::GRAVITATION_URANUS = 8.87f;
float Forces::GRAVITATION_VENUS = 8.87f;
float Forces::GRAVITATION_MARS = 3.71f;
float Forces::GRAVITATION_MERCURY = 3.7f;
float Forces::GRAVITATION_MOON = 1.62f;
float Forces::GRAVITATION_PLUTO = 0.58f;
float Forces::JUMP_SPEED_LOW = 40.0f;
float Forces::JUMP_SPEED_HIGH = 60.0f;
float Forces::ENERGYLOSS = 0.65f;
float Forces::MAX_SPEED = 300;
float Forces::GROUND_FRICTION = 0.6f;// friction with ground - 1=totally sticky, 0=ice
float Forces::DT = 0.2f;
//==================================================================================================================================
//==================================================================================================================================
PhysicsWorld2D::PhysicsWorld2D(D3D* d3d, TopdownMap* map)
:   m_TopdownMap(map)
,   m_PlatformerMap(NULL)
,   bGravityOn(false)
,   m_D3DSystem(d3d)
{
	m_EngineOptions = d3d->GetEngineOptions();
	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			text[i][j].SetD3D(m_D3DSystem);
			text[i][j].Initialize();
			text[i][j].CreateShadow();
			text[i][j].ChangeFont(L"Times New Roman", 24.0f, FontStyle::BoldItalic, true);
			text[i][j].TextName() = "Physics World Text";
		}
		
		m_ContactStatsPlayerToTile[i].collidingBodies = 0;
		m_ContactStatsPlayerToTile[i].boundingBoxes = 0;
		m_ContactStatsPlayerToTile[i].speculativeContacts = 0;
		m_ContactStatsPlayerToTile[i].speculativeDiscards = 0;
		m_ContactStatsPlayerToTile[i].penetrationCorrections = 0;
		m_ContactStatsPlayerToTile[i].contacts = 0;
	}
}
//==================================================================================================================================
PhysicsWorld2D::PhysicsWorld2D(D3D* d3d, PlatformerMap* map)
:   m_PlatformerMap(map)
,   m_TopdownMap(NULL)
,   bGravityOn(true)
,   m_D3DSystem(d3d)
{
	m_EngineOptions = d3d->GetEngineOptions();
	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			text[i][j].SetD3D(m_D3DSystem);
			text[i][j].Initialize();
			text[i][j].CreateShadow();
			text[i][j].ChangeFont(L"Times New Roman", 24.0f, FontStyle::BoldItalic, true);
			text[i][j].TextName() = "Physics World Text";
		}

		m_ContactStatsPlayerToTile[i].collidingBodies = 0;
		m_ContactStatsPlayerToTile[i].boundingBoxes = 0;
		m_ContactStatsPlayerToTile[i].speculativeContacts = 0;
		m_ContactStatsPlayerToTile[i].speculativeDiscards = 0;
		m_ContactStatsPlayerToTile[i].penetrationCorrections = 0;
		m_ContactStatsPlayerToTile[i].contacts = 0;
	}
}
//==================================================================================================================================
void PhysicsWorld2D::BuildTileBodies()
{
	if (m_Bodies.size() > 0)
		m_Bodies.clear();
	
	if (m_TopdownMap != NULL)
	{
		for (int y = 0; y < m_TopdownMap->Height(); y++)
		{
			for (int x = 0; x < m_TopdownMap->Width(); x++)
			{
				if (m_TopdownMap->GetTile(x, y) != NULL)
				{
					m_Bodies.push_back(m_TopdownMap->GetTile(x, y));
				}
			}
		}
	}
	
	if (m_PlatformerMap != NULL)
	{
		for (int y = 0; y < m_PlatformerMap->Height(); y++)
		{
			for (int x = 0; x < m_PlatformerMap->Width(); x++)
			{
				if (m_PlatformerMap->GetTile(x, y) != NULL)
				{
					m_Bodies.push_back( m_PlatformerMap->GetTile(x, y) );
				}
			}
		}
	}
}
//==================================================================================================================================
void PhysicsWorld2D::RenderContactStats()
{
	int oldY = 0;

	for (int i = 0; i < 3; i++)
	{
		BetterString collidingBodiesStr = ZShadeSandboxGlobal::Convert::ConvertIntToString(m_ContactStatsPlayerToTile[i].collidingBodies);
		BetterString boundingBoxesStr = ZShadeSandboxGlobal::Convert::ConvertIntToString(m_ContactStatsPlayerToTile[i].boundingBoxes);
		BetterString speculativeContactsStr = ZShadeSandboxGlobal::Convert::ConvertIntToString(m_ContactStatsPlayerToTile[i].speculativeContacts);
		BetterString speculativeDiscardsStr = ZShadeSandboxGlobal::Convert::ConvertIntToString(m_ContactStatsPlayerToTile[i].speculativeDiscards);
		BetterString penetrationCorrectionsStr = ZShadeSandboxGlobal::Convert::ConvertIntToString(m_ContactStatsPlayerToTile[i].penetrationCorrections);
		BetterString contactsStr = ZShadeSandboxGlobal::Convert::ConvertIntToString(m_ContactStatsPlayerToTile[i].contacts);
		string iStr = ZShadeSandboxGlobal::Convert::ConvertIntToString(i);
		BetterString iterStr = "Player to Tile Contact Iteration: ";
		iterStr += iStr;

		XMFLOAT4 color = XMFLOAT4(1, 0, 0, 1);

		int x = 25;
		int y = i * 15 * 2 + 10 + oldY;

		XMMATRIX textTransform0 = XMMatrixTranslation(x, y, 0);
		textTransform0 = XMMatrixMultiply(textTransform0, XMMatrixScaling(1, 1, 1));
		text[i][0].TextTransform() = textTransform0;
		text[i][0].TextColor() = color;
		text[i][0].Render11(iterStr);
		
		y += 24;

		XMMATRIX textTransform1 = XMMatrixTranslation(x, y, 0);
		textTransform1 = XMMatrixMultiply(textTransform1, XMMatrixScaling(1, 1, 1));
		text[i][1].TextTransform() = textTransform1;
		text[i][1].TextColor() = color;
		text[i][1].Render11(collidingBodiesStr + " nearby bodies");
		
		y += 24;

		XMMATRIX textTransform2 = XMMatrixTranslation(x, y, 0);
		textTransform2 = XMMatrixMultiply(textTransform2, XMMatrixScaling(1, 1, 1));
		text[i][2].TextTransform() = textTransform2;
		text[i][2].TextColor() = color;
		text[i][2].Render11(boundingBoxesStr + " nearby geometries");
		
		y += 24;

		XMMATRIX textTransform3 = XMMatrixTranslation(x, y, 0);
		textTransform3 = XMMatrixMultiply(textTransform3, XMMatrixScaling(1, 1, 1));
		text[i][3].TextTransform() = textTransform3;
		text[i][3].TextColor() = color;
		text[i][3].Render11(speculativeContactsStr + " speculative contact pixels");
		
		y += 24;

		XMMATRIX textTransform4 = XMMatrixTranslation(x, y, 0);
		textTransform4 = XMMatrixMultiply(textTransform4, XMMatrixScaling(1, 1, 1));
		text[i][4].TextTransform() = textTransform4;
		text[i][4].TextColor() = color;
		text[i][4].Render11(speculativeDiscardsStr + " speculative contact AABBs pruned");
		
		y += 24;

		XMMATRIX textTransform5 = XMMatrixTranslation(x, y, 0);
		textTransform5 = XMMatrixMultiply(textTransform5, XMMatrixScaling(1, 1, 1));
		text[i][5].TextTransform() = textTransform5;
		text[i][5].TextColor() = color;
		text[i][5].Render11(penetrationCorrectionsStr + " penetration adjustments");
		
		y += 24;

		XMMATRIX textTransform6 = XMMatrixTranslation(25, y, 0);
		textTransform6 = XMMatrixMultiply(textTransform6, XMMatrixScaling(1, 1, 1));
		text[i][6].TextTransform() = textTransform6;
		text[i][6].TextColor() = color;
		text[i][6].Render11(contactsStr + " contacts solved");

		oldY = y;
	}
}
//==================================================================================================================================
void PhysicsWorld2D::Update(float dt, Keyboard* keyboard)
{
	// No world to collide with
	if (m_Bodies.size() == 0) return;
	
	//
	// Are we in a Topdown world
	//
	
	if (m_TopdownMap != NULL)
	{
		// Topdown world has no gravity
		bGravityOn = false;
		
		// Check for collision of Sprite -> Tiles that have been clipped
		for (int i = 0; i < m_TopdownMap->TotalClippedSprites(); i++)
		{
			if (m_TopdownMap->ClippedSprite(i) == NULL) continue;
			
			//// In a topdown map the sprite only collides with a tile like running against a wall.
			//m_TopdownMap->ClippedSprite(i)->CollideBodiesTopdown(m_Bodies, m_BoundBodies, m_ContactStatsPlayerToTile, dt);
			//
			//// Check for collision of Sprite -> Sprite
			//for (int j = 0; j < m_TopdownMap->TotalClippedSprites(); j++)
			//{
			//	if (i == j) continue;
			//	if (m_TopdownMap->ClippedSprite(j) == NULL) continue;
			//	
			//	// Is current sprite colliding with another sprite
			//	if (m_TopdownMap->ClippedSprite(i)->CollidesBodyTopdown(m_TopdownMap->ClippedSprite(j), dt))
			//		break;// A collision occured and has been handled with the colliding sprite
			//}
			
			// If the AI decided to do something then get the force of what it has done
			// This must be done before update so that movement can be taken into account
			m_TopdownMap->ClippedSprite(i)->CalculateSteeringForce();
			
			// Update movement for Sprites, The player is updated with user input as well
			m_TopdownMap->ClippedSprite(i)->IsGravityOn() = bGravityOn;
			m_TopdownMap->ClippedSprite(i)->Update(dt, keyboard);
		}
	}
	
	//
	// Are we in a Platformer world
	//
	
	if (m_PlatformerMap != NULL)
	{
		// Update any moving platforms
		for (int i = 0; i < m_Bodies.size(); i++)
		{
			if (m_Bodies[i]->IsPlatform())
			{
				m_Bodies[i]->UpdateMovingPlatform();
			}
		}

		// Check for collision of Sprite -> Tiles that have been clipped
		for (int i = 0; i < m_PlatformerMap->TotalClippedSprites(); i++)
		{
			if (m_PlatformerMap->ClippedSprite(i) == NULL) continue;
			
			// Check if this sprite is colliding against a tile, ladder or platform
			m_PlatformerMap->ClippedSprite(i)->CollideBodiesPlatformer(m_Bodies, m_BoundBodies, m_ContactStatsPlayerToTile, dt);
			
			// Check for collision of Sprite -> Sprite
			for (int j = 0; j < m_PlatformerMap->TotalClippedSprites(); j++)
			{
				if (i == j) continue;
				if (m_PlatformerMap->ClippedSprite(j) == NULL) continue;
				
				// Is current sprite colliding with another sprite
				if (m_PlatformerMap->ClippedSprite(i)->CollidesBodyPlatformer(m_PlatformerMap->ClippedSprite(j), dt))
					break;// A collision occured and has been handled with another sprite
			}
			
			// If the AI decided to do something then get the force of what it has done
			// This must be done before update so that movement can be taken into account
			m_PlatformerMap->ClippedSprite(i)->CalculateSteeringForce();
			
			// Update movement for Sprites, The player is updated with user input as well
			m_PlatformerMap->ClippedSprite(i)->IsGravityOn() = bGravityOn;
			m_PlatformerMap->ClippedSprite(i)->Update(dt, keyboard);
		}
	}
}
//==================================================================================================================================