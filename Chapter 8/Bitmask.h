#ifndef BITMASK_H
#define BITMASK_H

#include <stdint.h>

using Bitset = uint32_t;
class Bitmask 
{
public:
	Bitmask() : m_bits(0) {}
	Bitmask(const Bitset & bits) : m_bits(bits) {}

	Bitset GetMask() const { return m_bits; }
	void SetMask(const Bitset & value) { m_bits = value; }

	bool Matches(const Bitmask & bits,
		const Bitset & relevant = 0) const
	{
		return (relevant ? 
			 ((bits.GetMask() & relevant) == (m_bits & relevant))
			: (bits.GetMask() == m_bits));
	}

	bool GetBit(const unsigned int & pos) const
	{
		return ((m_bits&(1 << pos)) != 0);
	}

	void  TurnOnBit(const unsigned int & pos)
	{
		m_bits |= 1 << pos;
	}

	void  TurnOnBit(const Bitset & bits)
	{
		m_bits |= bits;
	}

	void ClearBit(const unsigned int & pos)
	{
		m_bits &= ~(1 << pos);
	}

	void ToggleBit(const unsigned int & pos)
	{
		m_bits ^= 1 << pos;
	}

	void Clear() { m_bits = 0; }

private:
	Bitset m_bits;
};

#endif