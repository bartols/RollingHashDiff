#include "rolling_checksum.h"

#ifdef _DEBUG
//#define DEBUG_VERBOSE
#endif

#ifdef DEBUG_VERBOSE
#include <iostream>
#include <string>
#endif

using namespace rsyn;

#if defined(ADLER32_ALGO)

static const int32_t Base = 65521;

int64_t RollingChecksum::calculate(const std::vector<byte>& data)
{
    _sum1 = 1;
    _sum2 = 0;
    for (const auto ch : data)
    {
        _sum1 = (_sum1 + ch) % Base;
        _sum2 = (_sum2 + _sum1) % Base;
    }

    _block_size = data.size();

#ifdef DEBUG_VERBOSE
    std::string str(data.begin(), data.end());
    std::cout << "Checksum for: [" << str << "] = " << ((_sum2 << 16) | _sum1) << std::endl;
#endif

    return (_sum2 << 16) | _sum1;
}

int64_t RollingChecksum::roll(byte remove, byte add)
{
    _sum1 += add - remove;
    if (_sum1 >= Base)
    {
        _sum1 -= Base;
    }
    else if (_sum1 < 0)
    {
        _sum1 += Base;
    }

    _sum2 = (_sum2 - _block_size * remove + _sum1 - 1) % Base;
    if (_sum2 < 0)
    {
        _sum2 += Base;
    }

#ifdef DEBUG_VERBOSE
    std::cout << "Rolling remove [" << remove << "] add [" << add << "] = " << ((_sum2 << 16) | _sum1) << std::endl;
#endif

    return (_sum2 << 16) | _sum1;
}

#else

constexpr auto MODULO_FACTOR = 1 << 16;

const unsigned PRIME_BASE = 257;
const unsigned PRIME_MOD = 1000000007;

int64_t RollingChecksum::calculate(const std::vector<byte>& data)
{
    m_r1 = 0;
    m_r2 = 0;
    m_block_size = data.size();

    for (int i = 0; i < data.size(); ++i)
    {
        m_r1 += data[i];
        m_r2 += (m_block_size - i) * data[i];
    }

    m_r1 = m_r1 % MODULO_FACTOR;
    m_r2 = m_r2 % MODULO_FACTOR;

#ifdef DEBUG_VERBOSE
    std::string str(data.begin(), data.end());
    std::cout << "Checksum for: [" << str << "] = " << m_r1 + MODULO_FACTOR * m_r2 << std::endl;
#endif

    return m_r1 + MODULO_FACTOR * m_r2;
}

int64_t RollingChecksum::roll(byte remove, byte add)
{
    m_r1 = (m_r1 - remove + add) % MODULO_FACTOR;
    m_r2 = (m_r2 - m_block_size * remove + m_r1) % MODULO_FACTOR;

#ifdef DEBUG_VERBOSE
    std::cout << "Rolling remove [" << remove << "] add [" << add << "] = " << m_r1 + MODULO_FACTOR * m_r2 << std::endl;
#endif

    return m_r1 + MODULO_FACTOR * m_r2;
}

#endif