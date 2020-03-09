# 项目说明

- 项目在[Live2d Cubism](https://www.live2d.com/)提供的[样例](https://github.com/Live2D/CubismNativeSamples)上进行修改，仅能在Windows上运行和编译。

- 目前在Win10上可正常编译运行。

- 注意：仅支持Live2Dv3模型（模型为`xxx.model3.json`）

- 模型：预置模型为网上资源，侵删。另，血小板模型[推特地址](https://twitter.com/DenchiSoft/status/1036017773011525632)

- 效果：

  ![](https://github.com/murcherful/Live2D_Displyer/blob/master/screenshoot.png)

- 支持：
  - 窗口背景透明展示模型
  - 支持全屏鼠标追踪
  - 支持10个区域的点击触发区域和10个动作组
  - 支持鼠标静止3秒进入空闲动作组，鼠标运动时不会自动进入空闲动作组
  - 支持拖动，窗口大小实时调整
  - 支持模型切换
  - 使用`def.txt`添加新的模型和修改初始窗口大小
  
- 性能：
  - 在16G内存，i7第8代，GTX1060上运行，占用GPU1-2%（同时有桌面窗口管理器占用1-2%）
  - 帧数66FPS
  
- 不足：
  - 程序图标有BUG
  - 无法托盘
  - windows多桌面时，仅能显示一个
  - 无法鼠标穿透：即窗口没有图像的区域（透明的区域）不可以将事件传递至下面的窗口，这一点受限于`glfw`库（不过相关可发人员称，将会加入这个功能）
  
- 项目无聊时搞的，代码手册查了不少，主要练习读代码的能力，肯定有让大家不满意的地方，欢迎参与改进，也欢迎移植到其他平台（glfw是跨平台的Opengl的gui库）

## 使用方式

- 打开程序后，会出现模型和右上角的齿轮按钮
- 点击齿轮按钮解锁窗口，此时可以
  - 用右下角的电源按钮关闭
  - 用左侧的4个按钮调整窗口大小
  - 用右上角的移动按钮移动窗口
  - 用右上角的人物按钮切换人物
  - 再次点击齿轮按钮锁定窗口
- 模型的目光和姿势会跟踪鼠标，但要求你的模型支持
- 点击模型指定区域可以触发相应动作，但要求预先设置，具体设置方法见下文

## 运行

- 进入项目文件夹后打开`Sample`文件夹，运行`demo.exe`
- 应当能在Win10上直接运行，其他的不知道

## 编译

- 如果有想要看源码，改源码的小伙伴，可以试一下
- 需要预先下载VS2013及以上版本，CMAKE3.16及以上版本（其他库不用下载），详见[Live2D教程](https://docs.live2d.com/cubism-sdk-tutorials/sample-build-opengl/#)
- 进入项目文件夹后打开`Samples\OpenGL\thirdParty\scripts`文件夹，运行`setup_glew_glfw.bat`下载第三方库
- 打开`Samples\OpenGL\Demo\proj.win.cmake\scripts`文件夹，根据你的VS版本运行相应的`proj_msvc201X.bat`创建VS项目
  - 运行后出现控制台，根据你的电脑位数和编译需求，分别输入两次1或2
  - 成功后会出现文件夹`Samples\OpenGL\Demo\proj.win.cmake\build`
  - 如果失败，极有可能是对应VS和CMAKE版本有问题
- 打开`Samples\OpenGL\Demo\proj.win.cmake\build\proj_msvc2017_x64_md`文件夹，打开`Demo.sln`进入VS进行编辑，编译
- 以下介绍如何生成Release版
  - 将解决方案设置成`Release`，（就是点你一眼看到的`Debug`然后选`Release`）
  - 右侧资源管理器选中`Demo`
  - 点击`项目`选择`Demo属性`
  - 点击`连接器`选择`系统`，将`子系统`设置为`窗口(...)`
  - 点击`连接器`选择`高级`，将`入口点`设置为`mainCRTStartup`
  - 点击`确定`
  - 点击`调试`，选择`开始执行（不调试）`
  - 成功后，你应该可以看见血小板。
  - 中间设置项目属性是为了不显示控制台，详见[CSDN博客](https://blog.csdn.net/yz2zcx/article/details/100394890)
  - 生成的可执行文件在`Samples\OpenGL\Demo\proj.win.cmake\build\proj_msvc2017_x64_md\bin\Demo\Release`中

## 模型相关

- 添加自己的模型：

  - 将你的模型放在一个文件夹内，要求打开这个文件夹就能看到`xxx.model3.json`且这个文件夹的名字要和模型的名字一样（也是`xxx`）

  - 将这个文件夹复制到`Resources`文件夹中（注意与可执行文件同级，下同）

  - 打开`Resources`文件夹中的`def.txt`，将你的模型按如下形式添加：

    ```
    3
    xxx
    kesshouban_v1.1
    lafei_4
    400
    800
    ```

    `xxx`为模型的名字。注意第一行为一共有多少模型，剩下出现的模型名为切换的顺序，最后两行为初始窗口的宽和高

- 如果模型加载出错，大概率是模型的动作出问题，这时：

  - 打开`xxx.model3.json`，删除全部动作，将`Motions`中的全部内容删除，注意保留副本

    ```json
    {
    	"Version": 3,
    	"FileReferences": {
    		"Moc": "kesshouban_v1.1.moc3",
    		"Textures": [
    			"kesshouban_v1.1.4096/texture_00.png"
    		],
    		"Motions": {
    			"Idle": [
    			  {"File":"Idle.motion3.json" ,"FadeInTime":0.5, "FadeOutTime":0.5}			  
    			],
    			"TapArea3":[
    			  {"File":"Dance.motion3.json" ,"FadeInTime":0.5, "FadeOutTime":0.5}
    			],
    			"TapArea1":[
    			  {"File":"Anone_Synced.motion3.json" ,"FadeInTime":0.5, "FadeOutTime":0.5}
    			],
    			"TapArea2":[
    			  {"File":"Nemui.motion3.json" ,"FadeInTime":0.5, "FadeOutTime":0.5}
    			]
    		},
    		"Physics": "kesshouban_v1.1.physics3.json"
    	},
        ...
    ```

    ```json
    {
    	"Version": 3,
    	"FileReferences": {
    		"Moc": "kesshouban_v1.1.moc3",
    		"Textures": [
    			"kesshouban_v1.1.4096/texture_00.png"
    		],
    		"Motions": {
    		},
    		"Physics": "kesshouban_v1.1.physics3.json"
    	},
        ...
    ```

  - 再次加载，如果正常，表明是动作文件出错，否则是模型出错，换一个模型吧

  - 如果确定是动作出错，大概率是动作中点数不匹配，这时：

    - 打开其中一个动作文件`xxx.motion3.json`，将其中的`TotalPointCount`设置为原来的二倍

      ```json
      {
      	"Version": 3,
      	"Meta": {
      		"Duration": 10,
      		"Fps": 30.0,
      		"Loop": true,
      		"AreBeziersRestricted": true,
      		"CurveCount": 65,
      		"TotalSegmentCount": 298,
      		"TotalPointCount": 789,
      		"UserDataCount": 0,
      		"TotalUserDataSize": 0
      	},
      	...
      ```

      ```json
      {
      	"Version": 3,
      	"Meta": {
      		"Duration": 10,
      		"Fps": 30.0,
      		"Loop": true,
      		"AreBeziersRestricted": true,
      		"CurveCount": 65,
      		"TotalSegmentCount": 298,
      		"TotalPointCount": 1578,
      		"UserDataCount": 0,
      		"TotalUserDataSize": 0
      	},
      	...
      ```

    - 对其他动作文件做同样操作

  - 重新加载，幸运的话可以成功。

- 设置触发区域和相应动作组

  - 首先要了解`model3.json`中有关动作和触发区域的设定，以及文件的结构

    - 文件的结构大致为，模型文件可能不包含`Groups`和`HitAreas`，如果没有，请安以下规则填写，`Group`可无

      ```
      {
      	"Version":3,
      	"FileReferences": {...},
      	"Groups": [...],
      	"HitAreas": [...]
      }
      ```

    - `FileReferences`的结构

      ```
      "FileReferences": {
      	"Moc": ...,
      	"Textures": [...],
      	"Motions": {...},
      	...
      }
      ```

    - `Motions`的结构为，`path`为`yyy.motion3.json`与`xxx.model3.json`的相对路径

      ```
      "Motions": {
      	"Idle":[
      		{"File": "path", "FeedINTime": 0.5, "FeedOutTime": 0.5},
      		{"File": "path", "FeedINTime": 0.5, "FeedOutTime": 0.5},
      		...
      	],
      	"TapBody":[
      		{"File": "path", "FeedINTime": 0.5, "FeedOutTime": 0.5},
      		{"File": "path", "FeedINTime": 0.5, "FeedOutTime": 0.5},
      		...
      	],
      	"TapArea1":[
      		{"File": "path", "FeedINTime": 0.5, "FeedOutTime": 0.5},
      		{"File": "path", "FeedINTime": 0.5, "FeedOutTime": 0.5},
      		...
      	],
      	...
      }
      ```

      `Motions`下的分组，比如`Idle`，`TapBody`，`TapArea1`等，表示鼠标点击至相应区域会触发的动作组，触发时，会随机一个动作组中的动作。注意，程序定义的分组标识有

      ```
      Idle
      TapBody
      TapArea1
      TapArea2
      TapArea3
      TapArea4
      TapArea5
      TapArea6
      TapArea7
      TapArea8
      TapArea9
      ```

      其中`Idle`为空闲时的动作组，其他为触发相应区域的动作组

    - `HitAreas`的结构为

      ```
      "HitAreas": [
      	{"Name": "Body", "Id":"id"},
      	{"Name": "Area1", "Id":"id"},
      	{"Name": "Area2", "Id":"id"},
      	...
      ]
      ```

      其中`Name`为指定的区域名，程序定义的区域名有

      ```
      Body
      Area1
      Area2
      Area3
      Area4
      Area5
      Area6
      Area7
      Area8
      Area9
      ```

      这些区域与动作组的名称像对应

  - 了解了文件结构，想要设置相应的动作，首先要定义触发区域。而定义触发区域首先要知道模型区域的名字。这时：

    - 下载Live2D Cubsim3 Viewer，见[百度贴吧帖子](https://tieba.baidu.com/p/6066105365?red_tag=2138000797)或去官网下载，下载安装后自动有Viewer，这个是免费的
    - 打开Viewer，将模型打开
    - 点击左上角的`xxx.moc3`，左下角出现信息
    - 将信息顶端的`Parameter`替换为`ArtMesh`
    - 点击相应的`ID`可以看见相应的区域变蓝
    - 选择你想要的区域，记下`ID`
    - 根据上面的介绍，将`id`替换为你记下`ID`，选择一个区域名，
    - 将区域名对应的动作组表示加入`Motions`中，并添加相应的动作

    - 保存模型，运行程序即可

  - 建议参照模型`kesshouban_v1.1`修改

- 其他模型

  - 可以到[项目](https://github.com/Eikanya/Live2d-model)上找，但只有一部分是`xxx.model3.json`

## 其他

- 将会录制视频进行演示（不一定）



以下为原项目的README

# Cubism Native Samples

Live2D Cubism 4 Editor で出力したモデルを表示するアプリケーションのサンプル実装です。

Cubism Native Framework および Live2D Cubism Core と組み合わせて使用します。

## ライセンス

本 SDK を使用する前に[ライセンス](LICENSE.md)をご確認ください。


## 注意事項

本 SDK を使用する前に [注意事項](NOTICE.md)をご確認ください。


## ディレクトリ構成

```
.
├─ Core             # Live2D Cubism Core が含まれるディレクトリ
├─ Framework        # レンダリングやアニメーション機能などのソースコードが含まれるディレクトリ
└─ Samples
   ├─ Cocos2d-x     # Cocos2d-x のサンプルプロジェクトが含まれるディレクトリ
   ├─ D3D9          # DirectX 9.0c のサンプルプロジェクトが含まれるディレクトリ
   ├─ D3D11         # DirectX 11 のサンプルプロジェクトが含まれるディレクトリ
   ├─ OpenGL        # OpenGL のサンプルプロジェクトが含まれるディレクトリ
   └─ Resources     # モデルのファイルや画像などのリソースが含まれるディレクトリ
```


## Cubism Native Framework

モデルを表示、操作するための各種機能を提供します。

[Cubism Native Framework] は、当リポジトリのサブモジュールとして含まれています。
当リポジトリをクローンした後、サブモジュールのクローンを実行することでファイルが追加されます。

[Cubism Native Framework]: (https://github.com/Live2D/CubismNativeFramework)


## Live2D Cubism Core for Native

モデルをロードするためのライブラリです。
当リポジトリには Live2D Cubism Core for Native は同梱されていません。

ダウンロードするには[こちら](https://www.live2d.com/download/cubism-sdk/download-native/)のページを参照ください。
ダウンロードした Zip ファイルの中身を当リポジトリの `Core` ディレクトリにコピーし、プログラムにリンクさせてください。


## ビルド方法

ビルド方法についてはサンプルプロジェクトによって異なります。
各サンプルプロジェクトに同梱された `README.md` を参照ください。

### サンプルプロジェクトの成果物の生成場所

本サンプルでは、Android を除く CMake プロジェクトの成果物は `bin` ディレクトリに生成されます。

例）OpenGL サンプルの macOS プロジェクトのビルドを `make_gcc` スクリプトを使用して行なった場合
```
Demo
└─ proj.mac.cmake
   └─ build
      └─ make_gcc
         └─ bin
            └─ Demo
               ├─ Resources    # Samples/Resources と同じ
               └─ Demo         # 実行可能なアプリケーション
```


## コンパイルオプション

プロジェクトにおいて、マクロ `USE_RENDER_TARGET` または `USE_MODEL_RENDER_TARGET` が有効な場合、
モデルがテクスチャへレンダリングされるようになります。
詳細はサンプルディレクトリ内の `LAppLive2DManager.cpp` を参照ください。


## SDKマニュアル

[Cubism SDK Manual](https://docs.live2d.com/cubism-sdk-manual/top/)


## 変更履歴

当リポジトリの変更履歴については [CHANGELOG.md](CHANGELOG.md) を参照ください。


## 開発環境

| 開発ツール | バージョン |
| --- | --- |
| Android Studio | 3.5.3 |
| CMake | 3.16.3 |
| Visual Studio 2013 | Update 5 |
| Visual Studio 2015 | Update 3 |
| Visual Studio 2017 | 15.9.19 |
| Visual Studio 2019 | 16.4.3 |
| XCode | 11.3.1 |

### Android

| Android SDK Tools | バージョン |
| --- | --- |
| Android NDK | 21.0.6113669 |
| Android SDK | 29.0.2 |
| CMake | 3.10.2.4988404 |

### Linux

| 系統 | Docker イメージ | GCC |
| --- | --- | --- |
| Red Hat | `amazonlinux:1` | 6.3.1 ([devtoolset-6]) |
| Red Hat | `amazonlinux:2` | 7.3.1 |
| Red Hat | `centos:6` | 6.3.1 ([devtoolset-6]) |
| Red Hat | `centos:7` | 4.8.5 |
| Red Hat | `centos:8` | 8.3.1 |
| Debian | `ubuntu:16.04` | 5.4.0 |
| Debian | `ubuntu:18.04` | 7.4.0 |
| Debian | `ubuntu:19.10` | 9.2.1 |

#### Mesa ライブラリ

* Red Hat
  * `mesa-libGL-devel`
  * `mesa-libGLU-devel`
* Debian
  * `libgl1-mesa-dev`
  * `libglu1-mesa-dev`

NOTE: `amazonlinux:1` と `centos:6` はデフォルトの GCC のバージョンでビルドを行えないため [devtoolset-6] などを用いて新しいバージョンの GCC を使用する必要があります。

[devtoolset-6]: https://www.softwarecollections.org/en/scls/rhscl/devtoolset-6/


## 動作確認環境

| プラットフォーム | バージョン |
| --- | --- |
| iOS / iPadOS | 13.3 |
| macOS | 10.15.1 |
| Windows 10 | 1903 |

### Android

| バージョン | デバイス | Tegra |
| --- | --- | --- |
| 10 | Pixel 3a | |
| 4.1.2 | Nexus 7 (2012) | ✔︎ |

### Linux

| 系統 | ディストリビューション | バージョン |
| --- | --- | --- |
| Red Hat | Amazon Linux | 2 |
| Red Hat | CentOS | 8 |
| Debian | Ubuntu | 18.04 |
