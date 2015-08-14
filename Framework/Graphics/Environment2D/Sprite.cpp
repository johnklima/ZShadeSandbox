#include "Sprite.h"
#include "MaterialManager.h"
#include "Convert.h"
#include "SAT.h"
#include "Scalar.h"
#include "Forces.h"
#include "PlatformerMap.h"
#include "Scripting.h"
#include "AISprite.h"
#include "PhysicsBody2D.h"
//==================================================================================================================================
//==================================================================================================================================
Sprite::Sprite(D3D* d3d)
:   PhysicsBody2D(d3d),
	m_D3DSystem(d3d)
{
	m_EngineOptions = m_D3DSystem->GetEngineOptions();
	InitializeStats();
}
//==================================================================================================================================
Sprite::~Sprite()
{
}
//==================================================================================================================================
bool Sprite::Initialize(string base_path, string textureFilename, XMFLOAT3 position, int bitmapWidth, int bitmapHeight, PhysicsType pt)
{
	PhysicsBody2D::Initialize(position, pt, ESurfaceType::Type::eNormal, bitmapWidth, bitmapHeight);
	
	// Initialize the body with platformer or topdown stats in movement
	if (m_EngineOptions->m_GameType2D == TOPDOWN)
	{
		PhysicsBody2D::InitializeParametersTopdown();
	}
	else if (m_EngineOptions->m_GameType2D == PLATFORMER)
	{
		PhysicsBody2D::InitializeParametersPlatformer();
	}
	
	m_baseTextureFilename = textureFilename;

	InitializeStats();
	
	m_imageWidth = bitmapWidth;
	m_imageHeight = bitmapHeight;
	
	MaterialManager::Instance()->AddTransparentDiffuse(m_D3DSystem, base_path,
		textureFilename, textureFilename, textureFilename, m_EngineOptions->m_TransparencyKey);

	ZShadeSandboxMesh::MeshParameters mp;
	mp.pos = XMFLOAT3(0, 0, 0);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(1, 1, 1);
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp.useCustomShader = false;
	mp.material = MaterialManager::Instance()->GetMaterial(textureFilename);
	mp.in2D = true;
	m_bitmap = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, m_imageWidth, m_imageHeight, true);
	if (!m_bitmap) return false;

	m_text.SetD3D(m_D3DSystem);
	m_text.Initialize();
	m_text.CreateShadow();
	m_text.ChangeFont(L"Times New Roman", 24.0f, FontStyle::BoldItalic, true);
	m_text.TextName() = "I am a Sprite";
	m_text_transform = XMMatrixTranslation(position.x, position.y + 20, 0);
	m_text_color = XMFLOAT4(1, 0, 0, 1);
	m_display_text = false;
	
	return true;
}
//==================================================================================================================================
void Sprite::InitializeStats()
{
	kPlatformerJumpVelocity = 900.0f * 1.2f;
	kReachTargetScale = 0.7f;
	
	m_alwaysSeenByPlayer = true;
	m_seenByPlayer = true;
	m_inFowCircle = false;
	
	m_move_up = false;
	m_move_down = false;
	m_move_left = false;
	m_move_right = false;

	m_CurrentSequence = NULL;
	m_displaySprite = false;
	m_cloneDisplaySprite = false;
	m_displayHardbox = false;
	m_player = false;
	//m_static = true;
	m_platformerSprite = false;
	bFlyMode = false;
	bVisible = true;

	m_text_timer = new SleepTimer();
	
	m_script = NULL;

	m_old_speed = 0;
	m_depth = 0;
	m_damageAmount = 0;
	m_vision = 0;
	m_hard = 0;
	m_nohit = 0;
	m_animationProfileName = "NONE";
	default_sequence = 0;
	m_animationSpeed = 0;
	m_hp_max = 100;
	m_hp = 100;
	m_mana = 100;
	m_gold = 0;
	m_strength = 0;
	m_defense = 0;
	m_experience = 0;
	m_level = 1;
	m_script_name = "NONE";
	m_touch = 0;
	m_background = 0;
	m_entity = 1;
	m_invisible = 0;
	m_can_teleport = 0;
	m_teleport_map = "NONE";
	m_teleport_x = -1;
	m_teleport_y = -1;
	m_name = "NONE";
	m_inventoryMessage = "NONE";

	m_moving = false;
	m_thinking = false;
	m_talking = false;
	m_dying = false;
	m_hit = false;
	m_touched = false;
	m_attacked = false;
	m_targeted = false;
}
//==================================================================================================================================
void Sprite::OnCollision(bool bTop, bool bBottom, bool bLeft, bool bRight, int damageAmount)
{
	// I am hurt so leave me alone until I am not invincible
	if (IsHurt()) return;
	
	// This sprite can be hit
	if ((bTop && VulnerableFromAbove()) ||
	    (bBottom && VulnerableFromBelow()) ||
		(bLeft && VulnerableFromLeft()) ||
		(bRight && VulnerableFromRight()))
	{
		// I was hit make sure I don't take too much damage too fast
		MakeTemporarilyInvincible();
		
		// Tell the AI part of me that I was hit
		m_hit = true;
		
		// Tell Lua how much damage I am suppose to take after being hit
		m_damageAmount = damageAmount;
	}
}
//==================================================================================================================================
void Sprite::Update(float dt, Keyboard* keyboard)
{
	/*
	if (m_platformerSprite)
	{
		if (m_PlatformerMap == NULL)
			return;
	}
	else
	{
		if (m_TopdownMap == NULL)
			return;
	}

	// Need to edit keyboard control for the player, maybe add player control in sprite class
	if (m_player && m_platformerSprite)
	{
		PlayerPlatformerInput(dt, keyboard);
	}

	if (bFlyMode)
	{
		//m_Velocity.x *= 0.5f;
		//m_Velocity.y *= 0.5f;
	}
	else
	{
		//m_Velocity.y += GRAVITATION_MARS;
	}
	
	// Clamp the speed
	m_Velocity.x = Scalar::Clamp(m_Velocity.x, -MAX_SPEED, MAX_SPEED);
	m_Velocity.y = min( m_Velocity.y, MAX_SPEED * 2 );
	
	PhysicsBody::Update(dt);
	*/
	
	bPlayer = m_player;
	
	if (m_player)
	{
		if (m_platformerSprite)
			PlayerPlatformerInput(dt, keyboard);
		else
			PlayerTopdownInput(dt, keyboard);
	}
	
	// Move the body if it is dynamic
	if (m_platformerSprite)
		PhysicsBody2D::MovePlatformer(dt);
	else
		PhysicsBody2D::MoveTopdown(dt);
	
	if (iHurtTimer > 0)
	{
		bVisible = (iHurtTimer & 1) == 1;
		
		iHurtTimer--;
	}
}
//==================================================================================================================================
void Sprite::PlayerPlatformerInput(float dt, Keyboard* keyboard)
{
	//
	// normal walking controls
	//

	if (keyboard->RisingEdge(Keyboard::Key::A))
	{
		bMovingLeft = true;

		// Face Left
		SetMoveSpriteLeft(true);
		SetMoveSpriteRight(false);
	}
	else if (keyboard->RisingEdge(Keyboard::Key::D))
	{
		bMovingRight = true;

		// Face Right
		SetMoveSpriteRight(true);
		SetMoveSpriteLeft(false);
	}
	else
	{
		bMovingLeft = false;
		bMovingRight = false;
		
		SetMoveSpriteLeft(false);
		SetMoveSpriteRight(false);
	}
}
//==================================================================================================================================
void Sprite::PlayerTopdownInput(float dt, Keyboard* keyboard)
{
	//
	// normal walking controls
	//
	
	if (keyboard->IsKeyDown(Keyboard::Key::W))
	{
		bMovingUp = true;

		// Face Up
		SetMoveSpriteUp(true);
		SetMoveSpriteDown(false);
		
		accY = SpeedX();
	}
	else if (keyboard->IsKeyDown(Keyboard::Key::S))
	{
		bMovingDown = true;

		// Face Down
		SetMoveSpriteUp(true);
		SetMoveSpriteDown(false);
		
		accY = SpeedX();
	}
	else
	{
		bMovingUp = false;
		bMovingDown = false;
		
		SetMoveSpriteUp(false);
		SetMoveSpriteDown(false);
		
		// Reset the speed to 0, so that the sprite does not slide
		speedX = 0;
	}
	
	if (keyboard->IsKeyDown(Keyboard::Key::A))
	{
		bMovingLeft = true;

		// Face Left
		SetMoveSpriteLeft(true);
		SetMoveSpriteRight(false);
		
		accXf = SpeedX();
	}
	else if (keyboard->IsKeyDown(Keyboard::Key::D))
	{
		bMovingRight = true;

		// Face Right
		SetMoveSpriteRight(true);
		SetMoveSpriteLeft(false);
		
		accXb = SpeedX();
	}
	else
	{
		bMovingLeft = false;
		bMovingRight = false;
		
		SetMoveSpriteLeft(false);
		SetMoveSpriteRight(false);
		
		// Reset the acceleration speed to 0, so that the sprite does not slide
		speedX = 0;
	}
}
//==================================================================================================================================
void Sprite::CreateTexture(string base_path, string windowName, int screenWidth, int screenHeight)
{
	ZShadeSandboxMesh::MeshParameters mp;
	mp.pos = XMFLOAT3(0, 0, 0);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(1, 1, 1);
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp.useCustomShader = false;
	mp.in2D = true;
	m_bitmap = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, m_imageWidth, m_imageHeight, true);
}
//==================================================================================================================================
void Sprite::UpdateMeshTexture(string base_sprite_path)
{
	// Creates new material if one does not exist, if it does exist then do nothing
	MaterialManager::Instance()->AddTransparentDiffuse(m_D3DSystem, base_sprite_path,
		m_baseTextureFilename, m_baseTextureFilename, m_baseTextureFilename, m_EngineOptions->m_TransparencyKey);

	// Grab the material whether it existed or not
	m_bitmap->SetMaterial(MaterialManager::Instance()->GetMaterial(m_baseTextureFilename));
}
//==================================================================================================================================
void Sprite::UpdateMeshTexture(string base_sprite_path, string textureFilename)
{
	// Creates new material if one does not exist, if it does exist then do nothing
	MaterialManager::Instance()->AddTransparentDiffuse(m_D3DSystem, base_sprite_path,
		textureFilename, textureFilename, textureFilename, m_EngineOptions->m_TransparencyKey);

	// Grab the material whether it existed or not
	m_bitmap->SetMaterial(MaterialManager::Instance()->GetMaterial(textureFilename));
}
//==================================================================================================================================
void Sprite::say(string text)
{
	m_text.TextName() = text;
	m_text_transform = XMMatrixTranslation(TopLeftPosition().x, TopLeftPosition().y + 20, 0);
	m_display_text = true;
}
//==================================================================================================================================
void Sprite::say(string text, int font_size)
{
	m_text.TextName() = text;
	m_text.ChangeFont(L"Times New Roman", font_size, FontStyle::BoldItalic, true);
	m_text_transform = XMMatrixTranslation(TopLeftPosition().x, TopLeftPosition().y + 20, 0);
	m_display_text = true;
}
//==================================================================================================================================
void Sprite::say(string text, int x, int y, int font_size)
{
	m_text.TextName() = text;
	m_text.ChangeFont(L"Times New Roman", font_size, FontStyle::BoldItalic, true);
	m_text_transform = XMMatrixTranslation(x, y + 20, 0);
	m_display_text = true;
}
//==================================================================================================================================
void Sprite::say(string text, int r, int g, int b, int font_size)
{
	m_text.TextName() = text;
	m_text.ChangeFont(L"Times New Roman", font_size, FontStyle::BoldItalic, true);
	m_text_transform = XMMatrixTranslation(m_TopLeftPosition.x, m_TopLeftPosition.y + 20, 0);
	m_text_color = XMFLOAT4(r, g, b, 1);
	m_display_text = true;
}
//==================================================================================================================================
void Sprite::say(string text, int x, int y, int r, int g, int b, int font_size)
{
	m_text.TextName() = text;
	m_text.ChangeFont(L"Times New Roman", font_size, FontStyle::BoldItalic, true);
	m_text_transform = XMMatrixTranslation(x, y + 20, 0);
	m_text_color = XMFLOAT4(r, g, b, 1);
	m_display_text = true;
}
//==================================================================================================================================
void Sprite::say_stop(string text)
{
	m_old_speed = speedX;
	speedX = 0;
	speedY = 0;
	say(text);
}
//==================================================================================================================================
void Sprite::say_stop(string text, int font_size)
{
	m_old_speed = speedX;
	speedX = 0;
	speedY = 0;
	say(text, font_size);
}
//==================================================================================================================================
void Sprite::say_stop(string text, int x, int y, int font_size)
{
	m_old_speed = speedX;
	speedX = 0;
	speedY = 0;
	say(text, x, y, font_size);
}
//==================================================================================================================================
void Sprite::say_stop(string text, int r, int g, int b, int font_size)
{
	m_old_speed = speedX;
	speedX = 0;
	speedY = 0;
	say(text, r, g, b, font_size);
}
//==================================================================================================================================
void Sprite::say_stop(string text, int x, int y, int r, int g, int b, int font_size)
{
	m_old_speed = speedX;
	speedX = 0;
	speedY = 0;
	say(text, x, y, r, g, b, font_size);
}
//==================================================================================================================================
//bool valueInRange(int value, int min, int max)
//{ return (value >= min) && (value <= max); }
//==================================================================================================================================
bool Sprite::CollidesRect(Sprite* spr)
{
	/*if (!IsHard()) return false;
	if (!spr->IsHard()) return false;
	if (spr == this) return false;

	bool collided = false;

	Sprite* player_hb = GetHardBoxSpr();
	SAT_RECT p_rect(player_hb->TopLeftPosition().x, player_hb->TopLeftPosition().y, player_hb->Width(), player_hb->Height());

	Sprite* hb = spr->GetHardBoxSpr();
	if (hb != NULL)
	{
		SAT_RECT s_rect(hb->TopLeftPosition().x, hb->TopLeftPosition().y, hb->Width(), hb->Height());

		SAT sat;
		if (sat.collides2( p_rect, s_rect ))
		{
			//Flag both as touched
			m_touched = true;
			spr->IsTouched() = true;
			return true;
		}

		//bool xOverlap = valueInRange(p_rect.x, s_rect.x, s_rect.x + s_rect.width) ||
		//				valueInRange(s_rect.x, p_rect.x, p_rect.x + p_rect.width);

		//bool yOverlap = valueInRange(p_rect.y, s_rect.y, s_rect.y + s_rect.height) ||
		//				valueInRange(s_rect.y, p_rect.y, p_rect.y + s_rect.height);

		//if (xOverlap && yOverlap)
		//{
		//	//Flag both as touched
		//	SetTouched(true);
		//	spr->SetTouched(true);
		//}

		//return xOverlap && yOverlap;
	}*/

	return false;
}
//==================================================================================================================================
bool Sprite::CollidesWith(Sprite* spr)
{
	if (!IsHard()) return false;
	if (!spr->IsHard()) return false;
	if (spr == this) return false;

	bool collided = false;

	/*SAT sat;

	Sprite* player_hb = GetHardBoxSpr();

	SAT_RECT p_rect(player_hb->TopLeftPosition().x, player_hb->TopLeftPosition().y, player_hb->Width(), player_hb->Height());

	XMFLOAT3 p_vec = player_hb->TopLeftPosition();

	Sprite* hb = spr->GetHardBoxSpr();

	if (hb != NULL)
	{
		SAT_RECT s_rect(hb->TopLeftPosition().x, hb->TopLeftPosition().y, hb->Width(), hb->Height());
				
		collided = sat.collides( p_rect, s_rect );
		
		if (collided)
		{
			//Flag both as touched
			m_touched = true;
			spr->IsTouched() = true;
		}

		MTV mtv = sat.GetMTV();
		SAT_POINT mtv_vec;

		while (collided)
		{
			mtv = sat.GetMTV();

			collided = sat.collides( p_rect, s_rect );

			//collided = sat.collides2( p_rect, s_rect );
			//if (!sat.collides2( p_rect, s_rect ))
			//	break;

			//Move the player with the MTV
			//Normalize the MTV vector and multiply by the overlap

			mtv_vec = mtv._smallest.normalize();

			if (MoveSpriteUp())
			{
				mtv_vec.y = -mtv_vec.y;
			}

			if (MoveSpriteLeft())
			{
				mtv_vec.x = -mtv_vec.x;
			}

			//project the new m_TopLeftPosition to push the player away from the other sprite
			p_vec.x += mtv_vec.x;
			p_vec.y += mtv_vec.y;

			p_rect.x = p_vec.x;
			p_rect.y = p_vec.y;

			//Update the players m_TopLeftPosition and hardbox
			XMFLOAT3 p_pos = TopLeftPosition();

			p_pos.x += mtv_vec.x;
			p_pos.y += mtv_vec.y;

			Move(p_pos.x, p_pos.y, p_vec.x, p_vec.y);
		}
	}*/
	
	return collided;
}
//==================================================================================================================================
void Sprite::Move(float newPosX, float newPosY, float newPosHBX, float newPosHBY)
{
	m_TopLeftPosition = XMFLOAT3(newPosX, newPosY, 0);
	MoveHardboxMesh(newPosHBX, newPosHBY + this->Body().height);
	
	//if (m_hardBoxSpr != 0)
	//{
	//	float hb_x, hb_y;
	//	int hb_w, hb_h;
	//	GetHardbox(hb_x, hb_y, hb_w, hb_h);
	//	SetHardbox(newPosHBX, newPosHBY + hb_h, hb_w, hb_h);
	//	m_hardBoxSpr->Move(newPosHBX, newPosHBY, 0, 0);
	//}
}
//==================================================================================================================================
void Sprite::AddScript(string name)
{
	m_script_name = name;
	
	if (m_script_name != "NONE")
		m_script = Scripting::GetScript(name);
	else
		m_script = 0;
}
//==================================================================================================================================
/*void Sprite::CreateHardBoxSpr(string editor_base)
{
	m_hardBoxSpr = new Sprite(m_D3DSystem);
	m_hardBoxSpr->Initialize(editor_base, "white.png", XMFLOAT3(m_hard_box.m_x, m_hard_box.m_y, 0), m_hard_box.width, m_hard_box.height, DYNAMIC);
}*/
//==================================================================================================================================
void Sprite::AddAnimationSequence(Animation2D* sequence)
{
	if (sequence == NULL) return;
	sequence->LoadFrameTextures();
	mAnimationSequences.push_back(sequence);
}
//==================================================================================================================================
void Sprite::ClearAnimationList()
{
	if (mAnimationSequences.size() > 0) mAnimationSequences.clear();
}
//==================================================================================================================================
void Sprite::SetSequence(int i)
{
	m_CurrentSequence = mAnimationSequences[i];
}
//==================================================================================================================================
void Sprite::UpdateAnimation()
{
	if (m_CurrentSequence == NULL) return;

	m_CurrentSequence->Update(m_animationSpeed);
}
//==================================================================================================================================
void Sprite::AddAnimation()
{
	//Load a animation profile
	AnimationProfile2D* profile = new AnimationProfile2D(m_D3DSystem);

	bool something_wrong = false;

	if (profile->ReadProfile(m_animationProfileName, "SPRITE", Scripting::GetGameDirectory()->m_sprites_path, Scripting::GetGameDirectory()->GAME_FOLDER))
	{
		vector<Animation2D*> anims = profile->GetAnimations();

		ClearAnimationList();

		for (int i = 0; i < anims.size(); i++)
		{
			AddAnimationSequence(anims[i]);
		}

		SetSequence(default_sequence);
	}
	else something_wrong = true;

	if (something_wrong)
	{
		MessageBox(NULL, "The Animation2D does not exist", "Add Animations", MB_OK);
	}
}
//==================================================================================================================================
void Sprite::RenderDisplayText()
{
	if (m_display_text)
	{
		m_text_timer->Sleep( 300 );
		if (m_text_timer->IsTimeUp())
		{
			m_display_text = false;
			SpeedX() = m_old_speed;
			SpeedY() = m_old_speed;
		}
		else
		{
			m_text.TextTransform() = m_text_transform;
			m_text.TextColor() = m_text_color;
			m_text.Render11();
		}
	}
}
//==================================================================================================================================
bool Sprite::Render(Camera* camera)
{
	return Render(camera, XMFLOAT2(0, 0));
}
//==================================================================================================================================
bool Sprite::Render(Camera* camera, XMFLOAT2 offset)
{
	// bVisible and m_invisible are different. bVisible is what happens when the sprite gets hit and flashes
	// and m_invisible makes the sprite completely invisible
	
	if (!bVisible) return false;
	
	if (m_invisible && m_hard)
	{
		if (m_displayHardbox) RenderHardboxMesh(camera, offset);
		
		return false;
	}

	if (m_invisible) return false;

	if (m_CurrentSequence == NULL)
	{
		m_bitmap->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y, m_imageWidth, m_imageHeight);
		m_bitmap->BitmapWidth() = m_imageWidth;
		m_bitmap->BitmapHeight() = m_imageHeight;
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.camera = camera;
		mrp.specifyWorld = true;
		mrp.world = XMMatrixIdentity();
		m_bitmap->Render(mrp);

		if (m_displayHardbox && m_hard) RenderHardboxMesh(camera, offset);
	}
	else
	{
		RenderAnimation(camera, offset);
	}

	RenderDisplayText();

	return true;
}
//==================================================================================================================================
bool Sprite::Render(Camera* camera, float blendAmount)
{
	return Render(camera, blendAmount, XMFLOAT2(0, 0));
}
//==================================================================================================================================
bool Sprite::Render(Camera* camera, float blendAmount, XMFLOAT2 offset)
{
	if (!bVisible) return false;
	
	if (m_invisible && m_hard)
	{
		if (m_displayHardbox) RenderHardboxMesh(camera, offset);

		return false;
	}

	if (m_invisible) return false;

	if (m_CurrentSequence == NULL)
	{
		m_bitmap->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y, m_imageWidth, m_imageHeight);
		m_bitmap->BitmapWidth() = m_imageWidth;
		m_bitmap->BitmapHeight() = m_imageHeight;
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.camera = camera;
		mrp.blendAmount = blendAmount;
		mrp.specifyWorld = true;
		mrp.world = XMMatrixIdentity();
		m_bitmap->Render(mrp);

		if (m_displayHardbox && m_hard) RenderHardboxMesh(camera, offset);
	}
	else
	{
		RenderAnimation(camera, blendAmount, offset);
	}
	
	RenderDisplayText();

	return true;
}
//==================================================================================================================================
bool Sprite::RenderAnimation(Camera* camera)
{
	return RenderAnimation(camera, XMFLOAT2(0, 0));
}
//==================================================================================================================================
bool Sprite::RenderAnimation(Camera* camera, XMFLOAT2 offset)
{
	if (!bVisible) return false;
	
	if (m_invisible && m_hard)
	{
		if (m_displayHardbox) RenderHardboxMesh(camera, offset);

		return false;
	}

	if (m_invisible) return false;

	if (m_CurrentSequence->GetCurrentFrame() == NULL)
		return false;
	
	ZShadeSandboxMesh::QuadMesh* texture = m_CurrentSequence->GetCurrentFrame()->GetFrameTexture();
	
	if (texture == NULL) return false;

	texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y, m_imageWidth, m_imageHeight);
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = camera;
	mrp.specifyWorld = true;
	mrp.world = XMMatrixIdentity();
	texture->Render(mrp);

	if (m_displayHardbox && m_hard) RenderHardboxMesh(camera, offset);

	return true;
}
//==================================================================================================================================
bool Sprite::RenderAnimation(Camera* camera, float blendAmount)
{
	return RenderAnimation(camera, blendAmount, XMFLOAT2(0, 0));
}
//==================================================================================================================================
bool Sprite::RenderAnimation(Camera* camera, float blendAmount, XMFLOAT2 offset)
{
	if (!bVisible) return true;
	
	if (m_invisible && m_hard)
	{
		if (m_displayHardbox) RenderHardboxMesh(camera, offset);

		return true;
	}

	if (m_invisible) return true;
	
	AnimationFrame2D* frame = m_CurrentSequence->GetCurrentFrame();
	if (frame == NULL) return false;
	ZShadeSandboxMesh::QuadMesh* texture = frame->GetFrameTexture();
	if (texture == NULL) return false;

	texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y, m_imageWidth, m_imageHeight);
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = camera;
	mrp.blendAmount = blendAmount;
	mrp.specifyWorld = true;
	mrp.world = XMMatrixIdentity();
	texture->Render(mrp);

	if (m_displayHardbox && m_hard) RenderHardboxMesh(camera, offset);

	return true;
}
//==================================================================================================================================