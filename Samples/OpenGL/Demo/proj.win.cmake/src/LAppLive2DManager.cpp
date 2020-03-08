﻿/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppLive2DManager.hpp"
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Rendering/CubismRenderer.hpp>
#include "LAppPal.hpp"
#include "LAppDefine.hpp"
#include "LAppDelegate.hpp"
#include "LAppModel.hpp"
#include "LAppView.hpp"

using namespace Csm;
using namespace LAppDefine;
using namespace std;

namespace {
    LAppLive2DManager* s_instance = NULL;

    void FinishedMotion(ACubismMotion* self)
    {
        LAppPal::PrintLog("Motion Finished: %x", self);
    }
}

LAppLive2DManager* LAppLive2DManager::GetInstance()
{
    if (s_instance == NULL)
    {
        s_instance = new LAppLive2DManager();
    }

    return s_instance;
}

void LAppLive2DManager::ReleaseInstance()
{
    if (s_instance != NULL)
    {
        delete s_instance;
    }

    s_instance = NULL;
}

LAppLive2DManager::LAppLive2DManager()
    : _viewMatrix(NULL)
    , _sceneIndex(0)
{
    ChangeScene(_sceneIndex);
}

LAppLive2DManager::~LAppLive2DManager()
{
    ReleaseAllModel();
}

void LAppLive2DManager::ReleaseAllModel()
{
    for (csmUint32 i = 0; i < _models.GetSize(); i++)
    {
        delete _models[i];
    }

    _models.Clear();
}

LAppModel* LAppLive2DManager::GetModel(csmUint32 no) const
{
    if (no < _models.GetSize())
    {
        return _models[no];
    }

    return NULL;
}

void LAppLive2DManager::OnDrag(csmFloat32 x, csmFloat32 y) const
{
    for (csmUint32 i = 0; i < _models.GetSize(); i++)
    {
        LAppModel* model = GetModel(i);

        model->SetDragging(x, y);
    }
}

void LAppLive2DManager::OnTap(csmFloat32 x, csmFloat32 y)
{
    if (DebugLogEnable)
    {
        LAppPal::PrintLog("[APP]tap point: {x:%.2f y:%.2f}", x, y);
    }

    for (csmUint32 i = 0; i < _models.GetSize(); i++)
    {
        if (_models[i]->HitTest(HitAreaNameHead, x, y))
        {
            if (DebugLogEnable)
            {
                LAppPal::PrintLog("[APP]hit area: [%s]", HitAreaNameHead);
            }
            _models[i]->SetRandomExpression();
        }
        else if (_models[i]->HitTest(HitAreaNameBody, x, y))
        {
            if (DebugLogEnable)
            {
                LAppPal::PrintLog("[APP]hit area: [%s]", HitAreaNameBody);
            }
            _models[i]->StartRandomMotion(MotionGroupTapBody, PriorityNormal, FinishedMotion);
        }
		else if (_models[i]->HitTest(HitAreaNameArea1, x, y))
		{
			if (DebugLogEnable)
			{
				LAppPal::PrintLog("[APP]hit area: [%s]", HitAreaNameArea1);
			}
			_models[i]->StartRandomMotion(MotionGroupTapArea1, PriorityNormal, FinishedMotion);
		}
		else if (_models[i]->HitTest(HitAreaNameArea2, x, y))
		{
			if (DebugLogEnable)
			{
				LAppPal::PrintLog("[APP]hit area: [%s]", HitAreaNameArea2);
			}
			_models[i]->StartRandomMotion(MotionGroupTapArea2, PriorityNormal, FinishedMotion);
		}
		else if (_models[i]->HitTest(HitAreaNameArea3, x, y))
		{
			if (DebugLogEnable)
			{
				LAppPal::PrintLog("[APP]hit area: [%s]", HitAreaNameArea3);
			}
			_models[i]->StartRandomMotion(MotionGroupTapArea3, PriorityNormal, FinishedMotion);
		}
		else if (_models[i]->HitTest(HitAreaNameArea4, x, y))
		{
			if (DebugLogEnable)
			{
				LAppPal::PrintLog("[APP]hit area: [%s]", HitAreaNameArea4);
			}
			_models[i]->StartRandomMotion(MotionGroupTapArea4, PriorityNormal, FinishedMotion);
		}
		else if (_models[i]->HitTest(HitAreaNameArea5, x, y))
		{
			if (DebugLogEnable)
			{
				LAppPal::PrintLog("[APP]hit area: [%s]", HitAreaNameArea5);
			}
			_models[i]->StartRandomMotion(MotionGroupTapArea5, PriorityNormal, FinishedMotion);
		}
		else if (_models[i]->HitTest(HitAreaNameArea6, x, y))
		{
			if (DebugLogEnable)
			{
				LAppPal::PrintLog("[APP]hit area: [%s]", HitAreaNameArea6);
			}
			_models[i]->StartRandomMotion(MotionGroupTapArea6, PriorityNormal, FinishedMotion);
		}
		else if (_models[i]->HitTest(HitAreaNameArea7, x, y))
		{
			if (DebugLogEnable)
			{
				LAppPal::PrintLog("[APP]hit area: [%s]", HitAreaNameArea7);
			}
			_models[i]->StartRandomMotion(MotionGroupTapArea7, PriorityNormal, FinishedMotion);
		}
		else if (_models[i]->HitTest(HitAreaNameArea8, x, y))
		{
			if (DebugLogEnable)
			{
				LAppPal::PrintLog("[APP]hit area: [%s]", HitAreaNameArea8);
			}
			_models[i]->StartRandomMotion(MotionGroupTapArea8, PriorityNormal, FinishedMotion);
		}
		else if (_models[i]->HitTest(HitAreaNameArea9, x, y))
		{
			if (DebugLogEnable)
			{
				LAppPal::PrintLog("[APP]hit area: [%s]", HitAreaNameArea9);
			}
			_models[i]->StartRandomMotion(MotionGroupTapArea9, PriorityNormal, FinishedMotion);
		}
    }
}

void LAppLive2DManager::OnUpdate() const
{
    CubismMatrix44 projection;
    int width, height;
    glfwGetWindowSize(LAppDelegate::GetInstance()->GetWindow(), &width, &height);
    //projection.Scale(1.0f, static_cast<float>(width) / static_cast<float>(height));
	projection.Scale(2*static_cast<float>(height) / static_cast<float>(width), 2.0f);
	//projection.Scale(static_cast<float>(height) / static_cast<float>(width), 1.0f);
	//projection.Scale(1.0f, 1.0f);

    if (_viewMatrix != NULL)
    {
        projection.MultiplyByMatrix(_viewMatrix);
    }

    const CubismMatrix44    saveProjection = projection;
    csmUint32 modelCount = _models.GetSize();
    for (csmUint32 i = 0; i < modelCount; ++i)
    {
        LAppModel* model = GetModel(i);
        projection = saveProjection;

        // モデル1体描画前コール
        LAppDelegate::GetInstance()->GetView()->PreModelDraw(*model);

        model->Update();
        model->Draw(projection);///< 参照渡しなのでprojectionは変質する

        // モデル1体描画後コール
        LAppDelegate::GetInstance()->GetView()->PostModelDraw(*model);
    }
}

void LAppLive2DManager::NextScene()
{
    csmInt32 no = (_sceneIndex + 1) % ModelDirSize;
    ChangeScene(no);
}

void LAppLive2DManager::ChangeScene(Csm::csmInt32 index)
{
    _sceneIndex = index;
    if (DebugLogEnable)
    {
        LAppPal::PrintLog("[APP]model index: %d", _sceneIndex);
    }

    // ModelDir[]に保持したディレクトリ名から
    // model3.jsonのパスを決定する.
    // ディレクトリ名とmodel3.jsonの名前を一致させておくこと.
    std::string model = ModelDir[index];
    std::string modelPath = ResourcesPath + model + "/";
    std::string modelJsonName = ModelDir[index];
    modelJsonName += ".model3.json";

    ReleaseAllModel();
    _models.PushBack(new LAppModel());
    _models[0]->LoadAssets(modelPath.c_str(), modelJsonName.c_str());

    /*
     * モデル半透明表示を行うサンプルを提示する。
     * ここでUSE_RENDER_TARGET、USE_MODEL_RENDER_TARGETが定義されている場合
     * 別のレンダリングターゲットにモデルを描画し、描画結果をテクスチャとして別のスプライトに張り付ける。
     */
    {
#if defined(USE_RENDER_TARGET)
        // LAppViewの持つターゲットに描画を行う場合、こちらを選択
        LAppView::SelectTarget useRenderTarget = LAppView::SelectTarget_ViewFrameBuffer;
#elif defined(USE_MODEL_RENDER_TARGET)
        // 各LAppModelの持つターゲットに描画を行う場合、こちらを選択
        LAppView::SelectTarget useRenderTarget = LAppView::SelectTarget_ModelFrameBuffer;
#else
        // デフォルトのメインフレームバッファへレンダリングする(通常)
        LAppView::SelectTarget useRenderTarget = LAppView::SelectTarget_None;
#endif

#if defined(USE_RENDER_TARGET) || defined(USE_MODEL_RENDER_TARGET)
        // モデル個別にαを付けるサンプルとして、もう1体モデルを作成し、少し位置をずらす
        _models.PushBack(new LAppModel());
        _models[1]->LoadAssets(modelPath.c_str(), modelJsonName.c_str());
        _models[1]->GetModelMatrix()->TranslateX(0.2f);
#endif

        LAppDelegate::GetInstance()->GetView()->SwitchRenderingTarget(useRenderTarget);

        // 別レンダリング先を選択した際の背景クリア色
        float clearColor[3] = { 1.0f, 1.0f, 1.0f };
        LAppDelegate::GetInstance()->GetView()->SetRenderTargetClearColor(clearColor[0], clearColor[1], clearColor[2]);
    }
}

csmUint32 LAppLive2DManager::GetModelNum() const
{
    return _models.GetSize();
}
