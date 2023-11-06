#ifndef WCSSKILLS_SE_H
#define WCSSKILLS_SE_H

namespace stateff {
	class StatusEffect;
}

#include "../traits/traits.hpp"
#include "../player/wcsplayer.hpp"
#include "../managers/infomanager.hpp"
#include "../events/event.hpp"

namespace stateff {
	
	class StatusEffect {
	public:
		StatusEffect(
			const WCSPlayer* _owner,
			const managers::SEInfo* _info,
			const dataStorage::DataStorage* arguments,
			const float* multiplier
		);
		~StatusEffect();
		
		const WCSPlayer* owner;
		const managers::SEInfo* info;
		dataStorage::DataStorage* arguments;
		
		events::ReturnEvent execute(std::shared_ptr<events::Event>&);
		
		[[nodiscard]] traits::tr_set getTraits() const noexcept;
		[[nodiscard]] bool is_unremovable() const noexcept;
	private:
		events::eventReceiver eventReceiver;
	};
} // namespace stateff

#endif //WCSSKILLS_SE_H
