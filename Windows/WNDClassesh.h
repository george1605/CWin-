class WinObj {
public:
	WNDCLASSEX exClass = { 0 };
	HWND cHandle = NULL;
	void setBrush(COLORREF x) {
		exClass.hbrBackground = (HBRUSH)x;
	}
	void setMenu(LPWSTR x) {
		exClass.lpszMenuName = MAKEINTRESOURCE(x);
	}
};

