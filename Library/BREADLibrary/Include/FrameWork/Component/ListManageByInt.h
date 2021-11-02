#pragma once
#pragma once
#include <map>
#include <string>
#include <memory>

#include "Component.h"
#include "Math/BREADMath.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"


namespace Bread
{
	namespace FrameWork
	{
		class IntList : public Component
		{
		public:
			struct Element
			{
			public:
				std::map<s32, s32> integerAry;

				Element* AddElement(const s32 elementNum, const s32 num)
				{
					integerAry.insert(std::make_pair(elementNum, num));
					return this;
				}
			};
			std::map<std::string, std::unique_ptr<Element>> elementAry;

		public:
			explicit  IntList(const std::string& id)
			{
				SetID(id);
			}
			~IntList() override { Destruct(); }

		public:
			// コンストラクタ
			void Construct() override
			{
			}

			// デストラクタ
			void Destruct() override
			{

			}

			// 初期化
			void Initialize() override
			{

			}

			// 更新
			void Update() override
			{

			}

			// GUI
			void GUI() override
			{
				using namespace ImGui;
				std::string IDname = typeid(this).name();
				IDname += +"IntList";
				Text(IDname.c_str());
			}

		public:
			Element* AddList(const std::string listID)
			{
				elementAry.insert(std::make_pair(listID, std::make_unique<Element>()));
				return elementAry[listID].get();
			}
			Element* AddElement(const std::string ID, const s32 id,const s32 element)
			{
				elementAry[ID]->integerAry.insert(std::make_pair(id, element));
				return elementAry[ID].get();
			}
		};
	}
}