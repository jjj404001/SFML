#ifndef BITMASK_H
#define BITMASK_H

#include <stdint.h>

// a << 3	moves bits to the left by 3 
// a >> 3	moves bits to the right by 3
// ~a			switches 1 to 0, 0 to 1
// a | b		0100 | 1011 = 1111
// a ^ b		0100 ^ 1001 = 1101
// a & b		1100 & 1001 = 1000


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

	//Set a specific bit to 1
	void  TurnOnBit(const unsigned int & pos)
	{
		m_bits |= 1 << pos;
	}

	//Merge two bitsets
	void  TurnOnBit(const Bitset & bits)
	{
		m_bits |= bits;
	}

	//Set a specific bit to 0
	void ClearBit(const unsigned int & pos)
	{
		m_bits &= ~(1 << pos);
	}

	//Toggle a specific bit
	void ToggleBit(const unsigned int & pos)
	{
		m_bits ^= 1 << pos;
	}

	//Set all the bits to 0
	void Clear() { m_bits = 0; }

private:
	Bitset m_bits;
};

#endif