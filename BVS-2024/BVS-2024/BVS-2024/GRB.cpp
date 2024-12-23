#define _CRT_SECURE_NO_WARNINGS
#include "GRB.h"
#include <iostream>

typedef short GRBALPHABET;

#define GRB_ERROR_SERIES 600

using namespace std;

namespace GRB {
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)	

	/*
	
		S - ������ ������ ���������
		F - ��������� �������
		C - ��������� ������� ��� ���������� ��� ���������� ����������
		R - ����������� �������� �������
		N - ���� ������� ��� ����������� �����
		O - ��������� ����������� ������� display
		E - ��� ��������� ��������� ��� ������������ ���������� ������-���� ��������
		P - ��������� �������
		K - ��������������� ��������� ��� ���������� ���������� ����������
		W - �� ��� ����� ���� � ������� ����� if
		T - ��������������� ���� ��� ���� ��� ����� ���� ����� ������ if

	*/

	Greibach greibach(NS('S'), TS('$'),
		11,
		Rule(NS('S'), GRB_ERROR_SERIES + 0, // ������� � main
			4,
			Rule::Chain(),
			Rule::Chain(12, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), NS('R'), TS('}'), NS('S')),
			Rule::Chain(11, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}'))
		),
		Rule(NS('F'), GRB_ERROR_SERIES + 1, // �������� �������
			3,
			Rule::Chain(3, TS('t'), TS('i'), NS('C')),
			Rule::Chain(4, TS(','), TS('t'), TS('i'), NS('C')),
			Rule::Chain()
		),
		Rule(NS('C'), GRB_ERROR_SERIES + 1, // �������� ������� (�������� ��������� �� ��� ���-��)
			2,
			Rule::Chain(4, TS(','), TS('t'), TS('i'), NS('F')),
			Rule::Chain()
		),
		Rule(NS('R'), GRB_ERROR_SERIES + 2, // return
			3,
			Rule::Chain(),
			Rule::Chain(3, TS('r'), TS('i'), TS(';')),
			Rule::Chain(3, TS('r'), TS('l'), TS(';'))
		),
		Rule(NS('N'), GRB_ERROR_SERIES + 3, // ����
			7,
			Rule::Chain(),
			Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS(':'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS(':'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(6, TS('p'), TS('('), NS('O'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(6, TS('i'), TS('('), NS('P'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(8, TS('y'), TS('('), NS('W'), TS(')'), TS('{'), NS('N'), TS('}'), NS('N'))
		),
		Rule(NS('E'), GRB_ERROR_SERIES + 4, //������������ ���������� ������-���� ��������
			3,
			Rule::Chain(4, TS('i'), TS('('), NS('P'), TS(')')),
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l'))
		),
		Rule(NS('O'), GRB_ERROR_SERIES + 5, // �� ��� ��������� � display
			3,
			Rule::Chain(),
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l'))
		),
		Rule(NS('P'), GRB_ERROR_SERIES + 6, //��������� �������
			5,
			Rule::Chain(),
			Rule::Chain(2, TS('i'), NS('K')),
			Rule::Chain(2, TS('l'), NS('K')),
			Rule::Chain(3, TS(','), TS('i'), NS('K')),
			Rule::Chain(3, TS(','), TS('l'), NS('K'))
		),
		Rule(NS('K'), GRB_ERROR_SERIES + 6, //���������� ���������� ����������
			3,
			Rule::Chain(),
			Rule::Chain(3, TS(','), TS('i'), NS('P')),
			Rule::Chain(3, TS(','), TS('l'), NS('P'))
		),
		Rule(NS('W'), GRB_ERROR_SERIES + 7, //������ ����� if
			3,
			Rule::Chain(5, TS('i'), TS('('), NS('P'), TS(')'), NS('T')),
			Rule::Chain(2, TS('i'), NS('T')),
			Rule::Chain(2, TS('l'), NS('T'))
		),
		Rule(NS('T'), GRB_ERROR_SERIES + 7, //��������������� ���� ��� ���� ��� ����� ���� ����� ������ if
			4,
			Rule::Chain(),
			Rule::Chain(5, TS('k'), TS('i'), TS('('), NS('P'), TS(')')),
			Rule::Chain(2, TS('k'), TS('i')),
			Rule::Chain(2, TS('k'), TS('l'))
		)
		);



	Greibach getGreibach() { return greibach; }

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
	{
		nt = new GRBALPHABET[size = psize];
		int* p = (int*)&s;					
		for (short i = 0; i < size; i++)
			nt[i] = (GRBALPHABET)p[i];			
	}

	Rule::Rule(GRBALPHABET pnn, int iderror, short psize, Chain c, ...)
	{
		nn = pnn;
		idError = iderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (short i = 0; i < size; i++)
			chains[i] = p[i];
	}

	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottomT, short psize, Rule r, ...)
	{
		startN = pstartN;
		stbottomT = pstbottomT;
		rules = new Rule[size = psize];
		Rule* p = &r;
		for (short i = 0; i < size; i++)
			rules[i] = p[i];
	}

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)
	{
		short rc = -1;
		short k = 0;
		while (k < this->size && this->rules[k].nn != pnn)
			k++;
		if (k < this->size)
			prule = this->rules[rc = k];
		return rc;
	}

	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < this->size)
			rc = this->rules[n];
		return rc;
	}

	char* Rule::getCRule(char* b, short nchain)
	{
		char bchain[200];		
		b[0] = Chain::alphabet_to_char(this->nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0x00;
		chains[nchain].getCChain(bchain);		
		strcat_s(b, sizeof(bchain) + 5, bchain);	//��������� ������������� ������
		return b;
	}

	short Rule::getNextChain(GRBALPHABET t, Chain& pchain, short j)
	{
		if (size == 0) {
			// ���������� ����������� ���, ������������ ������ �������
			return -2; // -2 ��� ��� ��� ������ �������
		}

		short rc = -1;

		// ���� Rule::Chain() �� ������, ���������� ���������
		while (j < size) {
			if (chains[j].nt[0] == t || chains[j].nt[0] == 0) {
				// ����� ���������� �������
				rc = j;
				break;
			}
			j++;
		}

		if (rc >= 0) {
			// ���� ����� �������, ����������� �
			pchain = chains[rc];
		}

		return rc;
	}



	char* Rule::Chain::getCChain(char* b)
	{
		for (int i = 0; i < size; i++)
			b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	}
}