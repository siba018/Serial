#include "environment.hpp"
#include "utility.hpp"

#include <string.h>
#include <math.h>

namespace ats
{

environment* environment::m_p_instance = NULL;

/// ctor
environment::environment()
{
	memset(&m_current, 0, sizeof(ST_ENVIRONMENT_DATA));
	memset(&m_last, 0, sizeof(ST_ENVIRONMENT_DATA));
    memset(m_module_dir, 0, sizeof(char) * ENVIRONMENT_MAX_MODULE_DIR_LENGTH);
}

/// dtor
environment::~environment()
{

}

/// インスタンスを取得する。
environment* environment::get_instance()
{
	if (!m_p_instance)
	{
		m_p_instance = new environment;
	}

	return m_p_instance;
}

/// インスタンスを解放する。
void environment::terminate()
{
	SDELETE(m_p_instance);
}

/// 状態を初期化する。
void environment::initialize_status()
{
    memset(&m_current.vehicle_status, 0, sizeof(ATS_VEHICLESTATE));
    memset(&m_last.vehicle_status, 0, sizeof(ATS_VEHICLESTATE));

    m_current.vehicle_status.is_closed_doors = false;
    m_current.vehicle_status.abs_speed = 0.0f;

    m_last.vehicle_status.is_closed_doors = false;
    m_last.vehicle_status.abs_speed = 0.0f;
}

/// モジュールのディレクトリを設定する。
/// \param[in] module_dir このプラグインのコンピュータ上の絶対パス
void environment::set_module_dir(
                                 const char* module_dir
                                 )
{
    strncpy(m_module_dir, module_dir, ENVIRONMENT_MAX_MODULE_DIR_LENGTH);
}

/// 現在の車両の諸元を設定する。
/// \param[in] s 車両の諸元
void environment::set_current_specification(
											const ATS_VEHICLESPEC& s
											)
{
	memcpy(&m_current.vehicle_specification, &s, sizeof(ATS_VEHICLESPEC));

	m_current.vehicle_specification.emergency_brake_notch = m_current.vehicle_specification.num_of_brake_notch + 1;
	m_current.vehicle_specification.maximum_service_brake_notch = m_current.vehicle_specification.num_of_brake_notch;
}

/// 現在の車両の状態を入力する。
/// \param[in] s 車両の状態
void environment::set_current_status(
									 const ATS_VEHICLESTATE& s
									 )
{
	memcpy(&m_current.vehicle_status, &s, sizeof(ATS_VEHICLESTATE));
    m_current.vehicle_status.abs_speed = fabs(m_current.vehicle_status.speed);

    if (!m_last.vehicle_status.time)
    {
        m_last.vehicle_status.time = m_current.vehicle_status.time;
    }
}

/// 現在の力行ノッチの状態を設定する。
/// \param[in] notch 力行ノッチ位置
void environment::set_power_notch(
								  int notch
								  )
{
	m_current.vehicle_status.handles.Power = notch;
}

/// 現在のブレーキノッチの状態を設定する。
/// \param[in] notch ブレーキノッチ位置
void environment::set_brake_notch(
								  int notch
								  )
{
	m_current.vehicle_status.handles.Brake = notch;
}

/// 現在のレバーサーの状態を設定する。
/// \param[in] pos レバーサーの位置
void environment::set_reverser_position(
										int pos
										)
{
	m_current.vehicle_status.handles.Reverser = pos;
}

/// 現在のドアの状態を設定する。
/// \param[in] is_closed_doors ドアが閉まっているか？<br>true:ドアが閉まっている。 false:ドアが開いている。
void environment::set_door_status(
								  bool is_closed_doors
								  )
{
	m_current.vehicle_status.is_closed_doors = is_closed_doors;
}

/// 現在の信号現示を設定する。
/// \param[in] signal 信号現示
void environment::set_signal(
							 int signal
							 )
{
	m_current.signal = signal;
}

/// 現在のフレームの環境情報を取得する。
/// \return 環境情報
const ST_ENVIRONMENT_DATA& environment::get_current()
{
	return m_current;
}

/// 1フレーム前の環境情報を取得する。
/// \return 環境情報
const ST_ENVIRONMENT_DATA& environment::get_last()
{
	return m_last;
}

/// モジュールのディレクトリを取得する。
/// \return このプラグインのコンピュータ上の絶対パス
const char* environment::get_module_dir()
{
    return m_module_dir;
}

/// 情報を更新する。
void environment::update()
{
	m_last = m_current;
}

/// ATSプラグインで使用するキーが押された状態を設定する。
/// \param[in] ats_key_code ATSプラグインで使用するキーのインデックス
void environment::set_key_down(
							   int ats_key_code
							   )
{
	m_current.key_status |= (1 << ats_key_code);
}

/// ATSプラグインで使用するキーが押されていて、それが離された状態を設定する。
/// \param[in] ats_key_code ATSプラグインで使用するキーのインデックス
void environment::set_key_up(
							 int ats_key_code
							 )
{
	m_current.key_status ^= (1 << ats_key_code);
}

/// ATSプラグインで使用するキーが押されたかどうかを取得する。
/// \param[in] ats_key_code ATSプラグインで使用するキーのインデックス
/// \return true:押されている。 false:押されていない。
bool environment::is_trigger_key(
								 int ats_key_code
								 )
{
	return ((m_current.key_status & (1 << ats_key_code))) && (!(m_last.key_status & (1 << ats_key_code)));
}

/// ATSプラグインで使用するキーが押されていて、それが離されたかどうかを取得する。
/// \param[in] ats_key_code ATSプラグインで使用するキーのインデックス
/// \return true:離されている。 false:離されていない。
bool environment::is_release_key(
								 int ats_key_code
								 )
{
	return (!(m_current.key_status & (1 << ats_key_code))) && ((m_last.key_status & (1 << ats_key_code)));
}

/// ATSプラグインで使用するキーが押し続けられているかどうかを取得する。
/// \param[in] ats_key_code ATSプラグインで使用するキーのインデックス
/// \return true:押し続けられている。 false:押し続けられていない。
bool environment::is_press_key(
							   int ats_key_code
							   )
{
	return ((m_current.key_status & (1 << ats_key_code)) > 0);
}

/// 現在の刻み時間を取得する。 (dt, msec)
int environment::delta_t()
{
    //return (m_current.vehicle_status.time - m_last.vehicle_status.time) > 33 ? 33 : (m_current.vehicle_status.time - m_last.vehicle_status.time);
    return (m_current.vehicle_status.time - m_last.vehicle_status.time);
}

}	// ats
