// compress.cpp: compress �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "compress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

compress::compress()
{

}

compress::~compress()
{

}

BOOL compress::run(CString fname)
{
	buf2cnt = 0;
	CString fname2;
	if(file.Open(fname ,CFile::shareDenyNone | CFile::modeRead | CFile::typeBinary,NULL)==TRUE)
	{
		fname2 = fname + ".af"; //�ϊ���t�@�C��
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
				if(buf[i] == moji)
				{
					mojicnt++;
				}
				if(buf[i] != moji)
				{
					//bufout[0] = moji;
					if(mojicnt == 0)
						Write(moji);
					else
					{
						Write(moji);
						Write(moji);
						Write(mojicnt);
					}
					mojicnt = 0;
					moji = buf[i];
				}
				if(mojicnt == 255)
				{
/*					bufout[0] = moji;
					bufout[1] = moji;
					bufout[2] = mojicnt-1;
					file2.Write(bufout,3);
*/					Write(moji);
					Write(moji);
					Write(mojicnt-1);
					mojicnt = 0;
				}
			}
		}
		if(mojicnt != 0)
		{
/*			bufout[0] = moji;
			bufout[1] = moji;
			bufout[2] = mojicnt;
			file2.Write(bufout,3);
*/			Write(moji);
			Write(moji);
			Write(mojicnt);
			mojicnt = 0;
		}
		else
			Write(moji);

		file2.Write(buf2,buf2cnt);
		//file2.Close();
		//file.Close();
	}
	else
		return FALSE;
	return TRUE;
}

void compress::Write(BYTE b)
{
	buf2[buf2cnt] = b;
	buf2cnt++;
	if(buf2cnt == 10000)
	{
		file2.Write(buf2,10000);
		buf2cnt = 0;
	}
}

//af2�`��
BOOL compress::run2(CString fname)
{
	CString fname2,fname2a,fname2b;
	if(file.Open(fname ,CFile::shareDenyNone | CFile::modeRead | CFile::typeBinary,NULL)==TRUE)
	{
		fname2 = fname + ".af2"; //�ϊ���t�@�C��
		fname2a = fname + ".af2_";
		fname2b = fname + ".af2__";
		if(file2.Open(fname2 ,CFile::shareDenyNone | CFile::modeCreate | CFile::modeWrite | CFile::typeBinary,NULL)==FALSE) return FALSE;
		if(file3.Open(fname2a,CFile::shareDenyNone | CFile::modeCreate | CFile::modeWrite | CFile::typeBinary,NULL)==FALSE) return FALSE;
		if(file4.Open(fname2b,CFile::shareDenyNone | CFile::modeCreate | CFile::modeWrite | CFile::typeBinary,NULL)==FALSE) return FALSE;
		Inlen = file.GetLength();
		Outlen = Inlen / 8 + ((Inlen % 8) != 0);
		file.SeekToBegin();
		file2.SeekToBegin();
		file3.SeekToBegin();
		file4.SeekToBegin();

		BYTE s[5],ss[2]; //�ޯ̧�p
		BYTE f[8] = {7,6,5,4,3,2,1,0}; //�t���O1�̼�ėp
		BYTE f2[4] = {6,4,2,0}; //�t���O2�̼�ėp
		//   |   �l    | |��Ĳ��ޯ��  | |�f�[�^|
 		BYTE nFlg1,nFlg2,nf = 0,nf2 = 0,nfd =0 ,nf2d=0;
		//���̃t�@�C���̑傫��
		s[0] = (BYTE)(Inlen / 0x1000000) % 0x100;	file2.Write( s , 1);
		s[0] = (BYTE)(Inlen / 0x10000  ) % 0x100;	file2.Write( s , 1);
		s[0] = (BYTE)(Inlen / 0x100    ) % 0x100;	file2.Write( s , 1);
		s[0] = (BYTE)(Inlen / 0x1      ) % 0x100;	file2.Write( s , 1);
		//�t���O2�̏ꏊ��ݒ� (���̒l��12�𑫂�����)
		s[0] = (BYTE)(Outlen / 0x1000000) % 0x100; file2.Write( s , 1);
		s[0] = (BYTE)(Outlen / 0x10000  ) % 0x100; file2.Write( s , 1);
		s[0] = (BYTE)(Outlen / 0x100    ) % 0x100; file2.Write( s , 1);
		s[0] = (BYTE)(Outlen / 0x1      ) % 0x100; file2.Write( s , 1);
		//
		s[0] = (BYTE)(Outlen / 0x1000000) % 0x100; file2.Write( s , 1);
		s[0] = (BYTE)(Outlen / 0x10000  ) % 0x100; file2.Write( s , 1);
		s[0] = (BYTE)(Outlen / 0x100    ) % 0x100; file2.Write( s , 1);
		s[0] = (BYTE)(Outlen / 0x1      ) % 0x100; file2.Write( s , 1);

		//���k�J�n
		//���k�͌��޲Ă��� -1 -2 -4 ��3��Ŕ�r���邽��4�޲ĕ��̃f�[�^������
		s[0] = s[1] = s[2] = s[3] = s[4] = 0; //0�ŏ�����

		cnt_ = 0;

		for(;;)
		{
			//���k���[�v
			if(Read(s,1) == 0) break;
			if(s[0] == s[1])//�OByte�Ɠ�����?
			{
				nFlg1 = 1;
				nFlg2 = 1;
			}
			else if(s[0] == s[2])
			{
				nFlg1 = 0;
				nFlg2 = 1;
			}
			else if(s[0] == s[3])
			{
				nFlg1 = 0;
				nFlg2 = 2;
			}
			else if(s[0] == s[4])
			{
				nFlg1 = 0;
				nFlg2 = 3;
			}
			else
			{
				nFlg1 = 0;
				nFlg2 = 0;
			}
			nfd |= (nFlg1 << f[nf]);
			nf++;
			if(nf == 8)
			{
				nf = 0;
				ss[0] = nfd;
				file2.Write(ss,1);
				nfd = 0;
			}
			if(nFlg1 == 0)
			{
				nf2d |= (nFlg2 << f2[nf2]);
				nf2 ++;
			}
			if(nf2 == 4)
			{
				nf2 = 0;
				ss[0] = nf2d;
				file3.Write(ss,1);
				nf2d = 0;
			}
			if(nFlg2 == 0)
			{
				file4.Write(s,1);
			}
			nFlg1 = nFlg2 = 0;
			s[4] = s[3];
			s[3] = s[2];
			s[2] = s[1];
			s[1] = s[0];
		}
		if(nf != 0)
		{
			nf = 0;
			ss[0] = nfd;
			file2.Write(ss,1);
			nfd = 0;
		}
		if(nf2 != 0)
		{
			nf2 = 0;
			ss[0] = nf2d;
			file3.Write(ss,1);
			nf2d = 0;
		}
		//�o���オ�����t�@�C��������
		file3.Close();
		file4.Close();
		file3.Open(fname2a ,CFile::shareDenyNone | CFile::modeRead | CFile::typeBinary,NULL);
		file4.Open(fname2b ,CFile::shareDenyNone | CFile::modeRead | CFile::typeBinary,NULL);
		Outlen = file3.GetLength();
		file2.Seek(8,CFile::begin );
		s[0] = (BYTE)(Outlen / 0x1000000) % 0x100; file2.Write( s , 1);
		s[0] = (BYTE)(Outlen / 0x10000  ) % 0x100; file2.Write( s , 1);
		s[0] = (BYTE)(Outlen / 0x100    ) % 0x100; file2.Write( s , 1);
		s[0] = (BYTE)(Outlen / 0x1      ) % 0x100; file2.Write( s , 1);
		file2.Seek(0,CFile::end );

		for(;;)
		{
			cnt_ = file3.Read(s_,10000);
			file2.Write(s_,cnt_);
			if(cnt_ != 10000) break;
		}
		for(;;)
		{
			cnt_ = file4.Read(s_,10000);
			file2.Write(s_,cnt_);
			if(cnt_ != 10000) break;
		}
		file3.Close();
		file4.Close();
		CFile::Remove(fname2a);
		CFile::Remove(fname2b);
		return TRUE;
	}
	return FALSE;
}

UINT compress::Read(BYTE s[],int cnt)
{
	if(cnt_ == 0)
	{
		cnt_ = file.Read(s_,10000);
		if(cnt_ == 0) return 0;
		cnt2 = 0;
	}
	cnt_ --;
	s[0] = s_[cnt2];
	cnt2 ++;
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

BOOL compress::run3(CString fname)
{
	CString fname2;
	if(file.Open(fname ,CFile::shareDenyNone | CFile::modeRead | CFile::typeBinary,NULL)==TRUE)
	{
		fname2 = fname + ".af3"; //�ϊ���t�@�C��
		if(file2.Open(fname2 ,CFile::shareDenyNone | CFile::modeCreate | CFile::modeWrite | CFile::typeBinary,NULL)==FALSE) return FALSE;
		for(;;)
		{
			int l = file.Read(buf,60000); //�f�|�^�̓ǂݍ���
			if(l == 0) break;
			int r = enHuffman(buf2,60000,buf,l);
			if(r == -1L) 
			{
				//file2.Close();
				//file.Close();
				return FALSE;
			}
			buf[0] = (r / 256);
			buf[1] = (r % 256);
			file2.Write(buf,2);
			file2.Write(buf2,r);
		}
		//file2.Close();
		//file.Close();
		return TRUE;
	}
	return FALSE;
}

long compress::enHuffman(unsigned char *code, long clen,
               unsigned char *text, long tlen)
{
    int  i, k;
    long c, len;
    int  min1, min2;
    long max;
    int  root;
    NODE node[NODE_SIZE]; int  freeNode;
    char bit[NODE_SIZE];  char *bitp;
    static unsigned char bit0[8] = {~128,~64,~32,~16,~8,~4,~2,~1};
    static unsigned char bit1[8] = { 128, 64, 32, 16, 8, 4, 2, 1};

    if (tlen <= 0 || clen <= 2*CHAR_SIZE) return -1L;

    /* �e�����̏o���p�x���v�Z */
    for (i = 0; i < NODE_SIZE; i++) node[i].count = 0;
    for (c = 0; c < tlen; c++) node[text[c]].count++;

    /* �ő�o���p�x�̒l���Q�o�C�g���z���Ȃ��悤�ɒ������� */
    for (i = 0, max = 0; i < CHAR_SIZE; i++)
        if (node[i].count > max) max = node[i].count;
    if ((k = max/0x10000L) > 0)
        for (i = 0, k++; i < CHAR_SIZE; i++)
            if (node[i].count > 0) {
                node[i].count /= k;
                if (node[i].count == 0) node[i].count = 1;
            }

    /* �e�����̏o���p�x���o�� */
    for (i = 0; i < CHAR_SIZE; i++) {
        *code++ = (node[i].count >> 8) & 0xff;
        *code++ = node[i].count & 0xff;
    }
    
    /* EOF */
    node[CHAR_SIZE].count = 1;

    /* �n�t�}���؂����� */
    node[NODE_SIZE - 1].count = 0x10000L;          /* �ԕ� */
    for (freeNode = CHAR_SIZE+1; ; freeNode++) {
        min1 = min2 = NODE_SIZE - 1;
        for (i = NODE_SIZE - 2; i >= 0; i--)
            if (node[i].count > 0) {
                if (node[i].count < node[min1].count) {
                    min2 = min1;
                    min1 = i;
                } else if (node[i].count < node[min2].count)
                    min2 = i;
            }

        if (min2 == NODE_SIZE - 1) break;
        node[freeNode].left  = min1;
        node[freeNode].right = min2;
        node[freeNode].count = node[min1].count + node[min2].count;
        node[min1].parent = node[min2].parent = freeNode;
        node[min1].count = node[min2].count = 0;
    }
    root = min1;

    /* �e�����𕄍��� */
    for (c = 0, i = 0, len = 2*CHAR_SIZE; ; c++) {
        k = (c == tlen) ? CHAR_SIZE : text[c];
        
        /* ������𓾂� */
        bitp = bit;
        do {
            int parent = node[k].parent;
            *bitp++ = (node[parent].left == k) ? 0 : 1;
            k = parent;
        } while (k != root);
        
        /* �t���ɏo�� */
        do {
            if (*--bitp) *code |= bit1[i];
            else         *code &= bit0[i];
            if (++i == 8) {
                code++;
                if (++len > clen) return -1L;
                i = 0;
            }
        } while (bitp != bit);
        if (c == tlen) break;
    }

    if (i > 0) len++;
    return (len > clen) ? -1L : len;
}