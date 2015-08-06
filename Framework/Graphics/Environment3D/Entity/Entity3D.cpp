#include "Entity3D.h"
#include "Convert.h"
//==================================================================================================================================
//==================================================================================================================================
int Entity3D::iCounter = 0;
//==================================================================================================================================
Entity3D::Entity3D(D3D* d3d)
:	mD3DSystem(d3d)
{
	mEngineOptions = d3d->GetEngineOptions();
	
	Init();
	
	Entity3D::iCounter++;
}
//==================================================================================================================================
void Entity3D::Init()
{
	iHurtTimer = 0;
	mPosition = XMFLOAT3(0, 0, 0);
	mRotation = XMFLOAT3(0, 0, 0);
	mScale = XMFLOAT3(1, 1, 1);
	bVisible = false;
	bPlayer = false;
	bDead = false;
	bHostile = false;
	sScriptName = "";
	sName = "Entity" + ZShadeSandboxGlobal::Convert::ConvertToString<int>(Entity3D::iCounter);
	bMoving = false;
	bThinking = false;
	bTalking = false;
	bDying = false;
	bHit = false;
	bTouched = false;
	bAttacked = false;
	bTargeted = false;
	fInflictingDamage = 0.0f;
	fDamage = 0.0f;
	fHealth = 100.0f;
	fMaxHealth = 100.0f;
	fSpeed = 0.0f;
	iMana = 0;
	iGold = 0;
	iCoin = 0;
	iStrength = 0;
	iDefense = 0;
	iExperience = 0;
	iLevel = 0;
	
	m_text.SetD3D(mD3DSystem);
	m_text.Initialize();
	m_text.CreateShadow();
	m_text.ChangeFont(L"Times New Roman", 24.0f, FontStyle::BoldItalic, true);
	m_text.TextName() = "I am an Entity";
	m_text_transform = XMMatrixTranslation(mPosition.x, mPosition.y + 20, mPosition.z);
	m_text_color = XMFLOAT4(1, 1, 1, 1);
	m_display_text = false;
}
//==================================================================================================================================
void Entity3D::Update(float dt)
{
	
}
//==================================================================================================================================
void Entity3D::Render(Camera* camera)
{
	
	
	if (m_display_text)
	{
		m_text_timer->Sleep( 300 );
		if (m_text_timer->IsTimeUp())
		{
			m_display_text = false;
			fSpeed = fOldSpeed;
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
void Entity3D::say(string text)
{
	m_text.TextName() = text;
	m_text_transform = XMMatrixTranslation(mPosition.x, mPosition.y + 20, mPosition.z);
	m_display_text = true;
}
//==================================================================================================================================
void Entity3D::say(string text, int font_size)
{
	m_text.TextName() = text;
	m_text.ChangeFont(L"Times New Roman", font_size, FontStyle::BoldItalic, true);
	m_text_transform = XMMatrixTranslation(mPosition.x, mPosition.y + 20, mPosition.z);
	m_display_text = true;
}
//==================================================================================================================================
void Entity3D::say(string text, int r, int g, int b, int font_size)
{
	m_text.TextName() = text;
	m_text.ChangeFont(L"Times New Roman", font_size, FontStyle::BoldItalic, true);
	m_text_transform = XMMatrixTranslation(mPosition.x, mPosition.y + 20, mPosition.z);
	m_text_color = XMFLOAT4(r, g, b, 1);
	m_display_text = true;
}
//==================================================================================================================================
void Entity3D::say_stop(string text)
{
	fOldSpeed = fSpeed;
	fSpeed = 0;
	say(text);
}
//==================================================================================================================================
void Entity3D::say_stop(string text, int font_size)
{
	fOldSpeed = fSpeed;
	fSpeed = 0;
	say(text, font_size);
}
//==================================================================================================================================
void Entity3D::say_stop(string text, int r, int g, int b, int font_size)
{
	fOldSpeed = fSpeed;
	fSpeed = 0;
	say(text, r, g, b, font_size);
}
//==================================================================================================================================
