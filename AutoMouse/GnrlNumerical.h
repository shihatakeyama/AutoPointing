/***********************************************************************************
*   �i�[�e��̂̒�`���s��
*   GnrlNumerical.h
*
*
***********************************************************************************/


#ifndef GNRLNUMERICAL_H
#define GNRLNUMERICAL_H

#include <assert.h>


// *********************************************************************************
// �^���v���g�^�C�v
// *********************************************************************************
typedef	char			Sint8;
typedef	unsigned char	Uint8;
typedef	bool			Bool8;		// ���@�ɔp�~�\��
typedef	bool			Bool;
typedef	int				Bool32;
typedef	void*			pVoid;		// ���@�ɔp�~�\��
typedef	void*			Pvoid;		// 32bit-64bit �|�C���^�݊��̈�
typedef short			Sint16;
typedef unsigned short	Uint16;
typedef	int				Sint32;
typedef unsigned int	Uint32;
typedef __int64			Sint64;
typedef unsigned __int64 Uint64;
typedef float			Float32;
typedef double			Float64;

#ifndef TRUE
#define TRUE	0x01
#define FALSE	0x00
#endif

#ifndef NULL
#define NULL	0
#endif


// *********************************************************************************
// �e��}�N��
// *********************************************************************************
#if 1
// �召����}�N�� ��VS��<stdio.h>���C���N���[�h������ɒ�`����ƃG���[�ɂȂ�B
template <typename X> X MAX(X arg1, X arg2)	{ return arg1>arg2 ? arg1 : arg2; }
template <typename X> X MIN(X arg1, X arg2)	{ return arg1<arg2 ? arg1 : arg2; }
// �����̊���Z (���̒l�̂ݑΉ����Ă��܂��B)
template <typename X> X Division45(X Elements, X Denominator)
{
#ifdef _DEBUG
	assert((Elements >= 0) && (Denominator >= 0) && "The price of the negative was set as an argument.");
#endif

	return (Elements + (Denominator / 2)) / Denominator; 
}

#endif


// *********************************************************************************
//  �z��/���� �ϊ�
// *********************************************************************************
Uint16 Uint8ArrowToUint16(const Uint8 * data);
Uint32 Uint8ArrowToUint32(const Uint8 * data);
void Uint16ToUint8Arrow(Uint8 * Result ,Uint16 Src);


Uint8 asciiToInt(Sint8 c);
Sint32 intArrowToDump16(Sint8 *OutAscii ,const Uint8 *Src ,Sint32 Len);


// *********************************************************************************
// �z��̗v�f2���l���i�[�������ꍇ�B
// *********************************************************************************
template<class X>
struct DoubleNumerical{

	DoubleNumerical()
	{
		m_Float[0] = 0;
		m_Float[1] = 0;
	}

	DoubleNumerical(const DoubleNumerical &Arg)
	{
		m_Float[0] = Arg.m_Float[0];
		m_Float[1] = Arg.m_Float[1];
	}

	DoubleNumerical(X Arg0 ,X Arg1)
	{
		m_Float[0] = Arg0;
		m_Float[1] = Arg1;
	}
	DoubleNumerical(const X *Arg)
	{
		m_Float[0] = Arg[0];
		m_Float[1] = Arg[1];
	}

	const DoubleNumerical &operator = (const DoubleNumerical &Arg)
	{
		m_Float[0] = Arg.m_Float[0];
		m_Float[1] = Arg.m_Float[1];

		return *this;
	}

	DoubleNumerical operator + (const DoubleNumerical &Arg)
	{
		return DoubleNumerical(	m_Float[0] + Arg.m_Float[0] ,m_Float[1] + Arg.m_Float[1]);
	}/*
	DoubleNumerical operator - (const DoubleNumerical &Arg)
	{
		return DoubleNumerical(	m_Float[0] - Arg.m_Float[0] ,m_Float[1] - Arg.m_Float[1]);
	}	*/
	DoubleNumerical operator / (Sint32 Arg)
	{
		return DoubleNumerical(	m_Float[0] / Arg ,m_Float[1] / Arg);
	}


	const DoubleNumerical &operator += (const DoubleNumerical &Arg)
	{
		m_Float[0] += Arg.m_Float[0];
		m_Float[1] += Arg.m_Float[1];

		return *this;
	}
	const DoubleNumerical &operator -= (const DoubleNumerical &Arg)
	{
		m_Float[0] -= Arg.m_Float[0];
		m_Float[1] -= Arg.m_Float[1];

		return *this;
	}

	// ���
	X &operator [] (Sint32 Arg){
		assert(Arg >= 0  && Arg < 2);
		return m_Float[Arg];
	}

	// �Q��
	const X &operator [] (Sint32 Arg) const{
		assert(Arg >= 0  && Arg < 2);
		return m_Float[Arg];
	}

	X deff() const{
		return m_Float[1] - m_Float[0];
	}
	X deffAbs() const{
		return ::fabs(m_Float[1] - m_Float[0]);
	}


	X m_Float[2];
};

/*
template<class X,class Y>
DoubleNumerical operator / (Y Arg ,const DoubleNumerical &Arg0)
{
	return DoubleNumerical(	m_Float[0] / Y ,m_Float[1] / Y);
}
*/

#endif // #ifndef GNRLNUMERICAL_H
