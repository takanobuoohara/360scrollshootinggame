// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//

#if !defined(AFX_STDAFX_H__E4994E9F_4C54_40A9_A222_CCB27AA2E14F__INCLUDED_)
#define AFX_STDAFX_H__E4994E9F_4C54_40A9_A222_CCB27AA2E14F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B

#define PI 3.141592653589793238462643383279f 
#define PI180 0.017453292520498851324792463240439f
#define PIw180 57.295779513082320876798154814114f
#define		DEGtoRAD(x)			(float)((float)x * PI / 180.0f)
#define		RELEASE(x)			{ if (x != NULL) {x->Release(); x = NULL;} }
#define matdef 			D3DMATRIX mat; \
			mat._11 = mat._22 = mat._33 = mat._44 = 1.0f; \
			mat._12 = mat._13 = mat._14 = mat._41 = 0.0f; \
			mat._21 = mat._23 = mat._24 = mat._42 = 0.0f; \
			mat._31 = mat._32 = mat._34 = mat._43 = 0.0f; 


#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
#endif // _AFX_NO_AFXCMN_SUPPORT
#include	<d3d8.h>
#include	<d3dx8.h>
#include <dmusici.h>    // DirectAudio���g�p�\�ɂ���
#include <MMSystem.h>

struct CUSTOMVERTEX{
      float x, y, z, rhw;
      DWORD color;
	  float    tu, tv;

  };
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE| D3DFVF_TEX1)

#include "chacha_sdk8_graphic.h"
#include "chacha_sdk8_audio.h"
#include "font.h"
#include "UnCompress.h"
struct datasave{
	int mapdata[256][256]; //�p�[�c�f�[�^
	DWORD tekidata[128][8]; ////0:x 1:y 2:�|������̓G�ԍ� 3:�|������̓G�̔z�u 4,5,6,7:�p�[�c�t���O 
	int tekikazu; //�G�̐�
	int startx,starty; //���@�X�^�[�g�n�_
	int clear; //�N���A����
	int mapx,mapy;
	int data[30];  //���U�[�u�f�[�^
	float juryokudo;
	float dataf[31];
};
extern int tekipart[][31];

struct comment{
	char c[1024];
};
/*3:�|������̓G�̔z�u
0:�G����
1:�|���ꂽ�ʒu
2:���f�[�^
3:�~�b�V�����N���A�����P
4�F�y�i���e�B+1
9�F�Q�[���I�[�o�[
10�` 60��1�b�P�ʂŒx���o��
*/

extern datasave save;
extern comment cm[128];
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__E4994E9F_4C54_40A9_A222_CCB27AA2E14F__INCLUDED_)
