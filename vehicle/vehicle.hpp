/**
 * \file vehicle.hpp
 * \brief 車両の機能を提供する。
 * \author Takuya Itoh
 */

#ifndef ATS_VEHICLE_HPP_INCLUDED
#define ATS_VEHICLE_HPP_INCLUDED

#include "ats_define.hpp"
#include "wiring.hpp"

namespace ats
{

/// 車両の機能の定義クラス<br>このクラスは singleton です。
class vehicle
{
public:
	/// インスタンスを取得する。
	/// \return インスタンス
	static vehicle* get_instance();

	/// インスタンスを解放する。
	static void terminate();

	/// 初期化する。
	/// \param[in] param 初期化パラメータ
	void initialize(
					int param
					);

	/// 警笛を吹鳴した。
	/// \param[in] ats_horn 吹鳴された警笛インデックス
	void blow_horn(
				   int ats_horn
				   );

	/// 信号現示が変化した。
	/// \param[in] signal 現示
	void changed_signal(
						int signal
						);

	/// 地上子を通過した。
	/// \param[in] beacon_data 地上子の情報
	void received_beacon_data(
							  const ATS_BEACONDATA& beacon_data
							  );

	/// 処理を実行する。
	/// \param[out] p_panel 運転台の出力
	/// \param[out] p_sound 音の出力
	/// \return コントローラ (マスコン, ブレーキ, 逆転器) の状態
	ATS_HANDLES execute(
						int *const p_panel,
						int *const p_sound
						);

protected:
	/// ctor
	vehicle();

	/// dtor
	virtual ~vehicle();

private:
	vehicle(const vehicle& c);
	vehicle& operator=(const vehicle& c);

	/// 処理された結果をBVEへ反映する。
	/// \param[out] p_panel 運転台の出力
	/// \param[out] p_sound 音の出力
	/// \return コントローラ (マスコン, ブレーキ, 逆転器) の状態
	ATS_HANDLES update(
					   int *const p_panel,
					   int *const p_sound
					   );

	wiring m_wires;					///< ワイヤリング

	static vehicle* m_p_instance;	///< インスタンス

};	// vehicle

}	// ats

#endif	// ATS_VEHICLE_HPP_INCLUDED
