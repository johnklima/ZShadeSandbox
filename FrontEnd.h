#pragma once
#include "ZShadeSandboxGame.h"
#include "ZShadeSandboxEditor3D.h"
#include "ZShadeSandboxEditor2D.h"
#include "ZShadeFrontEnd.h"

namespace ZShadeSandbox {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for FrontEnd
	/// </summary>
	public ref class FrontEnd : public System::Windows::Forms::Form
	{
	public:
		FrontEnd(HINSTANCE hInstance)
		{
			InitializeComponent();
			
			m_hInstance = hInstance;

			mZShadeFrontEnd = new ZShadeFrontEnd();
			mZShadeFrontEnd->Init(m_hInstance);
		}

		EngineOptions* GetEngineOptions()
		{
			return mZShadeFrontEnd->GetEngineOptions();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FrontEnd()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
	private: System::Windows::Forms::Button^  EditorModeButton;

		
		HINSTANCE m_hInstance;
	private: System::Windows::Forms::Button^  GameModeButton;
	private: System::Windows::Forms::Button^  ExitButton;




	private: System::Windows::Forms::Button^  OptionsButton;



		ZShadeFrontEnd* mZShadeFrontEnd;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(FrontEnd::typeid));
			this->EditorModeButton = (gcnew System::Windows::Forms::Button());
			this->GameModeButton = (gcnew System::Windows::Forms::Button());
			this->ExitButton = (gcnew System::Windows::Forms::Button());
			this->OptionsButton = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// EditorModeButton
			// 
			this->EditorModeButton->Location = System::Drawing::Point(7, 12);
			this->EditorModeButton->Name = L"EditorModeButton";
			this->EditorModeButton->Size = System::Drawing::Size(75, 23);
			this->EditorModeButton->TabIndex = 1;
			this->EditorModeButton->Text = L"Editor Mode";
			this->EditorModeButton->UseVisualStyleBackColor = true;
			this->EditorModeButton->Click += gcnew System::EventHandler(this, &FrontEnd::EditorModeButton_Click);
			// 
			// GameModeButton
			// 
			this->GameModeButton->Location = System::Drawing::Point(7, 41);
			this->GameModeButton->Name = L"GameModeButton";
			this->GameModeButton->Size = System::Drawing::Size(75, 23);
			this->GameModeButton->TabIndex = 2;
			this->GameModeButton->Text = L"Game Mode";
			this->GameModeButton->UseVisualStyleBackColor = true;
			this->GameModeButton->Click += gcnew System::EventHandler(this, &FrontEnd::GameModeButton_Click);
			// 
			// ExitButton
			// 
			this->ExitButton->Location = System::Drawing::Point(7, 99);
			this->ExitButton->Name = L"ExitButton";
			this->ExitButton->Size = System::Drawing::Size(75, 23);
			this->ExitButton->TabIndex = 3;
			this->ExitButton->Text = L"Exit";
			this->ExitButton->UseVisualStyleBackColor = true;
			this->ExitButton->Click += gcnew System::EventHandler(this, &FrontEnd::ExitButton_Click);
			// 
			// OptionsButton
			// 
			this->OptionsButton->Location = System::Drawing::Point(7, 70);
			this->OptionsButton->Name = L"OptionsButton";
			this->OptionsButton->Size = System::Drawing::Size(75, 23);
			this->OptionsButton->TabIndex = 7;
			this->OptionsButton->Text = L"Options";
			this->OptionsButton->UseVisualStyleBackColor = true;
			// 
			// FrontEnd
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(292, 139);
			//this->Cursor = gcnew System::Windows::Forms::Cursor(GetType(), "cursor.cur");
			this->Cursor = gcnew System::Windows::Forms::Cursor(System::Reflection::Assembly::GetExecutingAssembly()->GetManifestResourceStream("cursor.cur"));
			this->CenterToScreen();
			this->ControlBox = false;
			this->Controls->Add(this->OptionsButton);
			this->Controls->Add(this->ExitButton);
			this->Controls->Add(this->GameModeButton);
			this->Controls->Add(this->EditorModeButton);
			this->Name = L"FrontEnd";
			this->Text = L"FrontEnd";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void EditorModeButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		switch (mZShadeFrontEnd->GetEngineOptions()->m_DimType)
		{
			case ZSHADE_2D:
			{
				mZShadeFrontEnd->SetInEditor(true);
				ZShadeSandboxEditor2D^ form = gcnew ZShadeSandboxEditor2D(mZShadeFrontEnd->GetEngineOptions());
				form->ClientSize = System::Drawing::Size(mZShadeFrontEnd->GetEngineOptions()->m_screenWidth, mZShadeFrontEnd->GetEngineOptions()->m_screenHeight);
				form->Init();
				form->Show();
			}
			break;
			case ZSHADE_3D:
			{
				mZShadeFrontEnd->SetInEditor(true);
				ZShadeSandboxEditor3D^ form = gcnew ZShadeSandboxEditor3D(mZShadeFrontEnd->GetEngineOptions());
				form->ClientSize = System::Drawing::Size(mZShadeFrontEnd->GetEngineOptions()->m_screenWidth, mZShadeFrontEnd->GetEngineOptions()->m_screenHeight);
				form->Show();
			}
			break;
		}
	}
	private: System::Void GameModeButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		mZShadeFrontEnd->SetInEditor(false);
		ZShadeSandboxGame^ form = gcnew ZShadeSandboxGame(mZShadeFrontEnd->GetEngineOptions());
		form->ClientSize = System::Drawing::Size(mZShadeFrontEnd->GetEngineOptions()->m_screenWidth, mZShadeFrontEnd->GetEngineOptions()->m_screenHeight);
		form->Show();
	}

	private: System::Void ExitButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		// Terminate the form
		System::Windows::Forms::Application::Exit();
	}
};
}
