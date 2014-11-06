/**
 * \file vehicle.cpp
 * \brief 車両の機能を提供する。
 * \author Takuya Itoh
 */

#include "vehicle.hpp"
#include "utility.hpp"
#include "system/environment.hpp"

namespace ats
{

vehicle* vehicle::m_p_instance = NULL;			///< インスタンス

/// ctor
vehicle::vehicle()
{
	m_wires.register_wires();
}

/// dtor
vehicle::~vehicle()
{

}

/// インスタンスを取得する。
/// \return インスタンス
vehicle* vehicle::get_instance()
{
	if (!m_p_instance)
	{
		m_p_instance = new vehicle;
	}

	return m_p_instance;
}

/// インスタンスを解放する。
void vehicle::terminate()
{
	SDELETE(m_p_instance);
}

/// 初期化する。
/// \param[in] param 初期化パラメータ
void vehicle::initialize(
						 int param
						 )
{
	m_wires.initialize(param);
}

/// 警笛を吹鳴した。
/// \param[in] ats_horn 吹鳴された警笛インデックス
void vehicle::blow_horn(
						int ats_horn
						)
{

}

/// 信号現示が変化した。
/// \param[in] signal 現示
void vehicle::changed_signal(
							 int signal
							 )
{

}

/// 地上子を通過した。
/// \param[in] beacon_data 地上子の情報
void vehicle::received_beacon_data(
								   const ATS_BEACONDATA& beacon_data
								  )
{

}

/// 処理を実行する。
/// \param[out] p_panel 運転台の出力
/// \param[out] p_sound 音の出力
/// \return コントローラ (マスコン, ブレーキ, 逆転器) の状態
/// \todo この関数内に主な実装を書いてください。
ATS_HANDLES vehicle::execute(
							 int *const p_panel,
							 int *const p_sound
							 )
{
	m_wires.initialize(-1);


    m_wires().outputs["power"]._s32 = environment::get_instance()->get_current().vehicle_status.handles.Power;
    m_wires().outputs["brake"]._s32 = environment::get_instance()->get_current().vehicle_status.handles.Brake;
    m_wires().outputs["const_speed"]._s32 = environment::get_instance()->get_current().vehicle_status.handles.ConstantSpeed;
    m_wires().outputs["reverser"]._s32 = environment::get_instance()->get_current().vehicle_status.handles.Reverser;

	return update(p_panel, p_sound);
}

/// 処理された結果をBVEへ反映する。
/// \param[out] p_panel 運転台の出力
/// \param[out] p_sound 音の出力
/// \return コントローラ (マスコン, ブレーキ, 逆転器) の状態
ATS_HANDLES vehicle::update(
							int *const p_panel,
							int *const p_sound
							)
{
	ATS_HANDLES ret;

	/* コントローラ出力 */
	{
		ret.Power = m_wires().outputs["power"]._s32;
		ret.Brake = m_wires().outputs["brake"]._s32;
		ret.Reverser = m_wires().outputs["reverser"]._s32;
		ret.ConstantSpeed = m_wires().outputs["const_speed"]._s32;
	}
    
    /* パネル */
    {

    }

    /* サウンド */
    {

    }

	return ret;
}

}	// ats
