# リポジトリ整理チェックリスト（優先順）

短期（すぐ実行）
- [.gitignore の確認]
  - .vs/, Debug/, Release/, build/ bin/ obj/ を ignore に含める
- [生成物の削除]
  - 既にコミットされているビルド生成物があれば履歴に残さないよう今後 ignore 対象にしつつ、必要なら別ブランチで削除
- [空ファイルの確認]
  - サイズ 0 のファイル（placeholder）は用途を確認。プロジェクトファイルで必須でなければ削除
- [.gitattributes の整備]
  - 大きなバイナリ（.fbx/.png/.psd 等）は LFS で管理（既に一部指定あり）。LFS 適用は履歴書換のリスク確認

中期（CMake 化準備）
- [extern ディレクトリ作成]
  - ImGui, cereal, stb などを `extern/` にまとめるか、CMake FetchContent で取得する方針を決定
- [ソース配置の見直し]
  - Library/BREADLibrary の内部をモジュール毎ディレクトリに整理（例: bread-fnd/, bread-math/...）
- [HLSL の位置決め]
  - shaders を `shaders/` に統一するか、現行の `Source/HLSL` を維持。CMake 側で出力ディレクトリへコピーする設定を追加

長期（CI / ポリシー）
- [CI の追加]
  - GitHub Actions で Windows ビルド（DX11 実装）と Linux（ヘッドレス）を作る
- [コード整備]
  - clang-format / clang-tidy のルール作成と CI チェック
- [ドキュメント]
  - 開発者向け README（CMake ビルド手順、依存関係、モジュール構成）

注意事項
- LFS の新規適用は履歴書換を招くため、チームで合意してから実行
- .sln や .vcxproj はしばらく残して、CMake 化を段階的に進めることを推奨
