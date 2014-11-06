/**
 * \file environment.hpp
 * \brief 環境情報保持モジュールの定義。
 * \author Takuya Itoh
 */

#ifndef ATS_ENVIRONMENT_HPP_INCLUDED
#define ATS_ENVIRONMENT_HPP_INCLUDED

#include "ats_define.hpp"

namespace ats
{

/// 環境情報用に定義された毎処理ごとの車両の状態
struct ST_ENVIRONMENT_VEHICLE_STATUS
{
	double location;	///< 列車の位置 (Z軸) (m)
	float speed;		///< 列車の速度 (km/h)
	int time;			///< 現在の時間 (ms)
	float bc_pressure;	///< ブレーキシリンダの圧力 (Pa)
	float mr_pressure;	///< 元空気溜まりの圧力 (Pa)
	float er_pressure;	///< 釣合空気溜まりの圧力 (Pa)
	float bp_pressure;	///< ブレーキ管の圧力 (Pa)
	float sap_pressure;	///< 直通管の圧力 (Pa)
	float current;		///< 電流 (A)

	/// 列車の操縦命令
	ATS_HANDLES handles;

	/// ドアの状態
	bool is_closed_doors;
	
	/// 列車の速度の絶対値 (km/h)
    float abs_speed;
};

/// 環境情報用に定義された車両の諸元
struct ST_ENVIRONMENT_VEHICLE_SPECIFICATION
{
	int num_of_brake_notch;				///< ブレーキノッチの数
	int num_of_power_notch;				///< 力行ノッチの数
	int ats_check_notch;				///< ATSをキャンセル可能なブレーキノッチ位置
	int b67_notch;						///< 80%のブレーキノッチ位置 (67度)
	int cars;							///< 車の数
	int emergency_brake_notch;			///< 非常ブレーキノッチの位置
	int maximum_service_brake_notch;	///< 常用最大ブレーキノッチの位置
};

/// 環境情報
struct ST_ENVIRONMENT_DATA
{
	/// 車両の諸元
	ST_ENVIRONMENT_VEHICLE_SPECIFICATION vehicle_specification;
	
	/// 車両の状態
	ST_ENVIRONMENT_VEHICLE_STATUS vehicle_status;

	/// ATSプラグインで使用するキーのインデックスの状態
	unsigned int key_status;

	/// 信号現示のインデックス
	int signal;
};

/// モジュールのディレクトリの文字列の長さ
const int ENVIRONMENT_MAX_MODULE_DIR_LENGTH = 260;

/// 環境情報保持クラス<br>このクラスは singleton です。
class environment
{
public:
	/// インスタンスを取得する。
	static environment* get_instance();
	
	/// インスタンスを解放する。
	static void terminate();

	/// 状態を初期化する。
    void initialize_status();

    /// モジュールのディレクトリを設定する。
	/// \param[in] module_dir このプラグインのコンピュータ上の絶対パス
    void set_module_dir(
                        const char* module_dir
                        );

	/// 現在の車両の諸元を設定する。
	/// \param[in] s 車両の諸元
	void set_current_specification(
								   const ATS_VEHICLESPEC& s
								   );

	/// 現在の車両の状態を入力する。
	/// \param[in] s 車両の状態
	void set_current_status(
							const ATS_VEHICLESTATE& s
							);

	/// 現在のフレームの環境情報を取得する。
	/// \return 環境情報
	const ST_ENVIRONMENT_DATA& get_current();

	/// 1フレーム前の環境情報を取得する。
	/// \return 環境情報
	const ST_ENVIRONMENT_DATA& get_last();

	/// モジュールのディレクトリを取得する。
	/// \return このプラグインのコンピュータ上の絶対パス
    const char* get_module_dir();

	/// 現在の力行ノッチの状態を設定する。
	/// \param[in] notch 力行ノッチ位置
	void set_power_notch(
						 int notch
						 );

	/// 現在のブレーキノッチの状態を設定する。
	/// \param[in] notch ブレーキノッチ位置
	void set_brake_notch(
						 int notch
						 );

	/// 現在のレバーサーの状態を設定する。
	/// \param[in] pos レバーサーの位置
	void set_reverser_position(
							   int pos
							   );

	/// 現在のドアの状態を設定する。
	/// \param[in] is_closed_doors ドアが閉まっているか？<br>true:ドアが閉まっている。 false:ドアが開いている。
	void set_door_status(
						 bool is_closed_doors
						 );

	/// 現在の信号現示を設定する。
	/// \param[in] signal 信号現示
	void set_signal(
					int signal
					);

	/// 情報を更新する。
	void update();

	/// ATSプラグインで使用するキーが押された状態を設定する。
	/// \param[in] ats_key_code ATSプラグインで使用するキーのインデックス
	void set_key_down(
					  int ats_key_code
					  );

	/// ATSプラグインで使用するキーが押されていて、それが離された状態を設定する。
	/// \param[in] ats_key_code ATSプラグインで使用するキーのインデックス
	void set_key_up(
					int ats_key_code
					);

	/// ATSプラグインで使用するキーが押されたかどうかを取得する。
	/// \param[in] ats_key_code ATSプラグインで使用するキーのインデックス
	/// \return true:押されている。 false:押されていない。
	bool is_trigger_key(
						int ats_key_code
						);

	/// ATSプラグインで使用するキーが押されていて、それが離されたかどうかを取得する。
	/// \param[in] ats_key_code ATSプラグインで使用するキーのインデックス
	/// \return true:離されている。 false:離されていない。
	bool is_release_key(
						int ats_key_code
						);

	/// ATSプラグインで使用するキーが押し続けられているかどうかを取得する。
	/// \param[in] ats_key_code ATSプラグインで使用するキーのインデックス
	/// \return true:押し続けられている。 false:押し続けられていない。
	bool is_press_key(
					  int ats_key_code
					  );

	/// 現在の刻み時間を取得する。 (dt, msec)
	int delta_t();

protected:
	/// ctor
	environment();

	/// dtor
	virtual ~environment();

private:
	environment(const environment& c);
	environment& operator=(const environment& c);

	static environment* m_p_instance;

	/// 環境情報
	ST_ENVIRONMENT_DATA m_current,			///< 現在のフレーム
						m_last;				///< 1フレーム前

    /// モジュールのディレクトリ
    char m_module_dir[ENVIRONMENT_MAX_MODULE_DIR_LENGTH];

};	// environment

}	// ats

#endif	// ATS_ENVIRONMENT_HPP_INCLUDED
