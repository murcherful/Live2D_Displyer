/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppView.hpp"
#include <math.h>
#include <string>
#include "LAppPal.hpp"
#include "LAppDelegate.hpp"
#include "LAppLive2DManager.hpp"
#include "LAppTextureManager.hpp"
#include "LAppDefine.hpp"
#include "TouchManager.hpp"
#include "LAppSprite.hpp"
#include "LAppModel.hpp"
#include <Windows.h>

using namespace std;
using namespace LAppDefine;

LAppView::LAppView() :
	_programId(0),
	_back(NULL),
	_gear(NULL),
	_power(NULL),
	_moveicon(NULL),
	_H_inc(NULL),
	_H_dec(NULL),
	_W_inc(NULL),
	_W_dec(NULL),
	_people(NULL),
    _renderSprite(NULL),
    _renderTarget(SelectTarget_None),
	_isMoving(0),
	_lockButton(1)
{
    _clearColor[0] = 1.0f;
    _clearColor[1] = 1.0f;
    _clearColor[2] = 1.0f;
    _clearColor[3] = 0.0f;

    // タッチ関係のイベント管理
    _touchManager = new TouchManager();

    // デバイス座標からスクリーン座標に変換するための
    _deviceToScreen = new CubismMatrix44();

    // 画面の表示の拡大縮小や移動の変換を行う行列
    _viewMatrix = new CubismViewMatrix();
}

LAppView::~LAppView()
{
    _renderBuffer.DestroyOffscreenFrame();
    delete _renderSprite;
    delete _viewMatrix;
    delete _deviceToScreen;
    delete _touchManager;
    delete _back;
    delete _gear;
    delete _power;
	delete _moveicon;
	delete _H_inc;
	delete _H_dec;
	delete _W_inc;
	delete _W_dec;
	delete _people;
}

void LAppView::Initialize()
{
    int width, height;
    glfwGetWindowSize(LAppDelegate::GetInstance()->GetWindow(), &width, &height);

    if(width==0 || height==0)
    {
        return;
    }

    //float ratio = static_cast<float>(height) / static_cast<float>(width);
    float ratio2 = static_cast<float>(width) / static_cast<float>(height);
	//float left = ViewLogicalLeft;
	//float right = ViewLogicalRight;
    //float bottom = -ratio;
    //float top = ratio;
	float left = -ratio2;
	float right = ratio2;
	float bottom = -1.0f;
	float top = 1.0f;
	
	//printf("<<INFO>>: %f, %f, %f, %f\n", left, right, bottom, top);

    _viewMatrix->SetScreenRect(left, right, bottom, top); // デバイスに対応する画面の範囲。 Xの左端, Xの右端, Yの下端, Yの上端

    float screenW = fabsf(left - right);
	float screenH = fabsf(bottom - top);
    _deviceToScreen->LoadIdentity(); // サイズが変わった際などリセット必須
    //_deviceToScreen->ScaleRelative(screenW / width, -screenW / width);
	_deviceToScreen->ScaleRelative(screenH / height / 2, -screenH / height / 2);
    _deviceToScreen->TranslateRelative(-width * 0.5f, -height * 0.5f);
	float* ma = _deviceToScreen->GetArray();
	//printf("ma: %f, %f, %f, %f\n", ma[0], ma[5], ma[12], ma[13]);

    // 表示範囲の設定
    _viewMatrix->SetMaxScale(ViewMaxScale); // 限界拡大率
    _viewMatrix->SetMinScale(ViewMinScale); // 限界縮小率

    // 表示できる最大範囲
    _viewMatrix->SetMaxScreenRect(
        ViewLogicalMaxLeft,
        ViewLogicalMaxRight,
        ViewLogicalMaxBottom,
        ViewLogicalMaxTop
    );
}

void LAppView::Render()
{
    //_back->Render();


    LAppLive2DManager* Live2DManager = LAppLive2DManager::GetInstance();

    // Cubism更新・描画
    Live2DManager->OnUpdate();
	 
	_gear->Render();
	if (!_lockButton) {
		_power->Render();
		_moveicon->Render();
		_H_inc->Render();
		_H_dec->Render();
		_W_inc->Render();
		_W_dec->Render();
		_people->Render();
	}

    // 各モデルが持つ描画ターゲットをテクスチャとする場合
    if (_renderTarget == SelectTarget_ModelFrameBuffer && _renderSprite)
    {
        const GLfloat uvVertex[] =
        {
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
        };

        for (csmUint32 i = 0; i < Live2DManager->GetModelNum(); i++)
        {
            float alpha = GetSpriteAlpha(i); // サンプルとしてαに適当な差をつける
            _renderSprite->SetColor(1.0f, 1.0f, 1.0f, alpha);

            LAppModel *model = Live2DManager->GetModel(i);
            if (model)
            {
                _renderSprite->RenderImmidiate( model->GetRenderBuffer().GetColorBuffer(), uvVertex);
            }
        }
    }
}

void LAppView::InitializeSprite()
{
    _programId = LAppDelegate::GetInstance()->CreateShader();

    int width, height;
    glfwGetWindowSize(LAppDelegate::GetInstance()->GetWindow(), &width, &height);

    LAppTextureManager* textureManager = LAppDelegate::GetInstance()->GetTextureManager();
    const string resourcesPath = ResourcesPath;

    string imageName = BackImageName;
    LAppTextureManager::TextureInfo* backgroundTexture = textureManager->CreateTextureFromPngFile(resourcesPath + imageName);

    float x = width * 0.5f;
    float y = height * 0.5f;
    float fWidth = static_cast<float>(backgroundTexture->width * 2.0f);
    float fHeight = static_cast<float>(height * 0.95f);
    _back = new LAppSprite(x, y, fWidth, fHeight, backgroundTexture->id, _programId);

    imageName = GearImageName;
    LAppTextureManager::TextureInfo* gearTexture = textureManager->CreateTextureFromPngFile(resourcesPath + imageName);

    x = static_cast<float>(width - gearTexture->width * 0.5f);
    y = static_cast<float>(height - gearTexture->height * 0.5f);
    fWidth = static_cast<float>(gearTexture->width);
    fHeight = static_cast<float>(gearTexture->height);
    _gear = new LAppSprite(x, y, fWidth, fHeight, gearTexture->id, _programId);

    imageName = PowerImageName;
    LAppTextureManager::TextureInfo* powerTexture = textureManager->CreateTextureFromPngFile(resourcesPath + imageName);

    x = static_cast<float>(width - powerTexture->width * 0.5f);
    y = static_cast<float>(powerTexture->height * 0.5f);
    fWidth = static_cast<float>(powerTexture->width);
    fHeight = static_cast<float>(powerTexture->height);
    _power = new LAppSprite(x, y, fWidth, fHeight, powerTexture->id, _programId);

	imageName = MoveiconImageName;
	LAppTextureManager::TextureInfo* moveiconTexture = textureManager->CreateTextureFromPngFile(resourcesPath + imageName);

	x = static_cast<float>(width - moveiconTexture->width * 0.5f);
	y = static_cast<float>(height - gearTexture->height - moveiconTexture->height * 0.5f);
	fWidth = static_cast<float>(moveiconTexture->width);
	fHeight = static_cast<float>(moveiconTexture->height);
	_moveicon = new LAppSprite(x, y, fWidth, fHeight, moveiconTexture->id, _programId);

	imageName = PeopleImageName;
	LAppTextureManager::TextureInfo* peopleTexture = textureManager->CreateTextureFromPngFile(resourcesPath + imageName);

	x = static_cast<float>(width - peopleTexture->width * 0.5f);
	y = static_cast<float>(height - gearTexture->height - moveiconTexture->height - peopleTexture->height * 0.5f);
	fWidth = static_cast<float>(peopleTexture->width);
	fHeight = static_cast<float>(peopleTexture->height);
	_people = new LAppSprite(x, y, fWidth, fHeight, peopleTexture->id, _programId);

	imageName = HeightincImageName;
	LAppTextureManager::TextureInfo* heightincTexture = textureManager->CreateTextureFromPngFile(resourcesPath + imageName);

	x = static_cast<float>(heightincTexture->width * 0.5f);
	y = static_cast<float>(height - heightincTexture->height * 0.5f);
	fWidth = static_cast<float>(heightincTexture->width);
	fHeight = static_cast<float>(heightincTexture->height);
	_H_inc = new LAppSprite(x, y, fWidth, fHeight, heightincTexture->id, _programId);

	imageName = HeightdecImageName;
	LAppTextureManager::TextureInfo* heightdecTexture = textureManager->CreateTextureFromPngFile(resourcesPath + imageName);

	x = static_cast<float>(heightdecTexture->width * 0.5f);
	y = static_cast<float>(height - heightincTexture->height - heightdecTexture->height * 0.5f);
	fWidth = static_cast<float>(heightdecTexture->width);
	fHeight = static_cast<float>(heightdecTexture->height);
	_H_dec = new LAppSprite(x, y, fWidth, fHeight, heightdecTexture->id, _programId);

	imageName = WidthincImageName;
	LAppTextureManager::TextureInfo* widthincTexture = textureManager->CreateTextureFromPngFile(resourcesPath + imageName);

	x = static_cast<float>(widthincTexture->width * 0.5f);
	y = static_cast<float>(height - heightincTexture->height - heightdecTexture->height - widthincTexture->height * 0.5f);
	fWidth = static_cast<float>(widthincTexture->width);
	fHeight = static_cast<float>(widthincTexture->height);
	_W_inc = new LAppSprite(x, y, fWidth, fHeight, widthincTexture->id, _programId);

	imageName = WidthdecImageName;
	LAppTextureManager::TextureInfo* widthdecTexture = textureManager->CreateTextureFromPngFile(resourcesPath + imageName);

	x = static_cast<float>(widthdecTexture->width * 0.5f);
	y = static_cast<float>(height - heightincTexture->height - heightdecTexture->height - widthincTexture->height - widthdecTexture->height * 0.5f);
	fWidth = static_cast<float>(widthdecTexture->width);
	fHeight = static_cast<float>(widthdecTexture->height);
	_W_dec = new LAppSprite(x, y, fWidth, fHeight, widthdecTexture->id, _programId);

    // 画面全体を覆うサイズ
    x = width * 0.5f;
    y = height * 0.5f;
    _renderSprite = new LAppSprite(x, y, static_cast<float>(width), static_cast<float>(height), 0, _programId);
}

void LAppView::OnTouchesBegan(float px, float py)
{
    _touchManager->TouchesBegan(px, py);
	if ((!_lockButton) && _moveicon && _moveicon->IsHit(px, py)) {
		_isMoving = 1;
		_moveicon_dx = (int)px;
		_moveicon_dy = (int)py;
		//printf("<MCF>: start moving\n");
	}
}

void LAppView::OnTouchesMoved(float px, float py)
{
	if ((!_lockButton) && _isMoving) {
		POINT p;
		GetCursorPos(&p);
		int newX = (int)p.x - _moveicon_dx;
		int newY = (int)p.y - _moveicon_dy;
		//printf("<MCF>: moving: %d, %d\n", newX, newY);
		glfwSetWindowPos(LAppDelegate::GetInstance()->GetWindow(), newX, newY);
	}
    float viewX = this->TransformViewX(_touchManager->GetX());
    float viewY = this->TransformViewY(_touchManager->GetY());

    _touchManager->TouchesMoved(px, py);

    LAppLive2DManager* Live2DManager = LAppLive2DManager::GetInstance();
    Live2DManager->OnDrag(viewX, viewY);
}

void LAppView::OnTouchesEnded(float px, float py)
{
	_isMoving = 0;
    // タッチ終了
    LAppLive2DManager* live2DManager = LAppLive2DManager::GetInstance();
    live2DManager->OnDrag(0.0f, 0.0f);
    {

        // シングルタップ
        float x = _deviceToScreen->TransformX(_touchManager->GetX()); // 論理座標変換した座標を取得。
        float y = _deviceToScreen->TransformY(_touchManager->GetY()); // 論理座標変換した座標を取得。
        if (DebugTouchLogEnable)
        {
            LAppPal::PrintLog("[APP]touchesEnded x:%.2f y:%.2f", x, y);
        }
        live2DManager->OnTap(x, y);

        // 歯車にタップしたか
        if (_gear->IsHit(px, py))
        {
            //live2DManager->NextScene();
			if (_lockButton) {
				_lockButton = 0;
			}
			else {
				_lockButton = 1;
			}
        }
		else if (!_lockButton) {
			// 電源ボタンにタップしたか
			if (_power->IsHit(px, py))
			{
				LAppDelegate::GetInstance()->AppEnd();
			}
			// move window
			else if (_moveicon->IsHit(px, py)) {
				printf("<MCF>: hit moveicon\n");
			}
			// max height 1000
			else if (_H_inc->IsHit(px, py)) {

				int width, height;
				glfwGetWindowSize(LAppDelegate::GetInstance()->GetWindow(), &width, &height);
				if (height <= 990) {
					height += 10;
					printf("<MCF>: window height increase: %d\n", height);
					glfwSetWindowSize(LAppDelegate::GetInstance()->GetWindow(), width, height);
				}
			}
			// min height 50
			else if (_H_dec->IsHit(px, py)) {

				int width, height;
				glfwGetWindowSize(LAppDelegate::GetInstance()->GetWindow(), &width, &height);
				if (height >= 60) {
					height -= 10;
					printf("<MCF>: window height decrease: %d\n", height);
					glfwSetWindowSize(LAppDelegate::GetInstance()->GetWindow(), width, height);
				}
			}
			// max Width 1900
			else if (_W_inc->IsHit(px, py)) {

				int width, height;
				glfwGetWindowSize(LAppDelegate::GetInstance()->GetWindow(), &width, &height);
				if (width <= 1890) {
					width += 10;
					printf("<MCF>: window width increase: %d\n", width);
					glfwSetWindowSize(LAppDelegate::GetInstance()->GetWindow(), width, height);
				}
			}
			// min width 50
			else if (_W_dec->IsHit(px, py)) {

				int width, height;
				glfwGetWindowSize(LAppDelegate::GetInstance()->GetWindow(), &width, &height);
				if (width >= 60) {
					width -= 10;
					printf("<MCF>: window width decrease: %d\n", width);
					glfwSetWindowSize(LAppDelegate::GetInstance()->GetWindow(), width, height);
				}
			}
			else if (_people->IsHit(px, py)) {
				live2DManager->NextScene();
			}
		}
    }
}

float LAppView::TransformViewX(float deviceX) const
{
    float screenX = _deviceToScreen->TransformX(deviceX); // 論理座標変換した座標を取得。
    return _viewMatrix->InvertTransformX(screenX); // 拡大、縮小、移動後の値。
}

float LAppView::TransformViewY(float deviceY) const
{
    float screenY = _deviceToScreen->TransformY(deviceY); // 論理座標変換した座標を取得。
    return _viewMatrix->InvertTransformY(screenY); // 拡大、縮小、移動後の値。
}

float LAppView::TransformScreenX(float deviceX) const
{
    return _deviceToScreen->TransformX(deviceX);
}

float LAppView::TransformScreenY(float deviceY) const
{
    return _deviceToScreen->TransformY(deviceY);
}

void LAppView::PreModelDraw(LAppModel& refModel)
{
    // 別のレンダリングターゲットへ向けて描画する場合の使用するフレームバッファ
    Csm::Rendering::CubismOffscreenFrame_OpenGLES2* useTarget = NULL;

    if (_renderTarget != SelectTarget_None)
    {// 別のレンダリングターゲットへ向けて描画する場合

        // 使用するターゲット
        useTarget = (_renderTarget == SelectTarget_ViewFrameBuffer) ? &_renderBuffer : &refModel.GetRenderBuffer();

        if (!useTarget->IsValid())
        {// 描画ターゲット内部未作成の場合はここで作成
            int width, height;
            glfwGetWindowSize(LAppDelegate::GetInstance()->GetWindow(), &width, &height);
            if (width != 0 && height != 0)
            {
                // モデル描画キャンバス
                useTarget->CreateOffscreenFrame(static_cast<csmUint32>(width), static_cast<csmUint32>(height));
            }
        }

        // レンダリング開始
        useTarget->BeginDraw();
        useTarget->Clear(_clearColor[0], _clearColor[1], _clearColor[2], _clearColor[3]); // 背景クリアカラー
    }
}

void LAppView::PostModelDraw(LAppModel& refModel)
{
    // 別のレンダリングターゲットへ向けて描画する場合の使用するフレームバッファ
    Csm::Rendering::CubismOffscreenFrame_OpenGLES2* useTarget = NULL;

    if (_renderTarget != SelectTarget_None)
    {// 別のレンダリングターゲットへ向けて描画する場合

        // 使用するターゲット
        useTarget = (_renderTarget == SelectTarget_ViewFrameBuffer) ? &_renderBuffer : &refModel.GetRenderBuffer();

        // レンダリング終了
        useTarget->EndDraw();

        // LAppViewの持つフレームバッファを使うなら、スプライトへの描画はここ
        if (_renderTarget == SelectTarget_ViewFrameBuffer && _renderSprite)
        {
            const GLfloat uvVertex[] =
            {
                1.0f, 1.0f,
                0.0f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f,
            };

            _renderSprite->SetColor(1.0f, 1.0f, 1.0f, GetSpriteAlpha(0));
            _renderSprite->RenderImmidiate(useTarget->GetColorBuffer(), uvVertex);
        }
    }
}

void LAppView::SwitchRenderingTarget(SelectTarget targetType)
{
    _renderTarget = targetType;
}

void LAppView::SetRenderTargetClearColor(float r, float g, float b)
{
    _clearColor[0] = r;
    _clearColor[1] = g;
    _clearColor[2] = b;
}


float LAppView::GetSpriteAlpha(int assign) const
{
    // assignの数値に応じて適当に決定
    float alpha = 0.25f + static_cast<float>(assign) * 0.5f; // サンプルとしてαに適当な差をつける
    if (alpha > 1.0f)
    {
        alpha = 1.0f;
    }
    if (alpha < 0.1f)
    {
        alpha = 0.1f;
    }

    return alpha;
}

void LAppView::ResizeSprite()
{
    LAppTextureManager* textureManager = LAppDelegate::GetInstance()->GetTextureManager();
    if (!textureManager)
    {
        return;
    }

    // 描画領域サイズ
    int width, height;
    glfwGetWindowSize(LAppDelegate::GetInstance()->GetWindow(), &width, &height);

    float x = 0.0f;
    float y = 0.0f;
    float fWidth = 0.0f;
    float fHeight = 0.0f;

    if (_back)
    {
        GLuint id = _back->GetTextureId();
        LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
        if (texInfo)
        {
            x = width * 0.5f;
            y = height * 0.5f;
            fWidth = static_cast<float>(texInfo->width * 2);
            fHeight = static_cast<float>(height) * 0.95f;
            _back->ResetRect(x, y, fWidth, fHeight);
        }
    }

    if (_power)
    {
        GLuint id = _power->GetTextureId();
        LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
        if (texInfo)
        {
            x = static_cast<float>(width - texInfo->width * 0.5f);
            y = static_cast<float>(texInfo->height * 0.5f);
            fWidth = static_cast<float>(texInfo->width);
            fHeight = static_cast<float>(texInfo->height);
            _power->ResetRect(x, y, fWidth, fHeight);
        }
    }

    if (_gear)
    {
        GLuint id = _gear->GetTextureId();
        LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
        if (texInfo)
        {
            x = static_cast<float>(width - texInfo->width * 0.5f);
            y = static_cast<float>(height - texInfo->height * 0.5f);
            fWidth = static_cast<float>(texInfo->width);
            fHeight = static_cast<float>(texInfo->height);
            _gear->ResetRect(x, y, fWidth, fHeight);
        }
    }

	if (_moveicon)
	{
		int dis = 0;
		if (_gear) {
			GLuint id = _gear->GetTextureId();
			LAppTextureManager::TextureInfo* texInfo2 = textureManager->GetTextureInfoById(id);
			dis = texInfo2->height;
		}
		GLuint id = _moveicon->GetTextureId();
		LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
		if (texInfo)
		{
			x = static_cast<float>(width - texInfo->width * 0.5f);
			y = static_cast<float>(height - dis - texInfo->height * 0.5f);
			fWidth = static_cast<float>(texInfo->width);
			fHeight = static_cast<float>(texInfo->height);
			_moveicon->ResetRect(x, y, fWidth, fHeight);
		}
	}

	if (_people) {
		int dis = 0;
		if (_gear) {
			GLuint id = _gear->GetTextureId();
			LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
			dis += texInfo->height;
		}
		if (_moveicon) {
			GLuint id = _moveicon->GetTextureId();
			LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
			dis += texInfo->height;
		}
		GLuint id = _people->GetTextureId();
		LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
		if (texInfo)
		{
			x = static_cast<float>(width - texInfo->width * 0.5f);
			y = static_cast<float>(height - dis - texInfo->height * 0.5f);
			fWidth = static_cast<float>(texInfo->width);
			fHeight = static_cast<float>(texInfo->height);
			_people->ResetRect(x, y, fWidth, fHeight);
		}
	}

	if (_H_inc) {
		GLuint id = _H_inc->GetTextureId();
		LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
		if (texInfo)
		{
			x = static_cast<float>(texInfo->width * 0.5f);
			y = static_cast<float>(height - texInfo->height * 0.5f);
			fWidth = static_cast<float>(texInfo->width);
			fHeight = static_cast<float>(texInfo->height);
			_H_inc->ResetRect(x, y, fWidth, fHeight);
		}
	}

	if (_H_dec) {
		int dis = 0;
		if (_H_inc) {
			GLuint id = _H_inc->GetTextureId();
			LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
			dis += texInfo->height;
		}
		GLuint id = _H_dec->GetTextureId();
		LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
		if (texInfo)
		{
			x = static_cast<float>(texInfo->width * 0.5f);
			y = static_cast<float>(height - dis - texInfo->height * 0.5f);
			fWidth = static_cast<float>(texInfo->width);
			fHeight = static_cast<float>(texInfo->height);
			_H_dec->ResetRect(x, y, fWidth, fHeight);
		}
	}

	if (_W_inc) {
		int dis = 0;
		if (_H_inc) {
			GLuint id = _H_inc->GetTextureId();
			LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
			dis += texInfo->height;
		}
		if (_H_dec) {
			GLuint id = _H_dec->GetTextureId();
			LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
			dis += texInfo->height;
		}
		GLuint id = _W_inc->GetTextureId();
		LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
		if (texInfo)
		{
			x = static_cast<float>(texInfo->width * 0.5f);
			y = static_cast<float>(height - dis - texInfo->height * 0.5f);
			fWidth = static_cast<float>(texInfo->width);
			fHeight = static_cast<float>(texInfo->height);
			_W_inc->ResetRect(x, y, fWidth, fHeight);
		}
	}

	if (_W_dec) {
		int dis = 0;
		if (_H_inc) {
			GLuint id = _H_inc->GetTextureId();
			LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
			dis += texInfo->height;
		}
		if (_H_dec) {
			GLuint id = _H_dec->GetTextureId();
			LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
			dis += texInfo->height;
		}
		if (_W_inc) {
			GLuint id = _H_dec->GetTextureId();
			LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
			dis += texInfo->height;
		}
		GLuint id = _W_dec->GetTextureId();
		LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
		if (texInfo)
		{
			x = static_cast<float>(texInfo->width * 0.5f);
			y = static_cast<float>(height - dis - texInfo->height * 0.5f);
			fWidth = static_cast<float>(texInfo->width);
			fHeight = static_cast<float>(texInfo->height);
			_W_dec->ResetRect(x, y, fWidth, fHeight);
		}
	}
}
