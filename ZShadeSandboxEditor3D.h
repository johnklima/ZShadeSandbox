#pragma once
#include <Windows.h>
#include "EngineOptions.h"
#include "ZShadeResource.h"
#include "ZShadeFrontEnd.h"
#include "DXGameWindow.h"

namespace ZShadeSandbox {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	/// <summary>
	/// Summary for ZShadeSandboxEditor3D
	/// </summary>
	public ref class ZShadeSandboxEditor3D : public System::Windows::Forms::Form
	{
	public:
		ZShadeSandboxEditor3D(EngineOptions* eo)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			bInitialized = false;
			mEngineOptions = eo;
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
		~ZShadeSandboxEditor3D()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		bool bInitialized;
		EngineOptions* mEngineOptions;
		DXGameWindow* mDXGameWindow;
	private: System::Windows::Forms::Timer^  StartTimer;
	private: System::Windows::Forms::SplitContainer^  ZShadeSandboxEditorSplitContainer;

	private: System::ComponentModel::IContainer^  components;

		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->StartTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->ZShadeSandboxEditorSplitContainer = (gcnew System::Windows::Forms::SplitContainer());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ZShadeSandboxEditorSplitContainer))->BeginInit();
			this->ZShadeSandboxEditorSplitContainer->SuspendLayout();
			this->SuspendLayout();
			// 
			// StartTimer
			// 
			this->StartTimer->Enabled = true;
			this->StartTimer->Tick += gcnew System::EventHandler(this, &ZShadeSandboxEditor3D::StartTimer_Tick);
			// 
			// ZShadeSandboxEditorSplitContainer
			// 
			this->ZShadeSandboxEditorSplitContainer->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ZShadeSandboxEditorSplitContainer->Location = System::Drawing::Point(0, 0);
			this->ZShadeSandboxEditorSplitContainer->Name = L"ZShadeSandboxEditorSplitContainer";
			this->ZShadeSandboxEditorSplitContainer->Size = System::Drawing::Size(1350, 729);
			this->ZShadeSandboxEditorSplitContainer->SplitterDistance = 926;
			this->ZShadeSandboxEditorSplitContainer->TabIndex = 0;
			// 
			// ZShadeSandboxEditor3D
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1350, 729);
			this->Controls->Add(this->ZShadeSandboxEditorSplitContainer);
			this->Name = L"ZShadeSandboxEditor3D";
			this->Text = L"ZShadeSandboxEditor3D";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ZShadeSandboxEditorSplitContainer))->EndInit();
			this->ZShadeSandboxEditorSplitContainer->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void StartTimer_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		if (!bInitialized)
		{
			HWND hwnd = static_cast<HWND>(ZShadeSandboxEditorSplitContainer->Panel1->Handle.ToPointer());

			bInitialized = true;

			this->StartTimer->Enabled = false;

			mDXGameWindow = new DXGameWindow();
			mDXGameWindow->Init(mEngineOptions, hwnd);

			ThreadStart^ threadStart = gcnew ThreadStart(this, &ZShadeSandbox::ZShadeSandboxEditor3D::Run);
			Thread^ thread = gcnew Thread(threadStart);
			thread->Start();
		}
	}
	};
}
