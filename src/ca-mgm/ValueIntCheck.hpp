/*---------------------------------------------------------------------\
|                                                                      |
|                     _     _   _   _     __     _                     |
|                    | |   | | | \_/ |   /  \   | |                    |
|                    | |   | | | |_| |  / /\ \  | |                    |
|                    | |__ | | | | | | / ____ \ | |__                  |
|                    |____||_| |_| |_|/ /    \ \|____|                 |
|                                                                      |
|                             core library                             |
|                                                                      |
|                                         (C) SUSE Linux Products GmbH |
\----------------------------------------------------------------------/

  File:       ValueIntCheck.hpp

  Author:     Marius Tomaschewski
  Maintainer: Marius Tomaschewski

  Purpose:

/-*/
/**
 * @file   ValueIntCheck.hpp
 * @brief  Implements an integer range check
 *
 * @todo
 * Any idea how to implement this as template class
 * like ValueRangeCheck<Int64>(min, max) inclusive
 * a proper differentiation of the string conversion
 * function... value.toUInt64() vs. value.toInt64() ?
 *
 */
#ifndef    CA_MGM_VALUE_INT_CHECK_HPP
#define    CA_MGM_VALUE_INT_CHECK_HPP

#include  <ca-mgm/config.h>
#include  <ca-mgm/CommonData.hpp>
#include  <ca-mgm/ValueCheck.hpp>
#include  <ca-mgm/String.hpp>


namespace CA_MGM_NAMESPACE
{

// -------------------------------------------------------------------
/**
 * @brief Integer range value check.
 *
 * The ValueIntCheck implements a simple integer range check
 * that can be used in ValueCheck.
 */
class ValueIntCheck: public ValueCheckBase
{
public:
	/**
	 * Constructor using a UInt64 range.
	 *
	 * @param minValue Minimal value for the range.
	 * @param maxValue Maximal value for the range.
	 * @param inclusiveRange Whether to check if the value is
	 * less (inclusiveRange=false) if less or equal the maxValue.
	 */
	ValueIntCheck(uint64_t minValue,
	              uint64_t maxValue,
	              bool inclusiveRange = true);

	/**
	 * Constructor using a Int64 range.
	 *
	 * @param minValue Minimal value for the range.
	 * @param maxValue Maximal value for the range.
	 * @param inclusiveRange Whether to check if the value is
	 * less (inclusiveRange=false) if less or equal the maxValue.
	 */
	ValueIntCheck(int64_t minValue,
	              int64_t maxValue,
	              bool inclusiveRange = true);

	/**
	 * Constructor using int range.
	 *
	 * Note: This constructor will be used by default,
	 * if you simply call:
	 *
	 *      ValueIntCheck(0, 42)
	 *
	 * To avoid conversion problems on 64 bit integers,
	 * you should choose the right constructors:
	 *
	 *      ValueIntCheck( Int64(0),  Int64(42))
	 *      ValueIntCheck(UInt64(0), UInt64(42))
	 *
	 * instead...
	 *
	 * @param minValue Minimal value for the range.
	 * @param maxValue Maximal value for the range.
	 * @param inclusiveRange Whether to check if the value is
	 * less (inclusiveRange=false) if less or equal the maxValue.
	 */
	ValueIntCheck(int            minValue,
	              int            maxValue,
	              bool inclusiveRange = true);

	/**
	 * Return whether the specified value is fits into
	 * the integer range.
	 *
	 * @param value A string value.
	 * @return true, if the value fits into the range.
	 * @throws std::stringConversionException if the
	 * value can't be converted to a integer value.
	 */
	virtual bool
	isValid(const std::string &value) const;

	/**
	 * Return a string showing the integer range check.
	 *
	 * @param value A string value.
	 * @return A string showing the check.
	 */
	virtual std::string
	explain(const std::string &value) const;

private:
	bool	m_sign;
	bool	m_incl;
	union {
		int64_t   s;
		uint64_t  u;
	}	m_min;
	union {
		int64_t   s;
		uint64_t  u;
	}	m_max;
};


}       // End of CA_MGM_NAMESPACE

#endif  // CA_MGM_VALUE_INT_CHECK_HPP
// vim: set ts=8 sts=8 sw=8 ai noet:
