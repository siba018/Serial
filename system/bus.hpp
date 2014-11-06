/**
 * \file bus.hpp
 * \brief ロジック同士で情報をやり取りするために使う信号線集合のモジュールの定義。
 * \author Takuya Itoh
 */

#ifndef ATS_BUS_HPP_INCLUDED
#define ATS_BUS_HPP_INCLUDED

#include <map>
#include <string>

namespace ats
{

/// 信号線のデータのための共用体定義
union UN_BUS_DATA
{
	double			_f64;			///< 倍精度浮動小数点数型
	float			_f32;			///< 単精度浮動小数点数型
	unsigned long	_ulong;			///< 符号無し長整数型
	long			_slong;			///< 符号付き長整数型
	unsigned int	_u32;			///< 符号無し整数型
	int				_s32;			///< 符号付き長整数型
	unsigned short	_u16;			///< 符号無し短整数型
	short			_s16;			///< 符号付き短整数型
	bool			_bool;			///< 単一のブーリアン型
	bool			_flags[4];		///< 4つの要素のブーリアン型
	unsigned char	_u8;			///< 単一の符号無しバイト型
	char			_s8;			///< 単一の符号付きバイト型
	unsigned char	_uchar[8];		///< 8つの要素の符号無しバイト型
	char			_schar[8];		///< 8つの要素の符号付きバイト型
};


/// ロジック同士で情報をやり取りするために使う信号線集合のモジュールクラス
class bus
{
public:
	/// ctor
	bus()
	{
		m_failed_value._f64 = 0.0;
	}

	/// dtor
	virtual ~bus()
	{

	}

	/// コピーコンストラクタ
	bus(const bus& c)
	{
		copy_members(c);
	}

	/// 代入
	bus& operator=(const bus& c)
	{
		if (this != &c)
		{
			copy_members(c);
		}

		return *this;
	}

	/// 信号線を作成する。
	/// \param[in] key 信号線名
	bool create(
				const std::string& key
				)
	{
		UN_BUS_DATA un;
		un._f64 = 0.0;
		std::pair< std::map< std::string, UN_BUS_DATA >::iterator, bool > result = m_current.insert(std::make_pair(key, un));
		return result.second;
	}

	/// 指定した信号線の値に対する入出力をする。
	/// \param[in] key 信号線名
	/// \return 信号線
	UN_BUS_DATA& operator[](
					   const std::string& key
					   )
	{
		std::map< std::string, UN_BUS_DATA >::iterator iter = m_current.find(key);

		if (iter != m_current.end())
		{
			/* 信号線が見付かった。 */
			return iter->second;
		}
		else
		{
			/* 信号線が見付からなかった。 */
			return m_failed_value;
		}
	}

private:
	/// メンバ変数をコピーする。
	/// \param[in] c クラス
	void copy_members(const bus& c)
	{
		m_current = c.m_current;
		m_failed_value = c.m_failed_value;
	}

	/// 信号線集合
	std::map<std::string, UN_BUS_DATA> m_current;

	/// 信号線名が見付からなかった時に参照を返す値
	UN_BUS_DATA m_failed_value;

};	// bus

}	// ats

#endif	// ATS_BUS_HPP_INCLUDED
