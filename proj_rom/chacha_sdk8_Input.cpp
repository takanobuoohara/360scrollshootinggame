// chacha_sdk8_Input.cpp: Cchacha_sdk8_Input �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chacha_sdk8_Input.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Joypad�p�ݒ�l
#define DEADZONE		1500			// �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX		1000			// �L���͈͂̍ő�l
#define RANGE_MIN		-1000			// �L���͈͂̍ŏ��l

//-----------------------------------------------------------------------------
// �O���[�o���ϐ��i���̃\�[�X��������p�������́j
//-----------------------------------------------------------------------------
/* �L�[�{�[�h�p */
BYTE					g_diKeyState[256];			// �L�[�{�[�h���
/* �W���C�p�b�h�p */
DWORD					g_diJoyState = 0x00000000L;	// �W���C�p�b�h���
DWORD					g_diJoyState2[JOYPADMAX];	// �W���C�p�b�h���i�����Ή��j
int						g_diJoyCount = 0;			// ���o�����W���C�p�b�h�̐�
/* �}�E�X�p */
DIMOUSESTATE2	g_diMouseState;				// �}�E�X���
DIJOYSTATE2		dijs;

//-----------------------------------------------------------------------------
// �O���[�o���ϐ��i���\�[�X�ł̂ݗ��p������́j
//-----------------------------------------------------------------------------
/* DirectInput���� */
LPDIRECTINPUT8		g_lpDInput = NULL;			// DirectInput�I�u�W�F�N�g
/* �L�[�{�[�h�p */
LPDIRECTINPUTDEVICE8	g_lpDIKeyboard = NULL;		// �L�[�{�[�h�f�o�C�X
/* �W���C�p�b�h�p */
LPDIRECTINPUTDEVICE8	g_lpDIJoypad = NULL;		// �W���C�p�b�h�f�o�C�X
LPDIRECTINPUTDEVICE8	g_lpDIJoypad2[JOYPADMAX] = {NULL,NULL,NULL,NULL};// �W���C�p�b�h�f�o�C�X
/* �}�E�X�p */
LPDIRECTINPUTDEVICE8	g_lpDIMouse = NULL;			// �}�E�X�f�o�C�X

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

Cchacha_sdk8_Input::Cchacha_sdk8_Input()
{
	g_lpDIJoypad=NULL;g_lpDIKeyboard = NULL;
	g_lpDIMouse=NULL;
}

Cchacha_sdk8_Input::~Cchacha_sdk8_Input()
{
	ReleaseDInput();
}

//-----------------------------------------------------------------------------
// Direct Input ������
//-----------------------------------------------------------------------------
BOOL Cchacha_sdk8_Input::InitDInput(HINSTANCE hInstApp,HWND hwnd)
{
	HRESULT		hr;
	hWnd=hwnd;
	hr = DirectInput8Create(hInstApp, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_lpDInput, NULL);
	if ( FAILED(hr) )
		return FALSE;	// DirectInput8�̍쐬�Ɏ��s

	return TRUE;

}

//-----------------------------------------------------------------------------
// Direct Input �J������
//-----------------------------------------------------------------------------
void Cchacha_sdk8_Input::ReleaseDInput(void)
{
	// DirectInput�̃f�o�C�X���J��
	if ( g_lpDIKeyboard ) g_lpDIKeyboard->Unacquire();
	RELEASE(g_lpDIKeyboard);
	if ( g_lpDIMouse ) g_lpDIMouse->Unacquire();
	RELEASE(g_lpDIMouse);
	if ( g_lpDIJoypad ) g_lpDIJoypad->Unacquire();
	RELEASE(g_lpDIJoypad);
	for (int i=0 ; i<JOYPADMAX ; i++) {
		if ( g_lpDIJoypad2[i] ) g_lpDIJoypad2[i]->Unacquire();
		RELEASE(g_lpDIJoypad2[i]);
	}
	RELEASE(g_lpDInput);

}

//-----------------------------------------------------------------------------
// �L�[�{�[�h�p�I�u�W�F�N�g�쐬
//-----------------------------------------------------------------------------
BOOL Cchacha_sdk8_Input::InitKeyboard(void)
{
	HRESULT		hr;

	//�L�[�{�[�h�p�Ƀf�o�C�X�I�u�W�F�N�g���쐬
	hr = g_lpDInput->CreateDevice(GUID_SysKeyboard, &g_lpDIKeyboard, NULL); 
	if ( FAILED(hr) )
		return FALSE;  // �f�o�C�X�̍쐬�Ɏ��s

	//�L�[�{�[�h�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	hr = g_lpDIKeyboard->SetDataFormat(&c_dfDIKeyboard);	
	if ( FAILED(hr) )
		return FALSE; // �f�o�C�X�̍쐬�Ɏ��s

	//���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_lpDIKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if ( FAILED(hr) )
		return FALSE; // ���[�h�̐ݒ�Ɏ��s

	//�L�[�{�[�h���͐���J�n
	g_lpDIKeyboard->Acquire();

	return TRUE;

}

//-----------------------------------------------------------------------------
// �֐����@�F�@GetKeyboardState()
// �@�\�T�v�F�@�L�[�{�[�h�̏�Ԃ��擾
//-----------------------------------------------------------------------------
void Cchacha_sdk8_Input::GetKeyboardState(void)
{
	HRESULT			hr;

	if ( g_lpDIKeyboard == NULL) return;

	hr = g_lpDIKeyboard->GetDeviceState(256, g_diKeyState);
	if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED ) {
		hr = g_lpDIKeyboard->Acquire();
		while ( hr == DIERR_INPUTLOST )
			hr = g_lpDIKeyboard->Acquire();
	}

}

//-----------------------------------------------------------------------------
//	�W���C�X�e�B�b�N�E�f�o�C�X��񋓁i�R�[���o�b�N�֐��j
//-----------------------------------------------------------------------------
BOOL CALLBACK Cchacha_sdk8_Input::GetJoystickCallback(LPDIDEVICEINSTANCE lpddi, LPVOID /*pvRef*/)
{
	HRESULT hr;

	hr = g_lpDInput->CreateDevice(lpddi->guidInstance, &g_lpDIJoypad, NULL);
	if ( FAILED(hr) )
		return DIENUM_CONTINUE;	// ���̃f�o�C�X���

	return DIENUM_STOP;	// �f�o�C�X�̗񋓂𒆎~

}

//-----------------------------------------------------------------------------
// Joypad�p�I�u�W�F�N�g�쐬
//-----------------------------------------------------------------------------
BOOL Cchacha_sdk8_Input::InitJoypad(void)
{
	HRESULT		hr;

	// �W���C�p�b�h��T��
	hr=g_lpDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetJoystickCallback, NULL, DIEDFL_ATTACHEDONLY);
	if(hr!=DI_OK){
		if ( g_lpDIJoypad ) g_lpDIJoypad->Unacquire();
		RELEASE(g_lpDIJoypad);
	}	
	if ( g_lpDIJoypad == NULL )
		return FALSE;

	// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	hr = g_lpDIJoypad->SetDataFormat(&c_dfDIJoystick);
	if ( FAILED(hr) )
		return FALSE; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

	// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
//	hr = g_lpDIJoypad->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//	if (FAILED(hr))
//		return FALSE; // ���[�h�̐ݒ�Ɏ��s

	// ���̒l�͈̔͂�ݒ�
	// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
	// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
	// ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�́A�֌W�Ȃ�)
	DIPROPRANGE				diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize		= sizeof(diprg); 
	diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
	diprg.diph.dwHow		= DIPH_BYOFFSET; 
	diprg.lMin				= RANGE_MIN;
	diprg.lMax				= RANGE_MAX;
	// X���͈̔͂�ݒ�
	diprg.diph.dwObj		= DIJOFS_X; 
	g_lpDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	// Y���͈̔͂�ݒ�
	diprg.diph.dwObj		= DIJOFS_Y;
	g_lpDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

	// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
	// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
	// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
	DIPROPDWORD				dipdw;
	dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
	dipdw.diph.dwHow		= DIPH_BYOFFSET;
	dipdw.dwData			= DEADZONE;
	//X���̖����]�[����ݒ�
	dipdw.diph.dwObj		= DIJOFS_X;
	g_lpDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	//Y���̖����]�[����ݒ�
	dipdw.diph.dwObj		= DIJOFS_Y;
	g_lpDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		
	//�W���C�X�e�B�b�N���͐���J�n
	g_lpDIJoypad->Acquire();
		
	return TRUE;

}

//-----------------------------------------------------------------------------
// �֐����@�F�@GetJoypadState()
// �@�\�T�v�F�@�W���C�p�b�h�̏�Ԃ��擾
//-----------------------------------------------------------------------------
void Cchacha_sdk8_Input::GetJoypadState(void)
{
	HRESULT			hr;

	g_diJoyState = 0x00000000l;	// ������

	hr = g_lpDIJoypad->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
	if ( FAILED(hr) ) {
		hr = g_lpDIJoypad->Acquire();
		while ( hr == DIERR_INPUTLOST )
			hr = g_lpDIJoypad->Acquire();
	}

	hr = g_lpDIJoypad->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// �f�o�C�X��Ԃ�ǂݎ��
	if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED ) {
		hr = g_lpDIJoypad->Acquire();
		while ( hr == DIERR_INPUTLOST )
			hr = g_lpDIJoypad->Acquire();
	}

	// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���
	//* y-axis (forward)
	if ( dijs.lY < 0 )					g_diJoyState |= BUTTON_UP;
	//* y-axis (backward)
	if ( dijs.lY > 0 )					g_diJoyState |= BUTTON_DOWN;
	//* x-axis (left)
	if ( dijs.lX < 0 )					g_diJoyState |= BUTTON_LEFT;
	//* x-axis (right)
	if ( dijs.lX > 0 )					g_diJoyState |= BUTTON_RIGHT;
	//* �`�{�^��
	if ( dijs.rgbButtons[0] & 0x80 )	g_diJoyState |= BUTTON_A;
	//* �a�{�^��
	if ( dijs.rgbButtons[1] & 0x80 )	g_diJoyState |= BUTTON_B;
	//* �b�{�^��
	if ( dijs.rgbButtons[2] & 0x80 )	g_diJoyState |= BUTTON_C;
	//* �w�{�^��
	if ( dijs.rgbButtons[3] & 0x80 )	g_diJoyState |= BUTTON_X;
	//* �x�{�^��
	if ( dijs.rgbButtons[4] & 0x80 )	g_diJoyState |= BUTTON_Y;
	//* �y�{�^��
	if ( dijs.rgbButtons[5] & 0x80 )	g_diJoyState |= BUTTON_Z;
    //* �k�{�^��
    if ( dijs.rgbButtons[6] & 0x80 )	g_diJoyState |= BUTTON_L;
	//* �q�{�^��
	if ( dijs.rgbButtons[7] & 0x80 )	g_diJoyState |= BUTTON_R;
	//* �r�s�`�q�s�{�^��
	if ( dijs.rgbButtons[8] & 0x80 )	g_diJoyState |= BUTTON_START;
	//* �l�{�^��
	if ( dijs.rgbButtons[9] & 0x80 )	g_diJoyState |= BUTTON_M;

}

//-----------------------------------------------------------------------------
//	�����̃W���C�X�e�B�b�N�E�f�o�C�X��񋓁i�R�[���o�b�N�֐��j
//-----------------------------------------------------------------------------
BOOL CALLBACK Cchacha_sdk8_Input::GetJoystickCallback2(LPDIDEVICEINSTANCE lpddi, LPVOID /*pvRef*/)
{
	HRESULT hr;

	hr = g_lpDInput->CreateDevice(lpddi->guidInstance, &g_lpDIJoypad2[g_diJoyCount++], NULL);
	return DIENUM_CONTINUE;	// ���̃f�o�C�X���

}

//-----------------------------------------------------------------------------
//  ������Joypad�p�I�u�W�F�N�g���쐬
//-----------------------------------------------------------------------------
BOOL Cchacha_sdk8_Input::InitJoypad2(void)
{
	HRESULT		hr;
	int			i;

	// �W���C�p�b�h��T��
	g_lpDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetJoystickCallback2, NULL, DIEDFL_ATTACHEDONLY);

	for ( i=0 ; i<g_diJoyCount ; i++ ) {
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		hr = g_lpDIJoypad2[i]->SetDataFormat(&c_dfDIJoystick);
		if ( FAILED(hr) )
			return FALSE; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

		// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
//		hr = g_lpDIJoypad2[i]->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//		if ( FAILED(hr) )
//			return FALSE; // ���[�h�̐ݒ�Ɏ��s

		// ���̒l�͈̔͂�ݒ�
		// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
		// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
		// ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�́A�֌W�Ȃ�)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize		= sizeof(diprg); 
		diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
		diprg.diph.dwHow		= DIPH_BYOFFSET; 
		diprg.lMin				= RANGE_MIN;
		diprg.lMax				= RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj		= DIJOFS_X; 
		g_lpDIJoypad2[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj		= DIJOFS_Y;
		g_lpDIJoypad2[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
		dipdw.diph.dwHow		= DIPH_BYOFFSET;
		dipdw.dwData			= DEADZONE;
		//X���̖����]�[����ݒ�
		dipdw.diph.dwObj		= DIJOFS_X;
		g_lpDIJoypad2[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph);
		//Y���̖����]�[����ݒ�
		dipdw.diph.dwObj		= DIJOFS_Y;
		g_lpDIJoypad2[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
			
		//�W���C�X�e�B�b�N���͐���J�n
		g_lpDIJoypad2[i]->Acquire();
	}
		
	return TRUE;

}

//-----------------------------------------------------------------------------
// �֐����@�F�@GetJoypadState2()
// �@�\�T�v�F�@�����̃W���C�p�b�h�̏�Ԃ��擾
//-----------------------------------------------------------------------------
void Cchacha_sdk8_Input::GetJoypadState2(void)
{
	HRESULT			hr=TRUE;
	int				i;

	for ( i=0 ; i<g_diJoyCount ; i++ ) {
		g_diJoyState2[i] = 0x00000000l;	// ������

		g_lpDIJoypad2[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if ( FAILED(hr) ) {
			hr = g_lpDIJoypad2[i]->Acquire();
			while ( hr == DIERR_INPUTLOST )
				hr = g_lpDIJoypad2[i]->Acquire();
		}

		hr = g_lpDIJoypad2[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// �f�o�C�X��Ԃ�ǂݎ��
		if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED ) {
			hr = g_lpDIJoypad2[i]->Acquire();
			while ( hr == DIERR_INPUTLOST )
				hr = g_lpDIJoypad2[i]->Acquire();
		}

		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���
		//* y-axis (forward)
		if ( dijs.lY < 0 )					g_diJoyState2[i] |= BUTTON_UP;
		//* y-axis (backward)
		if ( dijs.lY > 0 )					g_diJoyState2[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if ( dijs.lX < 0 )					g_diJoyState2[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if ( dijs.lX > 0 )					g_diJoyState2[i] |= BUTTON_RIGHT;
		//* �`�{�^��
		if ( dijs.rgbButtons[0] & 0x80 )	g_diJoyState2[i] |= BUTTON_A;
		//* �a�{�^��
		if ( dijs.rgbButtons[1] & 0x80 )	g_diJoyState2[i] |= BUTTON_B;
		//* �b�{�^��
		if ( dijs.rgbButtons[2] & 0x80 )	g_diJoyState2[i] |= BUTTON_C;
		//* �w�{�^��
		if ( dijs.rgbButtons[3] & 0x80 )	g_diJoyState2[i] |= BUTTON_X;
		//* �x�{�^��
		if ( dijs.rgbButtons[4] & 0x80 )	g_diJoyState2[i] |= BUTTON_Y;
		//* �y�{�^��
		if ( dijs.rgbButtons[5] & 0x80 )	g_diJoyState2[i] |= BUTTON_Z;
		//* �k�{�^��
		if ( dijs.rgbButtons[6] & 0x80 )	g_diJoyState2[i] |= BUTTON_L;
		//* �q�{�^��
		if ( dijs.rgbButtons[7] & 0x80 )	g_diJoyState2[i] |= BUTTON_R;
		//* �r�s�`�q�s�{�^��
		if ( dijs.rgbButtons[8] & 0x80 )	g_diJoyState2[i] |= BUTTON_START;
		//* �l�{�^��
		if ( dijs.rgbButtons[9] & 0x80 )	g_diJoyState2[i] |= BUTTON_M;
	}

}

//-----------------------------------------------------------------------------
// �}�E�X�p�I�u�W�F�N�g�쐬
//-----------------------------------------------------------------------------
BOOL Cchacha_sdk8_Input::InitMouse(void)
{
	HRESULT		hr;

	// �}�E�X�p�Ƀf�o�C�X�I�u�W�F�N�g���쐬
	hr = g_lpDInput->CreateDevice(GUID_SysMouse, &g_lpDIMouse, NULL);
	if ( FAILED(hr) )
		return FALSE;	// �f�o�C�X�̍쐬�Ɏ��s

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = g_lpDIMouse->SetDataFormat(&c_dfDIMouse2);	// �}�E�X�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	if ( FAILED(hr) )
		return FALSE;	// �f�[�^�t�H�[�}�b�g�Ɏ��s

	// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_lpDIMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if ( FAILED(hr) )
		return FALSE;	// ���[�h�̐ݒ�Ɏ��s

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD					diprop;
	diprop.diph.dwSize			= sizeof(diprop);
	diprop.diph.dwHeaderSize	= sizeof(diprop.diph);
	diprop.diph.dwObj			= 0;
	diprop.diph.dwHow			= DIPH_DEVICE;
	diprop.dwData				= DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j
	hr = g_lpDIMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if ( FAILED(hr) )
		return FALSE;	// �f�o�C�X�̐ݒ�Ɏ��s

	// ���͐���J�n
	g_lpDIMouse->Acquire();

	return TRUE;

}

//-----------------------------------------------------------------------------
// �֐����@�F�@GetMouseState()
// �@�\�T�v�F�@�}�E�X�̏�Ԃ��擾
//-----------------------------------------------------------------------------
void Cchacha_sdk8_Input::GetMouseState(void)
{
	HRESULT			hr;
	ZeroMemory(&g_diMouseState,sizeof g_diMouseState);
	DIDEVCAPS a;
	ZeroMemory(&a,sizeof DIDEVCAPS);
	a.dwSize=sizeof DIDEVCAPS;
	g_lpDIMouse->GetCapabilities(&a);
	hr = g_lpDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &g_diMouseState);
	if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED ) {
		hr = g_lpDIMouse->Acquire();
		while ( hr == DIERR_INPUTLOST )
			hr = g_lpDIMouse->Acquire();
	}
	for(int i=a.dwButtons;i<8;i++)
		g_diMouseState.rgbButtons[i]=0;
//#if DEBUG
//	char buf[80];
//	wsprintf(buf, "(%d, %d, %d) %s %s %s",
//		diMouseState.lX,
//		diMouseState.lY,
//		diMouseState.lZ,
//		(diMouseState.rgbButtons[0] & 0x80) ? "Left" : "--",
//		(diMouseState.rgbButtons[1] & 0x80) ? "Right" : "--",
//		(diMouseState.rgbButtons[2] & 0x80) ? "Center" : "--");
//	strOut(200, 0, buf, RGB(255, 255, 255), 10, "�l�r ����", FALSE);
//#endif

}
