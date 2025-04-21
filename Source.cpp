#include "Form1.h"

[System::STAThread]
int main()
{
	System::Windows::Forms::Application::EnableVisualStyles();
	System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false);
	System::Windows::Forms::Application::Run(gcnew SAP::Form1());
	return 0;
}