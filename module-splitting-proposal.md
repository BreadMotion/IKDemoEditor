# BREADLibrary モジュール分割提案（承認済）

目的
- コードベースを論理的なモジュールに分割し、ビルド速度、可読性、再利用性、テスト性を向上させる。
- 将来的な CMake 化と外部公開（find_package）を見据えたターゲット分割を行う。

提案モジュール（初期案）
1. bread-fnd
   - 内容: FND 系ユーティリティ（Logger, Instance, Util, STD, Flag, NonCopyable 等）
   - 依存: なし
   - ターゲット: static library `bread-fnd`

2. bread-math
   - 内容: BreadMath、Vector/Matrix/Quaternion、Primitive math utilities
   - 依存: `bread-fnd`
   - ターゲット: static library `bread-math`

3. bread-graphics
   - 内容: グラフィックス抽象（IDevice/IContext/IMesh/ITexture/RenderState 等）
   - 依存: `bread-fnd`, `bread-math`
   - 備考: プラットフォーム実装はサブパッケージ（例: `bread-graphics-dx11`）

4. bread-resource
   - 内容: Loader, ResourceManager, IResourceFactory, Model/Animation resource wrappers
   - 依存: `bread-fnd`, `bread-graphics`, `bread-math`

5. bread-framework
   - 内容: Actor/Component/Transform/ModelObject/Renderer、シーン/ActorManager、ゲームロジック層
   - 依存: `bread-fnd`, `bread-math`, `bread-graphics`, `bread-resource`
   - 備考: IK 系（ChainFABRIKManager, FARBIKManager, CCDIK 等）はこのモジュール内または sub-module（bread-ik）に分離可能

6. bread-ik (optional)
   - 内容: CCDIK, FABRIK, ChainFABRIKManager, HumanFARBIKManager 等
   - 依存: `bread-math`（軽い参照）、`bread-framework`（登録 API）
   - メリット: IK を独立テスト・独立改善できる

7. bread-editor (optional)
   - 内容: ImGui ベースのエディタ/デバッグ UI
   - 依存: `bread-framework`, `bread-graphics`, `bread-resource`

8. bread-examples
   - 内容: 実行サンプル、ツール、デモ
   - 依存: 必要な bread-* モジュールをリンク

9. extern / third_party
   - 内容: ImGui, cereal, stb, FBX SDK（配置 or FetchContent）

決定基準（モジュール化を行うか迷ったとき）
- 明確な API 境界があるか（ある → 切り出す）
- その部分を頻繁に変えるか（頻繁に変える → 小さく分割してビルド影響を限定）
- 循環依存を解消可能か
- 将来別プロジェクト／ライブラリとして公開したいか

移行手順（段階的）
1. 準備
   - `extern/` を作成。外部ライブラリの扱い方（LFS か FetchContent）を決定。
   - ルートに `CMakeLists.txt` を追加（骨子のみ）。

2. bread-fnd を抽出して CMake 化（最小且つ独立）
   - ソースと include を `Library/BREADLibrary` 内に `bread-fnd` のサブフォルダに配置（または同配置で CMake ターゲット化）。
   - テストターゲットを用意。

3. bread-math を CMake 化して bread-fnd にリンク
   - BreadMath.cpp のビルドを確認。SIMD オプション・コンパイラフラグの扱いを整理。

4. bread-graphics のインターフェース化
   - `bread-graphics` を作り、DX11 実装は `bread-graphics-dx11` （plugin）に分離。

5. bread-resource / bread-framework / bread-ik / bread-editor を逐次 CMake 化

6. examples をビルドして正しくリンクできることを確認

7. CI 設定（GitHub Actions）でビルド matrix を整備

注意点・リスク
- 既存の Visual Studio ソリューション（.sln/.vcxproj）があるため、CMake 化は段階的に行う。いきなり .sln を消すのは避ける。
- LFS 適用は履歴を書き換える可能性があるので注意（必要時は新規 LFS 適用のみを推奨）。
- グローバル静的変数やヘッダオンリーの設計がモジュール境界で問題になることがある（特に `FND::Instance` 系）。移行時に参照の再設計が必要になる可能性。

アウトプット（この後作るもの）
- パッケージ図（Mermaid）
- 高レベルクラス図（Mermaid）
- 代表シーケンス図（Mermaid）
- サンプル CMakeLists（トップ + bread-fnd + bread-math）
- リポジトリ整理チェックリスト（削除/移動/ignore/LFS）

次の作業（即行う）
- サンプル CMakeLists（トップ + bread-fnd + bread-math）を作成します（作業済、下に示します）。
- パッケージ図 / 高レベル UML（Mermaid）を作成します（下に示します）。
- リポジトリ整理チェックリストを作成します（下に示します）。
