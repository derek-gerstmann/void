// ============================================================================================== //
//
// License:   The Lesser GNU Public License (LGPL) v3.0.
// 
// Author(s):   Derek Gerstmann. The University of Western Australia (UWA). 
//        As well as the shoulders of many giants...
//
// This file is part of the Void framework.
//
// The Void framework is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// The Void framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the Void framework.  If not, see <http://www.gnu.org/licenses/>.
//
// ============================================================================================== //

#ifndef VD_CORE_STATISTICS_INCLUDED
#define VD_CORE_STATISTICS_INCLUDED

#include "core/core.h"
#include "core/locking.h"
#include "core/numerics.h"

#include <string>
#include <cstdio>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class Statistic
{
#ifndef VD_USE_STATISTICS
public:
  Statistic(void) { }
  Statistic(const vd::string& name, const vd::string& units) {}
  inline void SetName(const vd::string& str) { m_Name = str; }
  inline void SetUnits(const vd::string& str) { m_Units = str; }
  inline void Reset(void) { }
  inline void Add(vd::real) { }
  inline void Add(int) { }
  inline vd::real GetTotal(void) { return -1.0f; }
  inline vd::real GetAverage(void) { return -1.0f; }
  inline vd::real GetVariance(void) { return -1.0f; }
  inline vd::real GetDeviation(void) { return -1.0f; }
  inline vd::real GetMinimum(void) { return -1.0f; }
  inline vd::real GetMaximum(void) { return -1.0f; }
#else
public:

  Statistic(void) { Reset(); }
  Statistic(const vd::string& name, const vd::string& units) 
    : m_Name(name), m_Units(units) { Reset(); }
  
  inline void SetName(const vd::string& str) { m_Name = str; }
  inline void SetUnits(const vd::string& str) { m_Units = str; }
  inline const vd::string& GetName() { return m_Name; }
  inline const vd::string& GetUnits() { return m_Units; }
  inline void Reset(void)
  {
    m_Mutex.Lock();

    m_TotalSum = m_SumSqr = m_RecordCount = 0.0f;
    m_Minimum = VD_F32_MAX;
    m_Maximum = -VD_F32_MAX;

    m_Mutex.Unlock();
  }

  inline void Add(vd::real value=1.0f)
  {
    m_Mutex.Lock();

    m_TotalSum += value;
    m_SumSqr += (value * value);
    m_RecordCount += 1.0f;

    if (value < m_Minimum)
      m_Minimum = value;

    if (value > m_Maximum)
      m_Maximum = value;

    m_Mutex.Unlock();
  }

  inline void Add(vd::i32 value)
  {
    Add(static_cast<vd::real>(value));
  }

  inline vd::real GetTotal(void)
  {
    return m_TotalSum;
  }

  inline vd::real GetAverage(void)
  {
    return m_TotalSum/m_RecordCount;
  }

  inline vd::real GetVariance(void)
  {
    vd::real inverse = 1.0f / m_RecordCount;
    vd::real first = m_SumSqr * inverse;
    vd::real second = m_TotalSum * inverse;
    return first - second * second;
  }

  inline vd::real GetDeviation(void)
  {
    return Sqrt(GetVariance());
  }

  inline vd::real GetMinimum(void)
  {
    return m_Minimum;
  }

  inline vd::real GetMaximum(void)
  {
    return m_Maximum;
  }

  vd::real m_TotalSum;
  vd::real m_SumSqr;
  vd::real m_Minimum;
  vd::real m_Maximum;
  vd::real m_RecordCount;

  vd::string m_Name;
  vd::string m_Units;

private:
  Mutex m_Mutex;
#endif
};

// ============================================================================================== //
  
VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_STATISTICS_INCLUDED
