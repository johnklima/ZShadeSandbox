#pragma once
#include <Windows.h>
#include "EngineOptions.h"
#include "ZShadeResource.h"
#include "ZShadeFrontEnd.h"
#include "DXGameWindow.h"
#include "Scripting.h"

namespace ZShadeSandbox {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	/// <summary>
	/// Summary for ZShadeSandboxGame
	/// </summary>
	public ref class ZShadeSandboxGame : public System::Windows::Forms::Form
	{
	public:
		ZShadeSandboxGame(EngineOptions* eo)
		{
			mEngineOptions = eo;
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			bInitialized = false;
			mDXGameWindow = 0;
		}

		void Run()
		{
			if (mDXGameWindow)
			{
				mDXGameWindow->Run();
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ZShadeSandboxGame()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Timer^  StartTimer;
	protected:

	protected:
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		bool bInitialized;
		EngineOptions* mEngineOptions;
		DXGameWindow* mDXGameWindow;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->StartTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// StartTimer
			// 
			this->StartTimer->Enabled = true;
			this->StartTimer->Tick += gcnew System::EventHandler(this, &ZShadeSandboxGame::StartTimer_Tick);
			// 
			// ZShadeSandboxGame
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1366,768);
			this->ControlBox = false;
			this->Name = L"ZShadeSandboxGame";
			this->Text = L"ZShadeSandboxGame";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void StartTimer_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		if (!bInitialized)
		{
			HWND hwnd = FindWindow(NULL, ("ZShadeSandboxGame"));

			bInitialized = true;

			this->StartTimer->Enabled = false;

			mDXGameWindow = new DXGameWindow();
			mDXGameWindow->Init(mEngineOptions, hwnd);
			
			// After the environment has been created load the main script
			if (Scripting::Instance())
			{
				Scripting::Instance()->RunMain();
			}

			ThreadStart^ threadStart = gcnew ThreadStart(this, &ZShadeSandbox::ZShadeSandboxGame::Run);
			Thread^ thread = gcnew Thread(threadStart);
			thread->Start();
		}
	}
	};
}
