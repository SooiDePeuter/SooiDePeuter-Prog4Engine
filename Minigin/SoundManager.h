#include <memory>

namespace dae
{
	struct SoundRequest final
	{
		int Id;
		float Volume;
	};

	struct SoundObject final
	{
		int Id;
		const char* Path; //path should not be changed after creation
	};

	class SoundSystem final
	{
		class Impl;
		Impl* m_pImpl;

	public:
		SoundSystem();
		~SoundSystem();

		void play(const SoundRequest& sound);
		void RegisterSound(const SoundObject& soundObject);
	};

	class ServiceLocator final
	{
		static std::unique_ptr<SoundSystem> ServiceInstance;

	public:

		static SoundSystem& GetService();

		static void RegisterService(std::unique_ptr<SoundSystem>&& service);
	};
}
