//===================================================================================================================================
//�yWindow.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/17
// [�X�V��]2019/09/19
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Window.h"

//===================================================================================================================================
//�y�O���[�o���ϐ��z
//===================================================================================================================================
Window* window = NULL; //�E�B���h�E�N���X�̃|�C���^---�E�B���h�E�v���V�[�W���̂��߃X�^�e�B�b�N�̈�֕ۑ�

//===================================================================================================================================
//�O���Q�Ɛ錾
//===================================================================================================================================
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//===================================================================================================================================
//�y�E�B���h�E�v���V�[�W���F�R�[���o�b�N�֐��z
//===================================================================================================================================
LRESULT CALLBACK wndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK wndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return window->msgProc(wnd, msg, wparam, lparam);
}

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Window::Window()
{
	initialized = false;
	windowActivate = true;
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
HRESULT Window::initialize(HINSTANCE instance, INT x, INT y, INT width, INT height, LPSTR windowName) {
	window = this;
	//�E�B���h�E�̒�`
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = wndProc;
	wndClass.hInstance = instance;
	wndClass.hIcon = LoadIcon(instance, (LPCTSTR)IDI_ICON0);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszClassName = windowName;
	wndClass.hIconSm = LoadIcon(instance, (LPCTSTR)IDI_ICON0);
	RegisterClassEx(&wndClass);
	//�E�B���h�E�̍쐬
	wnd = CreateWindow(windowName, windowName, WS_OVERLAPPEDWINDOW, 0, 0, width, height, 0, 0, instance, 0);
	if (!wnd)
		return E_FAIL;

	// �E�B���h�E�𒆉��ɐݒu
	setWindowCenter(wnd);

	//�E�B���h�E�̕\��
	ShowWindow(wnd, SW_SHOW);
	UpdateWindow(wnd);
	initialized = true;
	return S_OK;
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Window::update()
{
	ShowWindow(wnd, SW_SHOW);
	UpdateWindow(wnd);
}

//===================================================================================================================================
//�y���b�Z�[�W�v���V�[�W���z
//===================================================================================================================================
LRESULT Window::msgProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(wnd, msg, wParam, lParam))
		return true;

	if (!initialized)	return DefWindowProc(wnd, msg, wParam, lParam);
	if (input == NULL)	return DefWindowProc(wnd, msg, wParam, lParam);
	switch (msg)
	{
	case WM_ACTIVATE:
		windowActivate = wParam != WA_INACTIVE;
		input->clearWheelFraction();
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:case WM_SYSKEYDOWN: // �L�[�������ꂽ
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		input->keyDown(wParam);
		return 0;
	case WM_KEYUP:case WM_SYSKEYUP: // �L�[�������ꂽ
		input->keyUp(wParam);
		return 0;
	case WM_CHAR:					// ���������͂��ꂽ
		input->keyIn(wParam);
		return 0;
	case WM_MOUSEMOVE:				// �}�E�X���ړ����ꂽ
		input->mouseIn(lParam);
		return 0;
	case WM_INPUT:					// �}�E�X����̃��[�f�[�^����
		input->mouseRawIn(lParam);
		return 0;
	case WM_LBUTTONDOWN:			// ���}�E�X�{�^���������ꂽ
		input->setMouseLButton(true);
		SetCapture(wnd);
		input->mouseIn(lParam);
		return 0;
	case WM_LBUTTONUP:				// ���}�E�X�{�^���������ꂽ
		input->setMouseLButton(false);
		ReleaseCapture();
		input->mouseIn(lParam);
		return 0;
	case WM_MBUTTONDOWN:			// �����}�E�X�{�^���������ꂽ
		input->setMouseMButton(true);
		SetCapture(wnd);
		input->mouseIn(lParam);
		return 0;
	case WM_MBUTTONUP:				// �����}�E�X�{�^���������ꂽ
		input->setMouseMButton(false);
		ReleaseCapture();
		input->mouseIn(lParam);
		return 0;
	case WM_RBUTTONDOWN:			// �E�}�E�X�{�^���������ꂽ
		input->setMouseRButton(true);
		SetCapture(wnd);
		input->mouseIn(lParam);
		return 0;
	case WM_RBUTTONUP:				// �E�}�E�X�{�^���������ꂽ
		input->setMouseRButton(false);
		ReleaseCapture();
		input->mouseIn(lParam);
		return 0;
	case WM_XBUTTONDOWN:case WM_XBUTTONUP:// �}�E�X��X�{�^���������ꂽ/�����ꂽ
		input->setMouseXButton(wParam);
		input->mouseIn(lParam);
		return 0;
	case WM_MOUSEWHEEL://�}�E�X�̃z�C�[������]����
		input->mouseWheelIn(wParam);
		return 0;
	case WM_DEVICECHANGE:			// �R���g���[�����`�F�b�N
		input->resetController();
		return 0;
	}
	return DefWindowProc(wnd, msg, wParam, lParam);
}


//===================================================================================================================================
//�y�E�B���h�E�̒����ʒu�ւ̕␳�����z
//===================================================================================================================================
BOOL Window::setWindowCenter(HWND _windowHandle)
{
	// �ϐ��錾
	RECT windowRegion;			//	�E�B���h�E�̈�
	RECT desktopRegion;			//	�f�X�N�g�b�v�̈�
	int windowPositionY = NULL;	//	�E�B���h�E�ʒu Y
	int windowPositionX = NULL;	//	�E�B���h�E�ʒu X
	int windowSizeX = NULL;		//	�E�B���h�E�T�C�Y X
	int windowSizeY = NULL;		//	�E�B���h�E�T�C�Y Y

	// �e�T�C�Y�̎擾
	GetMonitorRect(&desktopRegion);					//	�f�X�N�g�b�v�̃T�C�Y���擾
	GetWindowRect(_windowHandle, &windowRegion);	//	�E�B���h�E�̃T�C�Y���擾

	// �e���W�̊���o��
	windowSizeX = (windowRegion.right - windowRegion.left);													//	�E�C���h�E�̉����̊���o��
	windowSizeY = (windowRegion.bottom - windowRegion.top);													//	�E�C���h�E�̏c���̊���o��
	windowPositionX = (((desktopRegion.right - desktopRegion.left) - windowSizeX) / 2 + desktopRegion.left);//	�������̒������W���̊���o��
	windowPositionY = (((desktopRegion.bottom - desktopRegion.top) - windowSizeY) / 2 + desktopRegion.top);	//	�c�����̒������W���̊���o��

	// �E�B���h�E����ʒ����Ɉړ�
	return SetWindowPos	//	SetWindowPos�֐��F�q�E�B���h�E�A�|�b�v�A�b�v�E�B���h�E�A�܂��̓g�b�v���x���E�B���h�E�̃T�C�Y�A�ʒu�A����� Z �I�[�_�[��ύX����( �����̃E�B���h�E�́A���̉�ʏ�ł̕\���ɏ]���ď��������߂���A�őO�ʂɂ���E�B���h�E�͍ł����������N��^�����AZ�I�[�_�[�̐擪�ɒu����� )
	(
		_windowHandle,									//	�E�B���h�E�n���h��
		NULL,											//	�z�u�����̃n���h���F��s����E�B���h�E�̃n���h�����w��
		windowPositionX,								//	�E�B���h�E�������"X"���W���w��F�������̈ʒu X
		windowPositionY,								//	�E�B���h�E�������"Y"���W���w��F�c�����̈ʒu Y
		windowSizeX,									//	�E�B���h�E�̉������w�� X
		windowSizeY,									//	�E�B���h�E�̏c�����w�� Y
		(SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER)	//	�E�B���h�E�ʒu�̃I�v�V�����F�E�B���h�E�̃T�C�Y��A�ʒu�̕ύX�Ɋւ���t���O���w��
	);
}

//===================================================================================================================================
//�ysetter�z
//===================================================================================================================================
void Window::setInput(){input = getInput();}	//Input�N���X�̃Z�b�g

//===================================================================================================================================
//�ygetter�z
//===================================================================================================================================
//�E�B���h�E�̒����ʒu�擾
D3DXVECTOR2 Window::getCenter()
{
	GetWindowRect(wnd, &rect);//���݂̃E�B���h�E�̋�`�����擾
	D3DXVECTOR2 center((float)(rect.right - rect.left) / 2, (float)(rect.bottom - rect.top) / 2);//�E�B���h�E���ォ��̒����ʒu�ւ̍����v�Z
	center += D3DXVECTOR2((float)rect.left, (float)rect.top);//0,0�ʒu����E�B���h�E����ʒu�����Z	
	return center;
}
//�E�B���h�E�̋�`���̎擾
RECT Window::getRect()
{
	GetWindowRect(wnd, &rect);//���݂̃E�B���h�E�̋�`�����擾
	return rect;
}