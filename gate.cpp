#include "gate.h"

CGate::CGate(const char & gate_type,
			 const int & ypos,
			 const int & xpos,
			 const int & gate_ID) :

		     path(), m_gate_type(gate_type), m_ypos(ypos), m_xpos(xpos), m_gate_ID(gate_ID)
{
}


CGate & CGate::operator= (const CGate & src)
{
	m_gate_type = src.m_gate_type;
	m_xpos = src.m_xpos;
	m_ypos = src.m_ypos;
	m_gate_ID = src.m_gate_ID;
	path = src.path;

	return *this;
}

char CGate::GateType() const
{
	return m_gate_type;
}

int CGate::GateYpos() const
{
	return m_ypos;
}

int CGate::GateXpos() const
{
	return m_xpos;
}

int CGate::GateID() const
{
	return m_gate_ID;
}

void CGate::SetGateYpos(const int & ypos)
{
	m_ypos = ypos;
}

void CGate::SetGateXpos(const int & xpos)
{
	m_xpos = xpos;
}

void CGate::SetGateType(const char & type)
{
	m_gate_type = type;
}

