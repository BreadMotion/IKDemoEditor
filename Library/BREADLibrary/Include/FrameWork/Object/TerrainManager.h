#pragma once
#include <map>
#include <vector>
#include <memory>
#include <string>
#include <mutex>

#include "Math/BreadMath.h"
#include "FrameWork/Object/Object.h"
#include "FrameWork/Object/BasicObjectElement/INode.h"
#include "FND/Base.h"

namespace Bread
{
	namespace FrameWork
	{
		//TODO : 開放処理を追加しろ
		//地面判定のオブジェクトのメッシュがどこの空間にあるのかを管理するクラス
		class TerrainManager : public FND::Base
		{
			//どこの空間にどこのポリゴンがあるのかを保存する
			struct TerrainModel
			{
				TerrainModel() { registFace.clear(); }
				std::map<std::string, std::vector<IFace::VertexIndex>> registFace;
			};

			//アクターがどこの空間にどこのポリゴンがあるのかを保存する
			std::map<std::shared_ptr<Actor>, TerrainModel> terrains[2];

			//targetFace[2]の使用する配列を示す
			bool swapFlag : 0b1;

			//メインスレッドとの同期用フラグ
			bool sync{ false };

			//空間の取得範囲
			s32 Renge{ 1 };

			std::mutex terrainManagerMutex;

		public:
			TerrainManager() = default;
			~TerrainManager() {};

		public://public Initialize Stage Function

			//引数のモデルコンポーネントからどこの空間にポリゴンがあるのか調べて登録する
			void FirstRegisterPolygon(std::shared_ptr<Actor> actor, std::shared_ptr<ModelObject> model);

			//引数のモデルコンポーネントからどこの空間にポリゴンがあるのか調べて登録する(スレッドセーフ)
			void SecondRegisterPolygon(std::shared_ptr<Actor> model);

		public://public Update Stage Function

			//空間座標のインデックス番号を渡して
			//3*3*3の空間のポリゴンのワールド空間での頂点情報を渡す
			[[nodiscard]]
			const std::vector<IFace::VertexIndex> GetSpatialFaces(const Math::Vector3S32& index);

			//前フレームの時、TransformのDirtyFlagが立ったことのある
			//登録しているアクターはポリゴンの再登録を行う
			//並列処理用
			void ReRegisterDirtyActorPolygon();

			//使用しているswapFlagを入れ替える関数
			const bool& SwapFlag   () { return swapFlag = !swapFlag; }
			const bool& GetSwapFlag() { return swapFlag;             }
			const bool& GetSync()     { return sync;                 }

			void _fastcall SetRenge(const s32& renge) { Renge = renge; }

		public://public GUI Function

			//TerrainManagerの内部データを表示するための関数
			void GUI();

		private://private TerrainManager Function

			//空間番号をstringで保存できる形で残す
			std::string toStringFromSpatialIndex(const Math::Vector3S32& index);

		private://private GUI Function

			//FaceをImGUi::Node関数で表示するための関数
			void FaceInfomationNode(std::pair<const std::string, std::vector<IFace::VertexIndex>>& spatial);
		};
	}//namespace FrameWork
}//namespace Bread