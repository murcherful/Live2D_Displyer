/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */
#pragma once

#include <CubismFramework.hpp>

/**
* @brief  Sample Appで使用する定数
*
*/
namespace LAppDefine {

    using namespace Csm;

    extern const csmFloat32 ViewMaxScale;           ///< 拡大縮小率の最大値
    extern const csmFloat32 ViewMinScale;           ///< 拡大縮小率の最小値

    extern const csmFloat32 ViewLogicalLeft;        ///< 論理的なビュー座標系の左端の値
    extern const csmFloat32 ViewLogicalRight;       ///< 論理的なビュー座標系の右端の値

    extern const csmFloat32 ViewLogicalMaxLeft;     ///< 論理的なビュー座標系の左端の最大値
    extern const csmFloat32 ViewLogicalMaxRight;    ///< 論理的なビュー座標系の右端の最大値
    extern const csmFloat32 ViewLogicalMaxBottom;   ///< 論理的なビュー座標系の下端の最大値
    extern const csmFloat32 ViewLogicalMaxTop;      ///< 論理的なビュー座標系の上端の最大値

    extern const csmChar* ResourcesPath;            ///< 素材パス
    extern const csmChar* BackImageName;         ///< 背景画像ファイル
    extern const csmChar* GearImageName;         ///< 歯車画像ファイル
    extern const csmChar* PowerImageName;        ///< 終了ボタン画像ファイル
	extern const csmChar* MoveiconImageName;     ///< moveicon image
	extern const csmChar* HeightincImageName;    ///< height inc
	extern const csmChar* HeightdecImageName;    ///< height dec
	extern const csmChar* WidthincImageName;     ///< width inc
	extern const csmChar* WidthdecImageName;     ///< width dec
	extern const csmChar* PeopleImageName;       ///< people
	
    // モデル定義--------------------------------------------
    extern csmChar* ModelDir_set[];               ///< モデルを配置したディレクトリ名の配列. ディレクトリ名とmodel3.jsonの名前を一致させておく.
    extern csmChar** ModelDir;
	extern csmInt32 ModelDirSize;             ///< モデルディレクトリ配列のサイズ

                                                    // 外部定義ファイル(json)と合わせる
    extern const csmChar* MotionGroupIdle;          ///< アイドリング時に再生するモーションのリスト
    extern const csmChar* MotionGroupTapBody;       ///< 体をタップした時に再生するモーションのリスト
	extern const csmChar* MotionGroupTapArea1;
	extern const csmChar* MotionGroupTapArea2;
	extern const csmChar* MotionGroupTapArea3;
	extern const csmChar* MotionGroupTapArea4;
	extern const csmChar* MotionGroupTapArea5;
	extern const csmChar* MotionGroupTapArea6;
	extern const csmChar* MotionGroupTapArea7;
	extern const csmChar* MotionGroupTapArea8;
	extern const csmChar* MotionGroupTapArea9;

                                                    // 外部定義ファイル(json)と合わせる
    extern const csmChar* HitAreaNameHead;          ///< 当たり判定の[Head]タグ
    extern const csmChar* HitAreaNameBody;          ///< 当たり判定の[Body]タグ
	extern const csmChar* HitAreaNameArea1;
	extern const csmChar* HitAreaNameArea2;
	extern const csmChar* HitAreaNameArea3;
	extern const csmChar* HitAreaNameArea4;
	extern const csmChar* HitAreaNameArea5;
	extern const csmChar* HitAreaNameArea6;
	extern const csmChar* HitAreaNameArea7;
	extern const csmChar* HitAreaNameArea8;
	extern const csmChar* HitAreaNameArea9;

                                                    // モーションの優先度定数
    extern const csmInt32 PriorityNone;             ///< モーションの優先度定数: 0
    extern const csmInt32 PriorityIdle;             ///< モーションの優先度定数: 1
    extern const csmInt32 PriorityNormal;           ///< モーションの優先度定数: 2
    extern const csmInt32 PriorityForce;            ///< モーションの優先度定数: 3

                                                    // デバッグ用ログの表示
    extern const csmBool DebugLogEnable;            ///< デバッグ用ログ表示の有効・無効
    extern const csmBool DebugTouchLogEnable;       ///< タッチ処理のデバッグ用ログ表示の有効・無効

    // Frameworkから出力するログのレベル設定
    extern const CubismFramework::Option::LogLevel CubismLoggingLevel;

    // デフォルトのレンダーターゲットサイズ
    extern csmInt32 RenderTargetWidth;
    extern csmInt32 RenderTargetHeight;

	void load_def_file(const char*);
}
