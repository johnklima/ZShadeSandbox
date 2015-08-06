#include "Button.h"
using ZShadeSandboxGraphics::Button;
//-----------------------------------------------------------------------------------------
Button::Button(D3D* d3d
,	char* text
,	bool disableHighlight
,	float x
,	float y
,	int width
,	int height
,	string normalImageName
,	string pushedImageName
,	string highlightedImageName
,	string disabledImageName
,	string pNormalBackgroundColor
,	string pPushedBackgroundColor
,	string pHighlightedBackgroundColor
,	string pDisabledBackgroundColor
)
:   mXPos(x)
,	mYPos(y)
,	mWidth(width)
,	mHeight(height)
,	mDisableHighlight(disableHighlight)
,	mD3D(d3d)
,	mUP(false)
{
	mScript = NULL;
	mScriptType = EScriptType::eNone;
	
	m_eo = mD3D->GetEngineOptions();
	m_gd = m_eo->m_GD2D;

	mXPos = x;
	mYPos = y;

	//Button starts out in normal state
	mState = EButtonState::eNormal;

	mButtonNormalBackgroundColor = pNormalBackgroundColor;
	mButtonPushedBackgroundColor = pPushedBackgroundColor;
	mButtonHighlightedBackgroundColor = pHighlightedBackgroundColor;
	mButtonDisabledBackgroundColor = pDisabledBackgroundColor;

	//Make image the background if exists
	//Make the text the center of the button

	mButtonNormalName = normalImageName;
	mButtonPushedName = pushedImageName;
	mButtonHighlightedName = highlightedImageName;
	mButtonDisabledName = disabledImageName;

	mNormalButtonImage = new Image(d3d, normalImageName, m_gd->m_sprites_path, x, y, width, height, m_gd->m_required_textures, mButtonNormalBackgroundColor);
	mPushedButtonImage = new Image(d3d, pushedImageName, m_gd->m_sprites_path, x, y, width, height, m_gd->m_required_textures, mButtonPushedBackgroundColor);
	mHighlightedButtonImage = new Image(d3d, highlightedImageName, m_gd->m_sprites_path, x, y, width, height, m_gd->m_required_textures, mButtonHighlightedBackgroundColor);
	mDisabledButtonImage = new Image(d3d, disabledImageName, m_gd->m_sprites_path, x, y, width, height, m_gd->m_required_textures, mButtonDisabledBackgroundColor);

	//mButtonText = new Text( text, windowName, gd, x + width / 4 - 4, y + height / 4 - 2, d3d, text_color, p_color, hwnd, eo );

	mTextTransform = XMMatrixTranslation(x + width / 4 - 4, y + height / 4 - 2, 0);

	mButtonText.SetD3D(d3d);
	mButtonText.Initialize();
	mButtonText.ChangeFont(L"Times New Roman", 12.0f, FontStyle::BoldItalic, true);
	mButtonText.TextName() = text;
	if (mButtonText.TextName() == "NONE")
		mButtonText.TextName() = "";
	mButtonText.TextTransform() = mTextTransform;
	mButtonText.TextColor() = XMFLOAT4(1, 1, 1, 1);
}
//-----------------------------------------------------------------------------------------
Button::Button(D3D* d3d
,	char* text
,	bool disableHighlight
,	float x
,	float y
,	int width
,	int height
,	string base_button_path
,	string normalImageName
,	string pushedImageName
,	string highlightedImageName
,	string disabledImageName
,	string pNormalBackgroundColor
,	string pPushedBackgroundColor
,	string pHighlightedBackgroundColor
,	string pDisabledBackgroundColor
)
:   mXPos(x)
,	mYPos(y)
,	mWidth(width)
,	mHeight(height)
,	mDisableHighlight(disableHighlight)
,	mD3D(d3d)
,	mUP(false)
{
	mScript = NULL;
	mScriptType = EScriptType::eNone;

	m_eo = mD3D->GetEngineOptions();
	m_gd = m_eo->m_GD2D;

	//Button starts out in normal state
	mState = EButtonState::eNormal;

	mButtonNormalBackgroundColor = pNormalBackgroundColor;
	mButtonPushedBackgroundColor = pPushedBackgroundColor;
	mButtonHighlightedBackgroundColor = pHighlightedBackgroundColor;
	mButtonDisabledBackgroundColor = pDisabledBackgroundColor;

	//Make image the background if exists
	//Make the text the center of the button

	mButtonNormalName = normalImageName;
	mButtonPushedName = pushedImageName;
	mButtonHighlightedName = highlightedImageName;
	mButtonDisabledName = disabledImageName;

	mNormalButtonImage = new Image(d3d, normalImageName, base_button_path, x, y, width, height, m_gd->m_required_textures, mButtonNormalBackgroundColor);
	mPushedButtonImage = new Image(d3d, pushedImageName, base_button_path, x, y, width, height, m_gd->m_required_textures, mButtonPushedBackgroundColor);
	mHighlightedButtonImage = new Image(d3d, highlightedImageName, base_button_path, x, y, width, height, m_gd->m_required_textures, mButtonHighlightedBackgroundColor);
	mDisabledButtonImage = new Image(d3d, disabledImageName, base_button_path, x, y, width, height, m_gd->m_required_textures, mButtonDisabledBackgroundColor);

	//mButtonText = new Text( text, windowName, gd, x + width/4 - 4, y + height/4 - 2, d3d, text_color, p_color, hwnd, eo );

	mTextTransform = XMMatrixTranslation(x + width / 4 - 4, y + height / 4 - 2, 0);

	mButtonText.SetD3D(d3d);
	mButtonText.Initialize();
	mButtonText.ChangeFont(L"Times New Roman", 12.0f, FontStyle::BoldItalic, true);
	mButtonText.TextName() = text;
	if (mButtonText.TextName() == "NONE")
		mButtonText.TextName() = "";
	mButtonText.TextTransform() = mTextTransform;
	mButtonText.TextColor() = XMFLOAT4(1, 1, 1, 1);
}
//-----------------------------------------------------------------------------------------
Button::~Button()
{
	delete mNormalButtonImage;
	delete mPushedButtonImage;
	delete mHighlightedButtonImage;
	delete mDisabledButtonImage;
}
//-----------------------------------------------------------------------------------------
string Button::GetNormalImageName()
{
	if (mNormalButtonImage != NULL)
		return mNormalButtonImage->GetName();
	else
		return "NONE";
}
//-----------------------------------------------------------------------------------------
string Button::GetPushedImageName()
{
	if (mPushedButtonImage != NULL)
		return mPushedButtonImage->GetName();
	else
		return "NONE";
}
//-----------------------------------------------------------------------------------------
string Button::GetHighlightedImageName()
{
	if (mHighlightedButtonImage != NULL)
		return mHighlightedButtonImage->GetName();
	else
		return "NONE";
}
//-----------------------------------------------------------------------------------------
string Button::GetDisabledImageName()
{
	if (mDisabledButtonImage != NULL)
		return mDisabledButtonImage->GetName();
	else
		return "NONE";
}
//-----------------------------------------------------------------------------------------
void Button::SetText(string text)
{
	mButtonText.TextName() = BetterString(text);
}
//-----------------------------------------------------------------------------------------
string Button::GetText()
{
	return mButtonText.TextName().toString();
}
//-----------------------------------------------------------------------------------------
void Button::SetNormalImage(string name)
{
	mNormalButtonImage = new Image(m_d3d, name, m_gd->m_sprites_path, mXPos, mYPos, mWidth, mHeight);
}
//-----------------------------------------------------------------------------------------
void Button::SetPushedImage(string name)
{
	mPushedButtonImage = new Image(m_d3d, name, m_gd->m_sprites_path, mXPos, mYPos, mWidth, mHeight);
}
//-----------------------------------------------------------------------------------------
void Button::SetHighlightedImage(string name)
{
	mHighlightedButtonImage = new Image(m_d3d, name, m_gd->m_sprites_path, mXPos, mYPos, mWidth, mHeight);
}
//-----------------------------------------------------------------------------------------
void Button::SetDisabledImage(string name)
{
	mDisabledButtonImage = new Image(m_d3d, name, m_gd->m_sprites_path, mXPos, mYPos, mWidth, mHeight);
}
//-----------------------------------------------------------------------------------------
void Button::UpdateButtonMouseUp(WPARAM btnState, int x, int y)
{
	if (mState == EButtonState::eDisabled) return;

	if (strcmp(mButtonText.TextName(), "NONE") == 0)
		mButtonText.TextName() = "";

	RECT r;
	r.left = mNormalButtonImage->XPos();
	r.right = r.left + mNormalButtonImage->Width();// + 50;
	r.top = mNormalButtonImage->YPos();
	r.bottom = r.top + mNormalButtonImage->Height();// + 20;

	POINT pt;
	pt.x = x;
	pt.y = y;
	
	if (!PtInRect(&r, pt))
	{
		mState = EButtonState::eNormal;
	}
}
//-----------------------------------------------------------------------------------------
void Button::UpdateButtonMouseDown(WPARAM btnState, int x, int y)
{
	if (mState == EButtonState::eDisabled) return;

	if (strcmp(mButtonText.TextName(), "NONE") == 0)
		mButtonText.TextName() = "";

	RECT r;
	r.left = mNormalButtonImage->XPos();
	r.right = r.left + mNormalButtonImage->Width();// + 50;
	r.top = mNormalButtonImage->YPos();
	r.bottom = r.top + mNormalButtonImage->Height();// + 20;

	POINT pt;
	pt.x = x;
	pt.y = y;

	if (PtInRect(&r, pt))
	{
		mUP = true;
		mState = EButtonState::ePushed;
	}
	else
	{
		mState = EButtonState::eNormal;
	}
}
//-----------------------------------------------------------------------------------------
void Button::UpdateButtonMouseOn(WPARAM btnState, int x, int y)
{
	if (mState == EButtonState::eDisabled) return;

	if (strcmp(mButtonText.TextName(), "NONE") == 0)
		mButtonText.TextName() = "";

	RECT r;
	r.left = mNormalButtonImage->XPos();
	r.right = r.left + mNormalButtonImage->Width();// + 50;
	r.top = mNormalButtonImage->YPos();
	r.bottom = r.top + mNormalButtonImage->Height();// + 20;

	POINT pt;
	pt.x = x;
	pt.y = y;

	if (PtInRect(&r, pt))
	{
		mState = EButtonState::eHighlighted;
	}
	else
	{
		mState = EButtonState::eNormal;
	}
}
//-----------------------------------------------------------------------------------------
void Button::MoveButtonX(float amount)
{
	mNormalButtonImage->XPos() = amount;
	mPushedButtonImage->XPos() = amount;
	mHighlightedButtonImage->XPos() = amount;
	mDisabledButtonImage->XPos() = amount;
	
	mTextTransform = XMMatrixTranslation(amount + mWidth / 4 - 4, mYPos, 0);

	mButtonText.TextTransform() = mTextTransform;

	mXPos = mNormalButtonImage->XPos();
	mWidth = mNormalButtonImage->Width();
}
//-----------------------------------------------------------------------------------------
void Button::MoveButtonY(float amount)
{
	mNormalButtonImage->YPos() = amount;
	mPushedButtonImage->YPos() = amount;
	mHighlightedButtonImage->YPos() = amount;
	mDisabledButtonImage->YPos() = amount;
	
	mTextTransform = XMMatrixTranslation(mXPos, amount + mHeight / 4 - 2, 0);

	mButtonText.TextTransform() = mTextTransform;

	mYPos = mNormalButtonImage->YPos();
	mHeight = mNormalButtonImage->Height();
}
//-----------------------------------------------------------------------------------------
void Button::CloneMe()
{
	mNormalButtonImage = new Image(m_d3d, mButtonNormalName, m_gd->m_sprites_path, mXPos, mYPos, mWidth, mHeight, m_gd->m_required_textures, mButtonNormalBackgroundColor);
	mPushedButtonImage = new Image(m_d3d, mButtonPushedName, m_gd->m_sprites_path, mXPos, mYPos, mWidth, mHeight, m_gd->m_required_textures, mButtonPushedBackgroundColor);
	mHighlightedButtonImage = new Image(m_d3d, mButtonHighlightedName, m_gd->m_sprites_path, mXPos, mYPos, mWidth, mHeight, m_gd->m_required_textures, mButtonHighlightedBackgroundColor);
	mDisabledButtonImage = new Image(m_d3d, mButtonDisabledName, m_gd->m_sprites_path, mXPos, mYPos, mWidth, mHeight, m_gd->m_required_textures, mButtonDisabledBackgroundColor);
	//mButtonText = new Text( mButtonText->GetText(), m_windowName, m_gd, mXPos + mWidth/4 - 4, mYPos + mHeight/4 - 2, m_d3d, mButtonText->GetColor11(), mButtonText->GetColor10(), m_hwnd, m_eo );

	mTextTransform = XMMatrixTranslation(mXPos + mWidth / 4 - 4, mYPos + mHeight / 4 - 2, 0);

	mButtonText.SetD3D(m_d3d);
	mButtonText.Initialize();
	mButtonText.ChangeFont(L"Times New Roman", 12.0f, FontStyle::BoldItalic, true);
	mButtonText.TextName() = "";
	mButtonText.TextTransform() = mTextTransform;
	mButtonText.TextColor() = XMFLOAT4(1, 1, 1, 1);
}
//-----------------------------------------------------------------------------------------
Button* Button::Clone()
{
	Button* newButton = new Button(
		mD3D,
		ZShadeSandboxGlobal::Convert::ConvertStringToCharPointer(mButtonText.TextName().toString()),
		mDisableHighlight,
		mXPos,
		mYPos,
		mWidth,
		mHeight,
		mButtonNormalName,
		mButtonPushedName,
		mButtonHighlightedName,
		mButtonDisabledName,
		mButtonNormalBackgroundColor,
		mButtonPushedBackgroundColor,
		mButtonHighlightedBackgroundColor,
		mButtonDisabledBackgroundColor
	);
	
	newButton->ScriptType() = mScriptType;
	newButton->GetScript() = mScript;
	
	return newButton;
}
//-----------------------------------------------------------------------------------------
Button* Button::Clone(int x, int y)
{
	Button* newButton = new Button(
		mD3D,
		ZShadeSandboxGlobal::Convert::ConvertStringToCharPointer(mButtonText.TextName().toString()),
		mDisableHighlight,
		x,
		y,
		mWidth,
		mHeight,
		mButtonNormalName,
		mButtonPushedName,
		mButtonHighlightedName,
		mButtonDisabledName,
		mButtonNormalBackgroundColor,
		mButtonPushedBackgroundColor,
		mButtonHighlightedBackgroundColor,
		mButtonDisabledBackgroundColor
	);
	
	newButton->ScriptType() = mScriptType;
	newButton->GetScript() = mScript;
	
	return newButton;
}
//-----------------------------------------------------------------------------------------
Button* Button::Clone(string base_button_path)
{
	Button* newButton = new Button(
		mD3D,
		ZShadeSandboxGlobal::Convert::ConvertStringToCharPointer(mButtonText.TextName().toString()),
		mDisableHighlight,
		mXPos,
		mYPos,
		mWidth,
		mHeight,
		base_button_path,
		mButtonNormalName,
		mButtonPushedName,
		mButtonHighlightedName,
		mButtonDisabledName,
		mButtonNormalBackgroundColor,
		mButtonPushedBackgroundColor,
		mButtonHighlightedBackgroundColor,
		mButtonDisabledBackgroundColor
	);
	
	newButton->ScriptType() = mScriptType;
	newButton->GetScript() = mScript;
	
	return newButton;
}
//-----------------------------------------------------------------------------------------
Button* Button::Clone(string base_button_path, int x, int y)
{
	Button* newButton = new Button(
		mD3D,
		ZShadeSandboxGlobal::Convert::ConvertStringToCharPointer(mButtonText.TextName().toString()),
		mDisableHighlight,
		x,
		y,
		mWidth,
		mHeight,
		base_button_path,
		mButtonNormalName,
		mButtonPushedName,
		mButtonHighlightedName,
		mButtonDisabledName,
		mButtonNormalBackgroundColor,
		mButtonPushedBackgroundColor,
		mButtonHighlightedBackgroundColor,
		mButtonDisabledBackgroundColor
	);
	
	newButton->ScriptType() = mScriptType;
	newButton->GetScript() = mScript;
	
	return newButton;
}
//-----------------------------------------------------------------------------------------
Sprite* Button::ToSprite(string base_button_path)
{
	Sprite* spr = new Sprite(mD3D);
	spr->Initialize(base_button_path, mButtonNormalName, XMFLOAT3(mXPos, mYPos, 0), mWidth, mHeight, STATIC);
	return spr;
}
//-----------------------------------------------------------------------------------------
void Button::Draw(Camera* camera, int blendAmount)
{
	if (mState == EButtonState::eNormal)
	{
		if (mNormalButtonImage)
			mNormalButtonImage->Draw(camera, blendAmount);
	}

	if (!mDisableHighlight)
	{
		if (mState == EButtonState::eHighlighted)
		{
			if (mHighlightedButtonImage)
				mHighlightedButtonImage->Draw(camera, blendAmount);
		}
	}
	else
	{
		if (mNormalButtonImage)
			mNormalButtonImage->Draw(camera, blendAmount);
	}

	if (mState == EButtonState::ePushed)
	{
		if (mPushedButtonImage)
			mPushedButtonImage->Draw(camera, blendAmount);
	}

	if (mState == EButtonState::eDisabled)
	{
		if (mDisabledButtonImage)
			mDisabledButtonImage->Draw(camera, blendAmount);
	}

	mButtonText.Render11();
}
//-----------------------------------------------------------------------------------------