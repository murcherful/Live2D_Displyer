/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppDefine.hpp"
#include <CubismFramework.hpp>
#include <stdio.h>
#include <string.h>

namespace LAppDefine {

    using namespace Csm;

    // 画面
    const csmFloat32 ViewMaxScale = 2.0f;
    const csmFloat32 ViewMinScale = 0.8f;

    const csmFloat32 ViewLogicalLeft = -1.0f;
    const csmFloat32 ViewLogicalRight = 1.0f;

    const csmFloat32 ViewLogicalMaxLeft = -2.0f;
    const csmFloat32 ViewLogicalMaxRight = 2.0f;
    const csmFloat32 ViewLogicalMaxBottom = -2.0f;
    const csmFloat32 ViewLogicalMaxTop = 2.0f;

    // 相対パス
    const csmChar* ResourcesPath = "Resources/";

    // モデルの後ろにある背景の画像ファイル
    const csmChar* BackImageName = "back_class_normal.png";
    // 歯車
    const csmChar* GearImageName = "icon_gear.png";
    // 終了ボタン
    const csmChar* PowerImageName = "close.png";
	// moveicon
	const csmChar* MoveiconImageName = "move.png";
	// height inc
	const csmChar* HeightincImageName = "H_inc.png";
	// height dec
	const csmChar* HeightdecImageName = "H_dec.png";
	// width inc
	const csmChar* WidthincImageName = "W_inc.png";
	// width dec
	const csmChar* WidthdecImageName = "W_dec.png";
	// people
	const csmChar* PeopleImageName = "people.png";
	 

    // モデル定義------------------------------------------
    // モデルを配置したディレクトリ名の配列
    // ディレクトリ名とmodel3.jsonの名前を一致させておくこと
    csmChar* ModelDir_set[] = {
		"kesshouban_v1.1",
		"lafei_4",
        "Haru",
        "Hiyori",
        "Mark",
        "Natori",
        "Rice" 
    }; 
	csmChar** ModelDir = ModelDir_set;
    csmInt32 ModelDirSize = sizeof(ModelDir_set) / sizeof(csmChar*);

    // 外部定義ファイル(json)と合わせる
    const csmChar* MotionGroupIdle = "Idle"; // アイドリング
    const csmChar* MotionGroupTapBody = "TapBody"; // 体をタップしたとき
	const csmChar* MotionGroupTapArea1 = "TapArea1";
	const csmChar* MotionGroupTapArea2 = "TapArea2";
	const csmChar* MotionGroupTapArea3 = "TapArea3";
	const csmChar* MotionGroupTapArea4 = "TapArea4";
	const csmChar* MotionGroupTapArea5 = "TapArea5";
	const csmChar* MotionGroupTapArea6 = "TapArea6";
	const csmChar* MotionGroupTapArea7 = "TapArea7";
	const csmChar* MotionGroupTapArea8 = "TapArea8";
	const csmChar* MotionGroupTapArea9 = "TapArea9";
  
    // 外部定義ファイル(json)と合わせる
    const csmChar* HitAreaNameHead = "Head";
    const csmChar* HitAreaNameBody = "Body";
	const csmChar* HitAreaNameArea1 = "Area1";
	const csmChar* HitAreaNameArea2 = "Area2";
	const csmChar* HitAreaNameArea3 = "Area3";
	const csmChar* HitAreaNameArea4 = "Area4";
	const csmChar* HitAreaNameArea5 = "Area5";
	const csmChar* HitAreaNameArea6 = "Area6";
	const csmChar* HitAreaNameArea7 = "Area7";
	const csmChar* HitAreaNameArea8 = "Area8";
	const csmChar* HitAreaNameArea9 = "Area9";
	 
    // モーションの優先度定数
    const csmInt32 PriorityNone = 0;
    const csmInt32 PriorityIdle = 1;
    const csmInt32 PriorityNormal = 2;
    const csmInt32 PriorityForce = 3;

    // デバッグ用ログの表示オプション
    const csmBool DebugLogEnable = true;
    const csmBool DebugTouchLogEnable = false;

    // Frameworkから出力するログのレベル設定
    const CubismFramework::Option::LogLevel CubismLoggingLevel = CubismFramework::Option::LogLevel_Verbose;
	 
    // デフォルトのレンダーターゲットサイズ
    csmInt32 RenderTargetWidth = 800;
    csmInt32 RenderTargetHeight = 800;

	void load_def_file(const char* filename) {
		FILE *fp = fopen(filename, "r");
		if (fp == NULL) {
			printf("<MCF>: define file open failed\n");
			return;
		}
		printf("<MCF>: open define file: %s\n", filename);
		fscanf(fp, "%d", &ModelDirSize);
		ModelDir = (csmChar**)malloc(ModelDirSize * sizeof(csmChar*));
		csmChar temp_str[256];
		for (int i = 0; i < ModelDirSize; ++i) {
			fscanf(fp, "%s", temp_str);
			int len = strlen(temp_str);
			ModelDir[i] = (csmChar*)malloc((len + 1) * sizeof(csmChar));
			strcpy(ModelDir[i], temp_str);
			printf("<MCF>: get model name: %s\n", ModelDir[i]);
		}
		printf("<MCF>: model number: %d\n", ModelDirSize);
		fscanf(fp, "%d", &RenderTargetWidth);
		fscanf(fp, "%d", &RenderTargetHeight);
		fclose(fp);
		return;
	}
}
