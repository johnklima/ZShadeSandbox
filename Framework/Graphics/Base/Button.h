#ifndef __BUTTON_H
#define __BUTTON_H
//-----------------------------------------------------------------------------------------
#include "D3DIncludes.h"
#include <string>
#include "CGlobal.h"
#include "input.h"
#include "D3D.h"
#include "GameDirectory.h"
#include "Script.h"
using std::string;
//-----------------------------------------------------------------------------------------
#include "Text.h"
#include "Image.h"
//-----------------------------------------------------------------------------------------
namespace ZShadeSandboxGraphics {
	namespace EButtonState
	{
		enum
		{
			eNormal,
			ePushed,
			eHighlighted,
			eDisabled
		};
	}
	//-----------------------------------------------------------------------------------------
	namespace EScriptType
	{
		enum
		{
			eNone,
			eStart,
			eResume,
			eContinue,
			eOptions,
			eExit
		};
	}
	//-----------------------------------------------------------------------------------------
	//An Image/Color with Text,
	//Has 5 states: NORMAL, PUSHED, HIGHLIGHTED, DISABLED, UP

	class Button
	{
	public:
		
		Button(
			D3D* d3d,
			char* text,
			bool disableHighlight,
			float x,
			float y,
			int width,
			int height,
			string normalImageName,
			string pushedImageName,
			string highlightedImageName,
			string disabledImageName,
			string pNormalBackgroundColor,
			string pPushedBackgroundColor,
			string pHighlightedBackgroundColor,
			string pDisabledBackgroundColor
		);
		Button(
			D3D* d3d,
			char* text,
			bool disableHighlight,
			float x,
			float y,
			int width,
			int height,
			string base_button_path,
			string normalImageName,
			string pushedImageName,
			string highlightedImageName,
			string disabledImageName,
			string pNormalBackgroundColor,
			string pPushedBackgroundColor,
			string pHighlightedBackgroundColor,
			string pDisabledBackgroundColor
		);
		~Button();

		void UpdateButtonMouseUp(WPARAM btnState, int x, int y);
		void UpdateButtonMouseDown(WPARAM btnState, int x, int y);
		void UpdateButtonMouseOn(WPARAM btnState, int x, int y);

		bool WasLastStateUP() { return mUP && (mState == EButtonState::eNormal || mState == EButtonState::eHighlighted); }
		void ResetUPState()   { mUP = false; }
		
		void MoveButtonX(float amount);
		void MoveButtonY(float amount);
		
		void CloneMe();
		
		void SetNormalImage(string name);
		void SetPushedImage(string name);
		void SetHighlightedImage(string name);
		void SetDisabledImage(string name);
		
		string GetNormalImageName();
		string GetPushedImageName();
		string GetHighlightedImageName();
		string GetDisabledImageName();
		
		void SetText(string text);
		string GetText();
		
		Text GetTextComponent() { return mButtonText; }
		
		void Draw(Camera* camera, int blendAmount);
		
		Button* Clone();
		Button* Clone(int x, int y);
		Button* Clone(string base_button_path);
		Button* Clone(string base_button_path, int x, int y);
		Sprite* ToSprite(string base_button_path);
		
	public:
		
		float& XPos()				{ return mXPos; }
		float XPos() const			{ return mXPos; }
		float& YPos()				{ return mYPos; }
		float YPos() const			{ return mYPos; }
		int& Width()				{ return mWidth; }
		int Width() const			{ return mWidth; }
		int& Height()				{ return mHeight; }
		int Height() const			{ return mHeight; }
		int& State()				{ return mState; }
		int State() const			{ return mState; }
		int& ScriptType()			{ return mScriptType; }
		int ScriptType() const		{ return mScriptType; }
		Script*& GetScript()		{ return mScript; }
		Script* GetScript() const	{ return mScript; }
		int& ID()					{ return mID; }
		int ID() const				{ return mID; }
		
	private:
		
		D3D*              mD3D;
		int			      mState;
		int				  mScriptType;
		int				  mID;
		Script*           mScript;
		
		D3D* m_d3d;
		string m_windowName;
		GameDirectory2D* m_gd;
		EngineOptions* m_eo;
		HWND m_hwnd;

		bool              mUP;//Was last state up
		bool              mDisableHighlight;

		float             mXPos;
		float             mYPos;
		int               mWidth;
		int               mHeight;

		Text             mButtonText;
		XMMATRIX         mTextTransform;

		Image*    mNormalButtonImage;
		Image*    mPushedButtonImage;
		Image*    mHighlightedButtonImage;
		Image*    mDisabledButtonImage;

		string mButtonNormalName;
		string mButtonPushedName;
		string mButtonHighlightedName;
		string mButtonDisabledName;

		string mButtonNormalBackgroundColor;
		string mButtonPushedBackgroundColor;
		string mButtonHighlightedBackgroundColor;
		string mButtonDisabledBackgroundColor;
	};
}
//-----------------------------------------------------------------------------------------
#endif//__BUTTON_H