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
		class Enumer : public Component
		{
		public:
			struct Element
			{
			public:
				Element(s32 val) : state{val} {}
			public:
				std::map<std::string, s32> integerAry;
				s32                                     state;

				Element* AddEnumElement(const std::string enumElement, const s32 num)
				{
					integerAry.insert(std::make_pair(enumElement, num));
					return this;
				}
				s32 at(const std::string& s)
				{
					return integerAry.at(s);
				}
			};
			std::map<std::string, std::unique_ptr<Element>> elementAry;

		public:
			explicit  Enumer(/*std::string id*/) 
			{ 
				//ID = id;
			}
			~Enumer() override { Destruct(); }

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
			Element* AddEnum(const std::string EnumID, const s32 stateVal)
			{
				elementAry.insert(std::make_pair(EnumID, std::make_unique<Element>(stateVal)));
				return elementAry[EnumID].get();
			}

			Element* AddEnumElement(const std::string EnumID,const std::string enumElement, const s32 num)
			{
				elementAry[EnumID]->integerAry.insert(std::make_pair(enumElement, num));
				return elementAry[EnumID].get();
			}
			Element* at(const std::string& s)
			{
				return elementAry.at(s).get();
			}
		};
	}
}