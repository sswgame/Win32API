#include "pch.h"
#include "Window.h"

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	CWindow window{};
	if (true == window.Initialize())
	{
		window.Run();
	}
	return 0;
}
