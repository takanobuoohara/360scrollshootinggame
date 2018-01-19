// UnCompress.cpp: UnCompress �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UnCompress.h"
#include "direct.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

UnCompress::UnCompress()
{
	_fname = "";
	nom=FALSE;
	err=TRUE;
}

UnCompress::~UnCompress()
{
	Close();
}

BOOL UnCompress::run(CString fname)
{
	buf2cnt = 0;
	CString fname2;
	fname2 = fname.Left(fname.GetLength()-3); // .af ���O��
	if(file.Open(fname ,CFile::shareDenyNone | CFile::modeRead | CFile::typeBinary,NULL)==TRUE)
	{
		if(file2.Open(fname2,CFile::shareDenyNone | CFile::modeCreate | CFile::modeWrite | CFile::typeBinary,NULL)==FALSE) return FALSE;
		file.SeekToBegin();
		UINT cnt; cnt = 10000;
		file.Read(buf,1);
		moji = buf[0];
		mojicnt = 0;
		for(;;)
		{
			if(cnt != 10000) break;
			cnt = file.Read(buf,10000);
			if(cnt == 0) break;
			for(UINT i=0;i<cnt;i++)
			{
				if(moji == buf[i]) //���k���ꂽ�ް���?
				{
					i++;
					if(i == cnt)
					{
						cnt = file.Read(buf,10000);
						if(cnt == 0) break;
						i=0;
					}
					mojicnt = buf[i];
					for(int j=0;j<((int)mojicnt+1);j++)
						Write(moji);

					//Write(bufout,(int)mojicnt+1);
					i++;
					if(i == cnt)
					{
						cnt = file.Read(buf,10000);
						if(cnt == 0) break;
						i=0;
					}
					moji = buf[i];
				}
				else
				{
					//bufout[0] = moji;
					Write(moji);
					moji = buf[i];
				}
			}
		}
		Write(moji);
		file2.Write(buf2,buf2cnt);
		file2.Close();
		file.Close();
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

void UnCompress::Write(BYTE b)
{
	buf2[buf2cnt] = b;
	buf2cnt++;
	if(buf2cnt == 10000)
	{
		file2.Write(buf2,10000);
		buf2cnt = 0;
	}
}

BOOL UnCompress::run2(CString fname)
{
	buf2cnt = 0;
	CString fname2;
	fname2 = fname.Left(fname.GetLength()-4); // .af2 ���O��
	if(file.Open(fname ,CFile::shareDenyNone | CFile::modeRead | CFile::typeBinary,NULL)==TRUE)
	{
		if(file2.Open(fname2,CFile::shareDenyNone | CFile::modeCreate | CFile::modeWrite | CFile::typeBinary,NULL)==FALSE) return FALSE;
		file.SeekToBegin();
		cnt_1 = cnt_2 = cnt_d = 0;
		cnt21 = cnt22 = cnt2d = 0;
		//�t�@�C�����Ƃ��𓾂�
		BYTE s[5],s1[2],s2[2],d[2];
		file.Read(buf,12);
		Outlen = ((UINT)buf[0])*0x1000000 + ((UINT)buf[1])*0x10000 + ((UINT)buf[2])*0x100 + ((UINT)buf[3]);
		pFlg1  = 12;
		pFlg2  = ((UINT)buf[4])*0x1000000 + ((UINT)buf[5])*0x10000 + ((UINT)buf[6])*0x100 + ((UINT)buf[7]) + 12;
		pData  = pFlg2 + ((UINT)buf[8])*0x1000000 + ((UINT)buf[9])*0x10000 + ((UINT)buf[10])*0x100 + ((UINT)buf[11]);

		BYTE f[8] = {7,6,5,4,3,2,1,0}; //�t���O1�̼�ėp
		BYTE f2[4] = {6,4,2,0}; //�t���O2�̼�ėp
		s[0] = s[1] = s[2] = s[3] = s[4] = 0;
		BYTE su;
		int l1,l2; l1=l2 =0;
		//���[�s���O�J�n
		for(UINT i=0;i<Outlen;i++)
		{
			//�t���O1�ǂݍ���
			if(l1 == 0)
			{
				ReadFlg1(s1,1);
				l1 = 8;
			}
			l1 --;
			su = (s1[0] >> l1) & 0x01; //�A���t���O�`�F�b�N
			if(su == 0)
			{
				//�s�A��
				if(l2 == 0)
				{
					ReadFlg2(s2,1);
					l2 = 4;
				}
				l2 --;
				su = (s2[0] >> (l2*2)) & 0x03;
				switch(su)
				{
				case 0://�V�K
					ReadData(d,1);
					s[0] = d[0];
					Write(s[0]);
					break;
				case 1://2Byte�O
					s[0] = s[2];
					Write(s[0]);
					break;
				case 2://3Byte�O
					s[0] = s[3];
					Write(s[0]);
					break;
				case 3://4Byte�O
					s[0] = s[4];
					Write(s[0]);
					break;
				}
			}
			else
			{
				//�A��
				Write(s[0]);
			}
			s[4] = s[3];
			s[3] = s[2];
			s[2] = s[1];
			s[1] = s[0];
		}
		file2.Write(buf2,buf2cnt);
		file2.Close();
		file.Close();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

UINT UnCompress::ReadFlg1(BYTE s[],int cnt)
{
	if(cnt_1 == 0)
	{
		file.Seek(pFlg1,CFile::begin);
		cnt_1 = file.Read(s_1,10000);
		if(cnt_1 == 0) return 0;
		cnt21 = 0;
		pFlg1 += cnt_1;
	}
	cnt_1 --;
	s[0] = s_1[cnt21];
	cnt21 ++;
	return 1;
}

UINT UnCompress::ReadFlg2(BYTE s[],int cnt)
{
	if(cnt_2 == 0)
	{
		file.Seek(pFlg2,CFile::begin);
		cnt_2 = file.Read(s_2,10000);
		if(cnt_2 == 0) return 0;
		cnt22 = 0;
		pFlg2 += cnt_2;
	}
	cnt_2 --;
	s[0] = s_2[cnt22];
	cnt22 ++;
	return 1;
}

UINT UnCompress::ReadData(BYTE s[],int cnt)
{
	if(cnt_d == 0)
	{
		file.Seek(pData,CFile::begin);
		cnt_d = file.Read(s_d,10000);
		if(cnt_d == 0) return 0;
		cnt2d = 0;
		pData += cnt_d;
	}
	cnt_d --;
	s[0] = s_d[cnt2d];
	cnt2d ++;
	return 1;
}

typedef struct {
    long count;      /* �q�m�[�h�̏o���p�x�̘a */
    int  parent;     /* �e�m�[�h�� */
    int  left;       /* �����̎q�m�[�h�� */
    int  right;      /* �E���̎q�m�[�h�� */
} NODE;

#define CHAR_SIZE    256
#define NODE_SIZE    (2*CHAR_SIZE+2)

BOOL UnCompress::run3(CString fname)
{
	buf2cnt = 0;
	CString fname2;
	fname2 = fname.Left(fname.GetLength()-4); // .af3 ���O��
	if(file.Open(fname ,CFile::shareDenyNone | CFile::modeRead | CFile::typeBinary,NULL)==TRUE)
	{
		if(file2.Open(fname2,CFile::shareDenyNone | CFile::modeCreate | CFile::modeWrite | CFile::typeBinary,NULL)==FALSE) return FALSE;
		for(;;)
		{
			int r = file.Read(buf,2);
			if(r == 0) break;
			r = (int)((UINT)buf[0] * 256 + (UINT)buf[1]);
			int l = file.Read(buf,r); //�f�|�^�̓ǂݍ���
			if(l == 0) return FALSE;
			r = deHuffman(buf2,60000,buf,l);
			if(r == -1L) return FALSE;
			file2.Write(buf2,r);
		}
		file2.Close();
		file.Close();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

long UnCompress::deHuffman(unsigned char *text, long tlen,
               unsigned char *code, long clen)
{
    int  i, k;
    long c, len;
    int  min1, min2;
    int  root;
    NODE node[NODE_SIZE]; int  freeNode;
    static unsigned char bit1[8] = { 128, 64, 32, 16, 8, 4, 2, 1};

    if (clen <= 2*CHAR_SIZE) return -1L;

    /* �e�����̏o���p�x���Z�b�g */
    for (i = 0; i < NODE_SIZE; i++) node[i].count = 0;
    for (i = 0; i < CHAR_SIZE; i++) {
        node[i].count = *code++;
        node[i].count = (node[i].count << 8) | *code++;
    }
    
    /* EOF */
    node[CHAR_SIZE].count = 1;

    /* �n�t�}���؂����� */
    node[NODE_SIZE - 1].count = 0x10000L;          /* �ԕ� */
    for (freeNode = CHAR_SIZE+1; ; freeNode++) {
        min1 = min2 = NODE_SIZE - 1;
        for (i = NODE_SIZE - 2; i >= 0; i--)
            if (node[i].count > 0)
                if (node[i].count < node[min1].count) {
                    min2 = min1;
                    min1 = i;
                } else if (node[i].count < node[min2].count)
                    min2 = i;
        if (min2 == NODE_SIZE - 1) break;
        node[freeNode].left  = min1;
        node[freeNode].right = min2;
        node[freeNode].count = node[min1].count + node[min2].count;
        node[min1].parent = node[min2].parent = freeNode;
        node[min1].count = node[min2].count = 0;
    }
    root = min1;
            
    /* ������ */
    for (i = 0, len = 0, c = 2*CHAR_SIZE; ; ) {
        /* ������ɑΉ����镶���m�[�h�𓾂� */
        k = root;
        do {
            int parent = k;
            k = (*code & bit1[i]) ? node[k].right : node[k].left;
            if (k >= parent) return -1L;     /* ���蓾�Ȃ� */
            if (++i == 8) {
                code++;
                if (++c > clen) return -1L;
                i = 0;
            }
        } while (k > CHAR_SIZE);
        if (k == CHAR_SIZE) break;
        if (++len > tlen) return -1L;
        *text++ = k;
    }
    return len;
}

//extern CString flib_;

CString UnCompress::Open(CString fname,CString flib)
{
	Close();
	_chdir(".\\data");
	CString f,f1; f=fname;
	f.MakeLower();
	CString flib1;
	CString ex[4];ex[0]=".af";ex[1]=".af2";ex[2]=".af3";ex[3]="";
	flib1 = "gamedat";
	if(f.Right(3)==".af")
		f=f.Left(f.GetLength()-3);
	if(f.Right(4)==".af2")
		f=f.Left(f.GetLength()-4);
	if(f.Right(4)==".af3")
		f=f.Left(f.GetLength()-4);
	for(int i=0;i<3;i++)
	{
		f1=f+ex[i];
		la.Open(flib1,f1);
		if(f1.Right(4) == ".af3")
		{
			_fname = f1.Left(f1.GetLength()-4);
			if(run3(f1) == FALSE)
			{
			}else{
				_chdir("..");
				return "data\\"+_fname;
			}
		}
		if(f1.Right(4) == ".af2")
		{
			_fname = f1.Left(f1.GetLength()-4);
			if(run2(f1) == FALSE)
			{
			}else{
				_chdir("..");
				return "data\\"+_fname;
			}
		}
		if(f1.Right(3) == ".af")
		{
			_fname = f1.Left(f1.GetLength()-3);
			if(run(f1) == FALSE)
			{
			}else{
				_chdir("..");
				return "data\\"+_fname;
			}
		}
		if(la.openfile != "")
			la.Close();
	}
	if(file.Open(f ,CFile::shareDenyNone | CFile::modeRead | CFile::typeBinary,NULL)==TRUE)
	{
		file.Close();
			nom=TRUE;
		_chdir("..");
		return "data\\"+f;
	}
	else
	{
		if(err==TRUE)
		{
			CString str;
			str.Format(" File: %s ������܂���B\n��۸��т������I�����܂��BOK�������ĉ������B",f);
			_chdir("..");
			return str;
		}
		_chdir("..");
		return "";
	}
	_chdir("..");
	return "data\\"+fname;
}

void UnCompress::Close()
{
	_chdir(".\\data");
	if(la.openfile != "")
		la.Close();
	if(nom==FALSE)
		if(_fname != "")
			CFile::Remove(_fname);
	_fname = "";
	_chdir("..");
}

