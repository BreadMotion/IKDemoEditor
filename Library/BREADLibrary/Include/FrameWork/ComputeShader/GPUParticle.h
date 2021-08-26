#pragma once

#include <memory>
#include "Graphics\GraphicsDevice.h"
#include "Graphics\Shader.h"
#include "Graphics\Camera.h"
#include "Graphics\Mesh.h"
#include "Math\BreadMath.h"


namespace Bread
{
	namespace FrameWork
	{
		class GPUBuffer
		{
		public:
			std::unique_ptr<Graphics::IBuffer> buffer;
			std::unique_ptr<Graphics::ITexture> srv;
			std::unique_ptr<Graphics::ITexture> uav;

		public:
			GPUBuffer() {}
			~GPUBuffer() {}

		public:
			// 生成
			static std::unique_ptr<GPUBuffer> Create();

			// 初期化
			bool Initialize(Graphics::IDevice* device, Bread::Graphics::PhoenixUsage usage, u32 bindFlags, u32 byteWidth, u32 structureByteStride, s32 miscFlags, void* initData, Graphics::TextureFormatDx format = Graphics::TextureFormatDx::UNKNOWN);

			// 終了化
			void Finalize();
		};

		class GPUParticle
		{
		private:
			struct IndirectDispatchArgs
			{
				u32 threadGroupCountX = 0;
				u32 threadGroupCountY = 0;
				u32 threadGroupCountZ = 0;
			};
			struct IndirectDrawArgsInstanced
			{
				u32 vertexCountPerInstance = 0;
				u32 instanceCount = 0;
				u32 startVertexLocation = 0;
				u32 startInstanceLocation = 0;
			};

			struct Particle
			{
				Math::Vector3 position;
				f32 mass;
				Math::Vector3 force;
				f32 rotationalVelocity;
				Math::Vector3 velocity;
				f32 maxLife;
				Math::Vector2 sizeBeginEnd;
				f32 life;
				u32 colorMirror;
				Math::Vector4 motionVelocity;
				Math::Vector4 originPosition;
			};

			struct ParticleCounters
			{
				u32 aliveCount;
				u32 deadCount;
				u32 realEmitCount;
				u32 aliveCount_afterSimulation;
			};

			struct EmittedParticleCB
			{
				Math::Matrix emitterWorld;

				u32	emitCount;
				u32	emitterMeshIndexCount;
				u32	emitterMeshVertexPositionStride;
				f32	emitterRandomness;

				f32	particleSize;
				f32	particleScaling;
				f32	particleRotation;
				u32	particleColor;

				Math::Vector4 particleNormal;
				Math::Color particleMainColor;

				f32 particleRandomFactor;
				f32 particleNormalFactor;
				f32 particleLifeSpan;
				f32 particleLifeSpanRandomness;

				f32	particleMass;
				f32	particleMotionBlurAmount;
				f32	emitterOpacity;
				u32	emitterMaxParticleCount;

				f32 SPH_h;				// smoothing radius
				f32 SPH_h_rcp;			// 1.0f / smoothing radius
				f32 SPH_h2;				// smoothing radius ^ 2
				f32 SPH_h3;				// smoothing radius ^ 3

				f32 SPH_poly6_constant;	// precomputed Poly6 kernel constant term
				f32 SPH_spiky_constant;	// precomputed Spiky kernel function constant term
				f32 SPH_K;				// pressure constant
				f32 SPH_p0;				// reference density

				f32 SPH_e;					// viscosity constant
				u32 SPH_ENABLED;			// is SPH enabled?
				f32 emitterFixedTimestep;	// we can force a fixed timestep (>0) onto the simulation to avoid blowing up
				f32 particleEmissive;

				f32 seed;
				f32 randU;
				f32 randV;
				f32 randW;
			};

			struct FrameTimeCB
			{
				f32 frameTime;
				f32 dummy[3];
			};

			struct MeshVertexPos
			{
				Math::Vector3 position = Math::Vector3::Zero;
				u32 normalWind = 0;

				void SetParameter(const Math::Vector3& pos, const Math::Vector3& normal, u8 wind)
				{
					position.x = pos.x;
					position.y = pos.y;
					position.z = pos.z;
					MakeFromParameters(normal, wind);
				}

				inline void MakeFromParameters(const Math::Vector3& normal, u8 wind)
				{
					normalWind = 0;
					normalWind |= (u32)((normal.x * 0.5f + 0.5f) * 255.0f) << 0;
					normalWind |= (u32)((normal.y * 0.5f + 0.5f) * 255.0f) << 8;
					normalWind |= (u32)((normal.z * 0.5f + 0.5f) * 255.0f) << 16;
					normalWind |= (u32)wind << 24;
				}
			};

		private:
			static const u32 emitterMaxSize = 256;
			static const u32 particleMaxSize = 10000;

			static const u32 ARGUMENTBUFFER_OFFSET_DISPATCHEMIT = 0;
			static const u32 ARGUMENTBUFFER_OFFSET_DISPATCHSIMULATION = ARGUMENTBUFFER_OFFSET_DISPATCHEMIT + (3 * 4);
			static const u32 ARGUMENTBUFFER_OFFSET_DRAWPARTICLES = ARGUMENTBUFFER_OFFSET_DISPATCHSIMULATION + (3 * 4);

		private:
			std::unique_ptr<Graphics::IShader> drawShader;

			std::unique_ptr<Graphics::IComputeShader> kickoffUpdateCS;
			std::unique_ptr<Graphics::IComputeShader> finishUpdateCS;
			std::unique_ptr<Graphics::IComputeShader> emitCS;
			std::unique_ptr<Graphics::IComputeShader> emitCSVolume;
			std::unique_ptr<Graphics::IComputeShader> emitCSFromMesh;
			std::unique_ptr<Graphics::IComputeShader> sphpartitionCS;
			std::unique_ptr<Graphics::IComputeShader> sphpartitionoffsetsCS;
			std::unique_ptr<Graphics::IComputeShader> sphpartitionoffsetsresetCS;
			std::unique_ptr<Graphics::IComputeShader> sphdensityCS;
			std::unique_ptr<Graphics::IComputeShader> sphforceCS;
			std::unique_ptr<Graphics::IComputeShader> simulateCS;
			std::unique_ptr<Graphics::IComputeShader> simulateCSSorting;
			std::unique_ptr<Graphics::IComputeShader> simulateCSDepthCollisions;
			std::unique_ptr<Graphics::IComputeShader> simulateCSSortingDepthCollisions;

			std::unique_ptr<GPUBuffer> particleBuffer;
			std::unique_ptr<GPUBuffer> aliveList[2];
			std::unique_ptr<GPUBuffer> deadList;
			std::unique_ptr<GPUBuffer> distanceBuffer;

			std::unique_ptr<GPUBuffer> counterBuffer;
			std::unique_ptr<GPUBuffer> indirectBuffers;

			std::unique_ptr<GPUBuffer> vertexBuffer;
			std::unique_ptr<GPUBuffer> indexBuffer;

			std::unique_ptr<Graphics::IBuffer> emittedParticleCB;
			std::unique_ptr<Graphics::IBuffer> frameTimeCB;

			std::unique_ptr<Graphics::ITexture> mainTexTure;
			std::unique_ptr<Graphics::ITexture> alphaTexture;

		private:
			float fixedTimeStep = 1.0f / 60.0f; // -1 : variable timestep; >=0 : fixed timestep

			f32 emit = 0.0f;
			s32 burst = 0;
			s32 meshIndexCount = 0;
			Math::Vector3 center;

			f32 size = 1.0f;
			f32 randomFactor = 1.0f;
			f32 normalFactor = 1.0f;
			f32 count = 0.0f;
			f32 life = 1.0f;
			f32 randomLife = 1.0f;
			f32 scaleX = 1.0f;
			f32 scaleY = 1.0f;
			f32 rotation = 0.0f;
			f32 motionBlurAmount = 1.0f;
			f32 mass = 1.0f;
			Math::Vector4 normal = Math::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
			Math::Color mainColor = Math::Color(1.0f, 1.0f, 1.0f, 1.0f);


		public:
			GPUParticle() {}
			~GPUParticle() { Finalize(); }

		public:
			// 生成
			static std::unique_ptr<GPUParticle> Create();

			// 初期化
			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice, const char* simulateCSFileName, const char* textureFileName, bool isEmissive = false, bool isAlpha = false);

			// 終了化
			void Finalize();

			// 更新
			void UpdateCPU(Graphics::IGraphicsDevice* graphicsDevice, Math::Vector3 transform, f32 dt);

			void UpdateGPU(Graphics::IGraphicsDevice* graphicsDevice, Math::Matrix worldTransform, f32 dt);

			void Draw(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera);

			void Burst(s32 num);

			void Restart();

			bool CreateBuffers(Graphics::IDevice* device);

			bool CreateMeshBuffers(Graphics::IGraphicsDevice* graphicsDevice, Graphics::IMesh* mesh);

			void LoadShaders(Graphics::IDevice* device, const char* simulateCSFileName, bool isEmissive, bool isAlpha);

			void SetParticleSize(f32 particleSize) { size = particleSize; }

			void SetParticleLife(f32 particleLife) { life = particleLife; }

			void SetParticleScale(f32 particleScale) { scaleX = particleScale; }

			void SetParticleRotate(f32 particleRotate) { rotation = particleRotate; }

			void SetParticleNormal(Math::Vector4 particleNormal) { normal = particleNormal; }

			void SetParticleColor(Math::Color particleColor) { mainColor = particleColor; }

			void SetParticleMotionBlurAmount(f32 particleMotionBlurAmount) { motionBlurAmount = particleMotionBlurAmount; }

			void SetParticleFixedTimeStep(f32 particleFixedTimeStep) { fixedTimeStep = particleFixedTimeStep; }
		};

		/*
		class Item
		{
		public:
			Item() {}
			virtual ~Item() {}
		};
		class VelocityItem : public Item
		{
		public:
			Math::Vector3 velocity = Math::Vector3(0.0f, 0.0f, 0.0f);

		public:
			VelocityItem() {}
			~VelocityItem() {}
		};
		class RotateAnimItem : public Item
		{
		public:
			f32 rotate = 0.0f;

		public:
			RotateAnimItem() {}
			~RotateAnimItem() {}
		};
		class ScaleAnimItem : public Item
		{
		public:
			f32 scale = 1.0f;

		public:
			ScaleAnimItem() {}
			~ScaleAnimItem() {}
		};
		class SpawnItem : public Item
		{
		public:
			u32 spawnCount = 0;
			u32 spawnMaxCount = 0;
			u32 spawnInterval = 0;

		private:
			u32 timeCount = 0;
			u32 totalSpawnCount = 0;

		public:
			SpawnItem() {}
			~SpawnItem() {}

		public:
			u32 Update()
			{
				if (timeCount++ % spawnInterval == 0)
				{
					totalSpawnCount += spawnCount;

					u32 subCount = spawnMaxCount - totalSpawnCount;
					if (subCount < spawnCount) return subCount;

					return spawnCount;
				}

				return 0;
			}

			u32 GetTotalSpawnCount() { return totalSpawnCount; }
		};

		class ItemData
		{
		public:
			std::list<std::shared_ptr<Item>> items;
			u32 totalItemMax;

		public:
			ItemData() {}
			~ItemData() {}

		public:
			template<class T>
			T* GetItem()
			{
				for (auto item : items)
				{
					T* buff = dynamic_cast<T*>(item.get());
					if (buff != nullptr) return buff;
				}
				return nullptr;
			}

			template<class T>
			T* AddItem()
			{
				std::shared_ptr<T> buff = std::make_shared<T>();
				items.push_back(buff);

				return buff.get();
			}
		};
		class EmitterParameter
		{
		public:
			Math::Matrix transform;
			ItemData itemData;
			u32 spawnParticleNum;
			u32 life = 0;

		public:
			EmitterParameter() {}
			~EmitterParameter() {}
		};

		class EmitParticle
		{
		private:
			struct IndirectParticleNum
			{
				u32 particleNum;
				u32 prevParticleNum;
			};
			struct IndirectDispatchArgs
			{
				u32 threadGroupCountX = 0;
				u32 threadGroupCountY = 0;
				u32 threadGroupCountZ = 0;
			};
			struct IndirectDrawArgsInstanced
			{
				u32 vertexCountPerInstance = 0;
				u32 instanceCount = 0;
				u32 startVertexLocation = 0;
				u32 startInstanceLocation = 0;
			};

			struct EmitterHeader
			{
				u32 emitHead;
				u32 emitSize;
				u32 particleHead;
				u32 particleSize;
			};
			struct ParticleHeader
			{
				u32 tag; // alive + emitterID + index
				f32 depth;
			};

			struct EmitterData
			{
				u32 spawnHead;
				u32 spawnNum;
				u32 particleNum;
			};

			struct EmitterRange
			{
				u32 aliveHead;
				u32 aliveEnd;
				u32 deadHead;
				u32 deadEnd;
			};

			struct ParticleCB
			{
				u32 totalSpawnCount;
				u32 previousEmitterSpawnCount;
				u32 currentEmitterSpawnCount;
			};

		private:
			static const u32 IA_PARTICLE_COUNTER = 0;
			static const u32 IA_PREV_PARTICLE_COUNTER = IA_PARTICLE_COUNTER + 4;

			static const u32 PARTICLE_PER_THREAD = 256; // Dispatch(ceil(TotalParticleMax / (float)PARTICLE_PER_THREAD), 1 ,1)

			static const u32 INVALID_TAG = 0xffffffff;

			static const u32 TotalEmitterMax = 256;
			static const u32 TotalEmitterBinaryMax = 1024;
			static const u32 TotalParticleMax = 1024;

		private:
			std::unique_ptr<GPUBuffer> indirectArgsBuffer;

			std::unique_ptr<GPUBuffer> emitterTableBuffer;
			std::unique_ptr<GPUBuffer> emitterHeadersBuffer;
			std::unique_ptr<GPUBuffer> emitterBinaryBuffer;
			std::unique_ptr<GPUBuffer> emitterDataBuffer;
			std::unique_ptr<GPUBuffer> emitterRangeBuffer;

			std::unique_ptr<GPUBuffer> paticleHeadersBuffer;
			std::unique_ptr<GPUBuffer> paticleBinaryBuffer;
			std::unique_ptr<GPUBuffer> paticleIndexListBuffer;

			std::unique_ptr<Graphics::IBuffer> particleCB;

			std::unique_ptr<Graphics::IComputeShader> clearSystemCS;
			std::unique_ptr<Graphics::IComputeShader> clearParticleCS;

			std::unique_ptr<Graphics::IComputeShader> beginUpdateCS;
			std::unique_ptr<Graphics::IComputeShader> fillUnusedIndexCS;
			std::unique_ptr<Graphics::IComputeShader> spawnParticlesCS;

		private:
			u32 emitterTable[TotalEmitterMax];
			f32 emitterBinary[TotalEmitterBinaryMax];
			EmitterHeader emitterHeader[TotalEmitterMax];
			EmitterParameter emitterParameters[TotalEmitterMax];
			EmitterData emitterDatas[TotalEmitterMax];

			ParticleHeader particleHeader[TotalEmitterMax];

			u32 emitterRegisterCount = 0;
			u32 emitterBinaryCount = 0;

			u32 particleBinaryCount = 0;

			u32 currentSpawnParticleCount = 0;
			u32 currentStartUpEmitterCount = 0;
			u32 previousStartUpEmitterParticleCount = 0;
			u32 currentStartUpEmitterParticleCount = 0;
			u32 activeParticleCount = 0;

		public:
			EmitParticle() {}
			~EmitParticle() {}

		public:
			// 生成
			static std::unique_ptr<EmitParticle> Create();

			// 初期化
			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice);
			bool InitializeSystem(Graphics::IGraphicsDevice* graphicsDevice);

			// 終了化
			void Finalize();

			// 更新
			void UpdateCPU(Graphics::IGraphicsDevice* graphicsDevice, Math::Vector3 transform, float dt);

			void UpdateGPU(Graphics::IGraphicsDevice* graphicsDevice, Math::Matrix worldTransform, float dt);

			void Draw(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera);

			void RegisterEmitter(EmitterParameter emitterParameter);

			void StartUpEmitter(u32 emitterNum);

			void Burst(int num);

			void Restart();

			bool CreateBuffers(Graphics::IDevice* device);

			void LoadShaders(Graphics::IDevice* device);
		};
	*/
	}
}