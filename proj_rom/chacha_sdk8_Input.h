// chacha_sdk8_Input.h: Cchacha_sdk8_Input �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHACHA_SDK8_INPUT_H__E752EF9A_22C1_4AF0_8E77_807C1BD66E1B__INCLUDED_)
#define AFX_CHACHA_SDK8_INPUT_H__E752EF9A_22C1_4AF0_8E77_807C1BD66E1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* Joypad��� */
#define BUTTON_UP		0x00000001l	// �����L�[��(dijs.IY<0)
#define BUTTON_DOWN		0x00000002l	// �����L�[��(dijs.IY>0)
#define BUTTON_LEFT		0x00000004l	// �����L�[��(dijs.IX<0)
#define BUTTON_RIGHT	0x00000008l	// �����L�[�E(dijs.IX>0)
#define BUTTON_A		0x00000010l	// �`�{�^��(dijs.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020l	// �a�{�^��(dijs.rgbButtons[1]&0x80)
#define BUTTON_C		0x00000040l	// �b�{�^��(dijs.rgbButtons[2]&0x80)
#define BUTTON_X		0x00000080l	// �w�{�^��(dijs.rgbButtons[3]&0x80)
#define BUTTON_Y		0x00000100l	// �x�{�^��(dijs.rgbButtons[4]&0x80)
#define BUTTON_Z		0x00000200l	// �y�{�^��(dijs.rgbButtons[5]&0x80)
#define BUTTON_L		0x00000400l	// �k�{�^��(dijs.rgbButtons[6]&0x80)
#define BUTTON_R		0x00000800l	// �q�{�^��(dijs.rgbButtons[7]&0x80)
#define BUTTON_START	0x00001000l	// �r�s�`�q�s�{�^��(dijs.rgbButtons[8]&0x80)
#define BUTTON_M		0x00002000l	// �l�{�^��(dijs.rgbButtons[9]&0x80)
#define JOYPADMAX		4			// �����ɐڑ�����W���C�p�b�h�̍ő吔���Z�b�g
//-----------------------------------------------------------------------------
// �\����
//-----------------------------------------------------------------------------
/* �}�E�X�|�C���^�̏�� */
typedef struct _MSTATE {
    RECT    moveRect;     // ��ʏ�œ�����͈�
    int     x;            // X���W
    int     y;            // Y���W
    BOOL    lButton;      // ���{�^��
    BOOL    rButton;      // �E�{�^��
    BOOL    cButton;      // �^�񒆃{�^��
    int     moveAdd;      // �ړ���
    RECT    imgRect;      // �}�E�X�p�摜��`
    int     imgWidth;     // �}�E�X�摜��
    int     imgHeight;    // �}�E�X�摜����
	int		wheel;
} MSTATE, *LPMSTATE;

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
/* �L�[�{�[�h�p */
extern BYTE					g_diKeyState[256];
/* �W���C�p�b�h�p */
extern DWORD				g_diJoyState;
extern DWORD				g_diJoyState2[JOYPADMAX];
extern int					g_diJoyCount;
/* �}�E�X�p */
extern DIMOUSESTATE2			g_diMouseState;

extern 	DIJOYSTATE2		dijs;
extern LPDIRECTINPUTDEVICE8	g_lpDIKeyboard;
extern LPDIRECTINPUTDEVICE8	g_lpDIJoypad;
extern LPDIRECTINPUTDEVICE8	g_lpDIJoypad2[JOYPADMAX];
extern LPDIRECTINPUTDEVICE8	g_lpDIMouse;
class Cchacha_sdk8_Input  
{
public:
	Cchacha_sdk8_Input();
	virtual ~Cchacha_sdk8_Input();

	/* DirectInput���� */
	BOOL InitDInput(HINSTANCE,HWND hwnd);
	void ReleaseDInput(void);
	/* �L�[�{�[�h�p */
	BOOL InitKeyboard(void);
	void GetKeyboardState(void);
	/* �W���C�p�b�h�p */
	BOOL InitJoypad(void);
	void GetJoypadState(void);
	BOOL InitJoypad2(void);
	void GetJoypadState2(void);
	/* �}�E�X�p */
	BOOL InitMouse(void);
	void GetMouseState(void);

	HWND hWnd;
	static BOOL CALLBACK GetJoystickCallback2(LPDIDEVICEINSTANCE lpddi, LPVOID /*pvRef*/);
	static BOOL CALLBACK GetJoystickCallback(LPDIDEVICEINSTANCE lpddi, LPVOID /*pvRef*/);

};

#endif // !defined(AFX_CHACHA_SDK8_INPUT_H__E752EF9A_22C1_4AF0_8E77_807C1BD66E1B__INCLUDED_)
