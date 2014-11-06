/**
 * \file wiring.hpp
 * \brief ワイヤリングの機能を提供する。
 * \author Takuya Itoh
 */

#ifndef ATS_WIRING_HPP_INCLUDED
#define ATS_WIRING_HPP_INCLUDED

#include "system/bus.hpp"
#include "ats_define.hpp"

namespace ats
{

/// ワイヤ情報
struct ST_WIRING_DATA
{
	bus indicators,			///< 表示灯
		sounds,				///< 音
		outputs;			///< BVEへ返す出力結果

	bus command,			///< 指令線
		power;				///< 電源
};

/// ワイヤリング
class wiring
{
public:
	/// ctor
	wiring()
	{

	}

	/// dtor
	virtual ~wiring()
	{

	}

	/// ワイヤを登録する。
	void register_wires()
	{
        /* 表示灯 */
        {
        	
        }

        /* 音 */
		{

		}

		/* BVEへ返す出力結果 */
		{
			m_wires.outputs.create("power");
			m_wires.outputs.create("brake");
			m_wires.outputs.create("reverser");
			m_wires.outputs.create("const_speed");
		}

		/* 指令線 */
		{
			m_wires.command.create("power");
			m_wires.command.create("brake");
			m_wires.command.create("reverser");
			m_wires.command.create("const_speed");
		}
	}

	/// ワイヤを初期化する。
	/// \param[in] param 初期化パラメータ
	void initialize(
					int param
					)
	{
        /* 表示灯 */
        {
        	
        }

        /* 音 */
		{

		}

		/* BVEへ返す出力結果 */
		{
			m_wires.outputs["power"]._s32 = 0;
			m_wires.outputs["brake"]._s32 = 0;
			m_wires.outputs["reverser"]._s32 = 0;
			m_wires.outputs["const_speed"]._s32 = ATS_CONSTANTSPEED_DISABLE;
		}

		/* 指令線 */
		{
			m_wires.command["power"]._s32 = 0;
			m_wires.command["brake"]._s32 = 0;
			m_wires.command["reverser"]._s32 = 0;
			m_wires.command["const_speed"]._s32 = ATS_CONSTANTSPEED_DISABLE;
		}
	}

	/// ワイヤ群を取得する。
	/// \return ワイヤ群
	ST_WIRING_DATA& operator()()
	{
		return m_wires;
	}

private:
	wiring(const wiring& c);
	wiring& operator=(const wiring& c);

	ST_WIRING_DATA m_wires;			///< ワイヤ群

};	// wiring

}	// ats

#endif	// ATS_WIRING_HPP_INCLUDED
